#include "Umfeld.h"

using namespace umfeld;

PImage* umfeld_image;

void settings() {
    size(1024, 768);
}

void setup() {
    umfeld_image = loadImage("../umfeld.png");
    g->set_stroke_render_mode(STROKE_RENDER_MODE_LINE_SHADER);
    g->set_point_render_mode(POINT_RENDER_MODE_SHADER);
    hint(ENABLE_DEPTH_TEST);
    rectMode(CENTER);
}

void draw() {
    UMFELD_EMIT_WARNING("emit warning only once", " ... in draw loop");
    const float sphere_detail = mouseX / 40;

    background(0.85f);

    fill(0.0f);
    debug_text("FPS          : " + nf(frameRate, 3, 1), 10, 10);
    debug_text("SPHERE DETAIL: " + nf(sphere_detail, 3, 1), 10, 25);

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
    sphereDetail(sphere_detail);
    sphere(width * 0.125f);
    popMatrix();

    /* sphere */

    fill(1.0f);
    image(umfeld_image, mouseX, mouseY);
    noFill();
    stroke(0.0f);
    strokeWeight(10);
    circle(mouseX, mouseY, 512.0f);
    strokeWeight(1);

    /* points */

    hint(DISABLE_DEPTH_TEST);
    fill(0.0f);
    if (isMousePressed) {
        g->set_point_render_mode(POINT_RENDER_MODE_SHADER);
        debug_text("POINT_MODE   : POINT_RENDER_MODE_SHADER", 10, 40);
    } else {
        g->set_point_render_mode(POINT_RENDER_MODE_TRIANGULATE);
        debug_text("POINT_MODE   : POINT_RENDER_MODE_TRIANGULATE", 10, 40);
    }
    const float point_size = map(mouseX, 0, width, 1.0f, 20.0f);
    pointSize(point_size);
    debug_text(to_string("POINT_SIZE   : ", point_size), 10, 55);
    // TODO crashes at high number of points when resizing backing VBO?!?
    const int num_points = map(mouseY, 0, height, 1000, 25000);
    debug_text(to_string("NUMBER POINTS: ", num_points), 10, 70);
    beginShape(POINTS);
    for (int i = 0; i < num_points; ++i) {
        stroke_color(HSBA(random(1.0f), 1.0f, 1.0f, 1.0f));
        const float x = random(width);
        const float y = random(height);
        vertex(x, y, 0.0f);
    }
    endShape();
    hint(ENABLE_DEPTH_TEST);
}

void keyPressed() {
    if (key == 'q') {
        //request_shutdown = true;
        exit();
    }
}