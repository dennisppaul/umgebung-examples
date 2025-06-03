/*
 * this example shows how to use the MIDIListener interface
 */

#include "Umfeld.h"
#include "MIDI.h"

using namespace umfeld;

MIDI midi;

class MyMIDI final : public MIDIListener {
    void midi_message(const std::vector<unsigned char>& message) override {
        print("received midi_message via listener: ");
        for (size_t i = 0; i < message.size(); ++i) {
            print(static_cast<int>(message[i]), " ");
        }
        println();
    }
    void note_off(int channel, int note) override { println("received note_off via listener"); }
    void note_on(int channel, int note, int velocity) override { println("received note_on via listener"); }
    void control_change(int channel, int control, int value) override {}
    void program_change(int channel, int program) override {}
    void pitch_bend(int channel, int value) override {}
    void sys_ex(const std::vector<unsigned char>& message) override {}
};

MyMIDI midiListener;

void settings() {
    size(1024, 768);
}

void setup() {
    midi.print_available_ports();
    midi.open_input_port(0);
    midi.callback(&midiListener);
    background(0.0f);
}

void draw() {}
