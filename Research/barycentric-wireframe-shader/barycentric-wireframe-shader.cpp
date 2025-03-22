#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include "Umgebung.h"
#include "PShader.h"
#include "ShaderSource.h"
#include "VertexBuffer.h"
#include "Geometry.h"
#include "PMesh.h"

// see https://tchayen.github.io/posts/wireframes-with-barycentric-coordinates

using namespace umgebung;

ShaderSource shader_source_barycentric_wireframe{
    .vertex   = R"(
            #version 330 core

            layout(location = 0) in vec4 aPosition;
            layout(location = 1) in vec4 aNormal;
            layout(location = 2) in vec4 aColor;
            layout(location = 3) in vec2 aTexCoord;

            out vec4 vColor;
            out vec2 vTexCoord;
            out vec3 vBarycentric;

            uniform mat4 uProjection;
            uniform mat4 uViewMatrix;
            uniform mat4 uModelMatrix;

            void main() {
                gl_Position = uProjection * uViewMatrix * uModelMatrix * aPosition;
                vColor = aColor;
                vTexCoord = aTexCoord;
                vBarycentric = aNormal.xyz;
            }
        )",
    .fragment = R"(
            #version 330 core

            in vec4 vColor;
            in vec2 vTexCoord;
            in vec3 vBarycentric;

            out vec4 FragColor;

            uniform float line_width;
            uniform float feather_width;

            uniform sampler2D uTexture;

            float edgefactor(vec3 bary, float width) {
                vec3 d = fwidth(bary);
                vec3 a3 = smoothstep(d * (width - 0.5), d * (width + 0.5), bary);
                return 1.0 - min(min(a3.x, a3.y), a3.z);
            }

            float edgefactor_feather(vec3 bary, float width, float feather) {
                float w1 = width - feather * 0.5;
                vec3 d = fwidth(bary);
                vec3 a3 = smoothstep(d * w1, d * (w1 + feather), bary);
                return 1.0 - min(min(a3.x, a3.y), a3.z);
            }

            void main() {
                float alpha = edgefactor(vBarycentric, line_width);
                //float alpha = edgefactor_feather(vBarycentric, line_width, feather_width);
                alpha = clamp(alpha, 0.0, 1.0);
                FragColor = texture(uTexture, vTexCoord) * vec4(vColor.rgb, vColor.a * alpha);
                //FragColor = vec4(vColor.rgb, vColor.a * alpha);
            }
        )"};

PShader* shader_barycentric_wireframe;
PMesh*   mesh_sphere;
int      frame_counter = 0;

void settings() {
    size(1024, 768);
    antialiasing       = 8;
    render_to_buffer   = false;
    retina_support     = true;
    enable_audio       = false;
    subsystem_graphics = umgebung_create_subsystem_graphics_openglv33();
}

void setup() {
    shader_barycentric_wireframe = loadShader(shader_source_barycentric_wireframe.vertex, shader_source_barycentric_wireframe.fragment);

    /* use convenience function to generate a sphere */
    std::vector<glm::vec3> points;
    generate_sphere(points, 50, 50, height / 3.0f);

    /* compile list of vertices with attributes like color,  */
    constexpr glm::vec4 color{0.5f, 0.85f, 1.0f, 1.0f};
    constexpr glm::vec2 tex_coords{0.0f, 0.0f}; // not really used in this example
    std::vector<Vertex> sphere_vertices(points.size());
    int                 cycle = 0;
    for (auto& position: points) {
        /*
         * each vertex in a triangle needs to have barycentric coordinates (1,0,0), (0,1,0), or (0,0,1).
         * we use the normal information in the vertex to transmit them to the shader
         */
        glm::vec4 barycentric_coordinates{
            cycle == 0 ? 1.0f : 0.0f,
            cycle == 1 ? 1.0f : 0.0f,
            cycle == 2 ? 1.0f : 0.0f,
            0.0f};
        sphere_vertices.emplace_back(Vertex{position, color, tex_coords, barycentric_coordinates});
        cycle = (cycle + 1) % 3;
    }

    mesh_sphere = new PMesh();
    mesh_sphere->add_vertices(sphere_vertices);
    mesh_sphere->update();
}

void draw() {
    background(0.85f);

    fill(0);
    g->debug_text("FPS: " + nf(frameRate, 2), 20, 20);

    pushMatrix();
    translate(width / 2.0f, height / 2.0f);
    rotateX(frame_counter * 0.01f);
    rotateY(frame_counter * 0.027f);
    rotateZ(frame_counter * 0.043f);

    if (isKeyPressed && key == ' ') {
        frame_counter++;
    }

    shader(shader_barycentric_wireframe);
    /* set the line width */
    shader_barycentric_wireframe->set_uniform("line_width", 0.1f + mouseX / width * 3.0f);
    /* matrices need to be set every frame for the transforms to work */
    shader_barycentric_wireframe->set_uniform(SHADER_UNIFORM_VIEW_MATRIX, g->view_matrix);
    shader_barycentric_wireframe->set_uniform(SHADER_UNIFORM_PROJECTION_MATRIX, g->projection_matrix);
    shader_barycentric_wireframe->set_uniform(SHADER_UNIFORM_MODEL_MATRIX, g->model_matrix);
    mesh(mesh_sphere);

    pushMatrix();
    translate(height / 3.0f, 0);
    scale(0.5f);
    /* before drawing the mesh the model matrix needs to be sent to the shader */
    shader_barycentric_wireframe->set_uniform(SHADER_UNIFORM_MODEL_MATRIX, g->model_matrix);
    mesh(mesh_sphere);
    popMatrix();

    pushMatrix();
    translate(0, 100);
    scale(2.0f);
    /* before drawing the mesh the model matrix needs to be sent to the shader */
    shader_barycentric_wireframe->set_uniform(SHADER_UNIFORM_MODEL_MATRIX, g->model_matrix);
    mesh(mesh_sphere);
    popMatrix();

    /* reset the shader */
    shader();

    popMatrix();
}