// TODO move this to library

#include "Umgebung.h"

using namespace umgebung;

#define KLANG_SAMPLES_PER_AUDIO_BLOCK DEFAULT_FRAMES_PER_BUFFER
#define KLANG_SAMPLING_RATE           DEFAULT_AUDIO_SAMPLE_RATE

#include "ADSR.h"
#include "Reverb.h"
#include "Wavetable.h"

PFont*                 mFont{};
klangwellen::ADSR      fADSR;
klangwellen::Wavetable fWavetable{1024, klangwellen::KlangWellen::DEFAULT_SAMPLE_RATE};
klangwellen::Reverb    fReverb;

void settings() {
    size(1024, 768);
    audio(0, 2);
    antialiasing = 8;
}

void setup() {
    mFont = loadFont("../RobotoMono-Regular.ttf", 48);
    textFont(mFont);

    klangwellen::Wavetable::sawtooth(fWavetable.get_wavetable(), fWavetable.get_wavetable_size());
    fWavetable.set_frequency(55);
    textAlign(CENTER);
}

void draw() {
    background(0.85f);
    fill(0);
    noStroke();
    text("PRESS", mouseX, mouseY);
}

void audioEvent() {
    for (int i = 0; i < audio_buffer_size; i++) {
        float mSample = fWavetable.process();
        mSample       = fADSR.process(mSample);
        mSample       = fReverb.process(mSample);
        for (int j = 0; j < output_channels; ++j) {
            audio_output_buffer[i * output_channels + j] = mSample; // write sample to all channels
        }
    }
}

void mousePressed() {
    fADSR.start();
}

void mouseReleased() {
    fADSR.stop();
}