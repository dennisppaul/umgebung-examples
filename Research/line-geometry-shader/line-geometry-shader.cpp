#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include "Umgebung.h"
#include "PShader.h"
#include "ShaderSource.h"
#include "VertexBuffer.h"
#include "Geometry.h"
#include "VertexBuffer.h"

using namespace umgebung;

ShaderSource shader_source_line{
    .vertex   = R"(
            #version 330 core

            layout(location = 0) in vec3 aPosition;

            void main() {
                gl_Position = vec4(aPosition, 1.0); // Geometry shader handles MVP
            }
    )",
    .fragment = R"(
            #version 330 core

            out vec4 FragColor;

            void main() {
                FragColor = vec4(0.0, 0.0, 0.0, 1.0);
            }
    )",
    .geometry = R"(
            #version 330 core

            layout(lines_adjacency) in;
            layout(triangle_strip, max_vertices = 4) out;

            uniform mat4 uMVP;
            uniform float uLineWidth;       // in pixels
            uniform vec2 uViewportSize;     // in pixels
            uniform float uMiterLimit;      // max miter length (e.g. 4.0)

            void main() {
                // Project to clip space
                vec4 pos0 = uMVP * gl_in[0].gl_Position;
                vec4 pos1 = uMVP * gl_in[1].gl_Position;
                vec4 pos2 = uMVP * gl_in[2].gl_Position;
                vec4 pos3 = uMVP * gl_in[3].gl_Position;

                // Convert to NDC
                vec2 p0 = pos0.xy / pos0.w;
                vec2 p1 = pos1.xy / pos1.w;
                vec2 p2 = pos2.xy / pos2.w;
                vec2 p3 = pos3.xy / pos3.w;

                vec2 d1 = normalize(p1 - p0);
                vec2 d2 = normalize(p2 - p1);
                vec2 d3 = normalize(p3 - p2);

                vec2 n1 = vec2(-d1.y, d1.x);
                vec2 n2 = vec2(-d2.y, d2.x);
                vec2 n3 = vec2(-d3.y, d3.x);

                vec2 miterStart = normalize(n1 + n2);
                vec2 miterEnd   = normalize(n2 + n3);

                float pixelToNDC = 2.0 / uViewportSize.y;
                float halfWidth = uLineWidth * 0.5 * pixelToNDC;

                float lenStart = halfWidth / max(dot(miterStart, n2), 0.1);
                float lenEnd   = halfWidth / max(dot(miterEnd, n2), 0.1);

                // Clamp miter length to avoid spikes
                lenStart = min(lenStart, uLineWidth * uMiterLimit * pixelToNDC);
                lenEnd   = min(lenEnd, uLineWidth * uMiterLimit * pixelToNDC);

                // Offset in clip space (NDC * w)
                vec4 p1a = pos1 + vec4(miterStart * lenStart * pos1.w, 0.0, 0.0);
                vec4 p1b = pos1 - vec4(miterStart * lenStart * pos1.w, 0.0, 0.0);
                vec4 p2a = pos2 + vec4(miterEnd   * lenEnd   * pos2.w, 0.0, 0.0);
                vec4 p2b = pos2 - vec4(miterEnd   * lenEnd   * pos2.w, 0.0, 0.0);

                gl_Position = p1a; EmitVertex();
                gl_Position = p1b; EmitVertex();
                gl_Position = p2a; EmitVertex();
                gl_Position = p2b; EmitVertex();
                EndPrimitive();
            }
        )"};

PShader* shader_line;
int      frame_counter = 0;
PMesh*   mesh_shape;

void settings() {
    size(1024, 768);
    antialiasing     = 8;
    render_to_buffer = false;
    retina_support   = true;
    enable_audio     = false;
}

std::vector<glm::vec3> generate_line_adjacency_segments(const std::vector<glm::vec3>& polyline) {
    std::vector<glm::vec3> segments;
    if (polyline.size() < 4) {
        return segments;
    }
    for (size_t i = 0; i + 3 < polyline.size(); ++i) {
        segments.push_back(polyline[i]);
        segments.push_back(polyline[i + 1]);
        segments.push_back(polyline[i + 2]);
        segments.push_back(polyline[i + 3]);
    }
    return segments;
}

void setup() {
    shader_line = loadShader(shader_source_line);
    hint(ENABLE_DEPTH_TEST);

    mesh_shape = new VertexBuffer();
    mesh_shape->set_shape(GL_LINES_ADJACENCY);

    // const std::vector<glm::vec3> polyline = {
    //     glm::vec3(-100.0f, 0.0f, 0.0f),    // P0
    //     glm::vec3(0.0f, 0.0f, 0.0f),       // P1
    //     glm::vec3(100.0f, 0.0f, 0.0f),     // P2
    //     glm::vec3(150.0f, -50.0f, 0.0f),   // P3
    //     glm::vec3(100.0f, -100.0f, 0.0f),  // P4
    //     glm::vec3(50.0f, -75.0f, 0.0f),    // P5
    //     glm::vec3(0.0f, -100.0f, 0.0f),
    //     glm::vec3(-100.0f, -100.0f, 0.0f),
    //     glm::vec3(-100.0f, 0.0f, 0.0f),
    //     glm::vec3(0.0f, 0.0f, 0.0f)
    // };

    std::vector<glm::vec3> polyline;
    for (int i = 0; i < 360; i += 18) {
        const float x = cos(radians(i)) * 100.0f;
        const float y = sin(radians(i)) * 100.0f;
        polyline.emplace_back(x, y, 0.0f);
    }

    const std::vector<glm::vec3> lineSegments = generate_line_adjacency_segments(polyline);

    /* compile list of vertices with attributes like color,  */
    constexpr glm::vec4 color{0.5f, 0.85f, 1.0f, 1.0f};
    constexpr glm::vec4 normal{0.0f, 0.0f, 1.0f, 1.0f};
    constexpr glm::vec2 tex_coords{0.0f, 0.0f}; // not really used in this example
    for (auto& position: lineSegments) {
        mesh_shape->add_vertex(Vertex{position, color, tex_coords, normal});
    }
}

void draw() {
    background(0.85f);

    fill(0);
    noStroke();
    g->debug_text("FPS: " + nf(frameRate, 2), 20, 20);

    pushMatrix();

    translate(width / 2.0f, height / 2.0f);
    fill(0.5f, 0.85f, 1.0f);
    box(120);
    rotateX(frame_counter * 0.01f);
    rotateY(frame_counter * 0.027f);
    rotateZ(frame_counter * 0.043f);

    if (isKeyPressed && key == ' ') {
        frame_counter++;
    }


    shader(shader_line);
    shader_line->set_uniform("uMVP", g->projection_matrix * g->view_matrix * g->model_matrix);
    shader_line->set_uniform("uLineWidth", map(mouseX, 0, width, 1, 20));
    shader_line->set_uniform("uViewportSize", width, height);
    shader_line->set_uniform("uMiterLimit", 4.0f);
    mesh(mesh_shape);
    shader();

    popMatrix();
}