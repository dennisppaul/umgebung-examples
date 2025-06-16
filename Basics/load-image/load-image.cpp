#include "Umfeld.h"

using namespace umfeld;

PImage* umfeld_image;

void settings() {
    size(1024, 768);
    antialiasing = 8;
}

void setup() {
    umfeld_image = loadImage("../umfeld.png");
    rectMode(CENTER);
    noStroke();
}

void draw() {
    background(0.85f);

    fill(0.0f);
    g->debug_text("FPS          : " + nf(frameRate, 3, 1), 10, 10);

    fill(0.0f);
    circle(width * 0.5f, height * 0.5f, umfeld_image->width);

    fill(1.0f);
    image(umfeld_image, mouseX, mouseY);
}

void keyPressed() {
    if (key == 'q') {
        //request_shutdown = true;
        exit();
    }
}