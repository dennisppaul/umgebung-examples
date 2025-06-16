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

#include <cmath>

namespace klangwellen {
    class ExponentialMovingAverage {
    public:
        ExponentialMovingAverage(float alpha) : fAlpha(alpha), fEMA(0.0f), fInitialized(false) {}

        float process(float sample) {
            float absSample = std::abs(sample);
            if (!fInitialized) {
                fEMA         = absSample;
                fInitialized = true;
            } else {
                fEMA = fAlpha * absSample + (1.0f - fAlpha) * fEMA;
            }
            return fEMA;
        }

        float get_current() const {
            return fEMA;
        }

    private:
        float fAlpha;
        float fEMA;
        bool  fInitialized;
    };
} // namespace klangwellen