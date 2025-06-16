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
 * - [x] float process(float)
 * - [x] void process(AudioSignal&)
 * - [x] void process(float*, uint32_t)
 * - [x] void process(float*, float*, uint32_t)
 */

#pragma once

#include "KlangWellen.h"
#include "AudioSignal.h"

namespace klangwellen {
    class ADSR {
        /*
         *       @description(
         *
         *          |----->|-->|   |-->|
         *          |---A--|-D-|-S-|-R-|
         *          |      /\
         *          |     /  \
         *          |    /    \_____
         *          |   /        ^  \
         *          |  /         |   \
         *          | /          |    \
         *          |/___________|_____\
         *          |
         *          |Press          |Release
         *       )
         *
         */
    public:
        explicit ADSR(uint32_t sample_rate = KlangWellen::DEFAULT_SAMPLE_RATE) : fSampleRate(sample_rate), FADE_TO_ZERO_RATE_SEC(0.01f), USE_FADE_TO_ZERO_STATE(false) {
            fAmp     = 0.0f;
            fAttack  = KlangWellen::DEFAULT_ATTACK;
            fDecay   = KlangWellen::DEFAULT_DECAY;
            fDelta   = 0.0f;
            fRelease = KlangWellen::DEFAULT_RELEASE;
            fSustain = KlangWellen::DEFAULT_SUSTAIN;
            fState   = ENVELOPE_STATE::IDLE;
            setState(ENVELOPE_STATE::IDLE);
        }

        float process() {
            step();
            return fAmp;
        }

        float process(float signal) {
            step();
            return signal * fAmp;
        }

        void process(AudioSignal& signal) {
            step();
            signal.left *= fAmp;
            signal.right *= fAmp;
        }

        void process(float*         signal_buffer_left,
                     float*         signal_buffer_right,
                     const uint32_t buffer_length = KlangWellen::DEFAULT_AUDIOBLOCK_SIZE) {
            for (uint32_t i = 0; i < buffer_length; i++) {
                step();
                signal_buffer_left[i] *= fAmp;
                signal_buffer_right[i] *= fAmp;
            }
        }

        void process(float* signal_buffer, const uint32_t buffer_length = KlangWellen::DEFAULT_AUDIOBLOCK_SIZE) {
            for (uint32_t i = 0; i < buffer_length; i++) {
                step();
                signal_buffer[i] *= fAmp;
            }
        }

        void start() {
            check_scheduled_attack_state();
        }

        void stop() {
            check_scheduled_release_state();
        }

        float get_attack() const {
            return fAttack;
        }

        void set_attack(float pAttack) {
            fAttack = pAttack;
        }

        void set_adsr(float pAttack, float pDecay, float pSustain, float pRelease) {
            set_attack(pAttack);
            set_decay(pDecay);
            set_sustain(pSustain);
            set_release(pRelease);
        }

        float get_decay() const {
            return fDecay;
        }

        void set_decay(float pDecay) {
            fDecay = pDecay;
        }

        float get_sustain() const {
            return fSustain;
        }

        void set_sustain(float pSustain) {
            fSustain = pSustain;
        }

        float get_release() const;

        void set_release(float pRelease) {
            fRelease = pRelease;
        }

    private:
        enum class ENVELOPE_STATE {
            IDLE,
            ATTACK,
            DECAY,
            SUSTAIN,
            RELEASE,
            PRE_ATTACK_FADE_TO_ZERO
        };
        const uint32_t fSampleRate;
        const float    FADE_TO_ZERO_RATE_SEC;
        const bool     USE_FADE_TO_ZERO_STATE;
        float          fAmp;
        float          fAttack;
        float          fDecay;
        float          fDelta;
        float          fRelease;
        ENVELOPE_STATE fState;
        float          fSustain;

        void check_scheduled_attack_state() {
            if (fAmp > 0.0f) {
                if (USE_FADE_TO_ZERO_STATE) {
                    if (fState != ENVELOPE_STATE::PRE_ATTACK_FADE_TO_ZERO) {
                        fDelta = compute_delta_fraction(-fAmp, FADE_TO_ZERO_RATE_SEC);
                        setState(ENVELOPE_STATE::PRE_ATTACK_FADE_TO_ZERO);
                    }
                } else {
                    fDelta = compute_delta_fraction(1.0f, fAttack);
                    setState(ENVELOPE_STATE::ATTACK);
                }
            } else {
                fDelta = compute_delta_fraction(1.0f, fAttack);
                setState(ENVELOPE_STATE::ATTACK);
            }
        }

        void check_scheduled_release_state() {
            if (fState != ENVELOPE_STATE::RELEASE) {
                fDelta = compute_delta_fraction(-fAmp, fRelease);
                setState(ENVELOPE_STATE::RELEASE);
            }
        }

        float compute_delta_fraction(float pDelta, float pDuration) const {
            return pDuration > 0 ? (pDelta / static_cast<float>(fSampleRate)) / pDuration : pDelta;
        }

        void setState(ENVELOPE_STATE pState) {
            fState = pState;
        }

        void step() {
            switch (fState) {
                case ENVELOPE_STATE::IDLE:
                case ENVELOPE_STATE::SUSTAIN:
                    break;
                case ENVELOPE_STATE::ATTACK:
                    // increase amp to sustain_level in ATTACK sec
                    fAmp += fDelta;
                    if (fAmp >= 1.0f) {
                        fAmp   = 1.0f;
                        fDelta = compute_delta_fraction(-(1.0f - fSustain), fDecay);
                        setState(ENVELOPE_STATE::DECAY);
                    }
                    break;
                case ENVELOPE_STATE::DECAY:
                    // decrease amp to sustain_level in DECAY sec
                    fAmp += fDelta;
                    if (fAmp <= fSustain) {
                        fAmp = fSustain;
                        setState(ENVELOPE_STATE::SUSTAIN);
                    }
                    break;
                case ENVELOPE_STATE::RELEASE:
                    // decrease amp to 0.0 in RELEASE sec
                    fAmp += fDelta;
                    if (fAmp <= 0.0f) {
                        fAmp = 0.0f;
                        setState(ENVELOPE_STATE::IDLE);
                    }
                    break;
                case ENVELOPE_STATE::PRE_ATTACK_FADE_TO_ZERO:
                    fAmp += fDelta;
                    if (fAmp <= 0.0f) {
                        fAmp   = 0.0f;
                        fDelta = compute_delta_fraction(1.0f, fAttack);
                        setState(ENVELOPE_STATE::ATTACK);
                    }
                    break;
            }
        }
    };
    float ADSR::get_release() const {
        return fRelease;
    }
} // namespace klangwellen
