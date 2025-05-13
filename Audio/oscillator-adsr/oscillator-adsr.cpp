/*
 * this example demonstrates how to generate a wavetable oscillator sound with ADSR envelope and reverb.
 */

#include "Umfeld.h"
#include "audio/AudioUtilities.h"
#include "audio/ADSR.h"
#include "audio/Wavetable.h"
#include "audio/Reverb.h"

using namespace umfeld;

ADSR*         adsr;
Reverb*       reverb;
Wavetable*    wavetable_oscillator;

void settings() {
    size(1024, 768);
    audio(0, 2);
}

void setup() {
    adsr                 = new ADSR(audio_sample_rate);
    reverb               = new Reverb();
    wavetable_oscillator = new Wavetable(1024, audio_sample_rate);

    wavetable_oscillator->set_waveform(WAVEFORM_TRIANGLE);
    wavetable_oscillator->set_frequency(220.0f);
    wavetable_oscillator->set_amplitude(0.7f);
}

void draw() {
    background(0.85f);
    noFill();
    stroke(1.0f, 0.25f, 0.35f);
    const float size = 50.0f;
    const float x    = width / 2.0f;
    const float y    = height / 2.0f;
    line(x - size, y - size, x + size, y + size);
    line(x - size, y + size, x + size, y - size);

    wavetable_oscillator->set_frequency(map(mouseX, 0, width, 20.0f, 880.0f));
}

void keyPressed() {
    if (key == '1') {
        if (adsr->is_idle()) {
            adsr->start();
        }
    }
}

void keyReleased() {
    if (key == '1') {
        adsr->stop();
    }
}

void audioEvent() {
    float sample_buffer[audio_buffer_size];
    for (int i = 0; i < audio_buffer_size; i++) {
        float osc        = wavetable_oscillator->process();
        osc              = adsr->process(osc);
        osc              = reverb->process(osc);
        sample_buffer[i] = osc;
    }
    merge_interleaved_stereo(sample_buffer, sample_buffer, audio_output_buffer, audio_buffer_size);
}

void shutdown() {
    delete adsr;
    delete reverb;
    delete wavetable_oscillator;
}