#include "Umgebung.h"
#include "Capture.h"

using namespace umgebung;

Capture* capture_facetime;
Capture* capture_iphone;

class MyCaptureListener final : public CaptureListener {
    void captureEvent(Capture* capture) override {
        if (capture == capture_facetime) {
            console("capture_facetime");
        }
        if (capture == capture_iphone) {
            console("capture_iphone");
        }
        // TODO @WIP updating texture in non-OpenGL thread does not work
    }
};

MyCaptureListener capture_listener;

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
    capture_facetime->set_listener(&capture_listener);
    capture_facetime->start();

    capture_iphone = new Capture();
    capture_iphone->init("d3Phone Camera",
                         "1280x720",
                         "30",
                         "nv12");
    capture_iphone->set_listener(&capture_listener);
    capture_iphone->start();
}

void draw() {
    background(0.2f);

    fill(1.0f);
    // TODO if run from thread ( i.e `myMovie->play();` ) this needs to be called in draw
    if (capture_facetime != nullptr) {
        capture_facetime->reload();
        image(capture_facetime, 0, 0, width, height);
    }
    if (capture_iphone != nullptr) {
        capture_iphone->reload();
        image(capture_iphone,
              mouseX, mouseY,
              capture_iphone->width / 4, capture_iphone->height / 4);
    }
}

void keyPressed() {
    if (key == 'q') {
        exit();
    }
    println((char) key, " pressed");
}