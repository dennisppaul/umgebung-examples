#include "Umgebung.h"

using namespace umgebung;

#include "AudioFileReader.h"
#include "AudioFileWriter.h"

AudioFileReader audio_file_reader;
PAudio*         second_audio_device;
bool            toggle_pause = false;

void settings() {
    size(1024, 768);
    // TODO this function creates a audio device with 1 input, 2 output channels
    //      and a sample rate of 48000 with default values of 32-bit float buffer
    //      and default buffer size ( usually 8192 on macOS )
    //      ```
    //      audio(1,2,48000);
    //      ```
    //      this does the same except with an audio device info struct:
    //      ```
    //      AudioDeviceInfo audio_device_properties;
    //      audio_device_properties.input_channels  = 1;
    //      audio_device_properties.output_channels = 2;
    //      audio_device_properties.sample_rate     = 48000;
    //      audio(&audio_device_properties);
    //      ```
    audio(0, 2, 48000, 1024);
    // input_channels  = 0;
    // output_channels = 2;
    // sample_rate     = 48000;
}

void write_WAV_file() {
    float wav_sample_buffer[audio_buffer_size];
    /* write sine wave with duartion approx 1sec + frequency 220Hz */
    AudioFileWriter fAudioFileWriter;
    fAudioFileWriter.open("../sine-220Hz.wav");
    float r = 0;
    for (int j = 0; j < sample_rate / audio_buffer_size; ++j) {
        for (int i = 0; i < audio_buffer_size; ++i) {
            wav_sample_buffer[i] = sin(r);
            r += TWO_PI * 220.0f / sample_rate;
        }
        fAudioFileWriter.write(audio_buffer_size, wav_sample_buffer);
    }
    fAudioFileWriter.close();
}

void setup() {
    audio_file_reader.open("../teilchen.wav");

    console("sample_rate: ", audio_file_reader.sample_rate());
    console("channels   : ", audio_file_reader.channels());
    console("length     : ", audio_file_reader.length());

    write_WAV_file();

    console("SDL_GetBasePath: ", SDL_GetBasePath());
    console("sketchPath     : ", sketchPath());

    // TODO test this carefully
    AudioDeviceInfo info;
    info.output_channels = 2;
    second_audio_device  = createAudio(&info);
    console("created second audio devide with id: ", second_audio_device->id);
}

void draw() {
    background(1);
    const int   mPadding  = 10;
    const float mProgress = (float) audio_file_reader.current_position() / (float) audio_file_reader.length();
    stroke(0.0f);
    noFill();
    rect(mPadding, height * 0.5 - mPadding, width - mPadding * 2, mPadding * 2);
    fill(0.0f);
    noStroke();
    rect(mPadding, height * 0.5 - mPadding, (width - mPadding * 2) * mProgress, mPadding * 2);
}

void finish() {
    audio_file_reader.close();
}

void keyPressed() {
    if (key == ' ') {
        toggle_pause = !toggle_pause;
        if (toggle_pause) {
            audio_start();
            audio_start(second_audio_device);
        } else {
            audio_stop();
            audio_stop(second_audio_device);
        }
    }
}

void merge_interleaved(const float* left,
                       const float* right,
                       float*       interleaved,
                       const size_t frames) {
    for (size_t i = 0; i < frames; ++i) {
        interleaved[i * 2]     = left[i];
        interleaved[i * 2 + 1] = right[i];
    }
}

void read_wav(float* samples, const size_t frames) {
    audio_file_reader.read(frames, samples, AudioFileReader::ReadStyle::LOOP);
}

void audioEvent(const AudioDeviceInfo& device) {
    if (second_audio_device == nullptr) {
        return;
    }
    if (&device == second_audio_device) {
        float left[device.buffer_size];
        float right[device.buffer_size];
        for (int i = 0; i < device.buffer_size; i++) {
            left[i]  = random(-0.1, 0.1);
            right[i] = random(-0.1, 0.1);
        }
        merge_interleaved(left, right, device.output_buffer, device.buffer_size);
    }
    if (&device == a) {
        float wav_sample_buffer[audio_buffer_size];
        read_wav(wav_sample_buffer, audio_buffer_size);

        float left[audio_buffer_size];
        float right[audio_buffer_size];
        for (int i = 0; i < audio_buffer_size; i++) {
            const float sample = wav_sample_buffer[i];
            left[i]            = sample;
            right[i]           = sample;
        }
        merge_interleaved(left, right, audio_output_buffer, audio_buffer_size);
    }
}

// void audioEvent() {
//     float wav_sample_buffer[audio_buffer_size];
//     read_wav(wav_sample_buffer, audio_buffer_size);
//
//     float left[audio_buffer_size];
//     float right[audio_buffer_size];
//     for (int i = 0; i < audio_buffer_size; i++) {
//         const float sample = wav_sample_buffer[i];
//         left[i]            = sample;
//         right[i]           = sample;
//     }
//     merge_interleaved(left, right, audio_output_buffer, audio_buffer_size);
// }
