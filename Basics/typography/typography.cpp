#include "Umgebung.h"
#include "CircularBuffer.h"

using namespace umgebung;

/*
 * this example shows how to render text and work with typographic attributes
 * from https://processing.org/reference/textAlign_.html
 */

// TODO implement functions from https://processing.org/reference/
//    - [x] PFont :: Grayscale bitmap font class used by Processing
//    - [x] createFont() :: Dynamically converts a font to the format used by Processing
//    - [x] loadFont() :: Loads a font into a variable of type PFont
//    - [x] textFont() :: Sets the current font that will be drawn with the text() function
//    - [x] text() :: Draws text to the screen
//    - [x] textAlign() :: Sets the current alignment for drawing text
//    - [x] textLeading() :: Sets the spacing between lines of text in units of pixels
//    - [\] textMode() :: Sets the way text draws to the screen
//    - [x] textSize() :: Sets the current font size
//    - [x] textWidth() :: Calculates and returns the width of any character or text string
//    - [x] textAscent() :: Returns ascent of the current font at its current size
//    - [x] textDescent() :: Returns descent of the current font at its current size

const int light_blue = color(0.5f, 0.85f, 1.0f);
const int soft_red   = color(1.0f, 0.25f, 0.35f);

PFont* font;

void settings() {
    size(1024, 768);
    antialiasing = 4;
}

void setup() {
    font = loadFont("../RobotoMono-Regular.ttf", 44);
    textFont(font);
}

void draw() {
    background(0.85f);

    pushMatrix();
    translate(100, 100);
    stroke(0);
    fill_color(light_blue);
    textSize(map(mouseX, 0, width, 10, 88));
    textAlign(CENTER, BOTTOM);
    line(0, 120, width - 200, 120);
    text("CENTER,BOTTOM", 200, 120);
    textAlign(CENTER, CENTER);
    line(0, 200, width - 200, 200);
    text("CENTER,CENTER", 200, 200);
    textAlign(CENTER, BASELINE);
    line(0, 280, width - 200, 280);
    text("CENTER,BASELINE", 200, 280);
    textAlign(CENTER, TOP);
    line(0, 360, width - 200, 360);
    text("CENTER,TOP", 200, 360);

    fill_color(soft_red);
    translate(width / 2, 0);
    textAlign(RIGHT);
    text("ABCD", 200, 120);
    textAlign(CENTER);
    text("EFGH", 200, 200);
    textAlign(LEFT);
    text("IJKL", 200, 280);
    textAlign(CENTER);
    textLeading(map(mouseY, 0, height, 0, 88));
    text(nf(textAscent(), 2) + "\n" + nf(textDescent(), 2), 200, 360);
    popMatrix();

    /* --- BONUS: still a bit EXPERIMENTAL but outlines can be extracted from fonts ( curves are not implemented yet ) --- */
    noFill();
    stroke(0);
    std::vector<std::vector<glm::vec2>> outlines;
    font->outline(nf(mouseY), outlines);
    pushMatrix();
    translate(mouseX, mouseY);
    for (const auto& outline: outlines) {
        beginShape(LINE_STRIP);
        for (const auto& v: outline) {
            vertex(v.x, v.y);
        }
        endShape();
    }
    popMatrix();
}
