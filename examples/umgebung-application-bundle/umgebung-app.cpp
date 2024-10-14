#include "Umgebung.h"

using namespace umgebung;

class UmgebungApp : public PApplet {

    PFont*  mFont;
    int    mWidth  = 1024;
    int    mHeight = 768;

    void arguments(std::vector<std::string> args) override {
    }

    void settings() override {
        size(mWidth, mHeight);
    }

    void setup() override {
        if (!exists(sketchPath() + "../assets/RobotoMono-Regular.ttf")) {
            println("cannot find required files at:", sketchPath());
            println("... exiting");
            exit();
        }
        mFont = loadFont(sketchPath() + "../assets/RobotoMono-Regular.ttf", 48);
        textFont(mFont);
    }

    void draw() override {
        background(1.0f);

        noStroke();
        fill(0.0f);
        text("42", mouseX, mouseY);
    }
};

PApplet* umgebung::instance() {
    return new UmgebungApp();
}