#include "Umgebung.h"

using namespace umgebung;

class UmgebungApp : public PApplet {

    PImage*    mImage;
    PGraphics* pg_ptr;

    void settings() {
        size(1024, 768);
    }

    void setup() {
        if (!exists(sketchPath() + "../image.png")) {
            println("cannot find required files at:", sketchPath());
            println("... exiting");
            exit();
        }
        mImage = loadImage(sketchPath() + "../image.png");
        /* erase pixel buffer ( to red ) */
        for (int i = 0; i < mImage->width; ++i) {
            for (int j = 0; j < mImage->height; ++j) {
                mImage->set(i, j, color(1, 0, 0));
            }
        }
        /* do not upload pixel buffer to texture buffer with `mImage->updatePixels();` */
        /* recover pixel buffer from texture buffer */
        mImage->loadPixels();
        /* invert pixels */
        for (int i = 0; i < mImage->width; ++i) {
            for (int j = 0; j < mImage->height; ++j) {
                uint32_t c = mImage->get(i, j);
                c          = color(1 - red(c), 1 - green(c), 1 - blue(c), alpha(c));
                mImage->set(i, j, c);
            }
        }
        mImage->updatePixels();

        pg_ptr = createGraphics(128, 128);
    }

    void draw() {
        background(0.2);

        fill(1.0f);
        image(mImage, 10, 10);

        loadPixels();
        updatePixels();
        fill(1, 0.5);
        image(this, mImage->width + 20, 10, width / 4, height / 4);

        PGraphics& pg = *pg_ptr;
        pg.beginDraw();
        pg.background(0);
        pg.noStroke();
        pg.fill(1.0f);
        pg.rect(10, 10, 20, 20);
        pg.rect(10, 30, 10, 10);
        pg.stroke(1.0f);
        pg.line(10, 10, pg.width * (mouseX / (float) width), pg.height * (mouseY / (float) height));
        pg.endDraw();

        fill(1, 0.5);
        image(pg_ptr, mouseX, mouseY, pg.width, pg.height);

        fill(1.0f);
        texture(mImage);
        beginShape(QUADS);
        vertex(140, 80, 0, 0, 0);
        vertex(420, 20, 0, 1, 0);
        vertex(480, 360, 0, 1, 1);
        vertex(260, 380, 0, 0, 1);
        endShape();
    }
};

PApplet* umgebung::instance() {
    return new UmgebungApp();
}