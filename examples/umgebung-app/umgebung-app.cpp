#include "Umgebung.h"

using namespace umgebung;

class UmgebungApp : public PApplet {

    PFont*  mFont;
    PImage* mImage;
    PVector mVector{16, 16};
    PShape  mShape;
    int     mouseMoveCounter = 0;

    void settings() {
        size(1024, 768);
        audio_devices(DEFAULT_AUDIO_DEVICE, DEFAULT_AUDIO_DEVICE);
        antialiasing          = 8;
        enable_retina_support = true;
        headless              = false;
        no_audio              = false;
        monitor               = DEFAULT;
    }

    void setup() {
        if (!exists(sketchPath() + "../RobotoMono-Regular.ttf") || !exists(sketchPath() + "../image.png")) {
            println("cannot find required files at:", sketchPath());
            println("... exiting");
            exit();
        }
        if (!headless) {
            mImage = loadImage(sketchPath() + "../image.png");

            uint32_t pixels[64 * 64];
            for (int i = 0; i < 64 * 64; ++i) {
                pixels[i] = color(random(1), random(1), random(1), 0.5f);
            }
            mImage->update(pixels, 64, 64, 32, 32);

            mFont = loadFont(sketchPath() + "../RobotoMono-Regular.ttf", 48);
            textFont(mFont);
        }

        /* fill PShape with triangles */
        for (int i = 0; i < 81; ++i) {
            mShape.beginShape(TRIANGLES);
            mShape.vertex(random(width / 16.0), random(width / 16.0), 0, random(1), random(1), random(1));
            mShape.endShape();
        }

        println("width : ", width);
        println("height: ", height);
    }

    void draw() {
        if (headless) {
            return;
        }
        background(1);

        /* rectangle */
        const float padding = width / mVector.x;
        const float grid    = width / mVector.x;
        const float spacing = grid + width / mVector.x * 2;

        stroke(1, 0, 0);
        noFill();
        rect(padding, padding, grid, grid);

        noStroke();
        fill(0, 1, 0);
        rect(padding + spacing, padding, grid, grid);

        stroke(0.75f);
        fill(0, 0, 1);
        rect(padding + 2 * spacing, padding, grid, grid);

        /* line */
        stroke(0.0f);
        line(padding + 3 * spacing, padding, padding + 3 * spacing + grid, padding + grid);
        line(padding + 3 * spacing, padding + grid, padding + 3 * spacing + grid, padding);
        /* text + nf + push/popMatrix */
        fill(0.0f);
        noStroke();
        textSize(48);
        text("23", padding + 4 * spacing, padding + grid);

        pushMatrix();
        translate(mouseX, mouseY);
        rotate(PI * 0.25);
        textSize(11);
        fill(0, 0.5, 1);
        text(to_string((int) mouseX, ", ", (int) mouseY, " > ", nf(mouseMoveCounter * 0.01, 2)), 0, 0);
        popMatrix();

        /* image */
        constexpr int length = 64 * 64 * 4; // number of channels is always 4
        float         pixels[length];
        for (int i = 0; i < length; i += 4) {
            const float c = (i + frameCount) % 252 / 255.0;
            pixels[i + 0] = c;
            pixels[i + 1] = 1.0 - c;
            pixels[i + 2] = 1.0;
            pixels[i + 3] = 0.5;
        }
        mImage->update(pixels, 64, 64, 32, 32);

        fill(1.0f);
        image(mImage, padding, padding + spacing, grid, grid);
        image(mImage, padding + spacing, padding + spacing);

        /* noise + point */
        pushMatrix();
        translate(padding, padding + 2 * spacing);
        for (int i = 0; i < grid * grid; ++i) {
            float x    = i % (int) grid;
            float y    = i / grid;
            float grey = noise(x / (float) grid, y / (float) grid);
            stroke(grey);
            point(x, y, 1);
        }
        popMatrix();

        fill(1, 0, 0);
        beginShape();
        vertex(padding, padding + 3 * spacing);
        vertex(padding, padding + 3 * spacing + grid);
        vertex(padding + grid, padding + 3 * spacing + grid);
        vertex(padding + grid, padding + 3 * spacing);
        endShape();

        pushMatrix();
        translate(padding + spacing, padding + 3 * spacing);
        mShape.draw();
        popMatrix();

        noFill();
        stroke(1.0f, 0.5f, 0);
        line(340, 80, 40, 40);
        line(360, 360, 60, 320);
        stroke(0.0f);
        strokeWeight(20);
        bezier(340, 80, 40, 40, 360, 360, 60, 320);
        strokeWeight(1);
    }

    void audioblock(float** input, float** output, int length) {
        // NOTE length is the number of samples per channel
        // TODO change to `void audioblock(float** input_signal, float** output_signal) {}`
        static float phase     = 0.0;
        const float  frequency = 220.0 + sin(frameCount * 0.1) * 110.0;
        const float  amplitude = 0.5;

        for (int i = 0; i < length; i++) {
            float sample = amplitude * sin(phase);
            phase += (TWO_PI * frequency) / DEFAULT_AUDIO_SAMPLE_RATE;

            if (phase >= TWO_PI) {
                phase -= TWO_PI;
            }

#ifdef USE_INTERLEAVED_BUFFER
            float mInput = 0;
            for (int j = 0; j < audio_input_channels; ++j) {
                mInput += input[i * audio_input_channels + j];
            }
            for (int j = 0; j < audio_output_channels; ++j) {
                output[i * audio_output_channels + j] = sample + mInput * 0.5f;
            }
#else
            float mInput = 0;
            for (int j = 0; j < audio_input_channels; ++j) {
                mInput += input[j][i];
            }
            for (int j = 0; j < audio_output_channels; ++j) {
                output[j][i] = sample + mInput * 0.5f;
            }
#endif
        }
    }

    void keyPressed() {
        if (key == 'q') {
            exit();
        }
        println((char) key, " pressed");
    }

    void mouseMoved() {
        mouseMoveCounter++;
    }

    void mousePressed() {
        println("mouse pressed");
    }

    void finish() {
        println("application shutting down");
    }

    void dropped(std::string file_name) {
        println("dropped file: ", file_name);
    }
};

PApplet* umgebung::instance() {
    return new UmgebungApp();
}