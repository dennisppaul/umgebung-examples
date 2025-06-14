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

#pragma once

#include "PGraphics.h"

namespace umfeld {
    class PGraphicsTerminal final : public PGraphics {
    public:
        bool simple_clear = false;

        void set_point(const std::string& character) {
            if (character.empty()) {
                ch = DEFAULT_POINT_CHAR;
            } else {
                ch = character;
            }
        }

        void auto_refresh(bool auto_refresh) {
            // No need to implement auto-refresh for terminal graphics
            auto_refresh_enabled = auto_refresh;
        }

        void background(const float r, const float g, const float b, const float d) override {
            if (simple_clear) {
                clear();
                return;
            }

            if (!has_colors()) {
                clear();
                return;
            }

            // Convert float RGB [0,1] to ncurses RGB [0,1000]
            const short red   = static_cast<short>(r * 1000);
            const short green = static_cast<short>(g * 1000);
            const short blue  = static_cast<short>(b * 1000);

            // Use a custom color slot (e.g., 10) and a custom color pair (e.g., 1)
            constexpr short COLOR_CUSTOM      = 10;
            constexpr short COLOR_PAIR_CUSTOM = 1;

            if (can_change_color()) {
                init_color(COLOR_CUSTOM, red, green, blue);
                init_pair(COLOR_PAIR_CUSTOM, COLOR_BLACK, COLOR_CUSTOM);
            } else {
                init_pair(COLOR_PAIR_CUSTOM, COLOR_WHITE, COLOR_BLACK);
            }

            attron(COLOR_PAIR(COLOR_PAIR_CUSTOM));

            for (int y = 0; y < LINES; ++y) {
                move(y, 0);
                for (int x = 0; x < COLS; ++x) {
                    addch(' ');
                }
            }

            // attroff(COLOR_PAIR(COLOR_PAIR_CUSTOM));
            move(0, 0);
            refresh();
        }

        void point(const float x, const float y, float z) override {
            if (x >= 0 && y >= 0 && x < COLS && y < LINES) {
                // mvaddch(y, x, ch);
                mvaddstr(y, x, ch.c_str());
            }
        }

        void point(const float x, const float y) {
            point(x, y, 0);
        }

        void line(float x1, float y1, const float z1, const float x2, const float y2, const float z2) override {
            const int dx = std::abs(x2 - x1), dy = -std::abs(y2 - y1);
            const int sx  = (x1 < x2) ? 1 : -1;
            const int sy  = (y1 < y2) ? 1 : -1;
            int       err = dx + dy;

            while (true) {
                point(x1, y1);
                if (x1 == x2 && y1 == y2) {
                    break;
                }
                const int e2 = 2 * err;
                if (e2 >= dy) {
                    err += dy;
                    x1 += sx;
                }
                if (e2 <= dx) {
                    err += dx;
                    y1 += sy;
                }
            }
        }

        void rect(const float x, const float y, const float width, const float height) override {
            for (int i = x; i < x + width; ++i) {
                point(i, y);
                point(i, y + height - 1);
            }
            for (int i = y; i < y + height; ++i) {
                point(x, i);
                point(x + width - 1, i);
            }
        }

        void text_str(const std::string& text, const float x, const float y, const float z) override {
            mvaddstr(y, x, text.c_str());
        }

        void endDraw() override {
            if (auto_refresh_enabled) {
                refresh();
            }
            PGraphics::endDraw();
        }

        void IMPL_background(float a, float b, float c, float d) override {}
        void IMPL_bind_texture(int bind_texture_id) override {}
        void IMPL_set_texture(PImage* img) override {}
        void IMPL_emit_shape_fill_triangles(std::vector<Vertex>& triangle_vertices) override {}
        void IMPL_emit_shape_stroke_points(std::vector<Vertex>& point_vertices, float point_size) override {}
        void IMPL_emit_shape_stroke_line_strip(std::vector<Vertex>& line_strip_vertices, bool line_strip_closed) override {}

    private:
        inline static const std::string DEFAULT_POINT_CHAR = "*";

        std::string ch                   = DEFAULT_POINT_CHAR;
        bool        auto_refresh_enabled = true;
    };
} // namespace umfeld