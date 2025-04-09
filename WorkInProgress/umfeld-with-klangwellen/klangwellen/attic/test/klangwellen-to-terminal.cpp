#include <stdint.h>
#include <stdio.h>

#include <iostream>

#include "KlangWellen.h"
#include "Wavetable.h"

klangwellen::Wavetable fWavetable(1024, klangwellen::KlangWellen::DEFAULT_SAMPLE_RATE);
const float            mNumSamples = 512;

int main() {
    std::cout << "SAMPLE_RATE ...... : " << klangwellen::KlangWellen::DEFAULT_SAMPLE_RATE << std::endl;

    klangwellen::Wavetable::sine(fWavetable.get_wavetable(), fWavetable.get_wavetable_size());

    std::cout << "RENDERING SAMPLE ... :" << std::endl;
    for (size_t i = 0; i < mNumSamples; i++) {
        std::cout << fWavetable.process();
        if (i < mNumSamples - 1) {
            std::cout << ", ";
        }
    }
    std::cout << std::endl;

    return 0;
}
