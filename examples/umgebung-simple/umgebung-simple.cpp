#include "Umgebung.h"

using namespace umgebung;

PImage* mImage;
PFont*  mFont;

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
            console("subsystem: '", static_cast<char>(event->key.key), "' pressed");
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
    antialiasing = 8;
    // resizable             = true;
    // always_on_top         = true;
    // retina_support = true;
    // headless              = false;
    // no_audio              = false;
    subsystem_graphics = umgebung_subsystem_graphics_create_openglv33();
}

void setup() {
    println("width      : ", width);
    println("height     : ", height);
    println("pixelWidth : ", pixelWidth);
    println("pixelHeight: ", pixelHeight);
    add_generic_subsystem();

    mImage = loadImage(sketchPath() + "../image.png");
    mFont  = loadFont(sketchPath() + "../JetBrainsMono-Regular.ttf", 16);
    textFont(mFont);
}

void draw() {
    background(1); // TODO would this flush everything?!?

    /* rects */

    stroke(0.0f);
    noFill();
    rect(10, 10, 40, 40);

    noStroke();
    fill(0);
    rect(60, 10, 40, 40);

    /* circle ( + ellipse ) */

    fill(1, 0, 0);
    circle(mouseX, mouseY, 40);

    /* lines + bezier */

    noFill();

    stroke(0);
    line(width, 0, 0, height);
    line(0, 0, width, height);

    stroke(1, 0, 0);
    bezier(0, height, mouseX, mouseY, mouseX, mouseY, width, 0);
    bezier(width, height, mouseX, mouseY, mouseX, mouseY, 0, 0);

    /* points + pointSize */

    pointSize(2);
    stroke(0);
    for (int i = 0; i < 100; ++i) {
        const float x = random(110, 150);
        const float y = random(10, 50);
        point(x, y);
    }
    pointSize(1);

    /* shape + transforms */

    noStroke(); // TODO stroke shape do not work ATM
    fill(1, 0, 0);
    pushMatrix();
    translate(180, 30);
    rotate(PI / 4);
    const float s = 20 / sqrtf(20 * 20 + 20 * 20);
    scale(s);
    beginShape(POLYGON);
    vertex(-20, -20);
    vertex(20, -20);
    vertex(20, 20);
    vertex(-20, 20);
    endShape();
    popMatrix();

    /* image */

    fill(1);
    image(mImage, 210, 10, 80, 80);

    /* text + font */

    fill(0);
    text(to_string("(", static_cast<int>(mouseX), ",", static_cast<int>(mouseY), ")").c_str(), 300, 10 + 16);
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