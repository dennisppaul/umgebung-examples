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

#pragma once

#include <stdint.h>
#include <cmath>

#ifdef __cplusplus
extern "C" {
#endif

void    KLST_BSP_leds_init();
void    KLST_BSP_leds_set(int id, float intensity);
uint8_t KLST_BSP_leds_total();

#ifdef __cplusplus
}
#endif

#ifdef __cplusplus

class LEDs {
public:
    static const int8_t ALL = -1;

    LEDs() : fTotal(KLST_BSP_leds_total()) {
        fIntensities = new float[fTotal]{0};
    }

    virtual ~LEDs() {
        delete[] fIntensities;
    }

    void KLST_BSP_init();

    void init() {
        KLST_BSP_leds_init();
        KLST_BSP_init();
    }

    void set(const int id, float intensity) {
        intensity = fmax(fmin(intensity, 1), 0);
        if (id < 0) {
            for (uint8_t i = 0; i < fTotal; ++i) {
                fIntensities[i] = intensity;
                KLST_BSP_leds_set(i, intensity);
            }
        } else if (id >= 0 && id < fTotal) {
            fIntensities[id] = intensity;
            KLST_BSP_leds_set(id, intensity);
        }
    }

    float get(const int id) {
        if (id >= 0 && id < fTotal) {
            return fIntensities[id];
        }
        return 0;
    }

    void on(const int id) {
        set(id, 1);
    }

    void off(const int id) {
        set(id, 0);
    }

    void toggle(const int id) {
        if (id < 0) {
            for (uint8_t i = 0; i < fTotal; ++i) {
                if (fIntensities[i] > 0) {
                    fIntensities[i] = 0;
                } else {
                    fIntensities[i] = 1;
                }
                KLST_BSP_leds_set(i, fIntensities[i]);
            }
        } else if (id >= 0 && id < fTotal) {
            if (fIntensities[id] > 0) {
                fIntensities[id] = 0;
            } else {
                fIntensities[id] = 1;
            }
            KLST_BSP_leds_set(id, fIntensities[id]);
        }
    }

private:
    const uint8_t fTotal;
    float*        fIntensities;
};
#endif // __cplusplus
