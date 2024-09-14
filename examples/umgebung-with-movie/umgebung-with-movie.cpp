#include "Umgebung.h"
#include "Movie.h"

using namespace umgebung;

class UmgebungExampleAppWithMovie : public PApplet {

    Movie*  myMovie;
    PImage* mImage;

    void settings() override {
        size(1024, 768);
    }

    void setup() override {
        myMovie = new Movie("../video.mp4");
        myMovie->play();
        myMovie->loop();

        mImage = loadImage(sketchPath() + "../image.png");
    }

    void draw() override {
        background(0.2);
        // if (myMovie->available()) {
        //     myMovie->read();
        // }
        fill(1);
        myMovie->reload(); // TODO if run from thread ( i.e `myMovie->play();` ) this needs to be called in draw
        image(myMovie, mouseX, mouseY);
        image(mImage, 10, 10);
    }

    void keyPressed() override {
        if (key == 'q') {
            exit();
        }
        if (key == 'p') {
            myMovie->play();
        }
        if (key == 's') {
            myMovie->pause();
        }
    }
};

PApplet* umgebung::instance() {
    return new UmgebungExampleAppWithMovie();
}