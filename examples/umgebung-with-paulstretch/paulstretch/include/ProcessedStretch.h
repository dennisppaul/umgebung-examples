/*
  Copyright (C) 2011 Nasca Octavian Paul
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

#pragma once

#ifdef KISSFFT
#include <kiss_fftr.h>
#else
#include <fftw3.h>
#warning "FFTW3 not supported"
#endif

class FFT { // FFT class that considers phases as random
public:
    FFT(int nsamples_); // samples must be even
    ~FFT();
    enum FFTWindow { _W_RECTANGULAR,
                     _W_HAMMING,
                     _W_HANN,
                     _W_BLACKMAN,
                     _W_BLACKMAN_HARRIS };
    void   smp2freq(); // input is smp, output is freq (phases are discarded)
    void   freq2smp(); // input is freq,output is smp (phases are random)
    void   applywindow(FFTWindow type);
    float* smp;  // size of samples/2
    float* freq; // size of samples
    int    nsamples;

private:
#ifdef KISSFFT
    kiss_fftr_cfg    plankfft, plankifft;
    kiss_fft_scalar* datar;
    kiss_fft_cpx*    datac;
#else
    float*     data;
    fftwf_plan plan;
    fftwf_plan plani;
#endif
    struct {
        float*    data;
        FFTWindow type;
    } window;

    unsigned int        rand_seed;
    static unsigned int start_rand_seed;
};

class ProcessedStretch {
public:
    ProcessedStretch(float          rap_,
                     int            in_bufsize_,
                     FFT::FFTWindow w           = FFT::_W_HAMMING,
                     bool           bypass_     = false,
                     float          samplerate_ = 44100);
    ~ProcessedStretch();

    int get_max_bufsize() {
        return bufsize * 3;
    };

    int get_bufsize() {
        return bufsize;
    };

    void process(float* smps, int nsmps);

    void set_freezing(bool new_freezing) {
        freezing = new_freezing;
    };

    float* out_buf;                                  // I can put a variable "max_out_bufsize" and let this be the size of out_buf and use out_bufsize as an adaptive size
    int    get_nsamples(float current_pos_percents); // how many samples are required
    int    get_nsamples_for_fill();                  // how many samples are required to be added for a complete buffer refill (at start of the song or after seek)
    int    get_skip_nsamples();                      // used for shorten
    void   set_rap(float newrap);                    // set the current stretch value

    FFT::FFTWindow window_type;

private:
    void process_spectrum(float* freq) {};
    void do_analyse_inbuf(float* smps);
    void do_next_inbuf_smps(float* smps);

    void copy(float* freq1, float* freq2);
    void add(float* freq2, float* freq1, float a = 1.0);
    void mul(float* freq1, float a);
    void zero(float* freq1);
    void spread(float* freq1, float* freq2, float spread_bandwidth);

    FFT *infft, *outfft;
    FFT* fft;

    int    bufsize;
    float  samplerate;
    float  rap;
    float* old_out_smps;
    float* old_freq;
    float *new_smps, *old_smps, *very_old_smps;

    long double remained_samples; // 0..1
    float       c_pos_percents;
    int         skip_samples;
    bool        require_new_buffer;
    bool        bypass, freezing;

    int    nfreq;
    float* free_filter_freqs;
    float *infreq, *sumfreq, *tmpfreq1, *tmpfreq2;
};
