/*
  Copyright (C) 2009 Nasca Octavian Paul
  Author: Nasca Octavian Paul

  This program is free software; you can redistribute it and/or modify
  it under the terms of version 2 of the GNU General Public License
  as published by the Free Software Foundation.

  This program is distributed in the hope that it will be useful,
  but WITHOUT ANY WARRANTY; without even the implied warranty of
  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
  GNU General Public License (version 2) for more details.

  You should have received a copy of the GNU General Public License (version 2)
  along with this program; if not, write to the Free Software Foundation,
  Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307 USA
*/

#include <math.h>
#include <stdlib.h>
#include <stdio.h>
#include <math.h>

#include "ProcessedStretch.h"

ProcessedStretch::ProcessedStretch(float          rap_,
                                   int            in_bufsize_,
                                   FFT::FFTWindow w,
                                   bool           bypass_,
                                   float          samplerate_) {

    freezing = false;

    samplerate = samplerate_;
    rap        = rap_;
    bufsize    = in_bufsize_;
    bypass     = bypass_;
    if (bufsize < 8) bufsize = 8;

    out_buf  = new float[bufsize];
    old_freq = new float[bufsize];

    very_old_smps = new float[bufsize];
    new_smps      = new float[bufsize];
    old_smps      = new float[bufsize];

    old_out_smps = new float[bufsize * 2];
    for (int i = 0; i < bufsize * 2; i++) {
        old_out_smps[i] = 0.0;
    };
    for (int i = 0; i < bufsize; i++) {
        old_freq[i]      = 0.0;
        new_smps[i]      = 0.0;
        old_smps[i]      = 0.0;
        very_old_smps[i] = 0.0;
    };

    infft              = new FFT(bufsize * 2);
    fft                = new FFT(bufsize * 2);
    outfft             = new FFT(bufsize * 2);
    remained_samples   = 0.0;
    window_type        = w;
    require_new_buffer = false;
    c_pos_percents     = 0.0;
    skip_samples       = 0;

    nfreq             = bufsize;
    infreq            = new float[nfreq];
    sumfreq           = new float[nfreq];
    tmpfreq1          = new float[nfreq];
    tmpfreq2          = new float[nfreq];
    free_filter_freqs = new float[nfreq];
    for (int i = 0; i < nfreq; i++) {
        free_filter_freqs[i] = 1.0;
    };
};

ProcessedStretch::~ProcessedStretch() {
    delete[] infreq;
    delete[] sumfreq;
    delete[] tmpfreq1;
    delete[] tmpfreq2;
    delete[] free_filter_freqs;

    delete[] old_freq;
    delete[] out_buf;
    delete[] new_smps;
    delete[] old_smps;
    delete[] very_old_smps;
    delete[] old_out_smps;
    delete fft;
    delete infft;
    delete outfft;
};

void ProcessedStretch::copy(float* freq1, float* freq2) {
    for (int i = 0; i < nfreq; i++) freq2[i] = freq1[i];
};

void ProcessedStretch::add(float* freq2, float* freq1, float a) {
    for (int i = 0; i < nfreq; i++) freq2[i] += freq1[i] * a;
};

void ProcessedStretch::mul(float* freq1, float a) {
    for (int i = 0; i < nfreq; i++) freq1[i] *= a;
};

void ProcessedStretch::zero(float* freq1) {
    for (int i = 0; i < nfreq; i++) freq1[i] = 0.0;
};

