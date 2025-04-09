#include "Umfeld.h"
#include "PGraphics.h"

// TODO coordinates are flipped

using namespace umfeld;

PGraphics* pg = nullptr;

void settings() {
    size(200, 200);
    // TODO for debugging, remove these ASAP
    render_to_buffer   = true;
    antialiasing       = 0;
    subsystem_graphics = umfeld_create_subsystem_graphics_openglv33();
}

void setup() {
    noFill();
    stroke(1.0f, 0.25f, 0.35f);
    pg = createGraphics(100, 100, OPENGL);
}

void draw() {
    background(0.85f);

    pg->beginDraw();
    pg->background(0.4f);
    pg->stroke(1.0f);
    pg->line(pg->width * 0.5, pg->height * 0.5, mouseX, mouseY);
    pg->endDraw();
    fill(1);
    image(pg, 50, 50);

    const float size = 50.0f;
    const float x    = width / 2.0f;
    const float y    = height / 2.0f;
    line(x - size, y - size, x + size, y + size);
    line(x - size, y + size, x + size, y - size);
}
