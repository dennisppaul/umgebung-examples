#include "Umgebung.h"

using namespace umgebung;

void arguments(const std::vector<std::string>& args) {}

void settings() {
    size(1024, 768);
    display            = 0;
    antialiasing       = 8;
    render_to_buffer   = false;
    retina_support     = true;
    subsystem_graphics = umgebung_subsystem_graphics_create_openglv33();
}

void setup() {
    hint(HINT_ENABLE_SMOOTH_LINES);

    stroke(0.0f);
    fill(0.5f, 0.85f, 1.0f);
    strokeWeight(20);
}

void draw() {
    background(1.0f);

    beginShape(POLYGON);
    vertex(120, 80);
    vertex(230, 80);
    vertex(mouseX, mouseY);
    vertex(340, 190);
    vertex(340, 300);
    vertex(120, 300);
    endShape(CLOSE);
}