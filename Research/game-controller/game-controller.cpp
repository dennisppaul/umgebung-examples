#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <SDL3/SDL.h>

#include "Umgebung.h"
#include "Geometry.h"

using namespace umgebung;

#define MOTION_EVENT_COOLDOWN 40

int frame_counter = 0;

Subsystem* subsystem;

void add_generic_subsystem() {
    subsystem            = new Subsystem();
    subsystem->set_flags = [](uint32_t& subsystem_flags) -> void {
        console("--- subsystem: set_flags");
        subsystem_flags |= SDL_INIT_GAMEPAD;
    };
    subsystem->init = []() -> bool {
        console("--- subsystem: init");
        int count = 0;
        SDL_GetGamepads(&count);
        console("--- gamepads: ", count);
        // for (int i = 0; i < count; i++) {
        //     SDL_Gamepad* controller = SDL_OpenGamepad(i);
        //     console(SDL_GetError());
        //     if (controller) {
        //         console("--- controller: ", SDL_GetGamepadName(controller));
        //     }
        // }
        return true;
    };
    subsystem->shutdown = []() {
        console("--- subsystem: shutdown");
    };
    subsystem->event = [](SDL_Event* event) {
        if (event->type == SDL_EVENT_GAMEPAD_ADDED) {
            const SDL_JoystickID which   = event->gdevice.which;
            SDL_Gamepad*         gamepad = SDL_OpenGamepad(which);
            if (!gamepad) {
                console(which, "Gamepad #%u add, but not opened: %s", (unsigned int) which, SDL_GetError());
            } else {
                console(which, "Gamepad #%u ('%s') added", (unsigned int) which, SDL_GetGamepadName(gamepad));
            }
        }
        if (event->type == SDL_EVENT_GAMEPAD_REMOVED) {
            const SDL_JoystickID which   = event->gdevice.which;
            SDL_Gamepad*         gamepad = SDL_GetGamepadFromID(which);
            if (gamepad) {
                SDL_CloseGamepad(gamepad); /* the joystick was unplugged. */
            }
            console(which, "Gamepad #%u removed", (unsigned int) which);
        }
        if (event->type == SDL_EVENT_GAMEPAD_AXIS_MOTION) {
            static Uint64 axis_motion_cooldown_time = 0; /* these are spammy, only show every X milliseconds. */
            const Uint64  now                       = SDL_GetTicks();
            if (now >= axis_motion_cooldown_time) {
                const SDL_JoystickID which = event->gaxis.which;
                axis_motion_cooldown_time  = now + MOTION_EVENT_COOLDOWN;
                console("Gamepad #%u axis %d -> %d", which, ": ", (int) event->gaxis.axis, ", ", (int) event->gaxis.value);
            }
        }
        if (event->type == SDL_EVENT_JOYSTICK_BUTTON_UP || event->type == SDL_EVENT_JOYSTICK_BUTTON_DOWN) {
            const SDL_JoystickID which = event->gbutton.which;
            console("Gamepad #%u button %d -> %s", which, ": ", (int) event->gbutton.button, ", ", event->gbutton.down ? "PRESSED" : "RELEASED");
        }
        if (event->type == SDL_EVENT_GAMEPAD_TOUCHPAD_MOTION) {
            const SDL_JoystickID which = event->gbutton.which;
            console("Gamepad #%u button %d -> %s", which, ": ", (int) event->gtouchpad.touchpad);
        }
        // console("--- subsystem: '", static_cast<char>(event->key.key), "'");
    };
    subsystems.push_back(subsystem);
}

void settings() {
    size(1024, 768);
    add_generic_subsystem();
}

void setup() {
}

void draw() {
    background(0.85f);

    fill(0);
    noStroke();
    g->debug_text("FPS: " + nf(frameRate, 2), 20, 20);

    fill(0.5f, 0.85f, 1.0f, 0.5f);
    box(400, 400, 10);

    rotateX(frame_counter * 0.01f);
    rotateY(frame_counter * 0.027f);
    rotateZ(frame_counter * 0.043f);

    if (isKeyPressed && key == ' ') {
        frame_counter++;
    }

    popMatrix();
}

void keyPressed() {
    if (key == '1') {
    }
    if (key == '2') {
    }
    if (key == 'r') {
        frame_counter = 0;
    }
}