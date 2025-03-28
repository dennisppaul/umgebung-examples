#include "Umgebung.h"
#include "UmgebungOpenCV.h"

using namespace umgebung;

PFont* mFont{};

void settings() {
    size(1024, 768);
}

void setup() {
    mFont = loadFont("../RobotoMono-Regular.ttf", 48);
    textFont(mFont);
}

void draw() {
    background(1.0f);
    fill(0.0f);
    noStroke();
    text("23", mouseX, mouseY);
}

void mousePressed() {
    retrieve_contour();
}

void mouseReleased() {
}
