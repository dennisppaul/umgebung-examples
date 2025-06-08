#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include "Umfeld.h"
#include "PShader.h"
#include "ShaderSource.h"
#include "VertexBuffer.h"
#include "Geometry.h"

using namespace umfeld;

ShaderSource shader_source_barycentric_wireframe{
    .vertex   = R"(#version 330 core

                layout(location = 0) in vec4 aPosition;
                layout(location = 1) in vec4 aNormal;
                layout(location = 2) in vec4 aColor;

                out vec4 vColor;

                uniform mat4 uProjection;
                uniform mat4 uViewMatrix;
                uniform mat4 uModelMatrix;

                uniform vec4 viewport;
                uniform int perspective;
                uniform vec3 scale;

                uniform float line_width;

                void main() {
                  mat4 modelviewMatrix =  uViewMatrix * uModelMatrix;
                  mat4 projectionMatrix = uProjection;
                  vec4 direction = aNormal;
                  vec4 posp = modelviewMatrix * aPosition;
                  vec4 posq = modelviewMatrix * (aPosition + vec4(direction.xyz, 0));

                  // Moving vertices slightly toward the camera
                  // to avoid depth-fighting with the fill triangles.
                  // Discussed here:
                  // http://www.opengl.org/discussion_boards/ubbthreads.php?ubb=showflat&Number=252848
                  posp.xyz = posp.xyz * scale;
                  posq.xyz = posq.xyz * scale;

                  vec4 p = projectionMatrix * posp;
                  vec4 q = projectionMatrix * posq;

                  // formula to convert from clip space (range -1..1) to screen space (range 0..[width or height])
                  // screen_p = (p.xy/p.w + <1,1>) * 0.5 * viewport.zw

                  // prevent division by W by transforming the tangent formula (div by 0 causes
                  // the line to disappear, see https://github.com/processing/processing/issues/5183)
                  // t = screen_q - screen_p
                  //
                  // tangent is normalized and we don't care which direction it points to (+-)
                  // t = +- normalize( screen_q - screen_p )
                  // t = +- normalize( (q.xy/q.w+<1,1>)*0.5*viewport.zw - (p.xy/p.w+<1,1>)*0.5*viewport.zw )
                  //
                  // extract common factor, <1,1> - <1,1> cancels out
                  // t = +- normalize( (q.xy/q.w - p.xy/p.w) * 0.5 * viewport.zw )
                  //
                  // convert to common divisor
                  // t = +- normalize( ((q.xy*p.w - p.xy*q.w) / (p.w*q.w)) * 0.5 * viewport.zw )
                  //
                  // remove the common scalar divisor/factor, not needed due to normalize and +-
                  // (keep viewport - can't remove because it has different components for x and y
                  //  and corrects for aspect ratio, see https://github.com/processing/processing/issues/5181)
                  // t = +- normalize( (q.xy*p.w - p.xy*q.w) * viewport.zw )

                  vec2 tangent = (q.xy*p.w - p.xy*q.w) * viewport.zw;

                  // don't normalize zero vector (line join triangles and lines perpendicular to the eye plane)
                  tangent = length(tangent) == 0.0 ? vec2(0.0, 0.0) : normalize(tangent);

                  // flip tangent to normal (it's already normalized)
                  vec2 normal = vec2(-tangent.y, tangent.x);

                  float thickness = direction.w * line_width;
                  vec2 offset = normal * thickness;

                  // Perspective ---
                  // convert from world to clip by multiplying with projection scaling factor
                  // to get the right thickness (see https://github.com/processing/processing/issues/5182)
                  // invert Y, projections in Processing invert Y
                  vec2 perspScale = (projectionMatrix * vec4(1, -1, 0, 0)).xy;

                  // No Perspective ---
                  // multiply by W (to cancel out division by W later in the pipeline) and
                  // convert from screen to clip (derived from clip to screen above)
                  vec2 noPerspScale = p.w / (0.5 * viewport.zw);

                  gl_Position.xy = p.xy + offset.xy * mix(noPerspScale, perspScale, float(perspective > 0));
                  gl_Position.zw = p.zw;

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

PShader*      shader_line;
VertexBuffer* mesh_sphere;
int           frame_counter = 0;

void settings() {
    size(1024, 768);
    antialiasing     = 8;
    render_to_buffer = false;
    retina_support   = true;
}

void add_line_quad(const glm::vec3& p0, const glm::vec3& p1, float thickness, std::vector<Vertex>& out) {
    // glm::vec3 dir = glm::normalize(p1 - p0);
    glm::vec3 dir = p1 - p0; // NOTE no need to noralize, the shader will do it

    // The shader will use this direction to compute screen-space offset
    glm::aligned_vec4 normal(dir, thickness);
    glm::aligned_vec4 color(0.5f, 0.85f, 1.0f, 1.0f); // or pass in if needed

    glm::aligned_vec4 pos0(p0, 1.0f);
    glm::aligned_vec4 pos1(p1, 1.0f);

    // These 6 vertices form two triangles of the line quad
    Vertex v0, v1, v2, v3;

    v0.position = pos0;
    v1.position = pos1;
    v2.position = pos0;
    v3.position = pos1;

    v0.normal = normal;
    v1.normal = normal;
    normal.w  = -thickness;
    v2.normal = normal;
    v3.normal = normal;

    v0.color = color;
    v1.color = color;
    v2.color = color;
    v3.color = color;

    // Add first triangle
    out.push_back(v0);
    out.push_back(v1);
    out.push_back(v2);

    // Add second triangle
    out.push_back(v2);
    out.push_back(v1);
    out.push_back(v3);
}

void setup() {
    shader_line = loadShader(shader_source_barycentric_wireframe.vertex, shader_source_barycentric_wireframe.fragment);

    /* use convenience function to generate a sphere */
    std::vector<Vertex> sphere_vertices;
    generate_sphere(sphere_vertices, 10, 10);

    /* compile list of vertices with attributes like color,  */
    constexpr glm::vec4 color{0.5f, 0.85f, 1.0f, 1.0f};
    const glm::vec4     scale{height / 3.0f, height / 3.0f, height / 3.0f, 1.0f};


    for (auto& vertex: sphere_vertices) {
        vertex.position *= scale; // NOTE component 4 i.e w must not be scaled
        vertex.color = color;
    }

    std::vector<Vertex> expanded;

    float thickness = 3.0f;
    for (size_t i = 0; i + 1 < sphere_vertices.size(); i += 2) {
        glm::vec3 p0 = glm::vec3(sphere_vertices[i].position);
        glm::vec3 p1 = glm::vec3(sphere_vertices[i + 1].position);
        add_line_quad(p0, p1, thickness, expanded);
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

    shader(shader_line);
    /* matrices need to be set every frame for the transforms to work */
    shader_line->set_uniform(SHADER_UNIFORM_VIEW_MATRIX, g->view_matrix);
    shader_line->set_uniform(SHADER_UNIFORM_PROJECTION_MATRIX, g->projection_matrix);
    shader_line->set_uniform(SHADER_UNIFORM_MODEL_MATRIX, g->model_matrix);
    shader_line->set_uniform("viewport", glm::vec4(0, 0, width, height));
    shader_line->set_uniform("perspective", 0);
    // shader_line->set_uniform("perspective", mouseY > height / 2 ? 1 : 0);
    float scale_factor = map(mouseX, 0, width, 0.1f, 3.0f);
    shader_line->set_uniform("scale", glm::vec3(scale_factor, scale_factor, scale_factor));
    shader_line->set_uniform("line_width", map(mouseY, 0, height, 0.1f, 5.0f)); // NOTE added line width … might be helpful
    mesh(mesh_sphere);

    /* reset the shader */
    shader();

    popMatrix();
}