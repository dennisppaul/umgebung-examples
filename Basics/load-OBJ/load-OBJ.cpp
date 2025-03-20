#include "Umgebung.h"
#include "PMesh.h"

using namespace umgebung;

void settings() {
    size(1024, 768);
}

PMesh* mesh_shape;
int    number_vertices = 0;

void setup() {
    hint(ENABLE_DEPTH_TEST);

    const std::vector<Vertex> vertices = loadOBJ("../Panda.obj");
    number_vertices                    = vertices.size();
    mesh_shape                         = new PMesh();
    mesh_shape->add_vertices(vertices);
    mesh_shape->update();
}

void draw() {
    background(0.85f);

    if (isMousePressed) {
        mesh_shape->set_shape(LINES);
    } else {
        mesh_shape->set_shape(TRIANGLES);
    }

    hint(DISABLE_DEPTH_TEST);
    fill(0);
    g->debug_text("FPS     : " + nf(frameRate, 1), 10, 10);
    g->debug_text("VERTICES: " + nf(number_vertices, 1), 10, 25);

    hint(ENABLE_DEPTH_TEST);
    pushMatrix();
    translate(width * 0.5f, height * 0.75f);
    rotateX(PI);
    rotateX(sin(frameCount * 0.07f) * 0.07f);
    rotateY(sin(frameCount * 0.1f) * 0.1f);
    rotateZ(sin(frameCount * 0.083f) * 0.083f);
    scale(75);

    mesh(mesh_shape);

    pushMatrix();
    translate(2, 0);
    scale(0.4);
    rotateY(-0.7);
    mesh(mesh_shape);
    popMatrix();

    noStroke();
    fill(0.5f, 0.85f, 1.0f);
    pushMatrix();
    rotateX(HALF_PI);
    square(-3, -3, 6);
    popMatrix();

    popMatrix();
}
