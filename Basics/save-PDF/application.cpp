/* this example shows how to export shapes as PDF */
// TODO WIP a lot of things are not implemented yet and not tested

#include "Umfeld.h"

using namespace umfeld;

void settings() {
    size(1024, 768);
}

void setup() {
    noFill();
}

void draw() {
    background(0.85f);

    if (isKeyPressed && key == ' ') {
        beginRecord(PDF, to_string("example-", frameCount, ".pdf"));
    }

    pushMatrix();
    stroke(1.0f);
    fill(0.5f, 0.85f, 1.0f);
    translate(width * 0.33, height * 0.5f, 0);
    rotateX(mouseY * 0.03);
    rotateY(mouseX * 0.07);
    box(width * 0.25f);
    popMatrix();

    pushMatrix();
    stroke(1.0f);
    fill(1.0f, 0.25f, 0.35f);
    translate(width * 0.66, height * 0.5f, 0);
    rotateX(mouseY * 0.05f);
    rotateY(mouseX * 0.05f);
    sphereDetail(mouseX / 40);
    sphere(width * 0.25f);
    popMatrix();

    if (isKeyPressed && key == ' ') {
        endRecord();
    }
}