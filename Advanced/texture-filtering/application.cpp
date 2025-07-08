#include "Umfeld.h"

using namespace umfeld;

PImage* umfeld_image;

void settings() {
    size(1024, 768);
    antialiasing = 8;
}

void setup() {
    umfeld_image = loadImage("umfeld-logotype-64.png");
    rectMode(CENTER);
    noStroke();
}

void draw() {
    background(0.85f);

    fill(0.0f);
    debug_text("FPS: " + nf(frameRate, 3, 1), 10, 10);

    fill(1.0f);
    image(umfeld_image, mouseX - 256, mouseY, 512, 512);

    texture(umfeld_image);   // set texture to the loaded image
    texture_filter(NEAREST); // set to NEAREST for pixelated effect
    rect(mouseX + 256, mouseY, 512, 512);
    texture_filter(LINEAR); // reset to LINEAR for smooth texture filtering
    texture();              // reset texture to default
}
