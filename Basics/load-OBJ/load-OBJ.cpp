#include "Umgebung.h"
#include "VertexBuffer.h"

/*
 * this example shows how to load an OBJ and display it as a mesh.
 */

using namespace umgebung;

void settings() {
    size(1024, 768);
}

VertexBuffer* mesh_shape;
int           number_vertices = 0;

void setup() {
    hint(ENABLE_DEPTH_TEST);

    const std::vector<Vertex> vertices = loadOBJ("../Panda.obj");
    number_vertices                    = vertices.size();
    mesh_shape                         = new VertexBuffer();
    mesh_shape->add_vertices(vertices);
}

void draw() {
    background(0.85f);

    if (isMousePressed) {
        mesh_shape->set_shape(LINES);
    } else {
        mesh_shape->set_shape(TRIANGLES);
    }

    fill(0);
    debug_text("FPS     : " + nf(frameRate, 1), 10, 10);
    debug_text("VERTICES: " + nf(number_vertices, 1), 10, 25);

    pushMatrix();
    translate(width * 0.5f, height * 0.75f);
    rotateX(PI);
    rotateY(PI);
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

    if (isKeyPressed) {
        for (auto& v: mesh_shape->vertices_data()) {
            v.position.x += random(-0.02, 0.02);
            v.position.y += random(-0.02, 0.02);
            v.position.z += random(-0.02, 0.02);
        }
        mesh_shape->update();
    }
}
