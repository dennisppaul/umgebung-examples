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

#include "KlangstromEnvironment.h"
#if ((KLST_ENV & KLST_ARCH_MASK) == KLST_ARCH_EMU)
// #if defined(KLST_PANDA_EMU) || defined(KLST_CATERPILLAR_EMU)

#include <stdint.h>

#ifdef __cplusplus
extern "C" {
#endif

void KLST_BSP_leds_init() {}

void KLST_BSP_leds_set(int id, float intensity) {}

uint8_t KLST_BSP_leds_total() {
#if defined(GENERIC_EMU)
    return 0;
#elif defined(KLST_CORE_EMU)
    return 3;
#elif defined(KLST_TINY_EMU)
    return 2;
#elif defined(KLST_SHEEP_EMU)
    return 16;
#elif defined(KLST_PANDA_EMU)
    return 2;
#elif defined(KLST_CATERPILLAR_EMU)
    return 2;
#else
    return 0;
#endif
}

#ifdef __cplusplus
}
#endif

#include "KlangstromEmulator.h"
#include "KlangstromLEDs.h"

class DrawableLEDs : public Drawable {
public:
    DrawableLEDs(LEDs* leds) : mLEDs(leds) {}

    void draw(PGraphics* g) override {
        g->pushMatrix();
        g->translate(25, 80);
        const float mRadius  = 40;
        const float y        = 0;
        const float x_offset = mRadius / 2;
        for (uint8_t i = 0; i < KLST_BSP_leds_total(); ++i) {
            const float mIntensity = mLEDs->get(i);
            const float x          = x_offset + i * mRadius * 1.5f;
            g->fill(1, mIntensity);
            g->noStroke();
            g->ellipse(x, y, mRadius, mRadius);
            g->noFill();
            g->stroke(1);
            g->ellipse(x, y, mRadius, mRadius);
        }
        g->popMatrix();
    }

private:
    LEDs* mLEDs;
};

void LEDs::KLST_BSP_init() {
    KlangstromEmulator::instance()->register_drawable(new DrawableLEDs(this));
}

#endif // defined((KLST_ENV & KLST_ARCH_MASK) == KLST_ARCH_EMU)
