#include "Umfeld.h"
#include "Geometry.h"
#include "VertexBuffer.h"

using namespace umfeld;

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
    subsystem_graphics = umfeld_create_subsystem_graphics_openglv33();
}

void setup() {
    hint(ENABLE_SMOOTH_LINES);
}

void draw() {
    background(0.85f);

    if (!isMousePressed) {
        for (auto& v: mesh_shape.vertices_data()) {
            v.position.x += random(-1, 1);
            v.position.y += random(-1, 1);
            v.position.z += random(-1, 1);
        }
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
    g->debug_text("FPS   : " + nf(frameRate, 1), 10, 10);
    g->debug_text("SHAPES: " + to_string(mesh_shape.vertices_data().size()), 10, 25);
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
