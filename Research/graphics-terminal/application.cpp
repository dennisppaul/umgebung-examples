#include "Umfeld.h"

using namespace umfeld;

extern SubsystemGraphics* umfeld_create_subsystem_graphics_terminal();

void settings() {
    size(1024, 768); // TODO has no effect ATM
    create_subsystem_graphics = umfeld_create_subsystem_graphics_terminal;
}

void setup() {
    noFill();                   // TODO has no effect ATM
    stroke(1.0f, 0.25f, 0.35f); // TODO has no effect ATM
}

void draw() {
    background(map(mouseX, 0, width, 0.2f, 1)); // TODO still very much WIP

    line(0, 0, mouseX, mouseY);
    line(mouseX, mouseY, width, height);
    rect(mouseX, mouseY, 19, 9);
    point(mouseX + 9, mouseY + 4);

    std::string debug = std::string("size: ") + to_string(g->width) + " x " + to_string(g->height);
    text(debug, mouseX + 1, mouseY + 1);

    // g->auto_refresh(bool auto_refresh);
    // g->simple_clear = false;
}
