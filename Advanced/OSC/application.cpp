// TODO add callback for application

/*
 * this example shows how to use the OSC to send and receive OSC messages.
 */

#include "Umfeld.h"
#include "OSC.h"

using namespace umfeld;

class MOSCListener final : public OSCListener {
public:
    bool message_event = false;

    void receive(const OscMessage& msg) override {
        if (msg.typetag() == "ifs") {
            println("received address pattern: ",
                    msg.addrPattern(),
                    " : ",
                    msg.typetag(),
                    "(",
                    msg.get(0).intValue(),
                    ", ",
                    msg.get(1).floatValue(),
                    ", ",
                    msg.get(2).stringValue(),
                    ")");
            message_event = true;
        } else {
            println("could not parse OSC message: ", msg.typetag());
        }
    }
};

MOSCListener osc_listener;

OSC mOSC{"127.0.0.1", 7000, 7001};
int message_counter = 0;

void settings() {
    size(1024, 768);
}

void setup() {
    mOSC.callback(&osc_listener);
    background(0.0f);
}

void draw() {
    if (osc_listener.message_event) {
        osc_listener.message_event = false;
        fill(1.0f);
        constexpr int num_rects = 20;
        const float   size_rect = width / num_rects;
        const float   x         = (message_counter % num_rects) * size_rect;
        const float   y         = (message_counter / num_rects) * size_rect;
        rect(x, y, size_rect, size_rect);
        message_counter++;
        if (message_counter > num_rects * num_rects) {
            message_counter = 0;
            background(0.0f);
        }
    }
}

void keyPressed() {
    if (key == 's') {
        std::cout << "send OSC message" << std::endl;
        mOSC.send("/test_send_1", 23, "hello", 42);

        OscMessage msg("/test_send_2");
        msg.add(mouseY);
        mOSC.send(msg, NetAddress("localhost", 8000));

        osc_listener.message_event = true;
    }
}
