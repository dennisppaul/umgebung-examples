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
#include <math.h>
#include <assert.h>

#include <algorithm>
#include <random>

#include "KlangWellen.h"

namespace klangwellen {
    /**
    * supplies a collection of noise generators: white, gaussian, pink and simplex/perlin noise.
    */

    class WhiteNoise {
    public:
        float process() {
            return 2.0f * static_cast<float>(rand()) / static_cast<float>(RAND_MAX) - 1.0;
        }
    };

    class WhiteNoiseFast {
    public:
        float process() {
            return KlangWellen::random() * 2.0 - 1.0;
        }
    };

    class PinkNoise {
    public:
        PinkNoise() {
            srand(time(nullptr)); // initialize random generator
            clear();
        }

        void clear() {
            for (size_t i = 0; i < PINK_NOISE_NUM_STAGES; i++)
                state[i] = 0.0;
        }

        float process() {
            static constexpr float RMI2   = 2.0 / static_cast<float>(RAND_MAX); // + 1.0; // change for range [0,1)
            static constexpr float offset = A[0] + A[1] + A[2];

            // unrolled loop
            float temp = static_cast<float>(rand());
            state[0]   = P[0] * (state[0] - temp) + temp;
            temp       = static_cast<float>(rand());
            state[1]   = P[1] * (state[1] - temp) + temp;
            temp       = static_cast<float>(rand());
            state[2]   = P[2] * (state[2] - temp) + temp;
            return ((A[0] * state[0] + A[1] * state[1] + A[2] * state[2]) * RMI2 - offset) * fScale;
        }

    private:
        static constexpr uint8_t PINK_NOISE_NUM_STAGES    = 3;
        static constexpr float   A[PINK_NOISE_NUM_STAGES] = {0.02109238, 0.07113478, 0.68873558};
        static constexpr float   P[PINK_NOISE_NUM_STAGES] = {0.3190, 0.7756, 0.9613};
        float                    state[PINK_NOISE_NUM_STAGES];
        float                    fScale = 7.0f;
    };

    class SimplexNoise {
        // from https://github.com/charlesangus/FastNoise
        /*
            MIT License

            Copyright (c) 2017 Jordan Peck

            Permission is hereby granted, free of charge, to any person obtaining a copy
            of this software and associated documentation files (the "Software"), to deal
            in the Software without restriction, including without limitation the rights
            to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
            copies of the Software, and to permit persons to whom the Software is
            furnished to do so, subject to the following conditions:

            The above copyright notice and this permission notice shall be included in all
            copies or substantial portions of the Software.

            THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
            IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
            FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
            AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
            LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
            OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
            SOFTWARE.
         */
    public:
        explicit SimplexNoise(uint32_t seed = 1337) {
            set_seed(seed);
            fSimplexStepSize = 0.01;
            fSimplexStep     = 0.0;
        }

        float get(const float x) const {
            return get(0, x, 0, 0);
        }

