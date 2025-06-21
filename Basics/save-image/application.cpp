/*
 * this example shows how to use `saveImage()` to save a PImage to disk.
 */

#include "Umfeld.h"

using namespace umfeld;

PImage* umfeld_image;

void settings() {
    size(1024, 768);
}

void setup() {
    umfeld_image = loadImage("umfeld.png");
}

void draw() {
    background(0.85f);
    image(umfeld_image, mouseX, mouseY);
}

void keyPressed() {
    if (key == ' ') {
        saveImage(umfeld_image, sketchPath() + "image-" + nfs(frameCount, 4) + ".png");
    }
}
