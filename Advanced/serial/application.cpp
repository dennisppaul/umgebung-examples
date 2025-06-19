#include "Umfeld.h"
#include "Serial.h"

using namespace umfeld;

// socat -d -d pty,raw,echo=0 pty,raw,echo=0 // create virtual serial ports
// # possbile output:
// > 2025/05/19 12:13:34 socat[58594] N PTY is /dev/ttys001
// > 2025/05/19 12:13:34 socat[58594] N PTY is /dev/ttys002
// echo -n "hello world" > /dev/ttys001 // send message to application
// screen /dev/ttys003 115200 // listen to the virtual serial port

Serial serial("/dev/ttys002", 115200);

void settings() {
    size(1024, 768);
}

void setup() {
    noFill();
    stroke(1.0f, 0.25f, 0.35f);
    printArray(Serial::list());
}

void update() {
    serial.poll(); // this needs to be called to receive data
}

void draw() {
    background(0.85f);
    const float size = 50.0f;
    const float x    = width / 2.0f;
    const float y    = height / 2.0f;
    line(x - size, y - size, x + size, y + size);
    line(x - size, y + size, x + size, y - size);

    if (serial.available() > 0) {
        std::string message = serial.readString();
        console("Received: ", message.c_str());
    }
}

void keyPressed() {
    if (key == 's') {
        console("Sending message");
        serial.write("hello world\n");
    }
}
