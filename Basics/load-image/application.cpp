#include "Umfeld.h"

using namespace umfeld;

PImage* umfeld_image;

void settings() {
    size(1024, 768);
    antialiasing = 8;
}

void setup() {
    umfeld_image = loadImage("../umfeld.png");

    // NOTE loading images also works with URLs
    //      umfeld_image = loadImage("https://raw.githubusercontent.com/dennisppaul/umfeld-examples/52730a5113341dadb60046a7908bafa874ce8ab4/Basics/load-image/umfeld.png");

    rectMode(CENTER);
    noStroke();

    // const std::string msg = loadString("data:,Hello%20world");
    // std::string msg = loadString("data:text/html,%3Ch1%3EHello%20World%3C%2Fh1%3E");
    // const std::string msg = loadString("data:text/plain;base64,SGVsbG8gd29ybGQ=");
    // const std::string msg = loadString("../CMakeLists.txt");
    // const std::string msg = loadString("https://raw.githubusercontent.com/dennisppaul/umfeld/refs/heads/main/README.md");
    // console(msg);
}

void draw() {
    background(0.85f);

    fill(0.0f);
    debug_text("FPS: " + nf(frameRate, 3, 1), 10, 10);

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