#include "Umgebung.h"
#include "PGraphics.h"

using namespace umgebung;

void arguments(const std::vector<std::string>& args) {
    if (args.size() == 2) {
        println("arguments: ", args[0], ", ", args[1]);
    }
    // for (std::string& s: args) {
    //     println("> ", s);
    //     if (begins_with(s, "--width")) {
    //         mWidth = get_int_from_argument(s);
    //     }
    //     if (begins_with(s, "--height")) {
    //         mHeight = get_int_from_argument(s);
    //     }
    // }
}

Subsystem* subsystem;

void add_generic_subsystem() {
    subsystem       = new Subsystem();
    subsystem->init = []() -> bool {
        console("subsystem: init");
        return true;
    };
    subsystem->setup_pre = []() {
        console("subsystem: setup_pre");
    };
    subsystem->shutdown = []() {
        console("subsystem: shutdown");
    };
    subsystem->event = [](SDL_Event* event) {
        if (event->type == SDL_EVENT_KEY_DOWN) {
            console("subsystem: ", (char) event->key.key, " pressed");
        }
    };
    subsystems.push_back(subsystem);
}

void settings() {
    size(1024, 768);
    // audio_input_device  = DEFAULT_AUDIO_DEVICE;
    // audio_output_device = DEFAULT_AUDIO_DEVICE;
    // audio_input_channels  = DEFAULT_NUMBER_OF_INPUT_CHANNELS;
    // audio_output_channels = DEFAULT_NUMBER_OF_OUTPUT_CHANNELS;
    display = 0;
    // fullscreen            = true;
    // borderless            = true;
    // antialiasing          = 8;
    // resizable             = true;
    // always_on_top         = true;
    // retina_support = true;
    // headless              = false;
    // no_audio              = false;
}

void setup() {
    println("width      : ", width);
    println("height     : ", height);
    println("pixelWidth : ", pixelWidth);
    println("pixelHeight: ", pixelHeight);
    add_generic_subsystem();
}

void draw() {
    background(1);

    stroke(0.0f);
    noFill();
    rect(10, 10, width / 2 - 20, height / 2 - 20);

    noStroke();
    fill(random(0, 0.2));
    rect(20, 20, width / 2 - 40, height / 2 - random(20, 40));
}

// void audioblock(float** input, float** output, int length) {
//     for (int i = 0; i < length; i++) {
//         float sample = random(-0.1, 0.1);
//         for (int j = 0; j < audio_output_channels; ++j) {
//             output[j][i] = sample;
//         }
//     }
// }

void keyPressed() {
    if (key == 'q') {
        exit();
    }
    println((char) key, " pressed");
}

void mouseMoved() {}

void shutdown() {
    delete subsystem;
    println("finishing");
}