#include "Umgebung.h"
#include "PGraphics.h"
#include "Geometry.h"

using namespace umgebung;

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

int stroke_join_mode = MITER;
int stroke_cap_mode  = ROUND;

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


constexpr float stroke_join_miter_max_angle  = 165.0f;
constexpr float stroke_cap_round_resolution  = glm::radians(20.0f); // 20° resolution i.e 18 segment for whole circle
constexpr float stroke_join_round_resolution = glm::radians(20.0f); // 20° resolution i.e 18 segment for whole circle

void add_cap(const float half_width,
             const int   stroke_cap_mode,
             const float direction, std::vector<glm::vec2>& poly_list,
             const Segment& cap_seg) {
    /* POINTED */
    if (stroke_cap_mode == POINTED) {
        const glm::vec2 cap_point_project = cap_seg.position - cap_seg.direction * half_width * direction;
        poly_list.push_back(cap_point_project);
        fill(0, 0.5f, 1);
        circle(cap_point_project.x, cap_point_project.y, 10);
    }
    /* PROJECT */
    if (stroke_cap_mode == PROJECT) {
        const glm::vec2 cap_point_project = cap_seg.position - cap_seg.direction * half_width * direction;
        const glm::vec2 cap_norm          = cap_seg.normal * half_width;
        poly_list.push_back(cap_point_project + cap_norm);
        poly_list.push_back(cap_point_project - cap_norm);
    }
    /* ROUNG */
    if (stroke_cap_mode == ROUND) {
        const float angle       = atan2(cap_seg.normal.y, cap_seg.normal.x) - 3.0f * HALF_PI * direction;
        const float angle_start = angle - HALF_PI;
        const float angle_end   = angle + HALF_PI;
        for (float r = angle_start; r <= angle_end; r += stroke_cap_round_resolution) {
            glm::vec2 circle_segment = cap_seg.position + glm::vec2{
                                                              cos(r),
                                                              sin(r)} *
                                                              half_width;
            poly_list.push_back(circle_segment);
            fill(0, 0.5f, 1);
            circle(circle_segment.x, circle_segment.y, 3);
        }
    }
    /* SQUARE */
    // NOTE nothing to do here
}
void umgebung::create_stroke_join_tessellate(std::vector<glm::vec2>&     triangles,
                                             const bool                  close_shape,
                                             const float                 half_width,
                                             const int                   stroke_join_mode,
                                             const int                   stroke_cap_mode,
                                             const std::vector<Segment>& segments) {
    std::vector<glm::vec2> outline_left;
    std::vector<glm::vec2> outline_right;
    outline_left.reserve(segments.size());
    outline_right.reserve(segments.size());

    /* CAP START */
    if (!close_shape) {
        constexpr int           seg_index = 0;
        constexpr float         direction = 1.0f;
        std::vector<glm::vec2>& poly_list = outline_left;
        const Segment           cap_seg   = segments[seg_index];
        add_cap(half_width, stroke_cap_mode, direction, poly_list, cap_seg);
    }

    const int num_segments = segments.size() + (close_shape ? +1 : 0);
    for (int i = 0; i < num_segments; ++i) {
        const Segment& s1 = segments[(i) % segments.size()];
        const Segment& s2 = segments[(i + 1) % segments.size()];

        stroke(0);
        fill(0);
        circle(s1.position.x, s1.position.y, 10);
        noFill();
        circle(s2.position.x, s2.position.y, 15);

        const glm::vec2 s1_norm  = s1.normal * half_width;
        const glm::vec2 s1_left  = s1.position - s1_norm;
        const glm::vec2 s1_right = s1.position + s1_norm;
        const glm::vec2 s2_left  = s2.position - s1_norm;
        const glm::vec2 s2_right = s2.position + s1_norm;
        const glm::vec2 s2_norm  = s2.normal * half_width;

        fill(1, 0, 0);
        circle(s1_left.x, s1_left.y, 10 + i * 2);
        fill(0, 1, 0);
        circle(s1_right.x, s1_right.y, 10 + i * 2);

        noFill();
        stroke(1, 0, 0);
        circle(s2_left.x, s2_left.y, 10 + i * 2);
        stroke(0, 1, 0);
        circle(s2_right.x, s2_right.y, 10 + i * 2);

        if (close_shape) {
            outline_left.push_back(s1_left);
            outline_left.push_back(s2_left);
            outline_right.push_back(s1_right);
            outline_right.push_back(s2_right);
        } else {
            outline_left.push_back(s1_left);
            outline_right.push_back(s1_right);
            if (i < num_segments - 1) { // works for round & bevel but not vor miter
                outline_left.push_back(s2_left);
                outline_right.push_back(s2_right);
            }
        }

        /* ROUND */

        if (stroke_join_mode == ROUND) {
            // skip last one ( creates a cap )
            if (i != num_segments - 1) {
                float angle_s1 = atan2(s1.normal.y, s1.normal.x);
                float angle_s2 = atan2(s2.normal.y, s2.normal.x);
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
                for (float r = angle_s1; r <= angle_s1 + angular_diff; r += stroke_join_round_resolution) {
                    glm::vec2 circle_segment = s2.position + glm::vec2{
                                                                 cos(r + r_offset),
                                                                 sin(r + r_offset)} *
                                                                 half_width;
                    outline_left.push_back(circle_segment);
                    // TODO check if it would be better to add this to right side depending on angle
                }
            }
        }

        /* BEVEL */

        // NOTE no need to do anything special

        /* MITER */

        if (stroke_join_mode == MITER) {
            if (close_shape || i < num_segments - 1) {
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
    }

    /* CAP END */
    if (!close_shape) {
        const int               seg_index = segments.size() - 1;
        constexpr float         direction = -1.0f;
        std::vector<glm::vec2>& poly_list = outline_right;
        const Segment           cap_seg   = segments[seg_index];
        add_cap(half_width, stroke_cap_mode, direction, poly_list, cap_seg);

        // /* POINTED */
        // constexpr float         direction = -1.0f;
        // std::vector<glm::vec2>& poly_list = outline_right;
        // /* POINTED */
        // if (stroke_cap_mode == POINTED) {
        //     const Segment s_start   = segments[segments.size() - 1];
        //     glm::vec2     cap_start = s_start.position - s_start.direction * half_width * direction;
        //     poly_list.push_back(cap_start);
        //     fill(0, 0.5f, 1);
        //     circle(cap_start.x, cap_start.y, 10);
        // }
        // /* PROJECT */
        // /* ROUNG */
        // if (stroke_cap_mode == ROUND) {}
        // /* SQUARE */
        // // NOTE do nothing
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
    vertex(220, 80);
    vertex(330, 80);
    vertex(mouseX, mouseY);
    vertex(440, 190);
    vertex(440, 300);
    vertex(220, 300);
    vertex(120, 190);
    vertex(120, 80);
    endShape(!is_mouse_pressed);

    std::vector<glm::vec3> points;
    points.emplace_back(220, 80, 0);
    points.emplace_back(330, 80, 0);
    points.emplace_back(mouseX, mouseY, 0);
    points.emplace_back(440, 190, 0);
    points.emplace_back(440, 300, 0);
    points.emplace_back(220, 300, 0);
    points.emplace_back(120, 190, 0);
    points.emplace_back(120, 80, 0);
    std::vector<glm::vec2> triangles;
    line_strip(points,
               is_mouse_pressed,
               stroke_weight,
               stroke_join_mode,
               stroke_cap_mode,
               triangles);

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
        // fill(c.r, c.g, c.b, c.a);
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
        console("NONE");
    }
    if (key == '2') {
        stroke_join_mode = BEVEL;
        console("BEVEL");
    }
    if (key == '3') {
        stroke_join_mode = MITER;
        console("MITER");
    }
    if (key == '4') {
        stroke_join_mode = ROUND;
        console("ROUND");
    }
    if (key == '5') {
        stroke_join_mode = MITER_FAST;
        console("MITER_FAST");
    }
    if (key == '6') {
        stroke_join_mode = BEVEL_FAST;
        console("BEVEL_FAST");
    }
    if (key == 'q') {
        stroke_cap_mode = POINTED;
        console("POINTED");
    }
    if (key == 'w') {
        stroke_cap_mode = PROJECT;
        console("PROJECT");
    }
    if (key == 'e') {
        stroke_cap_mode = ROUND;
        console("ROUND");
    }
    if (key == 'r') {
        stroke_cap_mode = SQUARE;
        console("SQUARE");
    }
}