        float get(const unsigned char offset, float x, float y, float z) const {
            float t = (x + y + z) * F3;
            int   i = FastFloor(x + t);
            int   j = FastFloor(y + t);
            int   k = FastFloor(z + t);

            t              = (i + j + k) * G3;
            const float X0 = i - t;
            const float Y0 = j - t;
            const float Z0 = k - t;

            const float x0 = x - X0;
            const float y0 = y - Y0;
            const float z0 = z - Z0;

            int i1, j1, k1;
            int i2, j2, k2;

            if (x0 >= y0) {
                if (y0 >= z0) {
                    i1 = 1;
                    j1 = 0;
                    k1 = 0;
                    i2 = 1;
                    j2 = 1;
                    k2 = 0;
                } else if (x0 >= z0) {
                    i1 = 1;
                    j1 = 0;
                    k1 = 0;
                    i2 = 1;
                    j2 = 0;
                    k2 = 1;
                } else // x0 < z0
                {
                    i1 = 0;
                    j1 = 0;
                    k1 = 1;
                    i2 = 1;
                    j2 = 0;
                    k2 = 1;
                }
            } else // x0 < y0
            {
                if (y0 < z0) {
                    i1 = 0;
                    j1 = 0;
                    k1 = 1;
                    i2 = 0;
                    j2 = 1;
                    k2 = 1;
                } else if (x0 < z0) {
                    i1 = 0;
                    j1 = 1;
                    k1 = 0;
                    i2 = 0;
                    j2 = 1;
                    k2 = 1;
                } else // x0 >= z0
                {
                    i1 = 0;
                    j1 = 1;
                    k1 = 0;
                    i2 = 1;
                    j2 = 1;
                    k2 = 0;
                }
            }

            const float x1 = x0 - i1 + G3;
            const float y1 = y0 - j1 + G3;
            const float z1 = z0 - k1 + G3;
            const float x2 = x0 - i2 + 2 * G3;
            const float y2 = y0 - j2 + 2 * G3;
            const float z2 = z0 - k2 + 2 * G3;
            const float x3 = x0 - 1 + 3 * G3;
            const float y3 = y0 - 1 + 3 * G3;
            const float z3 = z0 - 1 + 3 * G3;

            float n0, n1, n2, n3;

            t = static_cast<float>(0.6) - x0 * x0 - y0 * y0 - z0 * z0;
            if (t < 0) n0 = 0;
            else {
                t *= t;
                n0 = t * t * GradCoord3D(offset, i, j, k, x0, y0, z0);
            }

            t = static_cast<float>(0.6) - x1 * x1 - y1 * y1 - z1 * z1;
            if (t < 0) n1 = 0;
            else {
                t *= t;
                n1 = t * t * GradCoord3D(offset, i + i1, j + j1, k + k1, x1, y1, z1);
            }

            t = static_cast<float>(0.6) - x2 * x2 - y2 * y2 - z2 * z2;
            if (t < 0) n2 = 0;
            else {
                t *= t;
                n2 = t * t * GradCoord3D(offset, i + i2, j + j2, k + k2, x2, y2, z2);
            }

            t = static_cast<float>(0.6) - x3 * x3 - y3 * y3 - z3 * z3;
            if (t < 0) n3 = 0;
            else {
                t *= t;
                n3 = t * t * GradCoord3D(offset, i + 1, j + 1, k + 1, x3, y3, z3);
            }

            return 32 * (n0 + n1 + n2 + n3);
        }

        void set_seed(const int seed) {
            m_seed = seed;

            std::mt19937_64 gen(seed);

            for (int i = 0; i < 256; i++)
                m_perm[i] = i;

            for (int j = 0; j < 256; j++) {
                const int rng = static_cast<int>(gen() % (256 - j));
                const int k   = rng + j;
                const int l   = m_perm[j];
                m_perm[j] = m_perm[j + 256] = m_perm[k];
                m_perm[k]                   = l;
                m_perm12[j] = m_perm12[j + 256] = m_perm[j] % 12;
            }
        }

        float get_step() const {
            return fSimplexStepSize;
        }

        void set_step(const float step) {
            fSimplexStepSize = step;
        }

        float process() {
            fSimplexStep += fSimplexStepSize;
            constexpr float STEP_LIMIT = 1 << 16;
            if (fSimplexStep > STEP_LIMIT) {
                fSimplexStep -= STEP_LIMIT;
            } else if (fSimplexStep < -STEP_LIMIT) {
                fSimplexStep += STEP_LIMIT;
            }
            return get(fSimplexStep);
            // return mSimplexNoise.noise(fSimplexStep, 0.0);
        }

    private:
        static constexpr float F3 = 1 / static_cast<float>(3);
        static constexpr float G3 = 1 / static_cast<float>(6);

        const float GRAD_X[12] =
            {
                1, -1, 1, -1,
                1, -1, 1, -1,
                0, 0, 0, 0};
        const float GRAD_Y[12] =
            {
                1, 1, -1, -1,
                0, 0, 0, 0,
                1, -1, 1, -1};
        const float GRAD_Z[12] =
            {
                0, 0, 0, 0,
                1, 1, -1, -1,
                1, 1, -1, -1};

        unsigned char m_perm[512]   = {};
        unsigned char m_perm12[512] = {};
        int           m_seed        = 1337;
        float         fSimplexStep;
        float         fSimplexStepSize;

