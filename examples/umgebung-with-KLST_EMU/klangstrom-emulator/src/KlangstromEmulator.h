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

#include "Arduino.h"
#include <iostream>
#include <vector>

#include "Umgebung.h"
#include "Drawable.h"
#include "PeriodicalTask.h"

using namespace umgebung;

class KlangstromEmulator : public PApplet {
    PVector           mVector{16, 16};
    PShape            mShape;
    std::string       mFontPath = sketchPath();
    const std::string mFontName = "JetBrainsMonoNL-Light.ttf";
    PFont*            mFont     = nullptr;

public:
    ~KlangstromEmulator() { task.stop(); }
    static KlangstromEmulator* instance();
    void                       arguments(std::vector<std::string> args);
    void                       settings();
    void                       setup();
    void                       draw();
    void                       audioblock(float** input, float** output, int length);
    void                       keyPressed();
    std::string                get_emulator_name();
    void                       register_drawable(Drawable* drawable);
    void                       delay_loop(uint32_t ms);
    void                       set_emulator_speed(float loop_frequency_hz) { task.set_frequency(loop_frequency_hz); }
    float**                    get_audio_output_buffers() { return mOutputBuffers; }
    float**                    get_audio_input_buffers() { return mInputBuffers; }

private:
    static KlangstromEmulator* fInstance;
    float                      DEFAULT_FONT_SIZE = 24;
    PeriodicalTask             task;
    std::vector<Drawable*>     drawables;
    float**                    mOutputBuffers = nullptr;
    float**                    mInputBuffers  = nullptr;
};
