#include "KlangWellen.h"
#include "Klangstrom.h"
#include "autotune/Autotune.h"

using namespace klangwellen;
bool is_initialized = false;
void setup() {
    autotune_init();
    is_initialized = true;
}

void loop() {}

void audioblock(SIGNAL_TYPE* output_signal_left, SIGNAL_TYPE* output_signal_right,
                SIGNAL_TYPE* input_signal_left, SIGNAL_TYPE* input_signal_right) {
    if (is_initialized) {
        autotune_process(input_signal_left, KLANG_SAMPLES_PER_AUDIO_BLOCK);
        autotune_process(input_signal_right, KLANG_SAMPLES_PER_AUDIO_BLOCK);
        KlangWellen::mult(input_signal_left, 5.0);
        KlangWellen::mult(input_signal_right, 5.0);
        KlangWellen::copy(input_signal_left, output_signal_left);
        KlangWellen::copy(input_signal_right, output_signal_right);
    }
}
