#include <glm/glm.hpp>

#include "Umgebung.h"
#include "Geometry.h"

using namespace umgebung;

/*
 * this example shows how to use the beginShape() + endShape() functions to draw a shape with different vertex types.
 * from https://processing.org/reference/beginShape_.html
 */

// TODO: the following combinations are partly broken:
//      - stroke_join_mode: BEVEL+MITER+POINTED
//      - stroke_join_mode: BEVEL+MITER+ROUND

int   stroke_join_mode = ROUND;
int   stroke_cap_mode  = ROUND;
float stroke_weight    = 30;

void settings() {
    size(1024, 768);
    display            = 0;
    antialiasing       = 8;
    render_to_buffer   = false;
    retina_support     = true;
    enable_audio       = false;
    subsystem_graphics = umgebung_create_subsystem_graphics_openglv33();
}

void setup() {
    hint(HINT_ENABLE_SMOOTH_LINES);
}

void draw() {
    background(1.0f);
    stroke(0.0f);
    fill(0.5f, 0.85f, 1.0f);
    strokeWeight(stroke_weight);
    pointSize(stroke_weight);
    strokeJoin(stroke_join_mode);
    strokeCap(stroke_cap_mode);

    scale(0.66f);

    beginShape();
    vertex(120, 80);
    vertex(340, 80);
    vertex(340, 300);
    vertex(120, 300);
    endShape(CLOSE);

    translate(280, 0);
    beginShape(POINTS);
    vertex(120, 80);
    vertex(340, 80);
    vertex(340, 300);
    vertex(120, 300);
    endShape();

    translate(280, 0);
    beginShape();
    vertex(120, 80);
    vertex(230, 80);
    vertex(230, 190);
    vertex(340, 190);
    vertex(340, 300);
    vertex(120, 300);
    endShape(CLOSE);

    translate(280, 0);
    beginShape(LINES);
    vertex(120, 80);
    vertex(340, 80);
    vertex(340, 300);
    vertex(120, 300);
    endShape();

    translate(-4 * 280, 280);

    translate(280, 0);
    noFill();
    beginShape();
    vertex(120, 80);
    vertex(340, 80);
    vertex(340, 300);
    vertex(120, 300);
    endShape();

    translate(280, 0);
    noFill();
    beginShape();
    vertex(120, 80);
    vertex(340, 80);
    vertex(340, 300);
    vertex(120, 300);
    endShape(CLOSE);

    stroke(0.0f);
    fill(0.5f, 0.85f, 1.0f);

    translate(280, 0);
    beginShape(TRIANGLES);
    vertex(120, 300);
    vertex(160, 120);
    vertex(200, 300);
    vertex(270, 80);
    vertex(280, 300);
    vertex(320, 80);
    endShape();

    translate(280, 0);
    beginShape(TRIANGLE_STRIP);
    vertex(120, 300);
    vertex(160, 80);
    vertex(200, 300);
    vertex(240, 80);
    vertex(280, 300);
    vertex(320, 80);
    vertex(360, 300);
    endShape();

    translate(-4 * 280, 280);

    translate(280, 0);
    beginShape(TRIANGLE_FAN);
    vertex(230, 200);
    vertex(230, 60);
    vertex(368, 200);
    vertex(230, 340);
    vertex(88, 200);
    vertex(230, 60);
    endShape();

    translate(280, 0);
    beginShape(QUAD_STRIP);
    vertex(120, 80);
    vertex(120, 300);
    vertex(200, 80);
    vertex(200, 300);
    vertex(260, 80);
    vertex(260, 300);
    vertex(340, 80);
    vertex(340, 300);
    endShape();

    translate(280, 0);
    beginShape(LINE_STRIP);
    vertex(120, 80);
    vertex(120, 300);
    vertex(200, 300);
    vertex(200, 80);
    vertex(260, 80);
    vertex(260, 300);
    vertex(340, 300);
    vertex(340, 80);
    endShape();
}

void keyPressed() {
    if (key == '-') {
        stroke_weight -= 0.25f;
        if (stroke_weight < 0) { stroke_weight = 0; }
        strokeWeight(stroke_weight);
        console("stroke_weight: ", stroke_weight);
    }
    if (key == '+') {
        stroke_weight += 0.25f;
        strokeWeight(stroke_weight);
        console("stroke_weight: ", stroke_weight);
    }
    if (key == '1') {
        stroke_join_mode = NONE;
        strokeJoin(NONE);
        console("NONE");
    }
    if (key == '2') {
        stroke_join_mode = BEVEL;
        strokeJoin(BEVEL);
        console("BEVEL");
    }
    if (key == '3') {
        stroke_join_mode = MITER;
        strokeJoin(MITER);
        console("MITER");
    }
    if (key == '4') {
        stroke_join_mode = ROUND;
        strokeJoin(ROUND);
        console("ROUND");
    }
    if (key == '5') {
        stroke_join_mode = MITER_FAST;
        strokeJoin(MITER_FAST);
        console("MITER_FAST");
    }
    if (key == '6') {
        stroke_join_mode = BEVEL_FAST;
        strokeJoin(BEVEL_FAST);
        console("BEVEL_FAST");
    }
    if (key == 'q') {
        stroke_cap_mode = POINTED;
        strokeCap(POINTED);
        console("POINTED");
    }
    if (key == 'w') {
        stroke_cap_mode = PROJECT;
        strokeCap(PROJECT);
        console("PROJECT");
    }
    if (key == 'e') {
        stroke_cap_mode = ROUND;
        strokeCap(ROUND);
        console("ROUND");
    }
    if (key == 'r') {
        stroke_cap_mode = SQUARE;
        strokeCap(SQUARE);
        console("SQUARE");
    }
}
