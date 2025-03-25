#include <glm/glm.hpp>

#include "Umgebung.h"
#include "Geometry.h"

using namespace umgebung;

int   stroke_join_mode = MITER;
int   stroke_cap_mode  = ROUND;
float stroke_weight    = 30;
bool  close_shape      = false;

void settings() {
    size(1024, 768);
    display            = 0;
    antialiasing       = 8;
    render_to_buffer   = true;
    retina_support     = true;
    enable_audio       = false;
    subsystem_graphics = umgebung_create_subsystem_graphics_openglv33();
}

void setup() {
    strokeJoin(stroke_join_mode);
    strokeCap(stroke_cap_mode);
    strokeWeight(stroke_weight);

    hint(ENABLE_SMOOTH_LINES);
    g->stroke_properties(radians(10), radians(10), 179);
    g->stroke_mode(STROKE_RENDER_MODE_TRIANGULATE_2D);
}

void draw() {
    background(0.85f);

    stroke(0.0f);
    fill(0.5f, 0.85f, 1.0f);
    beginShape(POLYGON);
    vertex(412, 204);
    vertex(522, 204);
    vertex(mouseX, mouseY);
    vertex(632, 314);
    vertex(632, 424);
    vertex(412, 424);
    vertex(312, 314);
    endShape(close_shape);

    noFill();
    stroke(1.0f, 0.25f, 0.35f);
    line(width / 2.0f - 30, height / 2 - 100, width / 2.0f + 30, height / 2 - 40);
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
    if (key == ' ') {
        close_shape = !close_shape;
    }
}