void ProcessedStretch::spread(float* freq1, float* freq2, float spread_bandwidth) {
    // convert to log spectrum
    float minfreq = 20.0;
    float maxfreq = 0.5 * samplerate;

    float log_minfreq = log(minfreq);
    float log_maxfreq = log(maxfreq);

    for (int i = 0; i < nfreq; i++) {
        float freqx = i / (float) nfreq;
        float x     = exp(log_minfreq + freqx * (log_maxfreq - log_minfreq)) / maxfreq * nfreq;
        float y     = 0.0;
        int   x0    = (int) floor(x);
        if (x0 >= nfreq) x0 = nfreq - 1;
        int x1 = x0 + 1;
        if (x1 >= nfreq) x1 = nfreq - 1;
        float xp = x - x0;
        if (x < nfreq) {
            y = freq1[x0] * (1.0 - xp) + freq1[x1] * xp;
        };
        tmpfreq1[i] = y;
    };

    // increase the bandwidth of each harmonic (by smoothing the log spectrum)
    int   n         = 2;
    float bandwidth = spread_bandwidth;
    float a         = 1.0 - pow(2.0, -bandwidth * bandwidth * 10.0);
    a               = pow(a, 8192.0 / nfreq * n);

    for (int k = 0; k < n; k++) {
        tmpfreq1[0] = 0.0;
        for (int i = 1; i < nfreq; i++) {
            tmpfreq1[i] = tmpfreq1[i - 1] * a + tmpfreq1[i] * (1.0 - a);
        };
        tmpfreq1[nfreq - 1] = 0.0;
        for (int i = nfreq - 2; i > 0; i--) {
            tmpfreq1[i] = tmpfreq1[i + 1] * a + tmpfreq1[i] * (1.0 - a);
        };
    };

    freq2[0]                    = 0;
    float log_maxfreq_d_minfreq = log(maxfreq / minfreq);
    for (int i = 1; i < nfreq; i++) {
        float freqx = i / (float) nfreq;
        float x     = log((freqx * maxfreq) / minfreq) / log_maxfreq_d_minfreq * nfreq;
        float y     = 0.0;
        if ((x > 0.0) && (x < nfreq)) {
            int x0 = (int) floor(x);
            if (x0 >= nfreq) x0 = nfreq - 1;
            int x1 = x0 + 1;
            if (x1 >= nfreq) x1 = nfreq - 1;
            float xp = x - x0;
            y        = tmpfreq1[x0] * (1.0 - xp) + tmpfreq1[x1] * xp;
        };
        freq2[i] = y;
    };
};

unsigned int FFT::start_rand_seed = 1;

FFT::FFT(int nsamples_) {
    nsamples = nsamples_;
    if (nsamples % 2 != 0) {
        nsamples += 1;
        printf("WARNING: Odd sample size on _FFT::_FFT() (%d)", nsamples);
    };
    smp = new float[nsamples];
    for (int i = 0; i < nsamples; i++) smp[i] = 0.0;
    freq = new float[nsamples / 2 + 1];
    for (int i = 0; i < nsamples / 2 + 1; i++) freq[i] = 0.0;
    window.data = new float[nsamples];
    for (int i = 0; i < nsamples; i++) window.data[i] = 0.707;
    window.type = _W_RECTANGULAR;

#ifdef KISSFFT
    datar = new kiss_fft_scalar[nsamples + 2];
    for (int i = 0; i < nsamples + 2; i++) datar[i] = 0.0;
    datac = new kiss_fft_cpx[nsamples / 2 + 2];
    for (int i = 0; i < nsamples / 2 + 2; i++) datac[i].r = datac[i].i = 0.0;
    plankfft  = kiss_fftr_alloc(nsamples, 0, 0, 0);
    plankifft = kiss_fftr_alloc(nsamples, 1, 0, 0);
#else
    data = new float[nsamples];
    for (int i = 0; i < nsamples; i++) data[i] = 0.0;
    plan  = fftwf_plan_r2r_1d(nsamples, data, data, _FFTW_R2HC, _FFTW_ESTIMATE);
    plani = fftwf_plan_r2r_1d(nsamples, data, data, _FFTW_HC2R, _FFTW_ESTIMATE);
#endif
    rand_seed = start_rand_seed;
    start_rand_seed += 161103;
};

