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

#ifdef KLANG_AUDIO_RATE
#warning "KLANG_AUDIO_RATE is already defined"
#else
#define KLANG_AUDIO_RATE              48000
#endif

#ifdef KLANG_AUDIO_BIT_DEPTH
#warning "KLANG_AUDIO_BIT_DEPTH is already defined"
#else
#define KLANG_AUDIO_BIT_DEPTH         16
#endif

#ifdef KLANG_SAMPLES_PER_AUDIO_BLOCK
#warning "KLANG_SAMPLES_PER_AUDIO_BLOCK is already defined"
#else
#define KLANG_SAMPLES_PER_AUDIO_BLOCK 128
#endif

#ifdef KLANG_OUTPUT_CHANNELS
#warning "KLANG_OUTPUT_CHANNELS is already defined"
#else
#define KLANG_OUTPUT_CHANNELS         2
#endif

#ifdef KLANG_INPUT_CHANNELS
#warning "KLANG_INPUT_CHANNELS is already defined"
#else
#define KLANG_INPUT_CHANNELS          2
#endif

#define KLANG_CHANNEL_LEFT            0
#define KLANG_CHANNEL_RIGHT           1
