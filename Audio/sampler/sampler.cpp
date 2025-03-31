#include "Umgebung.h"
#include "audio/AudioFileReader.h"
#include "audio/Sampler.h"
#include "audio/LowPassFilter.h"
#include "audio/ADSR.h"
#include "audio/Wavetable.h"
#include "audio/Reverb.h"

using namespace umgebung;

AudioFileReader audio_file_reader;
Sampler*        sampler;
LowPassFilter*  filter;
ADSR*           adsr;
Reverb*         reverb;
Wavetable*      wavetable_oscillator;

void settings() {
    size(1024, 768);
    audio(0, 2);
}

Sampler* loadSample(const std::string& filename) {
    unsigned int channels;
    unsigned int sample_rate;
    drwav_uint64 length;
    float*       sample_buffer = audio_file_reader.load(filename, channels, sample_rate, length);
    console("loading sample: ");
    console("channels   : ", channels);
    console("sample_rate: ", sample_rate);
    console("length     : ", length);
    console("size       : ", channels * length);
    if (channels > 1) {
        warning("only mono samples are supported for sampler. using first channel only.");
        const auto single_buffer = new float[length];
        for (int i = 0; i < length; i++) {
            single_buffer[i] = sample_buffer[i * channels];
        }
        delete[] sample_buffer;
        sample_buffer = single_buffer;
    }
    const auto sampler = new Sampler(sample_buffer, length, sample_rate);
    return sampler;
}

void setup() {
    sampler = loadSample("../teilchen.wav");

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