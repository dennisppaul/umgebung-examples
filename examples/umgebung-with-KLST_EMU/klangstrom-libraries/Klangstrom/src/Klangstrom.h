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

#include "KlangstromEnvironment.h"
#include "KlangstromDefines.h"

#ifdef KLST_PANDA_STM32
#include "Klangstrom_BSP_KLST_PANDA_STM32.h"
#elif ((KLST_ENV & KLST_ARCH_MASK) == KLST_ARCH_EMU)
#include "Klangstrom_BSP_KLST_EMU.h"
#else
#warning
#endif

class Klangstrom {
public:
    Klangstrom();
    void init();
    void setup();
    void loop();

private:
    bool is_initialized;
};

/* --- Board Specific Implementations (BSP) --- */

#ifdef __cplusplus
extern "C" {
#endif

void KLST_BSP_init();
void KLST_BSP_setup();
void KLST_BSP_loop();

#ifdef __cplusplus
}
#endif
