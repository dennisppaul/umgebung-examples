#include "Umgebung.h"
#include "PGraphics.h"
#include "Geometry.h"

using namespace umgebung;

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

int stroke_join_mode = ROUND;

float stroke_weight = 30;

void settings() {
    size(1024, 768);
    display            = 0;
    antialiasing       = 8;
    render_to_buffer   = false;
    retina_support     = true;
    subsystem_graphics = umgebung_subsystem_graphics_create_openglv33();
}

void setup() {
    hint(HINT_ENABLE_SMOOTH_LINES);
}


constexpr float max_angle = 165.0f;

void umgebung::create_stroke_join_tessellate(std::vector<glm::vec2>&     triangles,
                                             const bool                  close_shape,
                                             const float                 half_width,
                                             const int                   stroke_join_mode,
                                             const std::vector<Segment>& segments) {
    std::vector<glm::vec2> outline_left;
    std::vector<glm::vec2> outline_right;

    const int num_Segments = segments.size() + (close_shape ? +1 : 0);
    for (int i = 0; i < num_Segments; ++i) {
        const Segment& s1 = segments[(i) % segments.size()];
        const Segment& s2 = segments[(i + 1) % segments.size()];

        const glm::vec2 s1_norm  = s1.normal * half_width;
        const glm::vec2 s1_left  = s1.position - s1_norm;
        const glm::vec2 s1_right = s1.position + s1_norm;
        const glm::vec2 s2_left  = s2.position - s1_norm;
        const glm::vec2 s2_right = s2.position + s1_norm;
        const glm::vec2 s2_norm  = s2.normal * half_width;

        outline_left.push_back(s1_left);
        outline_left.push_back(s2_left);
        outline_right.push_back(s1_right);
        outline_right.push_back(s2_right);

        /* ROUND */

        if (stroke_join_mode == ROUND) {
            // skip last one ( creates a cap )
            if (i != num_Segments - 1) {
                constexpr float circle_resolution = glm::radians(20.0f); // 20° resolution i.e 18 segment for whole circle
                float           angle_s1          = atan2(s1.normal.y, s1.normal.x);
                float           angle_s2          = atan2(s2.normal.y, s2.normal.x);
                // compute the signed angular difference
                float angular_diff = angle_s2 - angle_s1;
                while (angular_diff <= -PI) {
                    angular_diff += TWO_PI;
                }
                while (angular_diff > PI) {
                    angular_diff -= TWO_PI;
                }
                float r_offset = (angular_diff > 0) ? PI : 0.0f;
                // flip the start/end if segment goes in the wrong direction
                if (angular_diff < 0) {
                    std::swap(angle_s1, angle_s2);
                    angular_diff = -angular_diff;
                }
                for (float r = angle_s1; r <= angle_s1 + angular_diff; r += circle_resolution) {
                    glm::vec2 circle_segment = s2.position + glm::vec2{
                                                                 cos(r + r_offset),
                                                                 sin(r + r_offset)} *
                                                                 half_width;
                    outline_left.push_back(circle_segment);
                    // TODO check if it would be better to add this to right side depending on angle
                }
            }
        }

        /* MITER */

        if (stroke_join_mode == MITER) {
            const float angle     = glm::degrees(angle_between_vectors(s1.direction, s2.direction));
            const float abs_angle = fabs(angle);
            if (abs_angle < max_angle && angle > 0) {
                const glm::vec2 s2_proj_left = s2.position - s2_norm;
                glm::vec2       intersection_left;
                const bool      result_left = intersect_lines(s1_left, s1.direction,
                                                              s2_proj_left, s2.direction,
                                                              intersection_left);
                if (result_left) {
                    outline_left.push_back(intersection_left);
                }
            }
            if (abs_angle < max_angle && angle < 0) {
                const glm::vec2 p2_proj_right = s2.position + s2_norm;
                glm::vec2       intersection_right;
                const bool      result_right = intersect_lines(s1_right, s1.direction,
                                                               p2_proj_right, s2.direction,
                                                               intersection_right);
                if (result_right) {
                    outline_right.push_back(intersection_right);
                }
            }
        }
    }
    std::vector<glm::vec2> polygon_outline;
    polygon_outline.insert(polygon_outline.end(), outline_left.begin(), outline_left.end());
    polygon_outline.insert(polygon_outline.end(), outline_right.rbegin(), outline_right.rend());
    tessellate_polygon(polygon_outline, triangles);
}

void draw() {
    background(1.0f);

    strokeWeight(stroke_weight);
    stroke(0.0f);
    fill(0.5f, 0.85f, 1.0f);
    beginShape(POLYGON);
    vertex(120, 80);
    vertex(230, 80);
    vertex(mouseX, mouseY);
    vertex(340, 190);
    vertex(340, 300);
    vertex(120, 300);
    endShape(is_mouse_pressed);

    std::vector<glm::vec3> points;
    points.emplace_back(120, 80, 0);
    points.emplace_back(230, 80, 0);
    points.emplace_back(mouseX, mouseY, 0);
    points.emplace_back(340, 190, 0);
    points.emplace_back(340, 300, 0);
    points.emplace_back(120, 300, 0);
    const std::vector<glm::vec2> triangles = line_strip(points,
                                                        is_mouse_pressed,
                                                        stroke_weight,
                                                        stroke_join_mode);

    const std::vector colors = {
        glm::vec4{1, 0, 0, 0.5},
        glm::vec4{0, 1, 0, 0.5},
        glm::vec4{0, 0, 1, 0.5},
    };

    int i = 0;
    noStroke();
    fill(0.0f, 0.5f);
    beginShape(TRIANGLES);
    for (const auto t: triangles) {
        const int       ii = (i / 3) % colors.size();
        const glm::vec4 c  = colors[ii];
        fill(c.r, c.g, c.b, c.a);
        vertex(t.x, t.y);
        i++;
    }
    endShape();
}

void keyPressed() {
    if (key == '-') {
        stroke_weight--;
        if (stroke_weight < 0) { stroke_weight = 0; }
    }
    if (key == '+') {
        stroke_weight++;
    }
    if (key == '1') {
        stroke_join_mode = NONE;
    }
    if (key == '2') {
        stroke_join_mode = BEVEL;
    }
    if (key == '3') {
        stroke_join_mode = MITER;
    }
    if (key == '4') {
        stroke_join_mode = ROUND;
    }
    if (key == '5') {
        stroke_join_mode = BEVEL_TESSELATOR;
    }
}