/*
 * this example is based on the Processing example
 * https://processing.org/reference/lights_.html
 * and shows how to use light with 3D primitives in umfeld.
 */

#include "Umfeld.h"

using namespace umfeld;

void settings() {
    size(400, 400);
}

void setup() {
    noStroke();
    fill(1.0f);
    hint(ENABLE_DEPTH_TEST); // enable depth testing for 3D rendering
}

void draw() {
    background(0.0f);

    // Include lights() at the beginning
    // of draw() to keep them persistent

    lights();

    pushMatrix();
    translate(80, 200, 0);
    sphere(120);
    popMatrix();

    pushMatrix();
    translate(320, 200, 0);
    sphere(120);
    popMatrix();

    noLights();
}