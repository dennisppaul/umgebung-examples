/*
 * this example shows how to use primitive shapes like arcs and ellipses.
 */

#include "Umfeld.h"

using namespace umfeld;

void settings() {
    size(1024, 768);
}

void draw() {
    background(0.85f);
    fill(1.0f);
    noStroke();
    arc(50, 55, 50, 50, 0, HALF_PI, PIE);
    stroke(0.0f);
    arc(50, 55, 50, 50, 0, HALF_PI, OPEN);
    noFill();
    arc(50, 55, 60, 60, HALF_PI, PI);
    arc(50, 55, 70, 70, PI, PI + QUARTER_PI);
    arc(50, 55, 80, 80, PI + QUARTER_PI, TWO_PI);

    fill(1.0f);
    stroke(0.0f);
    arc(200, 200, 300, 300, 0, PI + QUARTER_PI, OPEN);
    translate(320, 0);
    arc(200, 200, 300, 300, 0, PI + QUARTER_PI, PIE);
    translate(320, 0);
    arc(200, 200, 300, 300, 0, PI + QUARTER_PI, CHORD);

    translate(-640, 320);
    ellipseMode(RADIUS);         // Set ellipseMode to RADIUS
    fill(1.0f);                  // Set fill to white
    ellipse(200, 200, 120, 120); // Draw white ellipse using RADIUS mode

    ellipseMode(CENTER);         // Set ellipseMode to CENTER
    fill(0.4f);                  // Set fill to gray
    ellipse(200, 200, 120, 120); // Draw gray ellipse using CENTER mode

    translate(320, 0);
    ellipseMode(CORNER);         // Set ellipseMode is CORNER
    fill(1.0f);                  // Set fill to white
    ellipse(100, 100, 200, 200); // Draw white ellipse using CORNER mode

    ellipseMode(CORNERS);        // Set ellipseMode to CORNERS
    fill(0.4f);                  // Set fill to gray
    ellipse(100, 100, 200, 200); // Draw gray ellipse using CORNERS mode
}
