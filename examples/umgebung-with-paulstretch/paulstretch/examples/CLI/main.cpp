#define RUN_TEST
#ifdef RUN_TEST

#include <fstream>
#include <iostream>
#include <vector>

#include "input/MP3InputS.h"
#include "PaulStretch.h"

using namespace std;

string render_file(string inaudio, string outaudio) {
    InputS* ai = new MP3InputS;
    if (!ai->open(inaudio)) {
        return "Error: Could not open audio file (or file format not "
               "recognized): " +
               inaudio;
    }

    // 32bit PCM, little-endian, mono, 44100 Hz
    std::ofstream outfile(outaudio, std::ios::binary | std::ios::out);

    int         mBufferSize = 44100 * 0.25;
    short int*  mReadBuffer = new short int[mBufferSize * 8];
    PaulStretch stretch(8, mBufferSize, 44100);

    while (!ai->eof) {
        /**
         * steps to use PaulStretch
         * 1. get required samples
         * 2. fill input buffer with number of required samples
         * 3. process input buffer
         * 4. use output buffer samples
        */
        // 1. get required samples
        int mNumRequestedSamples = stretch.get_required_samples();
        cout << "get_required_samples: " << mNumRequestedSamples << endl;
        int mNumReadSamples = ai->read(mNumRequestedSamples, mReadBuffer);
#define PROCESS_BY_SEGMENT
#ifdef PROCESS_BY_SEGMENT
        // 2. fill input buffer with number of required samples
        for (int i = 0; i < mNumReadSamples; i++) {
            stretch.get_input_buffer()[i] = mReadBuffer[i * 2] / 32768.0;
        }
        const int mBufferSize        = stretch.get_output_buffer_size();
        float*    mSamples           = new float[mBufferSize];
        int       mNumOfTotalSamples = 0;
        bool      mHasMoreSamples    = true;
        while (mHasMoreSamples) {
            // 3. process input buffer
            mHasMoreSamples = stretch.process_segment(mSamples);
            // 4. use output buffer samples
            int* outbuf = new int[mBufferSize];
            for (int i = 0; i < mBufferSize; i++) {
                float l = mSamples[i];
                if (l < -1.0) {
                    l = -1.0;
                } else if (l > 1.0) {
                    l = 1.0;
                }
                outbuf[i] = (int) (l * 32767.0 * 65536.0);
            };
            outfile.write(reinterpret_cast<char*>(outbuf), mBufferSize * sizeof(int));
            delete[] outbuf;
            mNumOfTotalSamples += mBufferSize;
        }
        cout << "processed samples: " << mNumOfTotalSamples << endl;
        delete[] mSamples;
#else
#ifdef USE_FILL_BUFFER
        float* mInputBuffer = new float[mNumReadSamples];
        for (int i = 0; i < mNumReadSamples; i++) {
            mInputBuffer[i]               = mReadBuffer[i * 2] / 32768.0;
            stretch.get_input_buffer()[i] = mReadBuffer[i * 2] / 32768.0;
        }
        stretch.fill_input_buffer(mInputBuffer, mNumRequestedSamples);
        delete[] mInputBuffer;
#else
        for (int i = 0; i < mNumReadSamples; i++) {
            stretch.get_input_buffer()[i] = mReadBuffer[i * 2] / 32768.0;
        }
#endif
        vector<float> mSamples;
        mSamples.clear();
        mSamples.reserve(stretch.get_output_buffer_size() * 8);
        stretch.process(mSamples);
        cout << "processed samples: " << mSamples.size() << endl;
        int* outbuf = new int[mSamples.size()];
        for (int i = 0; i < mSamples.size(); i++) {
            float l = mSamples[i];
            if (l < -1.0)
                l = -1.0;
            else if (l > 1.0)
                l = 1.0;
            outbuf[i] = (int) (l * 32767.0 * 65536.0);
        };
        outfile.write(reinterpret_cast<char*>(outbuf), mSamples.size() * sizeof(int));
        delete[] outbuf;
#endif
    }
    outfile.close();

    return "OK";
}

int main(int argc, char** argv) {
    cout << "+++ paulstretch" << endl
         << endl;
    if (argc == 3) {
        cout << render_file(argv[1], argv[2]) << endl;
    }
}

#endif // RUN_TEST