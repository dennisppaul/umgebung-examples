#include "Umgebung.h"
#include "Capture.h"

using namespace umgebung;

class UmgebungApp : public PApplet, public CaptureListener {

    Capture* capture;

    void settings() {
        size(1280, 720);
    }

    void setup() {
        const std::vector<std::string> devices = Capture::list();
        if (!devices.empty()) {
            uint32_t i = 0;
            for (const auto& device: devices) {
                std::cout
                    << "[" << i << "]"
                    << " " << device
                    << std::endl;
                i++;
            }
            for (const auto& device: devices) {
                std::cout
                    << "Capabilities for " << device
                    << std::endl;
                Capture::list_capabilities(device);
            }
        } else {
            std::cerr << "No capture devices found." << std::endl;
        }

        capture = new Capture();
        capture->init("FaceTime HD Camera",
                      "1280x720",
                      "30",
                      "nv12");
        capture->set_listener(this);
        capture->start();
    }

    void draw() {
        background(0.2);

        fill(1);
        if (capture != nullptr) {
            capture->reload(); // TODO if run from thread ( i.e `myMovie->play();` ) this needs to be called in draw
            image(capture, 0, 0, width, height);
            image(capture,
                  mouseX, mouseY,
                  capture->width / 4, capture->height / 4);
        }
    }

    void captureEvent(Capture* capture) {
        // WIP
    }

    void keyPressed() {
        if (key == 'q') {
            exit();
        }
        println((char) key, " pressed");
    }
};

PApplet* umgebung::instance() {
    return new UmgebungApp();
}