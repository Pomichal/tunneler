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
    // Set random scale speed and rotation
    scale.x *=  2;
    scale.y *=  0.5f;
    color = c;
//    scale.z *=  f;
//    rotation.x = -PI/2.f;
//    speed = {linearRand(-2.0f, 2.0f), linearRand(-5.0f, -10.0f), 0.0f};
//    rotation = ballRand(PI);
//    rotMomentum = ballRand(PI);

    // Initialize static resources if needed
    if (!shader) shader = make_unique<Shader>(color_vert_glsl, color_frag_glsl);
//    if (!texture) texture = make_unique<Texture>(image::loadBMP("red.bmp"));
    if (!mesh) mesh = make_unique<Mesh>("cube.obj");
}

bool Life::update(Scene &scene, float dt) {

    // Generate modelMatrix from position, rotation and scale
    generateModelMatrix();

    return true;
}


void Life::render(Scene &scene, int player_number) {

    shader->use();

    // Set up light
    shader->setUniform("LightDirection", scene.lightDirection);

    // use cameras
//    for(int i=0; i<2;i++){
    shader->setUniform("ProjectionMatrix", scene.cameras[player_number]->projectionMatrix);
    shader->setUniform("ViewMatrix", scene.cameras[player_number]->viewMatrix);
//    }

    // render mesh
    shader->setUniform("OverallColor", color);
    shader->setUniform("ModelMatrix", modelMatrix);
//    shader->setUniform("Texture", *texture);
    mesh->render();
}