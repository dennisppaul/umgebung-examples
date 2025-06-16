/*
* KlangWellen
 *
 * This file is part of the *KlangWellen* library (https://github.com/dennisppaul/klangwellen).
 * Copyright (c) 2024 Dennis P Paul
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

/**
 * PROCESSOR INTERFACE
 *
 * - [x] float process()
 * - [ ] float process(float)
 * - [ ] void process(AudioSignal&)
 * - [x] void process(float*, uint32_t) *overwrite*
 * - [ ] void process(float*, float*, uint32_t)
 */

#pragma once

#include "KlangWellen.h"

namespace klangwellen {
    class OscillatorFunction {
    public:
        explicit OscillatorFunction(const uint32_t sample_rate = KlangWellen::DEFAULT_SAMPLE_RATE) : mSamplingRate(sample_rate) {
            mWaveform = KlangWellen::WAVEFORM_SINE;
            set_frequency(DEFAULT_FREQUENCY);
            set_amplitude(DEFAULT_AMPLITUDE);
        }

        static double mod(const double a, const double b) {
            return a >= b ? (a - b * static_cast<int>(a / b)) : a;
        }

        int get_waveform() const {
            return mWaveform;
        }

        void set_waveform(const int pWaveform) {
            mWaveform = pWaveform;
        }

        float get_amplitude() const {
            return mAmplitude;
        }

        void set_amplitude(const float pAmplitude) {
            mAmplitude = pAmplitude;
        }

        float get_offset() const {
            return mOffset;
        }

        void set_offset(const float pOffset) {
            mOffset = pOffset;
        }

        float get_frequency() const {
            return static_cast<float>(mFrequency);
        }

        void set_frequency(const float pFrequency) {
            if (mFrequency != pFrequency) {
                mFrequency = pFrequency;
                mStepSize  = mFrequency * (TWO_PI) / mSamplingRate;
            }
        }

        float process() {
            double s;
            switch (mWaveform) {
                case KlangWellen::WAVEFORM_SINE:
                    s = process_sine();
                    break;
                case KlangWellen::WAVEFORM_TRIANGLE:
                    s = process_triangle();
                    break;
                case KlangWellen::WAVEFORM_SAWTOOTH:
                    s = process_sawtooth();
                    break;
                case KlangWellen::WAVEFORM_SQUARE:
                    s = process_square();
                    break;
                case KlangWellen::WAVEFORM_NOISE:
                    s = KlangWellen::random();
                    break;
                default:
                    s = 0.0f;
            }
            s *= mAmplitude;
            s += mOffset;
            return static_cast<float>(s);
        }

        void process(float* signal_buffer, const uint32_t buffer_length) {
            for (uint16_t i = 0; i < buffer_length; i++) {
                signal_buffer[i] = process();
            }
        }

    private:
        static constexpr float DEFAULT_AMPLITUDE = 0.75f;
        static constexpr float DEFAULT_FREQUENCY = 220.0f;

        float        mAmplitude;
        double       mFrequency;
        float        mOffset;
        double       mPhase;
        const double mSamplingRate;
        double       mStepSize;
        int          mWaveform;

        double process_sawtooth() {
            mPhase += mFrequency;
            mPhase = mod(mPhase, mSamplingRate);
            return mPhase / (mSamplingRate / 2.0) + KlangWellen::SIGNAL_MIN;
        }

        double process_sine() {
            mPhase += mStepSize;
            if (mPhase > TWO_PI) {
                mPhase -= TWO_PI;
            }
            return sin(mPhase);
        }

        double process_square() {
            mPhase += mFrequency;
            mPhase = mod(mPhase, mSamplingRate);
            return mPhase > (mSamplingRate / 2.0f) ? KlangWellen::SIGNAL_MAX : KlangWellen::SIGNAL_MIN;
        }

        double process_triangle() {
            mPhase += mFrequency;
            mPhase                        = mod(mPhase, mSamplingRate);
            const double mPhaseShifted    = mPhase - (mSamplingRate / 2.0);
            const double mPhaseShiftedAbs = mPhaseShifted > 0 ? mPhaseShifted : -mPhaseShifted;
            return (mPhaseShiftedAbs - mSamplingRate / 4.0) / (mSamplingRate / 4.0);
        }
    };
} // namespace klangwellen