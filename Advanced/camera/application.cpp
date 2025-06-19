// TODO WIP there are still a lot of things very broken here …

/*
 * this example shows how to use the camera
 * from https://processing.org/reference/camera_.html
 */

#include "Umfeld.h"

using namespace umfeld;

void settings() {
    size(1024, 768);
}

void setup() {
    strokeWeight(3);
    rectMode(CENTER);
    g->set_stroke_render_mode(STROKE_RENDER_MODE_NATIVE);
    g->set_stroke_render_mode(STROKE_RENDER_MODE_TRIANGULATE_2D);
    g->set_stroke_render_mode(STROKE_RENDER_MODE_TUBE_3D);
    // g->set_stroke_render_mode(RENDER_MODE_SHAPE);
}

void draw() {
    background(0.85f);

    fill(0);
    debug_text("FPS   : " + nf(frameRate, 1), 10, 10);

    camera();

    fill(1, 0.25, 0.35);
    // noFill();
    stroke(1);
    noStroke();
    translate(width / 2, height / 2, 0);
    rotateX(frameCount * 0.01f);
    rotateY(frameCount * 0.0037f);

    constexpr float fov            = DEFAULT_CAMERA_FOV_RADIANS;
    const float     cameraDistance = (height / 2.0f) / tan(fov / 2.0f);
    camera(width / 2.0, height / 2.0 - 200, cameraDistance,
           mouseX, mouseY, 0,
           0, 1, 0);

    // sphere(200);
    // square(0, 0, 200);

    for (int x = -20; x < 20; ++x) {
        for (int y = -20; y < 20; ++y) {
            pushMatrix();
            translate(x * 60, 0, y * 60);
            // box(20);
            square(0, 0, 20);
            popMatrix();
        }
    }

    // noFill();
    // camera(70.0, 35.0, 120.0,
    //        50.0, 50.0, 0.0,
    //        0.0, 1.0, 0.0);
    // translate(50, 50, 0);
    // rotateX(-PI / 6);
    // rotateY(PI / 3);
    // box(45);
}

void keyPressed() {
    if (key == ' ') {
    }
}
