#include <queue>

#include "Umgebung.h"

using namespace umgebung;

#include "PaulStretch.h"
#include "audio/AudioFileReader.h"

PaulStretch       fStretcher{8, (int) (48000 * 0.25f), 48000};
AudioFileReader   fAudioFileReader;
float*            fStretchedSampleBuffer;
std::queue<float> fSampleFIFOBuffer;

void settings() {
    size(1024, 768);
    audio(0, 2);
}

void setup() {
    fAudioFileReader.open("../audio-input.wav");
    fStretchedSampleBuffer = new float[fStretcher.get_output_buffer_size()];
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

void refill_fifo() {
    int mNumRequiredSamples = fStretcher.get_required_samples();
    if (mNumRequiredSamples > 0) {
        float* mInputBuffer = new float[mNumRequiredSamples];
        int    mReadFrames  = fAudioFileReader.read(mNumRequiredSamples, mInputBuffer);
        if (mReadFrames == mNumRequiredSamples) {
            fStretcher.fill_input_buffer(mInputBuffer, mNumRequiredSamples);
        } else {
            for (int i = 0; i < mReadFrames; i++) {
                fStretcher.get_input_buffer()[i] = mInputBuffer[i];
            }
            fAudioFileReader.rewind();
            int mMissingSamples = mNumRequiredSamples - mReadFrames;
            mReadFrames         = fAudioFileReader.read(mMissingSamples, mInputBuffer);
            if (mReadFrames == mMissingSamples) {
                for (int i = 0; i < mMissingSamples; i++) {
                    fStretcher.get_input_buffer()[mReadFrames + i] = mInputBuffer[i];
                }
            }
        }
        delete[] mInputBuffer;
    }

    fStretcher.process_segment(fStretchedSampleBuffer);
    for (int i = 0; i < fStretcher.get_output_buffer_size(); i++) {
        fSampleFIFOBuffer.push(fStretchedSampleBuffer[i]);
    }
}

void audioEvent() {
    for (int i = 0; i < audio_buffer_size; i++) {
        if (fSampleFIFOBuffer.empty()) {
            refill_fifo();
        }
        const float mSample = fSampleFIFOBuffer.front();
        fSampleFIFOBuffer.pop();
        for (int j = 0; j < output_channels; ++j) {
            audio_output_buffer[i * output_channels + j] = mSample;
        }
    }
}

void finish() {
    delete[] fStretchedSampleBuffer;
    fAudioFileReader.close();
}