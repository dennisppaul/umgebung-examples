#include <GL/glew.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "Umgebung.h"

// see https://tchayen.github.io/posts/wireframes-with-barycentric-coordinates

using namespace umgebung;

const char* vertex_shader_barycentric_wireframe() {
    return R"(
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
)";
}

const char* fragment_shader_barycentric_wireframe() {
    return R"(
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
)";
}

const char* _fragment_shader_barycentric_wireframe() {
    return R"(
#version 330 core  // Use #version 310 es for OpenGL ES

precision mediump float;

in vec3 vBarycentric;
out vec4 FragColor;

uniform vec3 uLineColor; // Color of the wireframe
uniform float uLineWidth; // Thickness of the wireframe

float edgeFactor() {
  vec3 d = fwidth(vBarycentric);
  vec3 f = step(d * uLineWidth, vBarycentric);
  return min(min(f.x, f.y), f.z);
}

void main() {
    //vec3 d = fwidth(vBarycentric);
    //vec3 edge = smoothstep(vec3(0.0), d * uLineWidth, vBarycentric);
    //float line = min(min(edge.x, edge.y), edge.z);
    //FragColor = mix(vec4(uLineColor, 1.0), vec4(uLineColor, 0.0), line);
    FragColor = vec4(min(vec3(edgeFactor()), uLineColor), 1.0);
}
)";
}

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
GLuint shaderProgram;

void SHARED_checkShaderCompileStatus(const GLuint shader) {
    GLint success;
    glGetShaderiv(shader, GL_COMPILE_STATUS, &success);
    if (!success) {
        char infoLog[512];
        glGetShaderInfoLog(shader, 512, nullptr, infoLog);
        error("ERROR::SHADER::COMPILATION_FAILED\n", infoLog);
    }
}

void SHARED_checkProgramLinkStatus(const GLuint program) {
    GLint success;
    glGetProgramiv(program, GL_LINK_STATUS, &success);
    if (!success) {
        char infoLog[512];
        glGetProgramInfoLog(program, 512, nullptr, infoLog);
        error("ERROR::SHADER::COMPILATION_FAILED\n", infoLog);
    }
}

GLuint SHARED_build_shader(const char* vertexShaderSource, const char* fragmentShaderSource) {
    // Build shaders
    const GLuint vertexShader = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(vertexShader, 1, &vertexShaderSource, nullptr);
    glCompileShader(vertexShader);
    SHARED_checkShaderCompileStatus(vertexShader);

    const GLuint fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(fragmentShader, 1, &fragmentShaderSource, nullptr);
    glCompileShader(fragmentShader);
    SHARED_checkShaderCompileStatus(fragmentShader);

    const GLuint mShaderProgram = glCreateProgram();
    glAttachShader(mShaderProgram, vertexShader);
    glAttachShader(mShaderProgram, fragmentShader);
    glLinkProgram(mShaderProgram);
    SHARED_checkProgramLinkStatus(mShaderProgram);

    glDeleteShader(vertexShader);
    glDeleteShader(fragmentShader);

    return mShaderProgram;
}

void setup() {
    hint(HINT_ENABLE_SMOOTH_LINES);

    constexpr GLfloat vertices[] = {
        // Positions       // Barycentric Coords
        -0.5f, -0.5f, 0.0f, 1.0f, 0.0f, 0.0f,
        0.5f, -0.5f, 0.0f, 0.0f, 1.0f, 0.0f,
        0.f, 0.5f, 0.0f, 0.0f, 0.0f, 1.0f};

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

    shaderProgram = SHARED_build_shader(vertex_shader_barycentric_wireframe(), fragment_shader_barycentric_wireframe());
}

void render_triangle() {
    glm::mat4 mvp = g->projection_matrix_3D * g->view_matrix * g->model_matrix;
    glUniformMatrix4fv(glGetUniformLocation(shaderProgram, "uMVP"), 1, GL_FALSE, glm::value_ptr(mvp));
    glUniform4f(glGetUniformLocation(shaderProgram, "uLineColor"), 0.5f, 0.85f, 1.0f, 0.9f);
    glUniform1f(glGetUniformLocation(shaderProgram, "uLineWidth"), mouseX / width * 20);
    glUniform1f(glGetUniformLocation(shaderProgram, "uFeatherWidth"), mouseY / height * 3);

    glBindVertexArray(VAO);

    glDrawElements(GL_TRIANGLES, 3, GL_UNSIGNED_INT, 0);
    glBindVertexArray(0);
}

int counter = 0;

void draw() {
    background(0.85f);

    fill(0);
    g->debug_text("FPS: " + nf(frameRate, 2), 20, 20);

    pushMatrix();
    translate(width / 2.0f, height / 2.0f, 0.0f);
    translate(0.75f, 0, 0);
    scale(200);
    rotateX(counter * 0.01f);
    rotateY(counter * 0.027f);

    if (isKeyPressed && key == ' ') {
        counter++;
    }

    glUseProgram(shaderProgram);

    render_triangle();
    translate(0.5f, 0.0f);
    rotateZ(PI);
    render_triangle();
    rotateZ(PI);
    translate(0.5f, 0.0f);
    rotateY(PI);
    render_triangle();

    popMatrix();
}