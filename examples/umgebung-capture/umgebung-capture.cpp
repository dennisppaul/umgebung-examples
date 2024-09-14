#include "Umgebung.h"
#include "Capture.h"

using namespace umgebung;

class UmgebungApp : public PApplet {

    Capture* capture;

    void settings() {
        size(1024, 768);
    }

    void setup() {
        std::vector<std::string> devices;
        if (Capture::print_available_devices(devices)) {
            for (const auto& device: devices) {
                std::cout << "Available device: " << device << std::endl;
            }
        } else {
            std::cerr << "Failed to list devices" << std::endl;
        }

        capture = new Capture();
        capture->connect("FaceTime HD Camera",
                         "1920x1080",
                         "30",
                         "0rgb");
    }

    void draw() {
        background(0.2);

        fill(1);
        if (capture != nullptr) {
            image(capture, mouseX, mouseY);
        }
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