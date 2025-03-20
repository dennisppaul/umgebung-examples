#include <GL/glew.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "Umgebung.h"
#include "PShader.h"
#include "ShaderSource.h"
#include "VertexBuffer.h"

// see https://tchayen.github.io/posts/wireframes-with-barycentric-coordinates

using namespace umgebung;

ShaderSource shader_source_barycentric_wireframe{
    .vertex   = R"(
        #version 330 core  // Use #version 310 es for OpenGL ES

        precision mediump float;

        layout(location = 0) in vec3 aPosition;
        layout(location = 1) in vec3 aBarycentric;

        uniform mat4 uMVP;

        out vec3 vBarycentric;

        void main() {
            vBarycentric = aBarycentric;
            gl_Position = uMVP * vec4(aPosition, 1.0);
        }
    )",
    .fragment = R"(
        #version 330 core  // Use #version 310 es for OpenGL ES

        precision mediump float;

        in vec3 vBarycentric;
        out vec4 FragColor;

        uniform vec4 uLineColor; // Color of the wireframe
        uniform float uLineWidth; // Thickness of the wireframe
        uniform float uFeatherWidth;

        float edgefactor(vec3 bary, float width) {
            vec3 d = fwidth(bary);
            vec3 a3 = smoothstep(d * (width - 0.5), d * (width + 0.5), bary);
            return 1.0 - min(min(a3.x, a3.y), a3.z);
        }

        float _edgefactor_feather(vec3 bary, float width, float feather) {
            float w1 = width - feather * 0.5;
            vec3 d = fwidth(bary);
            vec3 a3 = smoothstep(d * w1, d * (w1 + feather), bary);

            float alpha = min(min(a3.x, a3.y), a3.z); // Compute raw edge factor
            alpha = clamp(alpha, 0.0, 1.0);

            // Remap 0 → 1 → 0 for smooth fading on both sides
            alpha = 1.0 - pow(abs(alpha - 0.5) * 2.0, 2.0);

            return alpha;
        }

        float edgefactor_feather(vec3 bary, float width, float feather) {
            float w1 = width - feather * 0.5;
            vec3 d = fwidth(bary);
            vec3 a3 = smoothstep(d * w1, d * (w1 + feather), bary);
            return 1.0 - min(min(a3.x, a3.y), a3.z);
        }

        void main() {
            float alpha = edgefactor(vBarycentric, uLineWidth);
            //float alpha = edgefactor_feather(vBarycentric, uLineWidth, uFeatherWidth);
            alpha = clamp(alpha, 0.0, 1.0);
            //alpha = 1.0 - pow(abs(alpha - 0.5) * 2.0, 2.0);
            //alpha = 1.0 - pow(abs(alpha - 0.5) * 2.0, 4.0);
            FragColor = vec4(uLineColor.rgb, uLineColor.a * alpha);
        }
    )"};

PShader* shader_barycentric_wireframe;

void settings() {
    size(1024, 768);
    display            = 0;
    antialiasing       = 8;
    render_to_buffer   = false;
    retina_support     = true;
    enable_audio       = false;
    subsystem_graphics = umgebung_create_subsystem_graphics_openglv33();
}

GLuint VAO, VBO, EBO;
// GLuint shaderProgram;

// void SHARED_checkShaderCompileStatus(const GLuint shader_barycentric_wireframe) {
//     GLint success;
//     glGetShaderiv(shader_barycentric_wireframe, GL_COMPILE_STATUS, &success);
//     if (!success) {
//         char infoLog[512];
//         glGetShaderInfoLog(shader_barycentric_wireframe, 512, nullptr, infoLog);
//         error("ERROR::SHADER::COMPILATION_FAILED\n", infoLog);
//     }
// }

// void SHARED_checkProgramLinkStatus(const GLuint program) {
//     GLint success;
//     glGetProgramiv(program, GL_LINK_STATUS, &success);
//     if (!success) {
//         char infoLog[512];
//         glGetProgramInfoLog(program, 512, nullptr, infoLog);
//         error("ERROR::SHADER::COMPILATION_FAILED\n", infoLog);
//     }
// }

