#pragma once

#include <vector>
#include <algorithm>
#include <iostream>

#include "ProcessedStretch.h"

class PaulStretch {
public:
    PaulStretch(int stretch_value, int buffer_size, int samplerate) : pInputBufferSize(buffer_size),
                                                                      pStretch(stretch_value),
                                                                      pSampleRate(samplerate) {
        stretch             = new ProcessedStretch(stretch_value,
                                                   buffer_size,
                                                   FFT::_W_HANN,
                                                   false,
                                                   samplerate);
        pOutputBufferSize   = stretch->get_bufsize();
        const int mPoolSize = stretch->get_max_bufsize();

        pInputBuffer = new float[mPoolSize];
        for (int i = 0; i < mPoolSize; i++) {
            pInputBuffer[i] = 0.0;
        }

        pRequiredSamples = stretch->get_nsamples_for_fill();
    }

    ~PaulStretch() {
        delete stretch;
        delete pInputBuffer;
    }

    void fill_input_buffer(float* input_buffer, int number_of_samples) {
        for (int i = 0; i < number_of_samples; i++) {
            pInputBuffer[i] = input_buffer[i];
        }
    }

    float* get_input_buffer() {
        return pInputBuffer;
    }

    bool process_segment(float* buffer) {
        stretch->process(pInputBuffer, pRequiredSamples);
        for (int i = 0; i < pOutputBufferSize; i++) {
            buffer[i] = stretch->out_buf[i];
        }
        pRequiredSamples = stretch->get_nsamples(0);
        return pRequiredSamples == 0;
    }

    void process(std::vector<float>& samples) {
        do {
            stretch->process(pInputBuffer, pRequiredSamples);
            std::copy(stretch->out_buf, stretch->out_buf + pOutputBufferSize, std::back_inserter(samples));
            pRequiredSamples = stretch->get_nsamples(0);
        } while (pRequiredSamples == 0);
    }

    int get_required_samples() {
        return pRequiredSamples;
    }

    int get_output_buffer_size() {
        return pOutputBufferSize;
    }

private:
    ProcessedStretch* stretch;
    int               pInputBufferSize;
    int               pOutputBufferSize;
    int               pStretch;
    int               pSampleRate;
    float*            pInputBuffer;
    int               pRequiredSamples;
};
