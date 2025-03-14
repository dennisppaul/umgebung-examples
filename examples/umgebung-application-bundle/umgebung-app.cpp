#include "Umgebung.h"

using namespace umgebung;

PFont* mFont;
int    mWidth  = 1024;
int    mHeight = 768;

void arguments(std::vector<std::string> args) {
}

void settings() {
    size(mWidth, mHeight);
}

void setup() {
    if (!exists(sketchPath() + "../assets/RobotoMono-Regular.ttf")) {
        println("cannot find required files at:", sketchPath());
        println("... exiting");
        exit();
    }
    mFont = loadFont(sketchPath() + "../assets/RobotoMono-Regular.ttf", 48);
    textFont(mFont);
}

void draw() {
    background(1.0f);

    noStroke();
    fill(0.0f);
    text("42", mouseX, mouseY);
}