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

#include <vector>
#include <cmath>

namespace klangwellen {
    class RootMeanSquare {
    public:
        RootMeanSquare(size_t windowSize = 16) : fWindowSize(windowSize), fSum(0.0f), fBufferIndex(0) {
            fBuffer.resize(windowSize, 0.0f);
            fRMS = 0.0f;
        }

        float process(float sample) {
            // Remove the oldest sample's contribution
            fSum -= fBuffer[fBufferIndex] * fBuffer[fBufferIndex];

            // Add the new sample to the buffer
            fBuffer[fBufferIndex] = sample;
            fBufferIndex          = (fBufferIndex + 1) % fWindowSize;

            // Add the new sample's contribution
            fSum += sample * sample;

            // Calculate the RMS value
            if (fSum > 0) {
                fRMS = std::sqrt(fSum / fWindowSize);
            } else {
                fRMS = 0.0f;
            }
            return fRMS;
        }

        float get_current() const {
            return fRMS;
        }

    private:
        const size_t       fWindowSize;
        std::vector<float> fBuffer;
        size_t             fBufferIndex;
        float              fSum;
        float              fRMS;
    };
} // namespace klangwellen