/*
 * this example demonstrates how to generate a wavetable oscillator sound with ADSR envelope and reverb.
 */

#include "Umfeld.h"
#include "audio/AudioUtilities.h"
#include "audio/Wavetable.h"
#include "audio/FFT.h"

using namespace umfeld;

Wavetable*                           wavetable_oscillator;
std::vector<std::pair<float, float>> spectrum;

void settings() {
    size(1024, 768);
    audio(0, 2);
}

void setup() {
    fft_start(audio_buffer_size, audio_sample_rate);

    wavetable_oscillator = new Wavetable(1024, audio_sample_rate);
    wavetable_oscillator->set_waveform(WAVEFORM_SINE);
    wavetable_oscillator->set_frequency(220.0f);
    wavetable_oscillator->set_amplitude(0.7f);
}

void draw() {
    background(0.85f);
    noFill();
    stroke(1.0f, 0.25f, 0.35f);

    wavetable_oscillator->set_frequency(map(mouseX, 0, width, 20.0f, 800.0f));
    wavetable_oscillator->set_amplitude(map(mouseY, 0, height, 0.7f, 0.0f));

    if (spectrum.size() > 0) {
        const float bin_width = width / spectrum.size();
        noStroke();
        fill(0.0f, 0.5f, 1.0f);
        for (const auto& [freq, db]: spectrum) {
            const float xx = map(freq, 20.0f, 800.0f, 0.0f, width);
            const float h  = map(db, 0.0f, 50.0f, height, 0.0f);
            rect(xx - bin_width * 0.5f, h, bin_width, height - h);
        }
    }
}

void audioEvent() {
    float sample_buffer[audio_buffer_size];
    for (int i = 0; i < audio_buffer_size; i++) {
        sample_buffer[i] = wavetable_oscillator->process();
    }
    spectrum = fft_process(sample_buffer, 20.0f, 800.0f);
    merge_interleaved_stereo(sample_buffer, sample_buffer, audio_output_buffer, audio_buffer_size);
}

void shutdown() {
    delete wavetable_oscillator;
    fft_stop();
}