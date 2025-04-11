/*
 * this example demonstrates how to integrate game controllers i.e Nintendo Joy-Con in detached mode.
 */

#include "Umfeld.h"
#include "Gamepad.h"
#include "PVector.h"

using namespace umfeld;

struct Circle {
    uint32_t color;
    PVector  position;
    PVector  speed;
    float    size;
};

Circle circle_red;
Circle circle_blue;

float move_speed = 10.0f;
float size_step  = 5.0f;

void settings() {
    size(1024, 768);
    enable_gamepads();
}

void setup() {
    circle_red.color = color(1.0f, 0.25f, 0.35f);
    circle_red.position.set(width / 2, height / 2 - 10);
    circle_red.size = 50;

    circle_blue.color = color(0.5f, 0.85f, 1.0f);
    circle_blue.position.set(width / 2, height / 2 + 10);
    circle_blue.size = 50;

    noStroke();
}

void draw() {
    circle_red.position.add(circle_red.speed);
    circle_blue.position.add(circle_blue.speed);

    background(0.85f);

    fill_color(circle_red.color);
    circle(circle_red.position.x, circle_red.position.y, circle_red.size);

    fill_color(circle_blue.color);
    circle(circle_blue.position.x, circle_blue.position.y, circle_blue.size);
}

void gamepadAxis(const int id, const int axis, const float value) {
    /*
     * right/red joy-con: ↑2↓ ←3→
     * left/blue joy-con: ↑0↓ ←1→
     */
    if (axis == 3) {
        circle_red.speed.x = -value;
    }
    if (axis == 2) {
        circle_red.speed.y = value;
    }
    if (axis == 1) {
        circle_blue.speed.x = value;
    }
    if (axis == 0) {
        circle_blue.speed.y = -value;
    }
}

void gamepadButton(const int id, const int button, const bool down) {
    /*
     * right/red joy-con: ↑2  ←0  →3  ↓1  + 8
     * left/blue joy-con: ↑14 ←11 →12 ↓13 + 7
     */
    if (down) {
        if (button == 2) {
            circle_red.size += size_step;
        }
        if (button == 1) {
            circle_red.size -= size_step;
        }
        if (button == 8) {
            circle_red.position.set(width / 2, height / 2 - 10);
        }
        if (button == 14) {
            circle_blue.size += size_step;
        }
        if (button == 13) {
            circle_blue.size -= size_step;
        }
        if (button == 7) {
            circle_blue.position.set(width / 2, height / 2 + 10);
        }
    }
}

void keyPressed() {
    if (key == ' ') {
        gamepad_connected();
    }
}