#include "Umgebung.h"
#include "Movie.h"

using namespace umgebung;

class UmgebungExampleAppWithMovie : public PApplet, MovieListener {

    Movie* movie;

    void settings() override {
        size(1024, 768);
    }

    void setup() override {
        movie = new Movie("../video.mp4");
        movie->play();
        movie->loop();
        movie->set_listener(this);
    }

    void draw() override {
        background(0.2f);
        // if (myMovie->available()) {
        //     myMovie->read();
        // }
        fill(1.0f);
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

    void movieVideoEvent(Movie* m, float* audio_buffer, int length, int channels) override {}

    void movieAudioEvent(Movie* m, float* audio_buffer, int length, int channels) override {
        std::cout << "+++ audio samples : " << length << std::endl;
        std::cout << "+++       channels: " << channels << std::endl;
    }
};

PApplet* umgebung::instance() {
    return new UmgebungExampleAppWithMovie();
}