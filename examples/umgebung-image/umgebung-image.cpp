#include "Umgebung.h"

using namespace umgebung;

PImage* mImage;

void settings() {
    size(1024, 768);
    subsystem_graphics = umgebung_subsystem_graphics_create_openglv33();
}

void setup() {
    if (!exists(sketchPath() + "../image.png")) {
        println("cannot find required files at:", sketchPath());
        println("... exiting");
        exit();
    }
    mImage = loadImage(sketchPath() + "../image.png");

    //         uint32_t pixels[64 * 64];
    //         for (int i = 0; i < 64 * 64; ++i) {
    //             pixels[i] = color(random(1), random(1), random(1), 0.5f);
    //         }
    // mImage->update(pixels, 64, 64, 32, 32);
    rectMode(CENTER);
}

void draw() {
    background(0.2f);

    /* image */
    // constexpr int length = 64 * 64 * 4; // number of channels is always 4
    // float         pixels[length];
    // for (int i = 0; i < length; i += 4) {
    //     const float c = (i + frameCount) % 252 / 255.0;
    //     pixels[i + 0] = c;
    //     pixels[i + 1] = 1.0 - c;
    //     pixels[i + 2] = 1.0;
    //     pixels[i + 3] = 0.5;
    // }
    // mImage->update(pixels, 64, 64, 32, 32);

    fill(1.0f);
    image(mImage, mouseX, mouseY);
    // image(mImage, padding, padding + spacing, grid, grid);

    if (is_mouse_pressed) {
        console(".");
    }
}

void keyPressed() {
    if (key == 'q') {
        exit();
    }
    println((char) key, " pressed");
}

void mouseMoved() {
    for (int i = 0; i < 1000; i++) {
        const int x = random(0, width);
        const int y = random(0, width);
        mImage->set(x, y, 0x00000000); // RGBA format
    }
    mImage->updatePixels(g);
}