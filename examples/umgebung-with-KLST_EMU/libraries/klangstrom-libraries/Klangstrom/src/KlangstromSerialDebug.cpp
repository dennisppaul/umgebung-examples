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

#include "KlangstromSerialDebug.h"

#include <string.h>
#include <stdio.h>
#include <stdarg.h>

#define SERIAL_DEBUG_BUFFER_SIZE 128

void SerialDebug::init() {
    KLST_BSP_serialdebug_init();
}

void SerialDebug::info() {
    KLST_BSP_serialdebug_info();
}

void SerialDebug::timestamp() {
    KLST_BSP_serialdebug_timestamp();
}

void SerialDebug::print(const char* format, ...) {
    char    buffer[SERIAL_DEBUG_BUFFER_SIZE];
    va_list args;
    va_start(args, format);
    vsnprintf(buffer, SERIAL_DEBUG_BUFFER_SIZE, format, args);
    va_end(args);
    KLST_BSP_serialdebug_printf("%s", buffer);
}

void SerialDebug::println(const char* format, ...) {
    char    buffer[SERIAL_DEBUG_BUFFER_SIZE];
    va_list args;
    va_start(args, format);
    vsnprintf(buffer, SERIAL_DEBUG_BUFFER_SIZE, format, args);
    va_end(args);
    KLST_BSP_serialdebug_printf("%s\r\n", buffer);
}
