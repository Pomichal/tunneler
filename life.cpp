#include <glm/gtc/random.hpp>
#include "life.h"
#include "missile.h"
#include "explosion.h"

#include <shaders/diffuse_vert_glsl.h>
#include <shaders/diffuse_frag_glsl.h>

using namespace std;
using namespace glm;
using namespace ppgso;

// Static resources
unique_ptr<Mesh> Life::mesh;
unique_ptr<Texture> Life::texture;
unique_ptr<Shader> Life::shader;

Life::Life() {
    // Set random scale speed and rotation
    scale.x *=  10;
    scale.y *=  10;
//    scale.z *=  f;
//    rotation.x = -PI/2.f;
//    speed = {linearRand(-2.0f, 2.0f), linearRand(-5.0f, -10.0f), 0.0f};
//    rotation = ballRand(PI);
//    rotMomentum = ballRand(PI);

    // Initialize static resources if needed
    if (!shader) shader = make_unique<Shader>(diffuse_vert_glsl, diffuse_frag_glsl);
    if (!texture) texture = make_unique<Texture>(image::loadBMP("lena.bmp"));
    if (!mesh) mesh = make_unique<Mesh>("cube.obj");
}

bool Life::update(Scene &scene, float dt) {
//    std::cout << position.x << " " << position.y << " " << position.z << endl;

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
    shader->setUniform("ModelMatrix", modelMatrix);
    shader->setUniform("Texture", *texture);
    mesh->render();
}