#include "Umgebung.h"
#include "Movie.h"

using namespace umgebung;

class UmgebungExampleAppWithMovie : public PApplet {

    Movie* movie;

    void settings() override {
        size(1024, 768);
    }

    void setup() override {
        movie = new Movie("../video.mp4");
        movie->play();
        movie->loop();
    }

    void draw() override {
        background(0.2);
        // if (myMovie->available()) {
        //     myMovie->read();
        // }
        fill(1);
        movie->reload(); // TODO if run from thread ( i.e `myMovie->play();` ) this needs to be called in draw
        image(movie, mouseX, mouseY);
    }

    void keyPressed() override {
        if (key == 'q') {
            exit();
        }
        if (key == 'p') {
            movie->play();
        }
        if (key == 's') {
            movie->pause();
        }
    }
};

PApplet* umgebung::instance() {
    return new UmgebungExampleAppWithMovie();
}