FFT::~FFT() {
    delete[] smp;
    delete[] freq;
    delete[] window.data;
#ifdef KISSFFT
    delete[] datar;
    delete[] datac;
    free(plankfft);
    free(plankifft);
#else
    delete[] data;
    fftwf_destroy_plan(plan);
    fftwf_destroy_plan(plani);
#endif
};

void FFT::smp2freq() {
#ifdef KISSFFT
    for (int i = 0; i < nsamples; i++) datar[i] = smp[i];
    kiss_fftr(plankfft, datar, datac);
#else
    for (int i = 0; i < nsamples; i++) data[i] = smp[i];
    fftwf_execute(plan);
#endif

    for (int i = 1; i < nsamples / 2; i++) {
#ifdef KISSFFT
        float c = datac[i].r;
        float s = datac[i].i;
#else
        float c = data[i];
        float s = data[nsamples - i];
#endif
        freq[i] = sqrt(c * c + s * s);
    };
    freq[0] = 0.0;
};

void FFT::freq2smp() {
    float inv_2p15_2pi = 1.0 / 16384.0 * M_PI;
    for (int i = 1; i < nsamples / 2; i++) {
        rand_seed          = (rand_seed * 1103515245 + 12345);
        unsigned int rand  = (rand_seed >> 16) & 0x7fff;
        float        phase = rand * inv_2p15_2pi;
#ifdef KISSFFT
        datac[i].r = freq[i] * cos(phase);
        datac[i].i = freq[i] * sin(phase);
#else
        data[i]            = freq[i] * cos(phase);
        data[nsamples - i] = freq[i] * sin(phase);
#endif
    };

#ifdef KISSFFT
    datac[0].r = datac[0].i = 0.0;
    kiss_fftri(plankifft, datac, datar);
    for (int i = 0; i < nsamples; i++) smp[i] = datar[i] / nsamples;
#else
    data[0] = data[nsamples / 2 + 1] = data[nsamples / 2] = 0.0;
    fftwf_execute(plani);
    for (int i = 0; i < nsamples; i++) smp[i] = data[i] / nsamples;
#endif
};

void FFT::applywindow(FFTWindow type) {
    if (window.type != type) {
        window.type = type;
        switch (type) {
            case _W_RECTANGULAR:
                for (int i = 0; i < nsamples; i++) window.data[i] = 0.707;
                break;
            case _W_HAMMING:
                for (int i = 0; i < nsamples; i++) window.data[i] = 0.53836 - 0.46164 * cos(2 * M_PI * i / (nsamples + 1.0));
                break;
            case _W_HANN:
                for (int i = 0; i < nsamples; i++) window.data[i] = 0.5 * (1.0 - cos(2 * M_PI * i / (nsamples - 1.0)));
                break;
            case _W_BLACKMAN:
                for (int i = 0; i < nsamples; i++) window.data[i] = 0.42 - 0.5 * cos(2 * M_PI * i / (nsamples - 1.0)) + 0.08 * cos(4 * M_PI * i / (nsamples - 1.0));
                break;
            case _W_BLACKMAN_HARRIS:
                for (int i = 0; i < nsamples; i++) window.data[i] = 0.35875 - 0.48829 * cos(2 * M_PI * i / (nsamples - 1.0)) + 0.14128 * cos(4 * M_PI * i / (nsamples - 1.0)) - 0.01168 * cos(6 * M_PI * i / (nsamples - 1.0));
                break;
        };
    };
    for (int i = 0; i < nsamples; i++) smp[i] *= window.data[i];
};

void ProcessedStretch::set_rap(float newrap) {
    rap = newrap;
};

void ProcessedStretch::do_analyse_inbuf(float* smps) {
    // get the frequencies
    for (int i = 0; i < bufsize; i++) {
        infft->smp[i]           = old_smps[i];
        infft->smp[i + bufsize] = smps[i];

        old_freq[i] = infft->freq[i];
    };
    infft->applywindow(window_type);
    infft->smp2freq();
};

