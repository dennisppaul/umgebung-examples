/*
 * this example shows how to send MIDI messages using the MIDI class
 */

#include "Umfeld.h"
#include "MIDI.h"

using namespace umfeld;

MIDI midi;

void settings() {
    size(1024, 768);
}

void setup() {
    midi.print_available_ports();
    midi.open_output_port("IAC Driver Bus 1");
    background(0.0f);
}

void draw() {}

void mousePressed() {
    println("send note_on");
    midi.note_on(0, 60, 112);
}

void mouseReleased() {
    println("send note_off");
    midi.note_off(0, 60);
}
