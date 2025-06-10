/*
 * ⚠️⚠️⚠️ THIS IS WORK IN PROGRESS!!! ⚠️⚠️⚠️
 * this example is based on the Processing example
 * https://processing.org/reference/lights_.html
 * and shows how to use light with 3D primitives in umfeld.
 */

#include "Umfeld.h"
#include "PShader.h"
#include "ShaderSource.h"
#include "PGraphicsOpenGL.h"

using namespace umfeld;

PShader* shader_lights;

inline ShaderSource shader_source_color_texture_lights{
    .vertex   = R"(#version 330 core

                    layout(location = 0) in vec4 aPosition;
                    layout(location = 1) in vec4 aNormal;
                    layout(location = 2) in vec4 aColor;
                    layout(location = 3) in vec3 aTexCoord;
                    layout(location = 4) in float userdata;

                    uniform mat4 uProjection;
                    uniform mat4 uViewMatrix;
                    uniform mat4 uModelMatrix;

                    uniform mat4 texMatrix;
                    uniform mat3 normalMatrix;

                    uniform vec4 ambient;
                    uniform vec4 specular;
                    uniform vec4 emissive;
                    uniform float shininess;

                    uniform int lightCount;
                    uniform vec4 lightPosition[8];
                    uniform vec3 lightNormal[8];
                    uniform vec3 lightAmbient[8];
                    uniform vec3 lightDiffuse[8];
                    uniform vec3 lightSpecular[8];
                    uniform vec3 lightFalloff[8];
                    uniform vec2 lightSpot[8];

                    out vec4 vertColor;
                    out vec4 backVertColor;
                    out vec4 vertTexCoord;

                    const float zero_float = 0.0;
                    const float one_float = 1.0;
                    const vec3 zero_vec3 = vec3(0);

                    float falloffFactor(vec3 lightPos, vec3 vertPos, vec3 coeff) {
                        vec3 lpv = lightPos - vertPos;
                        vec3 dist = vec3(one_float);
                        dist.z = dot(lpv, lpv);
                        dist.y = sqrt(dist.z);
                        return one_float / dot(dist, coeff);
                    }

                    float spotFactor(vec3 lightPos, vec3 vertPos, vec3 lightNorm, float minCos, float spotExp) {
                        vec3 lpv = normalize(lightPos - vertPos);
                        vec3 nln = -one_float * lightNorm;
                        float spotCos = dot(nln, lpv);
                        return spotCos <= minCos ? zero_float : pow(spotCos, spotExp);
                    }

                    float lambertFactor(vec3 lightDir, vec3 vecNormal) {
                        return max(zero_float, dot(lightDir, vecNormal));
                    }

                    float blinnPhongFactor(vec3 lightDir, vec3 vertPos, vec3 vecNormal, float shine) {
                        vec3 np = normalize(vertPos);
                        vec3 ldp = normalize(lightDir - np);
                        return pow(max(zero_float, dot(ldp, vecNormal)), shine);
                    }

                    void main() {
                        mat4 mv = uViewMatrix * uModelMatrix;
                        mat4 mvp = uProjection * mv;
                        gl_Position = mvp * aPosition;

                        vec3 ecVertex = vec3(mv * aPosition);
                        vec3 ecNormal = normalize(normalMatrix * aNormal.xyz);
                        vec3 ecNormalInv = -ecNormal;

                        vec3 totalAmbient = vec3(0.0);
                        vec3 totalFrontDiffuse = vec3(0.0);
                        vec3 totalFrontSpecular = vec3(0.0);
                        vec3 totalBackDiffuse = vec3(0.0);
                        vec3 totalBackSpecular = vec3(0.0);

                        for (int i = 0; i < 8; i++) {
                            if (i >= lightCount) break;

                            vec3 lightPos = lightPosition[i].xyz;
                            bool isDir = lightPosition[i].w < 1.0;
                            float spotCos = lightSpot[i].x;
                            float spotExp = lightSpot[i].y;

                            vec3 lightDir;
                            float falloff = 1.0;
                            float spotf = 1.0;

                            if (isDir) {
                                lightDir = -lightNormal[i];
                            } else {
                                falloff = falloffFactor(lightPos, ecVertex, lightFalloff[i]);
                                lightDir = normalize(lightPos - ecVertex);
                            }

                            if (spotExp > 0.0) {
                                spotf = spotFactor(lightPos, ecVertex, lightNormal[i], spotCos, spotExp);
                            }

                            if (any(greaterThan(lightAmbient[i], zero_vec3))) {
                                totalAmbient += lightAmbient[i] * falloff;
                            }

                            if (any(greaterThan(lightDiffuse[i], zero_vec3))) {
                                totalFrontDiffuse += lightDiffuse[i] * falloff * spotf * lambertFactor(lightDir, ecNormal);
                                totalBackDiffuse += lightDiffuse[i] * falloff * spotf * lambertFactor(lightDir, ecNormalInv);
                            }

                            if (any(greaterThan(lightSpecular[i], zero_vec3))) {
                                totalFrontSpecular += lightSpecular[i] * falloff * spotf * blinnPhongFactor(lightDir, ecVertex, ecNormal, shininess);
                                totalBackSpecular += lightSpecular[i] * falloff * spotf * blinnPhongFactor(lightDir, ecVertex, ecNormalInv, shininess);
                            }
                        }

                        vertColor = vec4(totalAmbient, 0.0) * ambient +
                                    vec4(totalFrontDiffuse, 1.0) * aColor +
                                    vec4(totalFrontSpecular, 0.0) * specular +
                                    vec4(emissive.rgb, 0.0);

                        backVertColor = vec4(totalAmbient, 0.0) * ambient +
                                        vec4(totalBackDiffuse, 1.0) * aColor +
                                        vec4(totalBackSpecular, 0.0) * specular +
                                        vec4(emissive.rgb, 0.0);

                        vertTexCoord = texMatrix * vec4(aTexCoord, 1.0);
                    }
        )",
    .fragment = R"(#version 330 core

                    in vec4 vertColor;
                    in vec4 backVertColor;
                    in vec4 vertTexCoord;

                    out vec4 FragColor;

                    uniform sampler2D uTexture;

                    void main() {
                        vec4 tex = texture(uTexture, vertTexCoord.st);
                        FragColor = tex * (gl_FrontFacing ? vertColor : backVertColor);
                    }
        )"};

