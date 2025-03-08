#include "Umgebung.h"
#include "PGraphics.h"

using namespace umgebung;


#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>


void add_quad_as_triangles(std::vector<glm::vec3>& triangles, glm::vec3& p1, glm::vec3& p2, glm::vec3& p3, glm::vec3& p4) {
    triangles.emplace_back(p1);
    triangles.emplace_back(p2);
    triangles.emplace_back(p4);
    triangles.emplace_back(p4);
    triangles.emplace_back(p2);
    triangles.emplace_back(p3);
}

std::vector<glm::vec3> render_line_strip_as_connected_quads(std::vector<glm::vec3>& points,
                                                            const bool              close_shape,
                                                            float                   stroke_weight) {
    std::vector<glm::vec3> triangles;

    if (points.size() < 2) {
        return triangles;
    }

    // transform all points to screen space first
    std::vector<glm::vec3> screen_points = points;
    std::vector<glm::vec3> normals(screen_points.size());
    std::vector<glm::vec3> directions(screen_points.size());

    for (auto& p: screen_points) {
        g->to_screen_space(p);
    }

    for (size_t i = 0; i < normals.size(); i++) {
        const size_t ii   = (i + 1) % screen_points.size();
        glm::vec3&   p1   = screen_points[i];
        glm::vec3&   p2   = screen_points[ii];
        glm::vec3    dir  = p2 - p1;
        glm::vec3    perp = glm::normalize(dir);
        perp              = glm::vec3(-perp.y, perp.x, 0);
        directions[i]     = dir;
        normals[i]        = perp;
    }

    const float half_width = stroke_weight * 0.5f;
    uint32_t    vertex_count{0};

    glm::vec3 p1_left{};
    glm::vec3 p1_right{};

    const uint32_t num_line_segments = screen_points.size() + (close_shape ? 1 : 0);

    for (size_t i = 0; i < num_line_segments; i++) {
        const glm::vec3 point     = screen_points[i % screen_points.size()];
        const glm::vec3 direction = directions[i % directions.size()];
        const glm::vec3 normal    = normals[i % normals.size()];

        const glm::vec3 next_point     = screen_points[(i + 1) % screen_points.size()];
        const glm::vec3 next_direction = directions[(i + 1) % directions.size()];
        const glm::vec3 next_normal    = normals[(i + 1) % normals.size()];

        const glm::vec3 p2_left  = point + normal * half_width;
        const glm::vec3 p2_right = point - normal * half_width;
        const glm::vec3 p3_left  = next_point + next_normal * half_width;
        const glm::vec3 p3_right = next_point - next_normal * half_width;

        glm::vec3  intersection_left;
        const bool result_left = PGraphics::intersect_lines(p2_left, direction,
                                                            p3_left, next_direction,
                                                            intersection_left);
        glm::vec3  intersection_right;
        const bool result_right = PGraphics::intersect_lines(p2_right, direction,
                                                             p3_right, next_direction,
                                                             intersection_right);

        bool line_segments_are_too_pointy = PGraphics::are_almost_parallel(normal, next_normal, RENDER_LINE_STRIP_AS_QUADS_MAX_ANGLE);

        if (!result_left) {
            intersection_left = p3_left;
        }

        if (!result_right) {
            intersection_right = p3_right;
        }

        /*
         * 1--2
         * |\ |
         * | \|
         * 0--3
         */
        if (close_shape) {
            /* closed */
            if (i != 0) {
                if (line_segments_are_too_pointy) {
                    glm::vec3 tmp_p2_left  = next_point + normal * half_width;
                    glm::vec3 tmp_p2_right = next_point - normal * half_width;
                    add_quad_as_triangles(triangles,
                                          p1_left,
                                          tmp_p2_left,
                                          tmp_p2_right,
                                          p1_right);
                    p1_left  = next_point - normal * half_width;
                    p1_right = next_point + normal * half_width;
                    vertex_count += 6;
                } else {
                    add_quad_as_triangles(triangles,
                                          p1_left,
                                          intersection_left,
                                          intersection_right,
                                          p1_right);
                    p1_left  = intersection_left;
                    p1_right = intersection_right;
                    vertex_count += 6;
                }
            } else {
                if (line_segments_are_too_pointy) {
                    p1_left  = next_point - normal * half_width;
                    p1_right = next_point + normal * half_width;
                } else {
                    p1_left  = intersection_left;
                    p1_right = intersection_right;
                }
            }
        } else {
            /* open */
            if (i == 0) {
                // first segment
                if (line_segments_are_too_pointy) {
                    glm::vec3 tmp_p2_left  = p2_left;
                    glm::vec3 tmp_p2_right = p2_right;
                    glm::vec3 tmp_p3_left  = p3_left;
                    glm::vec3 tmp_p3_right = p3_right;
                    add_quad_as_triangles(triangles,
                                          tmp_p2_left,
                                          tmp_p3_right,
                                          tmp_p3_left,
                                          tmp_p2_right);
                    p1_left  = p3_left;
                    p1_right = p3_right;
                    vertex_count += 6;
                } else {
                    glm::vec3 tmp_p2_left  = p2_left;
                    glm::vec3 tmp_p2_right = p2_right;
                    add_quad_as_triangles(triangles,
                                          tmp_p2_left,
                                          intersection_left,
                                          intersection_right,
                                          tmp_p2_right);
                    p1_left  = intersection_left;
                    p1_right = intersection_right;
                    vertex_count += 6;
                }
            } else if (i == num_line_segments - 2) {
                // last segment
                glm::vec3 tmp_p2_left  = next_point + normal * half_width;
                glm::vec3 tmp_p2_right = next_point - normal * half_width;
                add_quad_as_triangles(triangles,
                                      p1_left,
                                      tmp_p2_left,
                                      tmp_p2_right,
                                      p1_right);
                p1_left  = next_point + normal * half_width;
                p1_right = next_point - normal * half_width;
                vertex_count += 6;
            } else if (i < num_line_segments - 2) {
                // other segments
                if (line_segments_are_too_pointy) {
                    glm::vec3 tmp_p2_left  = next_point + normal * half_width;
                    glm::vec3 tmp_p2_right = next_point - normal * half_width;
                    add_quad_as_triangles(triangles,
                                          p1_left,
                                          tmp_p2_left,
                                          tmp_p2_right,
                                          p1_right);
                    p1_left  = next_point - normal * half_width;
                    p1_right = next_point + normal * half_width;
                    vertex_count += 6;
                } else {
                    add_quad_as_triangles(triangles,
                                          p1_left,
                                          intersection_left,
                                          intersection_right,
                                          p1_right);
                    p1_left  = intersection_left;
                    p1_right = intersection_right;
                    vertex_count += 6;
                }
            }
        }
    }
    return triangles;
}

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

    stroke(0.0f);
    fill(0.5f, 0.85f, 1.0f);
    strokeWeight(20);
}

void draw() {
    background(1.0f);

    stroke(0.0f);
    fill(0.5f, 0.85f, 1.0f);
    beginShape(POLYGON);
    vertex(120, 80);
    vertex(230, 80);
    vertex(mouseX, mouseY);
    vertex(340, 190);
    vertex(340, 300);
    vertex(120, 300);
    endShape(CLOSE);


    std::vector<glm::vec3> points;
    points.emplace_back(120, 80, 0);
    points.emplace_back(230, 80, 0);
    points.emplace_back(mouseX, mouseY, 0);
    points.emplace_back(340, 190, 0);
    points.emplace_back(340, 300, 0);
    points.emplace_back(120, 300, 0);

    const std::vector<glm::vec3> triangles = render_line_strip_as_connected_quads(points, true, 20);

    noStroke();
    fill(0.0f);
    beginShape(TRIANGLES);
    for (const auto t: triangles) {
        vertex(t.x, t.y);
    }
    endShape();
}