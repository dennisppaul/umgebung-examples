#include "Umfeld.h"

using namespace umfeld;

extern SubsystemGraphics* umfeld_create_subsystem_graphics_terminal();

PImage* umfeld_image;

void settings() {
    size(1024, 768); // TODO has no effect ATM
    create_subsystem_graphics = umfeld_create_subsystem_graphics_terminal;
}

void setup() {
    umfeld_image = loadImage("umfeld-logotype-16.png");
}

void draw() {
    background(0.2f); // TODO still very much WIP

    stroke(0.5f, 0.85f, 1.0f);
    line(0, 0, mouseX, mouseY);

    stroke(1.0f, 0.25f, 0.35f);
    point(mouseX, mouseY - 1);

    fill(1.0f, 1.0f, 1.0f);
    text(to_string("size: ", width, "x", height), 0, 0);
    text(to_string("mouse: ", mouseX, "x", mouseY), mouseX, mouseY);
    text(to_string("image: ", umfeld_image->width, "x", umfeld_image->height), mouseX, mouseY + 1);
    fill(1.0f, 0.25f, 0.35f);
    image(umfeld_image, mouseX, mouseY + 2, 32, 16);

    stroke(1.0f, 1.0f, 1.0f);
    rect(mouseX, mouseY + 2, 32, 16);

    // g->auto_refresh(bool auto_refresh);
    // g->simple_clear = false;
}
