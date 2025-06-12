/*
 * ⚠️⚠️⚠️ THIS IS WORK IN PROGRESS!!! ⚠️⚠️⚠️
 * this example is based on the Processing example
 * https://processing.org/reference/lights_.html
 * and shows how to use light with 3D primitives in umfeld.
 */

#include "Umfeld.h"
// #include "PShader.h"
// #include "ShaderSourceColorTextureLights.h"
// #include "PGraphicsOpenGL.h"

using namespace umfeld;

// add:
// - [`ambientLight()`](https://processing.org/reference/ambientLight_.html)
// - [`directionalLight()`](https://processing.org/reference/directionalLight_.html)
// - [`pointLight()`](https://processing.org/reference/pointLight_.html)
// - [`spotLight()`](https://processing.org/reference/spotLight_.html)

// PShader* shader_fill_texture_lights;

void settings() {
    size(400, 400);
    render_to_buffer = false;
}

void setup() {
    noStroke();
    fill(1.0f);
    sphereDetail(30);
    sphereDetail(5);
    // shader_fill_texture_lights = loadShader(shader_source_color_texture_lights.get_vertex_source(), shader_source_color_texture_lights.get_fragment_source());
    hint(ENABLE_DEPTH_TEST);
}

// void WIP_lights() {
//     if (isMousePressed) {
//         hint(DISABLE_DEPTH_TEST);
//     } else {
//         hint(ENABLE_DEPTH_TEST); // TODO lighting needs depth testing for spheres to be rendered correctly
//     }
//
//     shader(shader_fill_texture_lights);
//     checkOpenGLError("shader_fill_texture_lights->use()");
//
//     shader_fill_texture_lights->set_uniform(SHADER_UNIFORM_MODEL_MATRIX, g->model_matrix);
//     shader_fill_texture_lights->set_uniform(SHADER_UNIFORM_VIEW_MATRIX, g->view_matrix);
//     shader_fill_texture_lights->set_uniform(SHADER_UNIFORM_PROJECTION_MATRIX, g->projection_matrix);
//     shader_fill_texture_lights->set_uniform(SHADER_UNIFORM_TEXTURE_UNIT, 0);
//     shader_fill_texture_lights->set_uniform("normalMatrix", glm::mat3(glm::transpose(glm::inverse(g->view_matrix * g->model_matrix))));
//     shader_fill_texture_lights->set_uniform("texMatrix", glm::mat4(1.0f));                  // or a real matrix if you’re transforming texCoords
//     shader_fill_texture_lights->set_uniform("ambient", glm::vec4(0.2f, 0.2f, 0.2f, 1.0f));  // base ambient reflectance
//     shader_fill_texture_lights->set_uniform("specular", glm::vec4(0.5f, 0.5f, 0.5f, 1.0f)); // specular highlight color
//     shader_fill_texture_lights->set_uniform("emissive", glm::vec4(0.1f, 0.1f, 0.1f, 1.0f)); // emission/self-lighting
//     shader_fill_texture_lights->set_uniform("shininess", 64.0f);                            // specular exponent
//     shader_fill_texture_lights->set_uniform("lightCount", 1);
//     shader_fill_texture_lights->set_uniform("lightPosition[0]", glm::vec4(0, 0, -2.0f * height, 1.0f));
//     shader_fill_texture_lights->set_uniform("lightNormal[0]", glm::vec3(0.0f, -1.0f, 0.0f)); // ignored for point lights
//     shader_fill_texture_lights->set_uniform("lightAmbient[0]", glm::vec3(0.05f));
//     shader_fill_texture_lights->set_uniform("lightDiffuse[0]", glm::vec3(1.0f));
//     shader_fill_texture_lights->set_uniform("lightSpecular[0]", glm::vec3(1.0f));
//     shader_fill_texture_lights->set_uniform("lightFalloff[0]", glm::vec3(1.0f, 0.0f, 0.0f)); // constant falloff
//     shader_fill_texture_lights->set_uniform("lightSpot[0]", glm::vec2(-1.0f, 0.0f));         // disables spotlight
//     checkOpenGLError("set_uniform()");
// }

// void WIP_noLights() {
//     g->resetShader();
//     // TODO reload matrices?!?
// }

// void WIP_update_light_shader() {
//     shader_fill_texture_lights->set_uniform(SHADER_UNIFORM_MODEL_MATRIX, g->model_matrix);
//     shader_fill_texture_lights->set_uniform(SHADER_UNIFORM_VIEW_MATRIX, g->view_matrix);
//     shader_fill_texture_lights->set_uniform("normalMatrix", glm::mat3(glm::transpose(glm::inverse(g->view_matrix * g->model_matrix))));
// }

void draw() {
    background(0.0f);
    // Include lights() at the beginning
    // of draw() to keep them persistent

    // WIP_lights();
    lights();

    noStroke();
    fill(1.0f);
    pushMatrix();
    translate(80, 200, 0);
    rotateY(frameCount * 0.023f);
    rotateX(frameCount * 0.01f);
    // WIP_update_light_shader(); // TODO how to handle this?!?
    sphere(120);
    popMatrix();

    stroke(1,0,0);
    fill(1.0f);
    pushMatrix();
    translate(320, 200, 0);
    rotateX(frameCount * 0.023f);
    rotateY(frameCount * 0.01f);
    // WIP_update_light_shader(); // TODO how to handle this?!?
    sphere(120);
    popMatrix();

    // WIP_noLights();
    noLights();
}