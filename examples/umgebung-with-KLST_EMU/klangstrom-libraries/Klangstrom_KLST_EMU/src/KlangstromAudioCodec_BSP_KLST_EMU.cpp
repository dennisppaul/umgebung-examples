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

#include <stdint.h>

#include "KlangstromAudioCodec.h"

#ifdef __cplusplus
extern "C" {
#endif

uint8_t KLST_BSP_audiocodec_init() {
    return 0;
}

#ifdef __cplusplus
}
#endif

#include "KlangstromEmulator.h"
#include "KlangstromAudioCodec.h"

class DrawableAudioCodec : public Drawable {
public:
    DrawableAudioCodec(AudioCodec* audiocodec) : mAudioCodec(audiocodec) {}

    void draw(PGraphics* g) override {
        (void) mAudioCodec;
        g->stroke(1);
        g->noFill();
        g->pushMatrix();
        g->translate(20, 200);
        const int   mStrafe  = 16;
        const float mHeight  = 150;
        const float mWidth   = 512 - 20;
        float**     mBuffers = KlangstromEmulator::instance()->get_audio_output_buffers();
        for (int i = 0; i < audio_output_channels; i++) {
            g->stroke(1, 0.5);
            g->rect(0, i * mHeight - mHeight * 0.5, mWidth, mHeight);
            g->line(0, i * mHeight, mWidth, i * mHeight);
            g->stroke(1);
            for (int j = mStrafe; j < DEFAULT_FRAMES_PER_BUFFER; j += mStrafe) {
                float       mSample0 = mBuffers[i][j - mStrafe] * 0.5;
                float       mSample1 = mBuffers[i][j] * 0.5;
                const float x0       = mWidth * (float) (j - mStrafe) / DEFAULT_FRAMES_PER_BUFFER;
                const float y0       = i * mHeight + mSample0 * mHeight;
                const float x1       = mWidth * (float) j / DEFAULT_FRAMES_PER_BUFFER;
                const float y1       = i * mHeight + mSample1 * mHeight;
                g->line(x0, y0, x1, y1);
                g->line(x0, y0, x0, i * mHeight);
            }
        }
        g->translate(0, mHeight * 2 + 10);
        for (int i = 0; i < audio_input_channels; i++) {
            g->stroke(1, 0.5);
            g->rect(0, i * mHeight - mHeight * 0.5, mWidth, mHeight);
            g->line(0, i * mHeight, mWidth, i * mHeight);
            g->stroke(1);
            mBuffers = KlangstromEmulator::instance()->get_audio_input_buffers();
            for (int j = mStrafe; j < DEFAULT_FRAMES_PER_BUFFER; j += mStrafe) {
                float       mSample0 = mBuffers[i][j - mStrafe] * 0.5;
                float       mSample1 = mBuffers[i][j] * 0.5;
                const float x0       = mWidth * (float) (j - mStrafe) / DEFAULT_FRAMES_PER_BUFFER;
                const float y0       = i * mHeight + mSample0 * mHeight;
                const float x1       = mWidth * (float) j / DEFAULT_FRAMES_PER_BUFFER;
                const float y1       = i * mHeight + mSample1 * mHeight;
                g->line(x0, y0, x1, y1);
                g->line(x0, y0, x0, i * mHeight);
            }
        }
        g->popMatrix();
    }

private:
    AudioCodec* mAudioCodec;
};

void AudioCodec::KLST_BSP_init() {
    KlangstromEmulator::instance()->register_drawable(new DrawableAudioCodec(this));
}


#endif // defined((KLST_ENV & KLST_ARCH_MASK) == KLST_ARCH_EMU)