void settings() {
    size(400, 400);
    render_to_buffer = false;
}

void setup() {
    noStroke();
    fill(1.0f);
    sphereDetail(30);
    shader_lights = loadShader(shader_source_color_texture_lights.vertex,
                               shader_source_color_texture_lights.fragment);
}

void WIP_lights() {
    if (isMousePressed) {
        hint(DISABLE_DEPTH_TEST);
    } else {
        hint(ENABLE_DEPTH_TEST); // TODO lighting needs depth testing for spheres to be rendered correctly
    }

    shader(shader_lights);
    shader_lights->use();
    checkOpenGLError("shader_lights->use()");

    shader_lights->set_uniform(SHADER_UNIFORM_MODEL_MATRIX, g->model_matrix);
    shader_lights->set_uniform(SHADER_UNIFORM_VIEW_MATRIX, g->view_matrix);
    shader_lights->set_uniform(SHADER_UNIFORM_PROJECTION_MATRIX, g->projection_matrix);
    shader_lights->set_uniform(SHADER_UNIFORM_TEXTURE_UNIT, 0);
    shader_lights->set_uniform("normalMatrix", glm::mat3(glm::transpose(glm::inverse(g->view_matrix * g->model_matrix))));
    shader_lights->set_uniform("texMatrix", glm::mat4(1.0f));                  // or a real matrix if you’re transforming texCoords
    shader_lights->set_uniform("ambient", glm::vec4(0.2f, 0.2f, 0.2f, 1.0f));  // base ambient reflectance
    shader_lights->set_uniform("specular", glm::vec4(0.5f, 0.5f, 0.5f, 1.0f)); // specular highlight color
    shader_lights->set_uniform("emissive", glm::vec4(0.1f, 0.1f, 0.1f, 1.0f)); // emission/self-lighting
    shader_lights->set_uniform("shininess", 64.0f);                            // specular exponent
    shader_lights->set_uniform("lightCount", 1);
    shader_lights->set_uniform("lightPosition[0]", glm::vec4(0, 0, -2.0f * height, 1.0f));
    shader_lights->set_uniform("lightNormal[0]", glm::vec3(0.0f, -1.0f, 0.0f)); // ignored for point lights
    shader_lights->set_uniform("lightAmbient[0]", glm::vec3(0.05f));
    shader_lights->set_uniform("lightDiffuse[0]", glm::vec3(1.0f));
    shader_lights->set_uniform("lightSpecular[0]", glm::vec3(1.0f));
    shader_lights->set_uniform("lightFalloff[0]", glm::vec3(1.0f, 0.0f, 0.0f)); // constant falloff
    shader_lights->set_uniform("lightSpot[0]", glm::vec2(-1.0f, 0.0f));         // disables spotlight
    checkOpenGLError("set_uniform()");
}

void WIP_noLights() {
    g->resetShader();
    // TODO reload matrices?!?
}

void WIP_update_light_shader() {
    shader_lights->set_uniform(SHADER_UNIFORM_MODEL_MATRIX, g->model_matrix);
    shader_lights->set_uniform(SHADER_UNIFORM_VIEW_MATRIX, g->view_matrix);
    shader_lights->set_uniform("normalMatrix", glm::mat3(glm::transpose(glm::inverse(g->view_matrix * g->model_matrix))));
}

void draw() {
    background(0.0f);
    // Include lights() at the beginning
    // of draw() to keep them persistent

    WIP_lights(); // lights();

    pushMatrix();
    translate(80, 200, 0);
    rotateY(frameCount * 0.023f);
    rotateX(frameCount * 0.01f);
    WIP_update_light_shader(); // TODO how to handle this?!?
    sphere(120);
    popMatrix();

    pushMatrix();
    translate(320, 200, 0);
    rotateX(frameCount * 0.023f);
    rotateY(frameCount * 0.01f);
    WIP_update_light_shader(); // TODO how to handle this?!?
    sphere(120);
    popMatrix();

    WIP_noLights(); // noLights();
}