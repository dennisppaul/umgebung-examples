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
    display      = 0;
    fullscreen   = false;
    borderless   = false;
    antialiasing = 8;
    resizable    = false;
    // always_on_top         = true;
    retina_support = false;
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

    hint(HINT_ENABLE_SMOOTH_LINES);

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
    fill(1, 0, 0);
    rect(60, 10, 40, 40);

    /* circle ( + ellipse ) */

    noStroke();
    fill(1, 0, 0);
    circle(10 + 20, 60 + 20, 40);

    stroke(1, 0, 0);
    noFill();
    circle(mouseX, mouseY, 40);

    /* lines + bezier */

    strokeWeight(1);
    noFill();
    stroke(0);
    line(width, 0, 0, height);
    line(0, 0, width, height);

    stroke(1, 0, 0);
    bezier(0, height, 0, mouseX, mouseY, 0, mouseX, mouseY, 0, width, 0, 0);
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

    // pushMatrix();

    // translate(180, 30);
    // rotate(PI / 4);
    // const float s = 20 / sqrtf(20 * 20 + 20 * 20);
    // scale(s);

    // stroke(1, 0, 0);
    // noFill();
    // beginShape(POLYGON); // TODO POLYGON only drawn as stroke ATM
    // vertex(-20, -20);
    // vertex(20, -20);
    // vertex(20, 20);
    // vertex(-20, 20);
    // endShape(CLOSE);

    // translate(40, 40, 0);
    // stroke(0);
    // noFill();
    // beginShape(POLYGON);
    // vertex(-20, -20);
    // vertex(20, -20);
    // vertex(20, 20);
    // vertex(-20, 20);
    // endShape(CLOSE);

    // popMatrix();

    /* image */

    fill(1);
    image(mImage, 210, 10, 80, 80);

    /* text + font */

    fill(0);
    text(to_string("(", static_cast<int>(mouseX), ",", static_cast<int>(mouseY), ")").c_str(), 300, 10 + 16);

    // TODO add 3D shapes

    stroke(1, 0, 0);
    strokeWeight(10);
    pushMatrix();
    translate(mouseX, mouseY);
    rotateX(frameCount * 0.01f);
    scale(3);
    beginShape(LINE_STRIP);
    vertex(-50, -50, 0);
    vertex(50, -50, 0);
    vertex(50, 50, 0);
    vertex(-50, 50, 0);
    endShape(CLOSE);
    // line(-50, -50, 0, 50, -50, 0);
    // line(50, -50, 0, 50, 50, 0);
    // line(50, 50, 0, -50, 50, 0);
    // line(-50, 50, 0, -50, -50, 0);
    popMatrix();
    strokeWeight(1);

    strokeWeight(20);
    stroke(1, 0, 0);
    fill(0, 1, 0);
    beginShape(LINE_STRIP);
    vertex(width * 0.5 - 200, height * 0.5 - 200);
    vertex(width * 0.5 + 200, height * 0.5 - 200);
    vertex(width * 0.5 + 200, height * 0.5 + 200);
    vertex(width * 0.5 - 200, height * 0.5 + 200);
    vertex(width * 0.5 - 300, height * 0.5 + 20);
    vertex(mouseX, mouseY);
    endShape(CLOSE);
    strokeWeight(1);

    // strokeWeight(10);
    // stroke(0);
    // beginShape();
    // vertex(10, height / 2);
    // vertex(width / 2, height / 2);
    // vertex(mouseX, mouseY);
    // vertex(width - 10, height / 2);
    // endShape();
    // strokeWeight(1);
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