#include "Umgebung.h"
#include "Geometry.h"
#include "PMesh.h"

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
float stroke_weight    = 15.0f;

PMesh mesh_shape;

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
    // mesh_shape = new PMesh();
}

void draw() {
    background(0.85f);

    for (auto& v: mesh_shape.vertices_data()) {
        v.position.x += random(-1, 1);
        v.position.y += random(-1, 1);
        v.position.z += random(-1, 1);
    }
    if (!isMousePressed) {
        for (int i = 0; i < 256; ++i) {
            mesh_shape.add_vertex(Vertex(glm::vec3(mouseX + random(-10, 10), mouseY + random(-10, 10), random(-10, 10)),
                                          glm::vec4(random(1.0f), random(1.0f), random(1.0f), 1.0f),
                                          glm::vec2(0.0f)));
        }
        mesh_shape.update();
    }

    pushMatrix();
    translate(width * 0.5f, height * 0.5f);
    rotateX(sin(frameCount * 0.07f) * 0.07f);
    rotateY(sin(frameCount * 0.1f) * 0.1f);
    rotateZ(sin(frameCount * 0.083f) * 0.083f);
    translate(-width * 0.5f, -height * 0.5f);
    mesh(&mesh_shape);
    popMatrix();

    fill(0);
    g->debug_text("FPS: " + nf(frameRate, 1), 10, 10);
    g->debug_text(nf(mouseX, 0) + ", " + nf(mouseY, 0), 10, 20);
    g->debug_text(to_string(mesh_shape.vertices_data().size()), 10, 30);

    stroke(0.0f);
    fill(0.5f, 0.85f, 1.0f);
    strokeWeight(stroke_weight);
    pointSize(stroke_weight);
    strokeJoin(stroke_join_mode);
    strokeCap(stroke_cap_mode);

    scale(0.66667f);

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
    pushMatrix();
    translate(120, 80);
    rotateY(frameCount * 0.027f);
    rotateZ(frameCount * 0.01f);
    translate(-120, -80);
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
    popMatrix();

    translate(280, 0);
    bezier(120, 80, 120, 300, 340, 80, 340, 300);

    translate(280, 0);
    circle(230, 190, 220);
}

void keyPressed() {
    if (key == ' ') {
        mesh_shape.clear();
    }
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
    if (key == ',') {
        g->stroke_mode(STROKE_RENDER_MODE_TRIANGULATE);
    }
    if (key == '.') {
        g->stroke_mode(STROKE_RENDER_MODE_NATIVE);
    }
}
