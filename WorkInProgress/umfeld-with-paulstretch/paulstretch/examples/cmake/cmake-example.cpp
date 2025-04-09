#include <fstream>
#include <iostream>

#include "PaulStretch.h"

using namespace std;

int main(int argc, char** argv) {
    PaulStretch stretch(8, 44100 * 0.25, 44100);
    int         mNumRequestedSamples = stretch.get_required_samples();
    cout << "get_required_samples: " << mNumRequestedSamples << endl;
}
