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

#ifdef __cplusplus
extern "C" {
#endif

void KLST_BSP_serialdebug_init();
void KLST_BSP_serialdebug_printf(const char* format, ...);
void KLST_BSP_serialdebug_println(const char* format, ...); /* for debugging only */
void KLST_BSP_serialdebug_info();
void KLST_BSP_serialdebug_timestamp();

#ifdef __cplusplus
}
#endif

#ifdef __cplusplus

class SerialDebug {
public:
    void init();
    void info();
    void timestamp();
    void print(const char* format, ...);
    void println(const char* format, ...);
    void clear() { println("\033[H\033[J"); }

    void print_binary(uint32_t value, uint8_t number_of_bits) {
        for (int i = (number_of_bits - 1); i >= 0; i--) {
            print("%d", (value >> i) & 1);
        }
    }
};
#endif // __cplusplus
