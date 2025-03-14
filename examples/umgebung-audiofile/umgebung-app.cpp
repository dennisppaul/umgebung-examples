#include "Umgebung.h"

using namespace umgebung;

#include "AudioFileReader.h"
#include "AudioFileWriter.h"

AudioFileReader audio_file_reader;
PAudio*         second_audio_device = nullptr;
bool            toggle_pause        = false;

extern std::vector<AudioUnitInfo> get_audio_info();

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
    //      AudioUnitInfo audio_device_properties;
    //      audio_device_properties.input_channels  = 1;
    //      audio_device_properties.output_channels = 2;
    //      audio_device_properties.sample_rate     = 48000;
    //      audio(&audio_device_properties);
    //      ```
    // console("SDL_WasInit(SDL_INIT_AUDIO): ", SDL_WasInit(SDL_INIT_AUDIO));
    // SDL_Init(SDL_INIT_AUDIO);
    // console("SDL_WasInit(SDL_INIT_AUDIO): ", SDL_WasInit(SDL_INIT_AUDIO));
    // const std::vector<AudioUnitInfo> info = get_audio_info();
    // console("****************************************************************************************************");
    // console("****************************************************************************************************");
    // console("****************************************************************************************************");
    // console("****************************************************************************************************");
    // for (auto audio_device_info: info) {
    //     console(audio_device_info.name, " :: ", audio_device_info.id);
    // }
    // SDL_QuitSubSystem(SDL_INIT_AUDIO);
    // console("SDL_WasInit(SDL_INIT_AUDIO): ", SDL_WasInit(SDL_INIT_AUDIO));
    umgebung::subsystem_audio = umgebung_subsystem_audio_create_portaudio();
    audio(1, 2, 48000, 1024);
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

    console("WAV INFO ");
    console("sample_rate: ", audio_file_reader.sample_rate());
    console("channels   : ", audio_file_reader.channels());
    console("length     : ", audio_file_reader.length());

    write_WAV_file();

    console("SDL_GetBasePath: ", SDL_GetBasePath());
    console("sketchPath     : ", sketchPath());

    // AudioUnitInfo info;
    // info.id              = AUDIO_DEVICE_FIND_BY_NAME;
    // info.name            = "MacBook";
    // info.output_channels = 1;
    // info.sample_rate     = 48000;
    // second_audio_device  = createAudio(&info);
    // console("created second audio device with id: ", second_audio_device->id);
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

void audioEvent(const AudioUnitInfo& device) {
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
        merge_interleaved(left, right, audio_output_buffer, audio_buffer_size);
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
    merge_interleaved(left, right, audio_output_buffer, audio_buffer_size);
}
