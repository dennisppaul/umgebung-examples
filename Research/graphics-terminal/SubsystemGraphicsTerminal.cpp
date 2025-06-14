/*
 * Umfeld
 *
 * This file is part of the *Umfeld* library (https://github.com/dennisppaul/umfeld).
 * Copyright (c) 2025 Dennis P Paul.
 *
 * This library is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, version 3.
 *
 * This library is distributed in the hope that it will be useful, but
 * WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU
 * General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program. If not, see <http://www.gnu.org/licenses/>.
 */

#include <ncurses.h>
#include <cmath>
#include <unistd.h>

#include "Umfeld.h"
#include "PGraphicsTerminal.h"

extern int ESCDELAY;

namespace umfeld {

    static MEVENT curses_event;

    /* --- Subsystem --- */

    static void set_flags(uint32_t& subsystem_flags) {}

    static bool init() { return true; }

    static void setup_pre() {
        if (g == nullptr) { return; }
        g->init(nullptr, width, height, 4, false);

        ESCDELAY = 25;
        initscr();
        noecho();
        cbreak();
        curs_set(0);
        keypad(stdscr, TRUE);
        timeout(50);

        // Enable mouse events
        // mousemask(BUTTON1_PRESSED | BUTTON1_RELEASED | REPORT_MOUSE_POSITION, NULL);
        mousemask(ALL_MOUSE_EVENTS | REPORT_MOUSE_POSITION, nullptr);

        // Required in many terminals to receive mouse move events
        printf("\033[?1003h\n"); // Enable mouse tracking (Xterm)
        fflush(stdout);          // flush to apply

        if (has_colors()) { // TODO this should be handle in PGraphicsTerminal
            start_color();
            init_pair(1, COLOR_WHITE, COLOR_BLACK);
            attron(COLOR_PAIR(1));
        }

        // TODO ugly hack!!!
        resizeterm(0, 0);
        int rows, cols;
        getmaxyx(stdscr, rows, cols);
        width  = cols;
        height = rows;
        if (g != nullptr) {
            g->width  = width;
            g->height = height;
        }
    }

    static void setup_post() {}

    static void update_loop() {}

    static void draw_pre() {}

    static void draw_post() {
        if (g != nullptr) {
            g->endDraw();
        }

        // int rows, cols;
        // getmaxyx(stdscr, rows, cols);
        // mvaddstr(3, 0, to_string(rows, ", ", cols).c_str());

        const int ch = getch();

        if (ch == KEY_MOUSE && getmouse(&curses_event) == OK) {
            mouseX = curses_event.x;
            mouseY = curses_event.y;
        }

        if (use_esc_key_to_quit) {
            if (ch == 27) {
                request_shutdown = true;
            }
        }
    }

    static void shutdown() {
        printf("\033[?1003l\n"); // Disable mouse tracking
        fflush(stdout);
        endwin();
        console("terminal graphics shutdown.");
    }

    static void event(SDL_Event* event) {}

    static void event_in_update_loop(SDL_Event* event) {}

    static const char* name() { return "TERMINAL"; }

    /* --- SubsystemGraphics --- */

    static PGraphics* create_native_graphics(const bool render_to_offscreen) {
        return new PGraphicsTerminal();
    }

    static void post() {}

    static void set_title(std::string& title) {}

    static std::string get_title() { return ""; }

    static void set_window_position(const int x, const int y) {}

    static void get_window_position(int& x, int& y) {}

    static void set_window_size(const int width, const int height) {}

    static void get_window_size(int& width, int& height) {
        int rows, cols;
        getmaxyx(stdscr, rows, cols);
        width  = cols;
        height = rows;
    }

    static SDL_Window* get_sdl_window() { return nullptr; }

    static void* get_renderer() { return nullptr; }

    static int get_renderer_type() { return RENDERER_TERMINAL; }

} // namespace umfeld

umfeld::SubsystemGraphics* umfeld_create_subsystem_graphics_terminal() {
    auto* graphics                   = new umfeld::SubsystemGraphics{};
    graphics->set_flags              = umfeld::set_flags;
    graphics->init                   = umfeld::init;
    graphics->setup_pre              = umfeld::setup_pre;
    graphics->setup_post             = umfeld::setup_post;
    graphics->update_loop            = umfeld::update_loop;
    graphics->draw_pre               = umfeld::draw_pre;
    graphics->draw_post              = umfeld::draw_post;
    graphics->shutdown               = umfeld::shutdown;
    graphics->event                  = umfeld::event;
    graphics->event_in_update_loop   = umfeld::event_in_update_loop;
    graphics->name                   = umfeld::name;
    graphics->create_native_graphics = umfeld::create_native_graphics;
    graphics->post                   = umfeld::post; // TODO maybe remove this, as there is also a `draw_post()` method
    graphics->set_title              = umfeld::set_title;
    graphics->get_title              = umfeld::get_title;
    graphics->set_window_size        = umfeld::set_window_size;
    graphics->get_window_size        = umfeld::get_window_size;
    graphics->set_window_position    = umfeld::set_window_position;
    graphics->get_window_position    = umfeld::get_window_position;
    graphics->get_sdl_window         = umfeld::get_sdl_window;
    graphics->get_renderer           = umfeld::get_renderer;
    graphics->get_renderer_type      = umfeld::get_renderer_type;
    return graphics;
}
