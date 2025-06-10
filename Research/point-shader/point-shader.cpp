#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include "Umfeld.h"
#include "PShader.h"
#include "ShaderSource.h"
#include "VertexBuffer.h"
#include "Geometry.h"

using namespace umfeld;

ShaderSource shader_source_barycentric_wireframe{
/*
  Part of the Processing project - http://processing.org

  Copyright (c) 2012-21 The Processing Foundation
  Copyright (c) 2004-12 Ben Fry and Casey Reas
  Copyright (c) 2001-04 Massachusetts Institute of Technology

  This library is free software; you can redistribute it and/or
  modify it under the terms of the GNU Lesser General Public
  License as published by the Free Software Foundation, version 2.1.

  This library is distributed in the hope that it will be useful,
  but WITHOUT ANY WARRANTY; without even the implied warranty of
  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
  Lesser General Public License for more details.

  You should have received a copy of the GNU Lesser General
  Public License along with this library; if not, write to the
  Free Software Foundation, Inc., 59 Temple Place, Suite 330,
  Boston, MA  02111-1307  USA
*/
    .vertex   = R"(#version 330 core
                layout(location = 0) in vec4 aPosition;
                layout(location = 1) in vec4 aNormal; // offset
                layout(location = 2) in vec4 aColor;

                out vec4 vColor;

                uniform mat4 uProjection;
                uniform mat4 uViewMatrix;
                uniform mat4 uModelMatrix;

                uniform vec4 viewport;
                uniform int perspective;

                void main() {
                  mat4 modelviewMatrix =  uViewMatrix * uModelMatrix;
                  mat4 projectionMatrix = uProjection;
                  vec2 offset = aNormal.xy;

                  vec4 pos = modelviewMatrix * aPosition;
                  vec4 clip = projectionMatrix * pos;

                  // Perspective ---
                  // convert from world to clip by multiplying with projection scaling factor
                  // invert Y, projections in Processing invert Y
                  vec2 perspScale = (projectionMatrix * vec4(1, -1, 0, 0)).xy;

                  // formula to convert from clip space (range -1..1) to screen space (range 0..[width or height])
                  // screen_p = (p.xy/p.w + <1,1>) * 0.5 * viewport.zw

                  // No Perspective ---
                  // multiply by W (to cancel out division by W later in the pipeline) and
                  // convert from screen to clip (derived from clip to screen above)
                  vec2 noPerspScale = clip.w / (0.5 * viewport.zw);

                  gl_Position.xy = clip.xy + offset.xy * mix(noPerspScale, perspScale, float(perspective > 0));
                  gl_Position.zw = clip.zw;

                  vColor = aColor;
                }
    )",
    .fragment = R"(#version 330 core
                in vec4 vColor;

                out vec4 FragColor;

                void main() {
                  FragColor = vColor;
                }
    )"};

PShader*      shader_point;
VertexBuffer* mesh_sphere;
int           frame_counter = 0;

void settings() {
    size(1024, 768);
    antialiasing     = 8;
    render_to_buffer = false;
    retina_support   = true;
}

void setup() {
    shader_point = loadShader(shader_source_barycentric_wireframe.vertex, shader_source_barycentric_wireframe.fragment);

    /* use convenience function to generate a sphere */
    std::vector<Vertex> sphere_vertices;
    generate_sphere(sphere_vertices, 10, 10);

    constexpr glm::vec4 color{0.5f, 0.85f, 1.0f, 1.0f};
    const glm::vec4     scale{height / 3.0f, height / 3.0f, height / 3.0f, 1.0f};
    for (auto& vertex: sphere_vertices) {
        vertex.position *= scale; // NOTE component 4 i.e w must not be scaled
        vertex.color = color;
    }

    std::vector<Vertex> expanded;

    float point_size = 10;
    for (size_t i = 0; i + 1 < sphere_vertices.size(); i ++) {
        Vertex v0, v1, v2, v3;

        v0 = sphere_vertices[i];
        v1 = sphere_vertices[i];
        v2 = sphere_vertices[i];
        v3 = sphere_vertices[i];

        v0.normal.x = 0;
        v0.normal.y = 0;

        v1.normal.x = point_size;
        v1.normal.y = 0;

        v2.normal.x = point_size;
        v2.normal.y = point_size;

        v3.normal.x = 0;
        v3.normal.y = point_size;

        expanded.push_back(v0);
        expanded.push_back(v1);
        expanded.push_back(v2);

        expanded.push_back(v0);
        expanded.push_back(v2);
        expanded.push_back(v3);
    }

    mesh_sphere = new VertexBuffer();
    mesh_sphere->add_vertices(expanded);
    mesh_sphere->set_shape(TRIANGLES);
    mesh_sphere->update();
}

void draw() {
    background(0.85f);

    fill(0);
    g->debug_text("FPS: " + nf(frameRate, 2), 20, 20);

    noStroke();
    noFill();

    pushMatrix();
    translate(width / 2.0f, height / 2.0f);
    rotateX(frame_counter * 0.01f);
    rotateY(frame_counter * 0.027f);
    rotateZ(frame_counter * 0.043f);

    if (isKeyPressed && key == ' ') {
        frame_counter++;
    }

    shader(shader_point);
    // uniform mat4 projectionMatrix;
    // uniform mat4 modelviewMatrix;
    // uniform vec4 viewport;
    // uniform int perspective;

    /* matrices need to be set every frame for the transforms to work */
    shader_point->set_uniform(SHADER_UNIFORM_VIEW_MATRIX, g->view_matrix);
    shader_point->set_uniform(SHADER_UNIFORM_PROJECTION_MATRIX, g->projection_matrix);
    shader_point->set_uniform(SHADER_UNIFORM_MODEL_MATRIX, g->model_matrix);
    shader_point->set_uniform("viewport", glm::vec4(0, 0, width, height));
    // shader_point->set_uniform("perspective", 0);
    shader_point->set_uniform("perspective", mouseY > height / 2 ? 1 : 0);
    mesh(mesh_sphere);

    /* reset the shader */
    shader();

    popMatrix();
}