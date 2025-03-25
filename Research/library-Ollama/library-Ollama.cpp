#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <fstream>
#include <vector>
#include <iostream>
#include <string>
#include <curl/curl.h>

#include "Umgebung.h"
#include "Geometry.h"

using namespace umgebung;

int                    frame_counter = 0;
std::vector<glm::vec3> line_vertices;

size_t WriteCallback(void* contents, size_t size, size_t nmemb, std::string* output) {
    size_t totalSize = size * nmemb;
    output->append((char*)contents, totalSize);
    return totalSize;
}

std::string queryOllama(const std::string& model, const std::string& prompt) {
    CURL* curl = curl_easy_init();
    if (!curl) {
        return "Curl init failed";
    }

    std::string url = "http://localhost:11434/api/generate";
    std::string jsonPayload = R"({"model":")" + model + R"(","prompt":")" + prompt + R"("})";
    std::string response;

    struct curl_slist* headers = nullptr;
    headers = curl_slist_append(headers, "Content-Type: application/json");

    curl_easy_setopt(curl, CURLOPT_URL, url.c_str());
    curl_easy_setopt(curl, CURLOPT_POST, 1L);
    curl_easy_setopt(curl, CURLOPT_POSTFIELDS, jsonPayload.c_str());
    curl_easy_setopt(curl, CURLOPT_HTTPHEADER, headers);
    curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, WriteCallback);
    curl_easy_setopt(curl, CURLOPT_WRITEDATA, &response);

    CURLcode res = curl_easy_perform(curl);
    if (res != CURLE_OK) {
        response = "Curl request failed: " + std::string(curl_easy_strerror(res));
    }

    curl_slist_free_all(headers);
    curl_easy_cleanup(curl);
    return response;
}

int main() {
    std::string response = queryOllama("mistral", "Hello, how are you?");
    std::cout << "Ollama Response: " << response << std::endl;
    return 0;
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