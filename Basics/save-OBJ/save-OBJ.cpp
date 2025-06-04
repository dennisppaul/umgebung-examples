/* this example shows how to export shapes as OBJ */

#include "Umfeld.h"

using namespace umfeld;

void settings() {
    size(400, 400);
}

void setup() {
    noFill();
}

void draw() {
    background(0.85f);

    if (isKeyPressed && key == ' ') {
        // beginRecord(OBJ, to_string("example-", frameCount, ".obj"));
        beginRecord(PDF, to_string("example-", frameCount, ".pdf"));
    }

    pushMatrix();
    stroke(0.5f, 0.85f, 1.0f);
    translate(232, 192, 0);
    rotateY(0.5);
    noFill();
    box(160);
    popMatrix();

    pushMatrix();
    stroke(1.0f, 0.25f, 0.35f);
    translate(232, 192, 0);
    rotateY(0.5);
    noFill();
    box(160, 80, 200);
    popMatrix();

    stroke(1.0f, 0.2f);
    translate(200, 200, 0);
    rotateX(mouseY * 0.05);
    rotateY(mouseX * 0.05);
    fill(mouseX / width * 2, 0, 0.62f);
    // sphereDetail(mouseX / 4);
    sphere(100);

    if (isKeyPressed && key == ' ') {
        endRecord();
    }
}