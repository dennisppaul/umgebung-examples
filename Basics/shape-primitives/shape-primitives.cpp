#include <GL/glew.h>
#define STB_IMAGE_WRITE_IMPLEMENTATION
#include "stb_image_write.h"

#include "Umgebung.h"
#include "Geometry.h"

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

#include <string>

void saveFrame(const std::string& filename, const int width, const int height) {
    // Allocate memory for pixel data (RGBA)
    std::vector<unsigned char> pixels(width * height * 4);

    // Read pixels from OpenGL framebuffer (GL_RGBA, GL_UNSIGNED_BYTE)
    glReadPixels(0, 0, width, height, GL_RGBA, GL_UNSIGNED_BYTE, pixels.data());

    // glBindFramebuffer(GL_READ_FRAMEBUFFER, fboID); // Bind the correct framebuffer
    // glReadPixels(0, 0, width, height, GL_RGBA, GL_UNSIGNED_BYTE, pixels.data());
    // glBindFramebuffer(GL_READ_FRAMEBUFFER, 0); // Restore default framebuffer

    // Flip the image vertically because OpenGL's origin is bottom-left
    std::vector<unsigned char> flippedPixels(width * height * 4);
    for (int y = 0; y < height; ++y) {
        memcpy(&flippedPixels[(height - 1 - y) * width * 4], &pixels[y * width * 4], width * 4);
    }

    // Save as PNG using stb_image_write
    if (ends_with(filename, ".png")) {
        stbi_write_png((filename).c_str(), width, height, 4, flippedPixels.data(), width * 4);
    }
    if (ends_with(filename, ".jpg")) {
        stbi_write_jpg((filename).c_str(), width, height, 4, flippedPixels.data(), 100);
    }
}

void settings() {
    size(1024, 768);
    display            = 0;
    antialiasing       = 8;
    render_to_buffer   = true;
    retina_support     = true;
    subsystem_graphics = umgebung_create_subsystem_graphics_openglv33();
}

void setup() {
    hint(HINT_ENABLE_SMOOTH_LINES);
}

void draw() {
    background(0.85f);

    fill(0);
    g->debug_text("FPS: " + nf(frameRate, 1), 10, 10);
    g->debug_text(nf(mouseX, 0) + ", " + nf(mouseY, 0), 10, 20);

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
        saveFrame("screenshot-" + nfs(frameCount, 4) + ".png", width * 2, height * 2);
        saveFrame("screenshot-" + nfs(frameCount, 4) + ".jpg", width * 2, height * 2);
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
