#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <iostream>
#include <fstream>
#include <vector>
#include <cairo/cairo.h>
#include <vector>

#include "Umfeld.h"
#include "Geometry.h"

using namespace umfeld;

int                    frame_counter = 0;
std::vector<glm::vec3> line_vertices;

// Function to export vectors to a PDF
void exportToPDF(const std::vector<std::vector<Vec2>>& triangles,
                 const std::vector<std::vector<Vec2>>& line_strips,
                 const std::string& filename,
                 double stroke_width, cairo_line_cap_t stroke_cap) {
    cairo_surface_t* surface = cairo_pdf_surface_create(filename.c_str(), 600, 600);
    cairo_t* cr = cairo_create(surface);

    // Set stroke weight and cap
    cairo_set_line_width(cr, stroke_width);
    cairo_set_line_cap(cr, stroke_cap);

    // Draw triangles with a filled stroke
    cairo_set_source_rgb(cr, 0.2, 0.6, 1.0);
    for (const auto& tri : triangles) {
        if (tri.size() == 3) {
            cairo_move_to(cr, tri[0].x, tri[0].y);
            cairo_line_to(cr, tri[1].x, tri[1].y);
            cairo_line_to(cr, tri[2].x, tri[2].y);
            cairo_close_path(cr);
            cairo_fill_preserve(cr);
            cairo_stroke(cr);
        }
    }

    // Draw line strips with custom stroke settings
    cairo_set_source_rgb(cr, 1.0, 0.2, 0.2);
    for (const auto& strip : line_strips) {
        if (strip.empty()) continue;
        cairo_move_to(cr, strip[0].x, strip[0].y);
        for (size_t i = 1; i < strip.size(); ++i) {
            cairo_line_to(cr, strip[i].x, strip[i].y);
        }
        cairo_stroke(cr);
    }

    cairo_destroy(cr);
    cairo_surface_destroy(surface);
}

// TODO add normal
// Function to export 3D triangles and line strips to an OBJ file with colors & tex coords
void exportToOBJ(const std::vector<std::vector<Vertex>>& triangles,
                 const std::vector<std::vector<Vertex>>& line_strips,
                 const std::string& filename) {
    std::ofstream file(filename);
    if (!file) {
        std::cerr << "Error: Unable to open file " << filename << "\n";
        return;
    }

    file << "# Exported 3D geometry to OBJ with Colors & TexCoords (GLM)\n";

    std::vector<Vertex> all_vertices;
    std::vector<int> vertex_indices;

    // Function to add a vertex and return its index
    auto add_vertex = [&](const Vertex& v) -> int {
        all_vertices.push_back(v);
        return static_cast<int>(all_vertices.size()); // 1-based index
    };

    // Store vertices for triangles
    for (const auto& tri : triangles)
        for (const auto& v : tri)
            vertex_indices.push_back(add_vertex(v));

    // Store vertices for line strips
    for (const auto& strip : line_strips)
        for (const auto& v : strip)
            vertex_indices.push_back(add_vertex(v));

    // Write all vertices (Position & Color)
    for (const auto& v : all_vertices) {
        file << "v " << v.position.x << " " << v.position.y << " " << v.position.z
             << " " << v.color.r << " " << v.color.g << " " << v.color.b << "\n"; // Including color
    }

    // Write all texture coordinates
    for (const auto& v : all_vertices) {
        file << "vt " << v.texcoord.x << " " << v.texcoord.y << "\n";
    }

    // Write triangle faces with texture indices
    size_t index = 0;
    for (const auto& tri : triangles) {
        file << "f "
             << (index + 1) << "/" << (index + 1) << " "
             << (index + 2) << "/" << (index + 2) << " "
             << (index + 3) << "/" << (index + 3) << "\n";
        index += 3;
    }

    // Write line strips
    for (const auto& strip : line_strips) {
        file << "l";
        for (size_t i = 0; i < strip.size(); ++i) {
            file << " " << (++index);
        }
        file << "\n";
    }

    file.close();
    std::cout << "OBJ exported: " << filename << "\n";
}

void settings() {
    size(1024, 768);
}

void triangulate_line(std::vector<glm::vec3>& line_vertices, std::vector<Vertex>& vertices) {
    if (line_vertices.size() < 2) {
        return;
    }

    vertices.clear();

    for (int i = 0; i < line_vertices.size() - 1; ++i) {
        glm::vec3 p0 = line_vertices[i];
        glm::vec3 p1 = line_vertices[i + 1];
    }

    vertices.push_back(p3);
    vertices.push_back(p1);
    vertices.push_back(p2);
}

void setup() {}

void draw() {
    background(0.85f);

    fill(0);
    noStroke();
    g->debug_text("FPS: " + nf(frameRate, 2), 20, 20);

    pushMatrix();
    translate(width / 2.0f, height / 2.0f);
    rotateX(frame_counter * 0.01f);
    rotateY(frame_counter * 0.027f);
    rotateZ(frame_counter * 0.043f);

    if (isKeyPressed && key == ' ') {
        frame_counter++;
    }

    const std::vector<Vertex> vertices;
    triangulate_line(vertices);
    beginShape(TRIANGLES);
    for (const auto v: vertices) {
        vertex(v.position.x, v.position.y, v.position.z);
    }
    endShape();

    /* reset the shader */
    shader();

    popMatrix();
}