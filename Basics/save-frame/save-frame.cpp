#include "Umgebung.h"
#include "Geometry.h"

using namespace umgebung;

/*
 * this example shows how to use `saveFrame()` to save the current frame to a file.
 */

void settings() {
    size(1024, 768);
    display            = 0;
    antialiasing       = 8;
    render_to_buffer   = true;
    retina_support     = true;
    subsystem_graphics = umgebung_create_subsystem_graphics_openglv33();
}

void setup() {
    hint(ENABLE_SMOOTH_LINES);
}

void draw() {
    background(0.85f);

    fill(0);
    g->debug_text("FPS: " + nf(frameRate, 1), 10, 10);
    g->debug_text(nf(mouseX, 0) + ", " + nf(mouseY, 0), 10, 20);

    stroke(0.0f);
    fill(0.5f, 0.85f, 1.0f);
    strokeWeight(15);
    strokeJoin(ROUND);
    strokeCap(ROUND);

    translate(width / 2, height / 2);
    rotateY(frameCount * 0.027f);
    rotateZ(frameCount * 0.01f);

    beginShape();
    vertex(-110, -110);
    vertex(110, -110);
    vertex(110, 110);
    vertex(-110, 110);
    endShape(CLOSE);
}

void keyPressed() {
    if (key == '1') {
        saveFrame();
    }
    if (key == '2') {
        const std::string save_path = selectFolder("Choose a folder");
        if (!save_path.empty()) {
            saveFrame(save_path + "fast-uncompressed-frame.bmp");
        }
    }
}
