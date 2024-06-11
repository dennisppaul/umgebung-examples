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

#include "KlangstromEmulator.h"
#include <iostream>
#include <vector>

#ifdef __cplusplus
extern "C" {
#endif

#ifndef SERIAL_DEBUG_BUFFER_SIZE
#define SERIAL_DEBUG_BUFFER_SIZE 128
#endif

void KLST_BSP_serialdebug_init() {
    // KlangstromEmulator::instance()->foobar();
}

void KLST_BSP_serialdebug_printf(const char* format, ...) {
    va_list args;
    va_start(args, format);
    std::vector<char> buffer(SERIAL_DEBUG_BUFFER_SIZE);
    vsnprintf(buffer.data(), buffer.size(), format, args);
    va_end(args);
    std::cout
        << buffer.data()
        << std::flush;
}

void KLST_BSP_serialdebug_println(const char* format, ...) {
    va_list args;
    va_start(args, format);
    std::vector<char> buffer(SERIAL_DEBUG_BUFFER_SIZE);
    vsnprintf(buffer.data(), buffer.size(), format, args);
    va_end(args);
    std::cout
        << buffer.data()
        << std::endl
        << std::flush;
}

void KLST_BSP_serialdebug_info() {
    std::string mName = KlangstromEmulator::instance()->get_emulator_name();
    KLST_BSP_serialdebug_printf("\r\n---------------------------------------------------------\r\n\r\n");
    KLST_BSP_serialdebug_printf("KLST_EMU as %s (%s)\r\n", mName.c_str(), __TIME__);
    KLST_BSP_serialdebug_printf("\r\n---------------------------------------------------------\r\n\r\n");
}

void KLST_BSP_serialdebug_timestamp() {}

#ifdef __cplusplus
}
#endif

#endif // defined((KLST_ENV & KLST_ARCH_MASK) == KLST_ARCH_EMU)
