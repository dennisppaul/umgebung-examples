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
* - [ ] float process()
* - [x] float process(float)
* - [ ] void process(AudioSignal&)
* - [ ] void process(float*, uint32_t)
* - [ ] void process(float*, float*, uint32_t)
*/

#pragma once

#include <iostream>
#include <cmath>

#include "KlangWellen.h"

namespace klangwellen {
    class EnvelopeFollower {
    public:
        EnvelopeFollower(float attackTime  = 0.01,
                         float releaseTime = 0.1,
                         float sampleRate  = KlangWellen::DEFAULT_SAMPLE_RATE)
            : fAttackTime(attackTime), fReleaseTime(releaseTime), fSampleRate(sampleRate) {
            setAttackTime(attackTime);
            setReleaseTime(releaseTime);
            fEnvelope = 0.0f;
        }

        void setAttackTime(float attackTime) {
            this->fAttackTime = attackTime;
            fAttackCoef       = std::exp(-1.0f / (attackTime * fSampleRate));
        }

        void setReleaseTime(float release_time) {
            this->fReleaseTime = release_time;
            fReleaseCoef       = std::exp(-1.0f / (release_time * fSampleRate));
        }

        float process(float input) {
            float absInput = std::fabs(input);
            if (absInput > fEnvelope) {
                // Attack
                fEnvelope = fAttackCoef * (fEnvelope - absInput) + absInput;
            } else {
                // Release
                fEnvelope = fReleaseCoef * (fEnvelope - absInput) + absInput;
            }
            return fEnvelope;
        }

        float get_current() const {
            return fEnvelope;
        }

    private:
        const float fSampleRate;
        float       fAttackTime;
        float       fReleaseTime;
        float       fAttackCoef;
        float       fReleaseCoef;
        float       fEnvelope;
    };
} // namespace klangwellen