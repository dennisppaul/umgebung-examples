// TODO split up the example into multiple topics ( streaming WAV, multiple devices, etc. )

#include "Umfeld.h"

using namespace umfeld;

#include "audio/AudioFileReader.h"
#include "audio/AudioFileWriter.h"
#include "PAudio.h"

AudioFileReader audio_file_reader;
PAudio*         second_audio_device = nullptr;
bool            toggle_pause        = false;

void settings() {
    size(1024, 768);
    umfeld::subsystem_audio = umfeld_create_subsystem_audio_portaudio(); // NOTE use portaudio instead of SDL

    // NOTE this function creates a audio device with 1 input, 2 output channels
    //      and a sample rate of 48000 with default values of 32-bit float buffer
    //      ```
    //      audio(1,2,48000);
    //      ```
    //      this does the same except with an audio device info struct:
    //      ```
    //      AudioUnitInfo info;
    //      info.input_device_id    = AUDIO_DEVICE_FIND_BY_NAME;
    //      info.input_device_name  = "MacBook";
    //      info.input_channels     = 1;
    //      info.output_device_id   = AUDIO_DEVICE_FIND_BY_NAME;
    //      info.output_device_name = "MacBook";
    //      info.output_channels    = 2;
    //      info.sample_rate        = 48000;
    //      ```

    audio(1, 2);
    // audio(2, 3, 48000, 1024, 0, 1); // NOTE intentionally wrong number of channels
    // audio(1, 2, 48000, 1024, "MacBook", "MacBook");
    // AudioUnitInfo info;
    // info.input_device_id    = AUDIO_DEVICE_FIND_BY_NAME;
    // info.input_device_name  = "MacBook";
    // info.input_channels     = 1;
    // info.output_device_id   = AUDIO_DEVICE_FIND_BY_NAME;
    // info.output_device_name = "MacBook";
    // info.output_channels    = 2;
    // info.sample_rate        = 48000;
    // audio(info);
}

void write_WAV_file() {
    float wav_sample_buffer[audio_buffer_size];
    /* write sine wave with duartion approx 1sec + frequency 220Hz */
    AudioFileWriter audio_file_writer;
    audio_file_writer.open("../sine-220Hz.wav");
    float r = 0;
    for (int j = 0; j < sample_rate / audio_buffer_size; ++j) {
        for (int i = 0; i < audio_buffer_size; ++i) {
            wav_sample_buffer[i] = sin(r);
            r += TWO_PI * 220.0f / sample_rate;
        }
        audio_file_writer.write(audio_buffer_size, wav_sample_buffer);
    }
    audio_file_writer.close();
}

void setup() {
    console("AUDIO DEVICE");
    console("audio device name    : ", audio_input_device_name, " / ", audio_output_device_name);
    console("channels             : ", input_channels, " / ", output_channels);

    audio_file_reader.open("../teilchen.wav");

    console("WAV FILE INFO");
    console("sample_rate          : ", audio_file_reader.sample_rate());
    console("channels             : ", audio_file_reader.channels());
    console("length               : ", audio_file_reader.length());

    write_WAV_file();

    console("FILE PATH");
    console("SDL_GetBasePath      : ", SDL_GetBasePath());
    console("sketchPath           : ", sketchPath());

    // TODO second device does not work with PortAudio atm
    // AudioUnitInfo info;
    // info.input_device_id    = AUDIO_DEVICE_FIND_BY_NAME;
    // info.input_device_name  = "MacBook";
    // info.input_channels     = 1;
    // info.output_device_id   = AUDIO_DEVICE_FIND_BY_NAME;
    // info.output_device_name = "MacBook";
    // info.output_channels    = 2;
    // info.sample_rate        = 48000;
    // second_audio_device     = createAudio(&info);
    // audio_start(second_audio_device); // NOTE start second audio device
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
        if (toggle_pause) {
            audio_start();
            if (second_audio_device != nullptr) {
                audio_start(second_audio_device);
            }
        } else {
            audio_stop();
            if (second_audio_device != nullptr) {
                audio_stop(second_audio_device);
            }
        }
        toggle_pause = !toggle_pause;
    }
}

void read_wav(float* samples, const size_t frames) {
    audio_file_reader.read(frames, samples, AudioFileReader::ReadStyle::LOOP);
}

void audioEvent(const PAudio& device) {
    if (second_audio_device == nullptr) {
        return;
    }
    if (&device == second_audio_device) {
        for (int i = 0; i < device.buffer_size; i++) {
            device.output_buffer[i] = random(-0.1, 0.1);
        }
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
        merge_interleaved_stereo(left, right, audio_output_buffer, audio_buffer_size);
    }
}

void audioEvent() {
    float wav_sample_buffer[audio_buffer_size];
    read_wav(wav_sample_buffer, audio_buffer_size);
    float left[audio_buffer_size];
    float right[audio_buffer_size];
    for (int i = 0; i < audio_buffer_size; i++) {
        float sample = wav_sample_buffer[i];
        if (input_channels == 1) {
            sample += audio_input_buffer[i];
        }
        left[i]  = sample;
        right[i] = sample;
    }
    merge_interleaved_stereo(left, right, audio_output_buffer, audio_buffer_size);
}