        unsigned char Index3D_12(const unsigned char offset, int x, int y, int z) const {
            return m_perm12[(x & 0xff) + m_perm[(y & 0xff) + m_perm[(z & 0xff) + offset]]];
        }

        float GradCoord3D(const unsigned char offset,
                          const int x, const int y, const int z,
                          const float xd, const float yd, const float zd) const {
            const unsigned char lutPos = Index3D_12(offset, x, y, z);
            return xd * GRAD_X[lutPos] + yd * GRAD_Y[lutPos] + zd * GRAD_Z[lutPos];
        }

        static int FastFloor(const float f) { return f >= 0 ? static_cast<int>(f) : static_cast<int>(f) - 1; }
    };

    class GaussianWhiteNoise {
    public:
        GaussianWhiteNoise() : mPN_b{}, mPN_state{}, mWN_y2(0) {}

        float process() {
            // from [Gaussian White Noise](https://www.musicdsp.org/en/latest/Synthesis/109-gaussian-white-noise.html)
            float y1;

            if (mWN_pass) {
                y1 = mWN_y2;
            } else {
                float x1;
                float x2;
                float w;
                do {
                    x1 = 2.0 * KlangWellen::random() - 1.0;
                    x2 = 2.0 * KlangWellen::random() - 1.0;
                    w  = x1 * x1 + x2 * x2;
                } while (w >= 1.0);

                w      = static_cast<float>(sqrt(-2.0 * log(w) / w));
                y1     = x1 * w;
                mWN_y2 = x2 * w;
            }
            mWN_pass               = !mWN_pass;
            constexpr float mScale = 0.5;
            return y1 * mScale;
        }

    private:
        float mPN_b[7];
        float mPN_state[3];
        bool  mWN_pass = false;
        float mWN_y2;
    };

    class Noise {
        // @TODO( need(!) to understand noise in general and the implementations in detail ( i.e why do the gaussian and
        //      pink noises sound so different ) better and maybe add some more noise types ( i.e brown, grey ))

    public:
        Noise() {
            fAmplitude = 1.0f;
            fType      = KlangWellen::NOISE_WHITE;
        }

        float get_amplitude() const {
            return fAmplitude;
        }

        void set_amplitude(const float amplitude) {
            fAmplitude = amplitude;
        }

        int get_type() const {
            return fType;
        }

        void set_type(const int type) {
            fType = type;
        }

        float process() {
            float mSignal;
            switch (fType) {
                case KlangWellen::NOISE_WHITE_FAST:
                    mSignal = getWhiteNoise();
                    break;
                case KlangWellen::NOISE_GAUSSIAN_WHITE_FAST:
                    mSignal = getGaussianWhiteNoiseFast();
                    break;
                case KlangWellen::NOISE_GAUSSIAN_WHITE:
                    mSignal = mGaussianWhiteNoise.process();
                    break;
                case KlangWellen::NOISE_PINK:
                    mSignal = mPinkNoise.process();
                    break;
                case KlangWellen::NOISE_SIMPLEX:
                    mSignal = mSimplexNoise.process();
                    break;
                case KlangWellen::NOISE_WHITE:
                default:
                    mSignal = getWhiteNoise();
                    break;
            }
            return mSignal * fAmplitude;
        }

        static float getGaussianWhiteNoiseFast() {
            // from [Gaussian White Noise](https://www.musicdsp.org/en/latest/Synthesis/113-gaussian-white-noise.html)
            const float R1 = static_cast<float>(rand()) / static_cast<float>(RAND_MAX);
            const float R2 = static_cast<float>(rand()) / static_cast<float>(RAND_MAX);
            const float X  = sqrt(-2.0f * log(R1)) * cos(2.0f * PI * R2);
            return X;
        }

        static float getWhiteNoise() {
            return 2.0f * static_cast<float>(rand()) / static_cast<float>(RAND_MAX) - 1.0;
        }

        static float getWhiteNoiseFast() {
            return KlangWellen::random() * 2.0 - 1.0;
        }

    private:
        float              fAmplitude;
        int                fType;
        SimplexNoise       mSimplexNoise;
        PinkNoise          mPinkNoise;
        GaussianWhiteNoise mGaussianWhiteNoise;
    };
} // namespace klangwellen