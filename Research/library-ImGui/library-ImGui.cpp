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

//    IMGUI_CHECKVERSION();
//    ImGui::CreateContext();
//    ImGuiIO& io = ImGui::GetIO(); (void)io;
//    io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;     // Enable Keyboard Controls
//    io.ConfigFlags |= ImGuiConfigFlags_NavEnableGamepad;      // Enable Gamepad Controls
//
//    // Setup Dear ImGui style
//    ImGui::StyleColorsDark();
//    //ImGui::StyleColorsLight();
//
//    // Setup Platform/Renderer backends
//    ImGui_ImplSDL3_InitForOpenGL(window, gl_context);
//    ImGui_ImplOpenGL3_Init(glsl_version);
//
//    // Load Fonts ?!? see example
//
//    // in loop
//    {
//        // event
//        SDL_Event event;
//        ImGui_ImplSDL3_ProcessEvent(&event);
//
//        // [If using SDL_MAIN_USE_CALLBACKS: all code below would likely be your SDL_AppIterate() function]
//        if (SDL_GetWindowFlags(window) & SDL_WINDOW_MINIMIZED)
//        {
//            SDL_Delay(10);
//            continue;
//        }
//
//        // Start the Dear ImGui frame
//        ImGui_ImplOpenGL3_NewFrame();
//        ImGui_ImplSDL3_NewFrame();
//        ImGui::NewFrame();
//
//        // 1. Show the big demo window (Most of the sample code is in ImGui::ShowDemoWindow()! You can browse its code to learn more about Dear ImGui!).
//        if (show_demo_window)
//            ImGui::ShowDemoWindow(&show_demo_window);
//
//        // 2. Show a simple window that we create ourselves. We use a Begin/End pair to create a named window.
//        {
//            static float f = 0.0f;
//            static int counter = 0;
//
//            ImGui::Begin("Hello, world!");                          // Create a window called "Hello, world!" and append into it.
//
//            ImGui::Text("This is some useful text.");               // Display some text (you can use a format strings too)
//            ImGui::Checkbox("Demo Window", &show_demo_window);      // Edit bools storing our window open/close state
//            ImGui::Checkbox("Another Window", &show_another_window);
//
//            ImGui::SliderFloat("float", &f, 0.0f, 1.0f);            // Edit 1 float using a slider from 0.0f to 1.0f
//            ImGui::ColorEdit3("clear color", (float*)&clear_color); // Edit 3 floats representing a color
//
//            if (ImGui::Button("Button"))                            // Buttons return true when clicked (most widgets return true when edited/activated)
//                counter++;
//            ImGui::SameLine();
//            ImGui::Text("counter = %d", counter);
//
//            ImGui::Text("Application average %.3f ms/frame (%.1f FPS)", 1000.0f / io.Framerate, io.Framerate);
//            ImGui::End();
//        }
//
//        // 3. Show another simple window.
//        if (show_another_window)
//        {
//            ImGui::Begin("Another Window", &show_another_window);   // Pass a pointer to our bool variable (the window will have a closing button that will clear the bool when clicked)
//            ImGui::Text("Hello from another window!");
//            if (ImGui::Button("Close Me"))
//                show_another_window = false;
//            ImGui::End();
//        }
//
//        // Rendering
//        ImGui::Render();
//        glViewport(0, 0, (int)io.DisplaySize.x, (int)io.DisplaySize.y);
//        glClearColor(clear_color.x * clear_color.w, clear_color.y * clear_color.w, clear_color.z * clear_color.w, clear_color.w);
//        glClear(GL_COLOR_BUFFER_BIT);
//        ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
//        SDL_GL_SwapWindow(window);
//    }


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