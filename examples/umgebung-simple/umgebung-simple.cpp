#include "Umgebung.h"

using namespace umgebung;

PImage* m_image;
PFont*  font;

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
    display = 0;
    // fullscreen   = false;
    // borderless   = false;
    antialiasing = 8;
    // resizable    = false;
    // always_on_top         = true;
    retina_support = true;
    // headless              = false;
    // no_audio              = false;
    render_to_buffer   = false;
    subsystem_graphics = umgebung_create_subsystem_graphics_openglv33();
}

void setup() {
    add_generic_subsystem();

    hint(HINT_ENABLE_SMOOTH_LINES);

    m_image = loadImage(sketchPath() + "../image.png");
    font    = loadFont("../InterDisplay-Light.otf", 64);
    textFont(font);
}

void draw() {
    background(1.0); // TODO should this flush everything?!?

    noStroke();
    fill(1, 0, 0);
    g->debug_text("FPS: " + to_string(frameRate), 10, height - 20);

    /* rects */

    stroke(0);
    noFill();
    rect(10, 10, 40, 40);

    noStroke();
    fill(1, 0, 0);
    rect(60, 10, 40, 40);

    stroke(0);
    fill(1, 0, 0);
    rect(110, 10, 40, 40);

    /* circle ( + ellipse ) */

    stroke(0);
    noFill();
    circle(10 + 20, 60 + 20, 40);

    stroke(0);
    fill(1, 0, 0);
    circle(110 + 20, 60 + 20, 40);

    noStroke();
    fill(1, 0, 0);
    circle(60 + 20, 60 + 20, 40);

    stroke(0);
    noFill();
    ellipseDetail((int) (mouseX / width * 72));
    circle(mouseX, mouseY, 500);

    /* lines */

    stroke(0);
    strokeWeight(3);
    line(10, 10, mouseX, mouseY);
    strokeWeight(9);
    line(mouseX, mouseY, width - 10, height - 10);
    strokeWeight(1);

    /* bezier */

    stroke(0);
    strokeWeight(6);
    noFill();
    bezier(10, 10, mouseX, mouseY, mouseX, mouseY, width - 10, height - 10);
    strokeWeight(1);

    /* triangle */

    stroke(0);
    strokeWeight(3);
    fill(1, 0, 0);
    triangle(width * 0.66f + 5, height * 0.33f, 0,
             width * 0.66f + 5, height * 0.66f, 0,
             mouseX, mouseY, 0);
    strokeWeight(1);

    /* shape */

    stroke(0);
    strokeWeight(5);
    fill(1, 0, 0);
    beginShape(POLYGON);
    vertex(width * 0.33f, height * 0.33f);
    vertex(width * 0.66f, height * 0.33f);
    vertex(width * 0.66f, height * 0.66f);
    vertex(width * 0.33f, height * 0.66f);
    vertex(mouseX, mouseY);
    endShape(CLOSE);
    strokeWeight(1);

    /* image */

    fill(1);
    noStroke();
    pushMatrix();
    translate(width - m_image->width, m_image->height);
    rotateX(frameCount * 0.007f);
    rotateY(frameCount * 0.02f);
    rectMode(CENTER);
    image(m_image, 0, 0, 80, 80);
    rectMode(CORNER);
    popMatrix();

    /* textured shape */

    noStroke();
    fill(1);
    texture(m_image);
    // TODO textured polygon only works in `polygon_triangulation_strategy = POLYGON_TRIANGULATION_FASTER`
    // beginShape(POLYGON);
    // beginShape(QUADS);
    // vertex(10, 110, 0, 1, 1);
    // vertex(10 + m_image->width, 110, 0, 0, 1);
    // vertex(10 + m_image->width, 110 + m_image->height, 0, 0, 0);
    // // vertex(10, 110 + m_image->height, 0, 1, 0);
    // vertex(mouseX, mouseY, 0, 1, 0);
    // endShape(CLOSE);
    quad(10, 110, 0,
         10 + m_image->width, 110, 0,
         10 + m_image->width, 110 + m_image->height, 0,
         mouseX, mouseY, 0);

    /* points + pointSize */

    pointSize(mouseX / width * 64); // NOTE does not work on all machines ... and looks ugly
    stroke(0);
    for (int i = 0; i < 100; ++i) {
        const float x = random(110, 150);
        const float y = random(10, 50);
        point(x, y);
    }
    pointSize(1);

    /* text + font */

    fill(0);
    noStroke();
    text("AVTAWaToVAWeYoyo Hamburgefonts", mouseX, mouseY);
    pushMatrix();
    translate(300, 10 + 16 + 0.5f);
    scale(0.5f);
    // TODO change this to std::string
    text(to_string("( ", static_cast<int>(mouseX), ", ", static_cast<int>(mouseY), " )").c_str(), 0, 0);
    popMatrix();

    /* box + sphere */

    strokeJoin(NONE);
    strokeCap(NONE);
    stroke(0);
    strokeWeight(2);
    fill(1, 0, 0);
    pushMatrix();
    translate(mouseX, mouseY);
    rotateX(frameCount * 0.01f);
    rotateY(frameCount * 0.027f);
    box(100);
    sphere(120);
    popMatrix();
    strokeWeight(1);

    /* -------------------- CUT BELOW THE LINE --- */

    /* shape + transforms */

    // pushMatrix();

    // translate(180, 30);
    // rotate(PI / 4);
    // const float s = 20 / sqrtf(20 * 20 + 20 * 20);
    // scale(s);

    // stroke(1, 0, 0);
    // noFill();
    // beginShape(POLYGON);
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

    /* transformed lines */

    // stroke(1, 0, 0);
    // strokeWeight(10);
    // pushMatrix();
    // translate(mouseX, mouseY);
    // rotateX(frameCount * 0.01f);
    // scale(3);
    // beginShape(LINE_STRIP);
    // vertex(-50, -50, 0);
    // vertex(50, -50, 0);
    // vertex(50, 50, 0);
    // vertex(-50, 50, 0);
    // endShape(CLOSE);
    // // line(-50, -50, 0, 50, -50, 0);
    // // line(50, -50, 0, 50, 50, 0);
    // // line(50, 50, 0, -50, 50, 0);
    // // line(-50, 50, 0, -50, -50, 0);
    // popMatrix();
    // strokeWeight(1);

    // strokeWeight(20);
    // stroke(1, 0, 0);
    // fill(0, 1, 0);
    // fill(0);
    // beginShape(POINTS);
    // vertex(width * 0.5f - 200, height * 0.5f - 200);
    // vertex(width * 0.5f + 200, height * 0.5f - 200);
    // vertex(width * 0.5f + 200, height * 0.5f + 200);
    // vertex(width * 0.5f - 200, height * 0.5f + 200);
    // vertex(width * 0.5f - 300, height * 0.5f + 20);
    // vertex(mouseX, mouseY);
    // endShape(CLOSE);
    // strokeWeight(1);

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
    if (key == ',') {
        g->stroke_mode(STROKE_RENDER_MODE_TRIANGULATE);
    }
    if (key == '.') {
        g->stroke_mode(STROKE_RENDER_MODE_NATIVE);
    }
}

void mouseMoved() {
    // console((int) mouseX, "x", (int) mouseY);
}

void shutdown() {
    println("finishing");
}