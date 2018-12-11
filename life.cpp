#include <glm/gtc/random.hpp>
#include "life.h"
#include "missile.h"
#include "explosion.h"

#include <shaders/color_vert_glsl.h>
#include <shaders/color_frag_glsl.h>

using namespace std;
using namespace glm;
using namespace ppgso;

// Static resources
unique_ptr<Mesh> Life::mesh;
unique_ptr<Texture> Life::texture;
unique_ptr<Shader> Life::shader;

Life::Life(glm::vec3 c) {
    scale.x *=  2;
    scale.y *=  0.5f;
    scale.z *=  0.5f;
    color = c;

    // Initialize static resources if needed
    if (!shader) shader = make_unique<Shader>(color_vert_glsl, color_frag_glsl);
    if (!mesh) mesh = make_unique<Mesh>("cube.obj");
}

bool Life::update(Scene &scene, float dt) {

    generateModelMatrix();

    return true;
}


void Life::render(Scene &scene, int player_number) {

    shader->use();

    // Set up light
    shader->setUniform("LightDirection", scene.lightDirection);

    // use cameras
    shader->setUniform("ProjectionMatrix", scene.cameras[player_number]->projectionMatrix);
    shader->setUniform("ViewMatrix", scene.cameras[player_number]->viewMatrix);

    // render mesh
    shader->setUniform("OverallColor", color);
    shader->setUniform("ModelMatrix", modelMatrix);
    mesh->render();
}