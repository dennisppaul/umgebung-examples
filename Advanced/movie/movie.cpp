#include "Umfeld.h"
#include "Movie.h"

using namespace umfeld;


class MyMovieListener final : public MovieListener {
    void movieVideoEvent(Movie* m, float* audio_buffer, int length, int channels) override {}

    void movieAudioEvent(Movie* m, float* audio_buffer, int length, int channels) override {
        std::cout << "+++ audio samples : " << length << std::endl;
        std::cout << "+++       channels: " << channels << std::endl;
    }
};

Movie*          movie;
MyMovieListener movie_listener;

void settings() {
    size(1024, 768);
}

void setup() {
    movie = new Movie("../video.mp4");
    movie->play();
    movie->loop();
    movie->set_listener(&movie_listener);
}

void draw() {
    background(0.2f);
    // if (myMovie->available()) {
    //     myMovie->read();
    // }
    fill(1.0f);
    movie->reload(); // TODO if run from thread ( i.e `myMovie->play();` ) this needs to be called in draw
    image(movie, mouseX, mouseY);
}

void keyPressed() {
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
