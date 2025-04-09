#define KLANG_SAMPLES_PER_AUDIO_BLOCK 256
#define KLANG_SAMPLE_RATE 44100

#include <stdint.h>
#include <stdio.h>

#include <iostream>

#include "ADSR.h"
#include "KlangWellen.h"
#include "Reverb.h"
#include "Wavetable.h"
#include "utilities.h"

klangwellen::ADSR      fADSR;
klangwellen::Wavetable fWavetable(1024, klangwellen::KlangWellen::DEFAULT_SAMPLE_RATE);
klangwellen::Reverb    fReverb;

static const int mNumSamples = klangwellen::KlangWellen::DEFAULT_SAMPLE_RATE * 4; // 4 seconds
float            mAudioBuffer[mNumSamples];

int main() {
    std::cout << "SAMPLE_RATE .... : " << klangwellen::KlangWellen::DEFAULT_SAMPLE_RATE << std::endl;
    std::cout << "NUM OF SAMPLES ... : " << mNumSamples << std::endl;

    klangwellen::Wavetable::sawtooth(fWavetable.get_wavetable(), fWavetable.get_wavetable_size(), 5);
    fWavetable.set_frequency(27.5);

    bool mToggleADSR = false;
    fADSR.start();
    for (size_t i = 0; i < mNumSamples; i++) {
        if (i % (klangwellen::KlangWellen::DEFAULT_SAMPLE_RATE / 4) == 0) {
            mToggleADSR = !mToggleADSR;
            if (mToggleADSR) {
                fADSR.start();
                fWavetable.set_frequency(fWavetable.get_frequency() * 1.5);
            } else {
                fADSR.stop();
            }
        }
        float mSample   = fWavetable.process();
        mSample         = fADSR.process(mSample);
        mSample         = fReverb.process(mSample);
        mAudioBuffer[i] = mSample;
    }
    std::cout << std::endl;

    write_WAV_file("klangwellen-to-wav.wav",
                   klangwellen::KlangWellen::DEFAULT_SAMPLE_RATE,
                   1,
                   mAudioBuffer,
                   mNumSamples);

    return 0;
}
