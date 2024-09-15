#include "Umgebung.h"
#include "Capture.h"

using namespace umgebung;

class UmgebungApp : public PApplet, public CaptureListener {

    Capture* capture_facetime;
    Capture* capture_iphone;

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

        capture_facetime = new Capture();
        capture_facetime->init("FaceTime HD Camera",
                               "1280x720",
                               "30",
                               "nv12");
        capture_facetime->set_listener(this);
        capture_facetime->start();

        capture_iphone = new Capture();
        capture_iphone->init("d3Phone Camera",
                             "1280x720",
                             "30",
                             "nv12");
        capture_iphone->set_listener(this);
        capture_iphone->start();
    }

    void draw() {
        background(0.2);

        fill(1);
        if (capture_facetime != nullptr && capture_iphone != nullptr) {
            // TODO if run from thread ( i.e `myMovie->play();` ) this needs to be called in draw
            capture_facetime->reload();
            capture_iphone->reload();
            image(capture_facetime, 0, 0, width, height);
            image(capture_iphone,
                  mouseX, mouseY,
                  capture_iphone->width / 4, capture_iphone->height / 4);
        }
    }

    void captureEvent(Capture* capture) {
        // WIP TODO updating texture in non-OpenGL thread does not work
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