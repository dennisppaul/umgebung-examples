#include "Umgebung.h"

using namespace umgebung;

#include "AudioFileReader.h"
#include "AudioFileWriter.h"

AudioFileReader fAudioFileReader;
float           fSampleBuffer[DEFAULT_FRAMES_PER_BUFFER];

void settings() {
    size(1024, 768);
    // TODO this function creates a audio device with 1 input, 2 output channels
    //      and a sample rate of 48000 with default values of 32-bit float buffer
    //      and default buffer size ( usually 8192 on macOS )
    //      ```
    //      audio(1,2,48000);
    //      ```
    //      this does the same expect with an audio device info struct:
    //      ```
    //      AudioDeviceInfo audio_device_properties;
    //      audio_device_properties.input_channels  = 1;
    //      audio_device_properties.output_channels = 2;
    //      audio_device_properties.sample_rate     = 48000;
    //      audio(&audio_device_properties);
    //      ```

    audio_device_id = 23;
    input_channels  = 1;
    output_channels = 2;
    sample_rate     = 48000;
}

void write_WAV_file() {
    /* write sine wave with duartion approx 1sec + frequency 220Hz */
    AudioFileWriter fAudioFileWriter;
    fAudioFileWriter.open("../sine-220Hz.wav");
    float r = 0;
    for (int j = 0; j < DEFAULT_AUDIO_SAMPLE_RATE / DEFAULT_FRAMES_PER_BUFFER; ++j) {
        for (int i = 0; i < DEFAULT_FRAMES_PER_BUFFER; ++i) {
            fSampleBuffer[i] = sin(r);
            r += TWO_PI * 220.0f / DEFAULT_AUDIO_SAMPLE_RATE;
        }
        fAudioFileWriter.write(DEFAULT_FRAMES_PER_BUFFER, fSampleBuffer);
    }
    fAudioFileWriter.close();
}

void setup() {
    fAudioFileReader.open("../teilchen.wav");

    std::cout << "sample_rate: " << fAudioFileReader.sample_rate() << std::endl;
    std::cout << "channels   : " << fAudioFileReader.channels() << std::endl;
    std::cout << "length     : " << fAudioFileReader.length() << std::endl;

    write_WAV_file();

    console("SDL_GetBasePath: ", SDL_GetBasePath());
    console("sketchPath     : ", sketchPath());
}

void draw() {
    background(1);
    const int   mPadding  = 10;
    const float mProgress = (float) fAudioFileReader.current_position() / (float) fAudioFileReader.length();
    stroke(0.0f);
    noFill();
    rect(mPadding, height * 0.5 - mPadding, width - mPadding * 2, mPadding * 2);
    fill(0.0f);
    noStroke();
    rect(mPadding, height * 0.5 - mPadding, (width - mPadding * 2) * mProgress, mPadding * 2);
}

void audio(AudioDeviceInfo* audio_device) { // TODO this could be the callback
    audio_device->id = 0;
}

void audioblock(float** input, float** output, int length) {
    fAudioFileReader.read(length, fSampleBuffer, AudioFileReader::ReadStyle::LOOP);

    // for (int i = 0; i < length; i++) {
    //     const float mSample = fSampleBuffer[i];
    //     for (int j = 0; j < audio_output_channels; ++j) {
    //         output[j][i] = mSample;
    //     }
    // }
}

void finish() {
    fAudioFileReader.close();
}


void audioEvent(const AudioDeviceInfo& device) {
    // console("audioEvent(AudioDeviceInfo* device)");
    // console("audioEvent for: ", device.id);
}

void audioEvent() {
    // console("audioEvent()");
}
