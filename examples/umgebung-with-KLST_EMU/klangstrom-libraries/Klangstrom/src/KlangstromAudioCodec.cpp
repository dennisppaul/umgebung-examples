/*
 * Klangstrom
 *
 * This file is part of the *wellen* library (https://github.com/dennisppaul/wellen).
 * Copyright (c) 2024 Dennis P Paul.
 *
 * This library is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, version 3.
 *
 * This library is distributed in the hope that it will be useful, but
 * WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU
 * General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program. If not, see <http://www.gnu.org/licenses/>.
 */

#include <cstddef>
#include <cmath>

#include "KlangstromDefines.h"
#include "KlangstromAudioCodec.h"

/* --- callback_interface --- */

extern "C" {
static AudioCodec* mAudioCodecClass = nullptr;

void audiocodec_register_audio_device(AudioCodec* pAudioCodecClass) {
    mAudioCodecClass = pAudioCodecClass;
}

/**
 * callback for underlying hardware layer … with either uint32 or float type
*/
void audiocodec_callback_class_i(uint32_t* input, uint32_t* output, uint16_t length) {
    if (mAudioCodecClass) {
        mAudioCodecClass->callback_class(input, output, length);
    }
}

void audiocodec_callback_class_f(float** input, float** output, uint16_t length) {
    if (mAudioCodecClass) {
        mAudioCodecClass->callback_class_f(input, output, length);
    }
}
} // extern "C"

void WEAK audioblock(float** input_signal, float** output_signal, uint16_t length) {
    for (int i = 0; i < length; ++i) {
        for (int j = 0; j < KLANG_OUTPUT_CHANNELS; ++j) {
            output_signal[j][i] = 0.0;
        }
    }
}

// TODO clean up the code below. flexible bit depth, buffer length etcetera

static const uint8_t  M_NUM_OF_BITS = KLANG_AUDIO_BIT_DEPTH;
static const float    M_INT_SCALE   = (1 << (M_NUM_OF_BITS - 1)); // - 1.0;  // @todo(see if `-1.0` is required)
static const uint32_t M_MASK_LEFT   = (1 << M_NUM_OF_BITS) - 1;
static const uint32_t M_MASK_RIGHT  = ~(M_MASK_LEFT);

/**
 * fills buffer with new block of samples
 */
void AudioCodec::callback_class(uint32_t* input, uint32_t* output, uint16_t length) {
    // TODO `length` does not necessarily equal `KLANG_SAMPLES_PER_AUDIO_BLOCK`
    // TODO if length > KLANG_SAMPLES_PER_AUDIO_BLOCK try to chop it down
    if (!callback_audioblock) {
        for (int i = 0; i < KLANG_SAMPLES_PER_AUDIO_BLOCK; i++) {
            output[i] = 0;
        }
        return;
    }

    float fInputBuffers[KLANG_INPUT_CHANNELS][KLANG_SAMPLES_PER_AUDIO_BLOCK];
    float fOutputBuffers[KLANG_OUTPUT_CHANNELS][KLANG_SAMPLES_PER_AUDIO_BLOCK];

    /* unpack receive buffer to sample */
    for (int i = 0; i < KLANG_SAMPLES_PER_AUDIO_BLOCK; i++) {
        const uint32_t p                      = input[i];
        const int16_t  mLeftInt               = (p & M_MASK_LEFT);
        const int16_t  mRightInt              = (p & M_MASK_RIGHT) >> M_NUM_OF_BITS;
        fInputBuffers[KLANG_CHANNEL_LEFT][i]  = mLeftInt / M_INT_SCALE;
        fInputBuffers[KLANG_CHANNEL_RIGHT][i] = mRightInt / M_INT_SCALE;
    }

    /* calculate next audio block */
    float* mInputBuffer[KLANG_INPUT_CHANNELS];
    float* mOutputBuffer[KLANG_OUTPUT_CHANNELS];
    for (size_t j = 0; j < KLANG_INPUT_CHANNELS; j++) {
        mInputBuffer[j] = fInputBuffers[j];
    }
    for (size_t j = 0; j < KLANG_OUTPUT_CHANNELS; j++) {
        mOutputBuffer[j] = fOutputBuffers[j];
    }

    callback_audioblock(mInputBuffer, mOutputBuffer, KLANG_SAMPLES_PER_AUDIO_BLOCK);

    /* pack sample for transmit buffer */
    for (int i = 0; i < KLANG_SAMPLES_PER_AUDIO_BLOCK; i++) {
        int16_t mLeftInt  = (int16_t) (fOutputBuffers[KLANG_CHANNEL_LEFT][i] * M_INT_SCALE);
        int16_t mRightInt = (int16_t) (fOutputBuffers[KLANG_CHANNEL_RIGHT][i] * M_INT_SCALE);
        output[i]         = ((uint32_t) (uint16_t) mLeftInt) << 0 | ((uint32_t) (uint16_t) mRightInt) << M_NUM_OF_BITS;
    }
}

/**
 * fills buffer with new block of samples
 */
void AudioCodec::callback_class_f(float** input, float** output, uint16_t length) {
    if (!callback_audioblock) {
        for (int i = 0; i < length; i++) {
            output[i] = 0;
        }
        return;
    }

    /* if (length > KLANG_SAMPLES_PER_AUDIO_BLOCK) try to chop it down */
    if (length > KLANG_SAMPLES_PER_AUDIO_BLOCK) {
        // TODO this scenario is not ideal and should be avoided … maybe issue a warning somewhere?
        const uint16_t blockSize        = KLANG_SAMPLES_PER_AUDIO_BLOCK;
        const uint8_t  mIterations      = length / blockSize;
        const uint16_t remainingSamples = length % blockSize;

        for (uint8_t i = 0; i < mIterations; i++) {
            float* mInputSection[KLANG_INPUT_CHANNELS];
            float* mOutputSection[KLANG_OUTPUT_CHANNELS];

            for (int channel = 0; channel < KLANG_INPUT_CHANNELS; channel++) {
                mInputSection[channel] = &input[channel][i * blockSize];
            }

            for (int channel = 0; channel < KLANG_OUTPUT_CHANNELS; channel++) {
                mOutputSection[channel] = &output[channel][i * blockSize];
            }

            callback_audioblock(mInputSection, mOutputSection, blockSize);
        }

        /* process any remaining samples */
        if (remainingSamples > 0) {
            float* mInputSection[KLANG_INPUT_CHANNELS];
            float* mOutputSection[KLANG_OUTPUT_CHANNELS];

            for (int channel = 0; channel < KLANG_INPUT_CHANNELS; channel++) {
                mInputSection[channel] = &input[channel][mIterations * blockSize];
            }

            for (int channel = 0; channel < KLANG_OUTPUT_CHANNELS; channel++) {
                mOutputSection[channel] = &output[channel][mIterations * blockSize];
            }

            callback_audioblock(mInputSection, mOutputSection, remainingSamples);
        }
    } else {
        callback_audioblock(input, output, length);
    }
}

/* --- callback_interface --- */

void AudioCodec::callback_audioblock_method(float** input_signal, float** output_signal, uint16_t length) {
    if (callback_audioblock) {
        callback_audioblock(input_signal, output_signal, length);
    }
}

AudioCodec::AudioCodec() : isInitialized(false) {
    // TODO check if it is ok to move this to `.init()`
    // audiocodec_register_audio_device(this);
}

void AudioCodec::init() {
    if (!isInitialized) {
        audiocodec_register_audio_device(this);
        register_audioblock(audioblock);
        KLST_BSP_audiocodec_init();
        KLST_BSP_init();
        isInitialized = true;
    }
}
