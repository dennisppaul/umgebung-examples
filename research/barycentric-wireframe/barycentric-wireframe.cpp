#include <GL/glew.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "Umgebung.h"

using namespace umgebung;

const char* vertex_shader_barycentric_wireframe() {
    return R"(
#version 330 core  // Use #version 310 es for OpenGL ES

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

in vec3 vBarycentric;
out vec4 FragColor;

uniform vec3 uLineColor; // Color of the wireframe
uniform float uLineWidth; // Thickness of the wireframe

void main() {
    vec3 d = fwidth(vBarycentric);
    vec3 edge = smoothstep(vec3(0.0), d * uLineWidth, vBarycentric);
    float line = min(min(edge.x, edge.y), edge.z);
    FragColor = mix(vec4(uLineColor, 1.0), vec4(uLineColor, 0.0), line);
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
    // hint(HINT_ENABLE_SMOOTH_LINES);

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

    shaderProgram = SHARED_build_shader(vertex_shader_barycentric_wireframe(), fragment_shader_barycentric_wireframe());
}

void draw() {
    background(1.0f);

    pushMatrix();
    translate(width / 2.0f, height / 2.0f, 0.0f);
    scale(100);
    rotateX(frameCount * 0.01f);
    rotateY(frameCount * 0.027f);

    glUseProgram(shaderProgram);

    glm::mat4 mvp = g->projection_matrix_3D * g->view_matrix * g->model_matrix_client;
    glUniformMatrix4fv(glGetUniformLocation(shaderProgram, "uMVP"), 1, GL_FALSE, glm::value_ptr(mvp));
    glUniform3f(glGetUniformLocation(shaderProgram, "uLineColor"), 1.0f, 0.0f, 0.0f); // White lines
    glUniform1f(glGetUniformLocation(shaderProgram, "uLineWidth"), mouseX / width * 20);

    glBindVertexArray(VAO);

    glDrawElements(GL_TRIANGLES, 3, GL_UNSIGNED_INT, 0);
    glBindVertexArray(0);

    popMatrix();

    // stroke(0.0f);
    // fill(0.5f, 0.85f, 1.0f);
    // beginShape(POLYGON);
    // vertex(412, 204);
    // vertex(522, 204);
    // vertex(mouseX, mouseY);
    // vertex(632, 314);
    // vertex(632, 424);
    // vertex(412, 424);
    // vertex(312, 314);
    // endShape();
}

void keyPressed() {
    if (key == ' ') {}
}
