/*
 * this example shows how to draw a mesh by using a vertex buffer. the mesh data is
 * uploaded to the GPU and rendered in 3D space. the vertices can be dynamically
 * added to the mesh, and the mesh will be updated accordingly. this is very fast
 * for high numbers of vertices.
 */

// TODO adding vertices dynamically is currently not working on Windows

#include "Umfeld.h"
#include "Geometry.h"
#include "VertexBuffer.h"

using namespace umfeld;

VertexBuffer mesh_shape;

void settings() {
    size(1024, 768);
}

void setup() {
    hint(ENABLE_SMOOTH_LINES);
    mesh_shape.set_shape(TRIANGLES);
    for (int i = 0; i < 2048; ++i) {
        mesh_shape.add_vertex(Vertex(glm::vec3(width / 2 + random(-10, 10), height / 2 + random(-10, 10), random(-10, 10)),
                                     glm::vec4(random(1.0f), random(1.0f), random(1.0f), 1.0f),
                                     glm::vec3(0.0f)));
    }
    mesh_shape.update(); // NOTE this forces an update of the vertex buffer … it also called on first draw if required
}

void draw() {
    background(0.85f);

    if (!isMousePressed) {
        for (auto& v: mesh_shape.vertices_data()) {
            v.position.x += random(-1, 1);
            v.position.y += random(-1, 1);
            v.position.z += random(-1, 1);
        }
        // TODO adding vertices dynamically is currently not working on Windows
        for (int i = 0; i < 256; ++i) {
            mesh_shape.add_vertex(Vertex(glm::vec3(mouseX + random(-10, 10), mouseY + random(-10, 10), random(-10, 10)),
                                         glm::vec4(random(1.0f), random(1.0f), random(1.0f), 1.0f),
                                         glm::vec3(0.0f)));
        }
        // NOTE vertices are uploaded to GPU next time mesh/vertexbuffer is drawn
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
    debug_text("FPS   : " + nf(frameRate, 1), 10, 10);
    debug_text("SHAPES: " + to_string(mesh_shape.vertices_data().size()), 10, 25);
}

void keyPressed() {
    if (key == ' ') {
        mesh_shape.clear();
    }
    if (key == '1') {
        mesh_shape.set_shape(TRIANGLES);
    }
    if (key == '2') {
        mesh_shape.set_shape(LINES);
    }
    if (key == '3') {
        mesh_shape.set_shape(LINE_STRIP);
    }
}
