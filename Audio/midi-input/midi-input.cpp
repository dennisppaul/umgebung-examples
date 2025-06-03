/*
 * this example shows how to use the MIDI to receive MIDI messages
 */

#include "Umfeld.h"
#include "MIDI.h"

using namespace umfeld;

MIDI     midi;
bool     print_all_midi_messages = false;
uint32_t red_color               = color(1.0f, 0.25f, 0.35f);
uint32_t blue_color              = color(0.5f, 0.85f, 1.0f);

static constexpr int PAD_COUNT = 8;
bool                 pad_pressed[PAD_COUNT]{false};

void settings() {
    size(1024, 768);
    antialiasing = 8;
}

void setup() {
    midi.print_available_ports();
    midi.open_input_port("Arturia BeatStep");
    strokeWeight(6);
    stroke(0.0f);
}

void draw() {
    background(0.85f);
    for (int i = 0; i < PAD_COUNT; ++i) {
        float circle_size;
        if (pad_pressed[i]) {
            circle_size = 100;
            if (i >= PAD_COUNT / 2) {
                fill_color(red_color);
            } else {
                fill_color(blue_color);
            }
        } else {
            circle_size = 50;
            noFill();
        }
        circle(width / 2.0f + ((float) i - PAD_COUNT * 0.5f + 0.5f) * 100, height / 2.0f, circle_size);
    }
}

void note_on(const int channel, const int note, const int velocity) {
    println("received note_on via callback : ", channel, ", ", note, ", ", velocity);
    const int index    = (note + 4) % PAD_COUNT;
    pad_pressed[index] = true;
    println("index: ", index);
}

void note_off(const int channel, const int note) {
    println("received note_off via callback: ", channel, ", ", note);
    const int index    = (note + 4) % PAD_COUNT;
    pad_pressed[index] = false;
    println("index: ", index);
}

void midi_message(const std::vector<unsigned char>& message) {
    if (print_all_midi_messages) {
        if (message.size() > 0) {
            print("received midi_message via callback: ");
            for (size_t i = 0; i < message.size(); ++i) {
                print(static_cast<int>(message[i]), " ");
            }
            println();
        }
    }
}
