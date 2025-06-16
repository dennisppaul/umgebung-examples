#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "autotalent.h"

#ifndef KLANG_SAMPLES_PER_AUDIO_BLOCK
#error KLANG_SAMPLES_PER_AUDIO_BLOCK is not defined
#endif

#define BUF_SIZE KLANG_SAMPLES_PER_AUDIO_BLOCK
#define CONCERT_A 440.0
#define KEY_C 'c'
#define FIXED_PITCH 0.0
#define FIXED_PULL 0.2
#define CORR_STR 1.0
#define CORR_SMOOTH 0.0
#define PITCH_SHIFT 0.0
#define SCALE_ROTATE 0
#define LFO_DEPTH 0.0
#define LFO_RATE 5.0
#define LFO_SHAPE 0.0
#define LFO_SYMM 0.0
#define LFO_QUANT 0
#define FORM_CORR 0
#define FORM_WARP 0.0
#define MIX 0.4

void autotune_init() {
    float concert_a    = CONCERT_A;
    char  key_c        = KEY_C;
    float fixed_pitch  = FIXED_PITCH;
    float fixed_pull   = FIXED_PULL;
    float corr_str     = CORR_STR;
    float corr_smooth  = CORR_SMOOTH;
    float pitch_shift  = PITCH_SHIFT;
    int   scale_rotate = SCALE_ROTATE;
    float lfo_depth    = LFO_DEPTH;
    float lfo_rate     = LFO_RATE;
    float lfo_shape    = LFO_SHAPE;
    float lfo_symm     = LFO_SYMM;
    int   lfo_quant    = LFO_QUANT;
    int   form_corr    = FORM_CORR;
    float form_warp    = FORM_WARP;
    float mix          = MIX;

    /* set up autotalent */
    const uint32_t _SAMPLE_RATE_ = 48000;
    instantiateAutotalentInstance(_SAMPLE_RATE_);
    initializeAutotalent(&concert_a,
                         &key_c,
                         &fixed_pitch,
                         &fixed_pull,
                         &corr_str,
                         &corr_smooth,
                         &pitch_shift,
                         &scale_rotate,
                         &lfo_depth,
                         &lfo_rate,
                         &lfo_shape,
                         &lfo_symm,
                         &lfo_quant,
                         &form_corr,
                         &form_warp,
                         &mix);
}

void autotune_process(float* buf, int samples_read) {
    processSamples(buf, samples_read);
}

void autotune_free() {
    freeAutotalentInstance();
}