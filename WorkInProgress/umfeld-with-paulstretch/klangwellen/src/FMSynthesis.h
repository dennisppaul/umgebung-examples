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
 * - [x] void process(float*, uint32_t)
 * - [ ] void process(float*, float*, uint32_t)
 */

#pragma once

#include <stdint.h>
#include <stdio.h>
#include <cmath>

#include "KlangWellen.h"
#include "Wavetable.h"

namespace klangwellen {
    class FMSynthesis {
    public:
        FMSynthesis(Wavetable* pCarrier, Wavetable* pModulator) : mAmplitude(1.0f),
                                                                  mCarrier(pCarrier),
                                                                  mModulationDepth(1.0f),
                                                                  mModulator(pModulator) {
            fDeleteCarrier = false;
        }

        FMSynthesis(const uint32_t wavetable_size, const uint32_t sampling_rate) : mAmplitude(1.0f),
                                                                                   mCarrier(new Wavetable(wavetable_size, sampling_rate)),
                                                                                   mModulationDepth(1.0f),
                                                                                   mModulator(new Wavetable(wavetable_size, sampling_rate)) {
            mCarrier->set_interpolation(KlangWellen::WAVESHAPE_INTERPOLATE_LINEAR);
            Wavetable::fill(mCarrier->get_wavetable(), mCarrier->get_wavetable_size(), KlangWellen::WAVEFORM_SINE);

            mModulator->set_interpolation(KlangWellen::WAVESHAPE_INTERPOLATE_LINEAR);
            Wavetable::fill(mModulator->get_wavetable(), mModulator->get_wavetable_size(), KlangWellen::WAVEFORM_SINE);
            fDeleteCarrier = true;
        }

        Wavetable* get_modulator() const {
            return mModulator;
        }

        void set_modulator(Wavetable* pModulator) {
            mModulator = pModulator;
        }

        Wavetable* get_carrier() const {
            return mCarrier;
        }

        void set_carrier(Wavetable* pCarrier) {
            mCarrier = pCarrier;
        }

        float get_amplitude() const {
            return mAmplitude;
        }

        void set_amplitude(float pAmplitude) {
            mAmplitude = pAmplitude;
        }

        float get_modulation_depth() const {
            return mModulationDepth;
        }

        void set_modulation_depth(float pModulationDepth) {
            mModulationDepth = pModulationDepth;
        }

        float process() const {
            const float mCarrierSignal   = mCarrier->process();
            const float mModulatorSignal = mModulator->process();
            const float mSignal          = mAmplitude * std::sin(TWO_PI * mCarrierSignal + mModulationDepth * std::sin(TWO_PI * mModulatorSignal));
            return mSignal;
        }

        void process(float* signal_buffer, const uint32_t buffer_length) const {
            for (uint16_t i = 0; i < buffer_length; i++) {
                signal_buffer[i] = process();
            }
        }

    private:
        float      mAmplitude;
        Wavetable* mCarrier;
        float      mModulationDepth;
        Wavetable* mModulator;
        bool       fDeleteCarrier;
    };
} // namespace klangwellen