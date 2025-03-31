#include "Umgebung.h"
#include "audio/AudioUtilities.h"
#include "audio/Sampler.h"
#include "audio/LowPassFilter.h"
#include "audio/ADSR.h"
#include "audio/Wavetable.h"
#include "audio/Reverb.h"

using namespace umgebung;

Sampler*       sampler;
LowPassFilter* filter;
ADSR*          adsr;
Reverb*        reverb;
Wavetable*     wavetable_oscillator;

void settings() {
    size(1024, 768);
    audio(0, 2);
    subsystem_audio = umgebung_create_subsystem_audio_sdl();
}

void setup() {
    sampler = loadSample("../teilchen.wav");
    sampler->resample(48000, 48000 * 2);

    const float sample_rate = sampler->get_sample_rate();
    filter                  = new LowPassFilter(sample_rate);
    adsr                    = new ADSR(sample_rate);
    reverb                  = new Reverb();
    wavetable_oscillator    = new Wavetable(1024, sample_rate);

    wavetable_oscillator->set_waveform(WAVEFORM_TRIANGLE);
    wavetable_oscillator->set_frequency(220.0f);
    wavetable_oscillator->set_amplitude(0.7f);

    sampler->set_looping();
    sampler->play();
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
    filter->set_frequency(map(mouseX, 0, width, 20.0f, 8000.0f));
    filter->set_resonance(map(mouseY, 0, height, 0.1f, 0.9f));
}

void keyPressed() {
    if (key == '1') {
        if (adsr->is_idle()) {
            adsr->start();
        }
    }
    if (key == '2') {
        sampler->rewind();
        sampler->play();
    }
}

void keyReleased() {
    if (key == '1') {
        adsr->stop();
    }
    if (key == '3') {
        sampler->stop();
    }
}

void audioEvent() {
    float sample_buffer[audio_buffer_size];
    for (int i = 0; i < audio_buffer_size; i++) {
        float osc        = wavetable_oscillator->process();
        osc              = adsr->process(osc);
        osc              = reverb->process(osc);
        float sample     = sampler->process();
        sample           = filter->process(sample);
        sample_buffer[i] = sample + osc;
    }
    merge_interleaved_stereo(sample_buffer, sample_buffer, audio_output_buffer, audio_buffer_size);
}

void shutdown() {
    delete[] sampler->get_buffer();
    delete sampler;
    delete filter;
    delete adsr;
    delete reverb;
    delete wavetable_oscillator;
}