// GLuint SHARED_build_shader(const char* vertexShaderSource, const char* fragmentShaderSource) {
//     // Build shaders
//     const GLuint vertexShader = glCreateShader(GL_VERTEX_SHADER);
//     glShaderSource(vertexShader, 1, &vertexShaderSource, nullptr);
//     glCompileShader(vertexShader);
//     SHARED_checkShaderCompileStatus(vertexShader);
//
//     const GLuint fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
//     glShaderSource(fragmentShader, 1, &fragmentShaderSource, nullptr);
//     glCompileShader(fragmentShader);
//     SHARED_checkShaderCompileStatus(fragmentShader);
//
//     const GLuint mShaderProgram = glCreateProgram();
//     glAttachShader(mShaderProgram, vertexShader);
//     glAttachShader(mShaderProgram, fragmentShader);
//     glLinkProgram(mShaderProgram);
//     SHARED_checkProgramLinkStatus(mShaderProgram);
//
//     glDeleteShader(vertexShader);
//     glDeleteShader(fragmentShader);
//
//     return mShaderProgram;
// }

void setup() {
    hint(HINT_ENABLE_SMOOTH_LINES);
    shader_barycentric_wireframe = loadShader(shader_source_barycentric_wireframe.vertex, shader_source_barycentric_wireframe.fragment);
    // shader_barycentric_wireframe = new PShader();
    // shader_barycentric_wireframe->load(vertex_shader_barycentric_wireframe(), fragment_shader_barycentric_wireframe());

    constexpr GLfloat vertices[] = {
        // Positions       // Barycentric Coords
        -0.5f, -0.5f, 0.0f, 1.0f, 0.0f, 0.0f,
        0.5f, -0.5f, 0.0f, 0.0f, 1.0f, 0.0f,
        0.0f, 0.5f, 0.0f, 0.0f, 0.0f, 1.0f};

    constexpr GLuint indices[] = {0, 1, 2};

    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);
    glGenBuffers(1, &EBO);

    glBindVertexArray(VAO);

    // Upload vertex data
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

    // Upload index data
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

    // Position attribute
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(GLfloat), (void*) 0);
    glEnableVertexAttribArray(0);

    // Barycentric attribute
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(GLfloat), (void*) (3 * sizeof(GLfloat)));
    glEnableVertexAttribArray(1);

    glBindVertexArray(0);
}

void configure_shader() {
    const glm::mat4 mvp = g->projection_matrix_3D * g->view_matrix * g->model_matrix;
    shader_barycentric_wireframe->set_uniform("uMVP", mvp);
    shader_barycentric_wireframe->set_uniform("uLineColor", glm::vec4(0.5f, 0.85f, 1.0f, 0.9f));
    shader_barycentric_wireframe->set_uniform("uLineWidth", mouseX / width * 20);
    shader_barycentric_wireframe->set_uniform("uFeatherWidth", mouseY / height * 3);
}

void render_triangle() {
    glBindVertexArray(VAO);
    glDrawElements(GL_TRIANGLES, 3, GL_UNSIGNED_INT, 0);
    glBindVertexArray(0);
}

int counter = 0;

void draw() {
    background(0.85f);

    fill(0);
    g->debug_text("FPS: " + nf(frameRate, 2), 20, 20);

    fill(0.5f, 0.85f, 1.0f);
    stroke(0.0f);
    strokeWeight(3);

    pushMatrix();
    translate(width / 2.0f, height / 2.0f, 0.0f);

    rotateX(counter * 0.01f);
    rotateY(counter * 0.027f);

    if (isKeyPressed && key == ' ') {
        counter++;
    }

    shader(shader_barycentric_wireframe);
    sphere(height / 2.0f);
    shader();
    popMatrix();

    //    pushMatrix();
    //    translate(width / 2.0f, height / 2.0f, 0.0f);
    //    translate(0.75f, 0, 0);
    //    scale(200);
    // rotateX(counter * 0.01f);
    // rotateY(counter * 0.027f);
    //
    // if (isKeyPressed && key == ' ') {
    //     counter++;
    // }
    //
    //    shader(shader_barycentric_wireframe);
    //
    //    configure_shader();
    //    render_triangle();
    //
    //    translate(0.5f, 0.0f);
    //    rotateZ(PI);
    //
    //    configure_shader();
    //    render_triangle();
    //
    //    rotateZ(PI);
    //    translate(0.5f, 0.0f);
    //    rotateY(PI);
    //
    //    configure_shader();
    //    render_triangle();
    //
    //    shader(); // revert to default shader
    //
    //    popMatrix();
}