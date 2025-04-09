#include "Umfeld.h"
#include "MIDI.h"

using namespace umfeld;

MIDI midiHandler;

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
    midiHandler.print_available_ports();
    midiHandler.open_input_port("IAC Driver Bus 1");
    midiHandler.open_output_port(0);
    midiHandler.callback(&midiListener);
    background(0.0f);
}

void draw() {
    background(0.0f);
}

void mousePressed() {
    println("send note_on");
    midiHandler.note_on(0, 60, 112);
}

void mouseReleased() {
    println("send note_off");
    midiHandler.note_off(0, 60);
}

void note_on(int, int, int) {
    println("received note_on via callback");
}

void note_off(int, int) {
    println("received note_off via callback");
}

void keyPressed() {
    if (key == 'q') {
        exit();
    }
}

void midi_message(const std::vector<unsigned char>& message) {
    if (message.size() > 0) {
        print("received midi_message via callback: ");
        for (size_t i = 0; i < message.size(); ++i) {
            print(static_cast<int>(message[i]), " ");
        }
        println();
    }
}
