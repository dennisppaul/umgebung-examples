#include "Umgebung.h"
#include "Capture.h"

using namespace umgebung;

class UmgebungApp : public PApplet {

    Capture* capture;

    void settings() {
        size(1280, 720);
    }

    void setup() {
        // const std::vector<DeviceCapability> capabilities = getDeviceCapabilities();
        // for (const auto& capability: capabilities) {
        //     std::cout
        //         << capability.deviceName << " "
        //         << " resolution: " << capability.width << "x" << capability.height
        //         << " frame rate: " << capability.minFrameRate << "-" << capability.maxFrameRate
        //         << std::endl;
        // }

        std::vector<std::string> devices;
        if (Capture::print_available_devices(devices)) {
            uint32_t i = 0;
            for (const auto& device: devices) {
                std::cout << "[" << i << "]"
                          << " " << device << std::endl;
                i++;
            }
        } else {
            std::cerr << "No capture devices found." << std::endl;
        }

        Capture::list_capabilities("FaceTime HD Camera");

        capture = new Capture("FaceTime HD Camera",
                              "1280x720",
                              "30",
                              "0rgb");
        capture->play();
    }

    void draw() {
        background(0.2);

        fill(1);
        if (capture != nullptr) {
            capture->reload(); // TODO if run from thread ( i.e `myMovie->play();` ) this needs to be called in draw
            image(capture, 0, 0, width, height);
            image(capture, mouseX, mouseY,
                  capture->width / 4, capture->height / 4);
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