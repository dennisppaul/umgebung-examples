#pragma once

#include <cstdint>
#include <fstream>
#include <iostream>

// void write_WAV_file(const char* filename, int sampleRate, int numChannels, int bitsPerSample, const int16_t* data, int numSamples);
// void write_WAV_file(const char* filename, int sampleRate, int numChannels, const float* data, int numSamples);

void write_WAV_file(const char* filename, int sampleRate, int numChannels, int bitsPerSample, const int16_t* data, int numSamples) {
    std::ofstream file(filename, std::ios::binary);

    if (!file.is_open()) {
        std::cerr << "Failed to open the file for writing." << std::endl;
        return;
    }

    // Write WAV header
    file.write("RIFF", 4);
    int32_t fileSize = 36 + numSamples * numChannels * bitsPerSample / 8;
    file.write(reinterpret_cast<char*>(&fileSize), 4);
    file.write("WAVE", 4);
    file.write("fmt ", 4);
    int32_t fmtSize = 16;
    file.write(reinterpret_cast<char*>(&fmtSize), 4);
    int16_t audioFormat = 1; // PCM format
    file.write(reinterpret_cast<char*>(&audioFormat), 2);
    file.write(reinterpret_cast<char*>(&numChannels), 2);
    file.write(reinterpret_cast<char*>(&sampleRate), 4);
    int32_t byteRate = sampleRate * numChannels * bitsPerSample / 8;
    file.write(reinterpret_cast<char*>(&byteRate), 4);
    int16_t blockAlign = numChannels * bitsPerSample / 8;
    file.write(reinterpret_cast<char*>(&blockAlign), 2);
    file.write(reinterpret_cast<char*>(&bitsPerSample), 2);
    file.write("data", 4);
    int32_t dataSize = numSamples * numChannels * bitsPerSample / 8;
    file.write(reinterpret_cast<char*>(&dataSize), 4);

    // Write audio data
    file.write(reinterpret_cast<const char*>(data), dataSize);

    file.close();
}

void write_WAV_file(const char*  filename,
                    int          sampleRate,
                    int          numChannels,
                    const float* data,
                    int          numSamples) {
    int16_t mAudioData[numSamples * numChannels];
    for (int i = 0; i < numSamples; i++) {
        for (int j = 0; j < numChannels; j++) {
            const int k   = i * numChannels + j;
            mAudioData[k] = static_cast<int16_t>(data[k] * 0.5 * INT16_MAX);
        }
    }

    write_WAV_file(filename,
                   sampleRate,
                   numChannels,
                   16,
                   mAudioData,
                   numSamples);
}