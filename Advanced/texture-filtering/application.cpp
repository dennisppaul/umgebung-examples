#include "Umfeld.h"

using namespace umfeld;

PImage* umfeld_image;

void settings() {
    size(1024, 768);
    antialiasing = 8;
}

void setup() {
    umfeld_image = loadImage("umfeld-logotype-64.png");
    rectMode(CENTER);
    noStroke();
}

void draw() {
    background(0.85f);

    fill(0.0f);
    debug_text("FPS: " + nf(frameRate, 3, 1), 10, 10);

    fill(1.0f);
    image(umfeld_image, mouseX - 256, mouseY, 512, 512);

    texture(umfeld_image);                // set texture to the loaded image
    texture_filter(NEAREST);              // set to NEAREST for pixelated effect
    rect(mouseX + 256, mouseY, 512, 512); // draw rectangle with texture
    texture_filter(LINEAR);               // reset to LINEAR for smooth texture filtering
    texture();                            // reset texture to default
}

// NOTE to test wrapping modes ( e.g REPEAT, CLAMP_TO_EDGE, ... ) the texture coordinates need to be manipulated e.g:
// ```
// stroke(1.0f, 1.0f, 1.0f, 0.0f); // use `stroke` to set the border color
// texture_wrap(CLAMP_TO_BORDER); // set texture wrap to clamp to border
// constexpr float texture_scale = 4.0f;
// beginShape(QUADS);
// vertex(mouseX, mouseY - 256, 0, 0, 0);
// vertex(mouseX + 512, mouseY - 256, 0, texture_scale, 0);
// vertex(mouseX + 512, mouseY + 256, 0, texture_scale, texture_scale);
// vertex(mouseX, mouseY + 256, 0, 0, texture_scale);
// endShape();
// ```