void ProcessedStretch::do_next_inbuf_smps(float* smps) {
    for (int i = 0; i < bufsize; i++) {
        very_old_smps[i] = old_smps[i];
        old_smps[i]      = new_smps[i];
        new_smps[i]      = smps[i];
    };
};

void ProcessedStretch::process(float* smps, int nsmps) {
    if (bypass) {
        for (int i = 0; i < bufsize; i++) out_buf[i] = smps[i];
        return;
    };

    if (smps != NULL) {
        if ((nsmps != 0) && (nsmps != bufsize) && (nsmps != get_max_bufsize())) {
            printf("Warning wrong nsmps on ProcessedStretchCompact::process() %d,%d\n", nsmps, bufsize);
            return;
        };
        if (nsmps != 0) { // new data arrived: update the frequency components
            do_analyse_inbuf(smps);
            if (nsmps == get_max_bufsize()) {
                for (int k = bufsize; k < get_max_bufsize(); k += bufsize) do_analyse_inbuf(smps + k);
            };
        };

        // move the buffers
        if (nsmps != 0) { // new data arrived: update the frequency components
            do_next_inbuf_smps(smps);
            if (nsmps == get_max_bufsize()) {
                for (int k = bufsize; k < get_max_bufsize(); k += bufsize) do_next_inbuf_smps(smps + k);
            };
        };

        // construct the input fft
        int start_pos = (int) (floor(remained_samples * bufsize));
        if (start_pos >= bufsize) start_pos = bufsize - 1;
        for (int i = 0; i < bufsize - start_pos; i++) fft->smp[i] = very_old_smps[i + start_pos];
        for (int i = 0; i < bufsize; i++) fft->smp[i + bufsize - start_pos] = old_smps[i];
        for (int i = 0; i < start_pos; i++) fft->smp[i + 2 * bufsize - start_pos] = new_smps[i];
        // compute the output spectrum
        fft->applywindow(window_type);
        fft->smp2freq();
        for (int i = 0; i < bufsize; i++) outfft->freq[i] = fft->freq[i];

        process_spectrum(outfft->freq);

        outfft->freq2smp();

        // make the output buffer
        float tmp        = 1.0 / (float) bufsize * M_PI;
        float hinv_sqrt2 = 0.853553390593; //(1.0+1.0/sqrt(2))*0.5;

        float ampfactor = 2.0;

        // remove the resulted unwanted amplitude modulation (caused by the interference of N and N+1 windowed buffer and compute the output buffer
        for (int i = 0; i < bufsize; i++) {
            // TODO optimize with faster cos function
            float a    = (0.5 + 0.5 * cos(i * tmp));
            float out  = outfft->smp[i + bufsize] * (1.0 - a) + old_out_smps[i] * a;
            out_buf[i] = out * (hinv_sqrt2 - (1.0 - hinv_sqrt2) * cos(i * 2.0 * tmp)) * ampfactor;
        };

        // copy the current output buffer to old buffer
        for (int i = 0; i < bufsize * 2; i++) old_out_smps[i] = outfft->smp[i];
    };

    if (!freezing) {
        long double used_rap                  = rap;
        long double r                         = 1.0 / used_rap;
        long double old_remained_samples_test = remained_samples;
        remained_samples += r;
        int result = 0;
        if (remained_samples >= 1.0) {
            skip_samples       = (int) (floor(remained_samples - 1.0) * bufsize);
            remained_samples   = remained_samples - floor(remained_samples);
            require_new_buffer = true;
        } else {
            require_new_buffer = false;
        };
    };
};

int ProcessedStretch::get_nsamples(float current_pos_percents) {
    if (bypass) return bufsize;
    if (freezing) return 0;
    c_pos_percents = current_pos_percents;
    return require_new_buffer ? bufsize : 0;
};

int ProcessedStretch::get_nsamples_for_fill() {
    return get_max_bufsize();
};

int ProcessedStretch::get_skip_nsamples() {
    if (freezing || bypass) return 0;
    return skip_samples;
};
