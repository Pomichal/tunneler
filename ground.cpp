#include <glm/gtc/random.hpp>
#include "ground.h"
#include "missile.h"
#include "explosion.h"

#include <shaders/diffuse_vert_glsl.h>
#include <shaders/diffuse_frag_glsl.h>

using namespace std;
using namespace glm;
using namespace ppgso;

// Static resources
unique_ptr<Mesh> Ground::mesh;
unique_ptr<Texture> Ground::texture;
unique_ptr<Shader> Ground::shader;

Ground::Ground(int size_x, int size_y) {
    // Set random scale speed and rotation
    scale.x *=  size_x;
    scale.y *=  size_y;
//    scale.z *=  f;
//    rotation.x = -PI/2.f;
//    speed = {linearRand(-2.0f, 2.0f), linearRand(-5.0f, -10.0f), 0.0f};
//    rotation = ballRand(PI);
//    rotMomentum = ballRand(PI);

    // Initialize static resources if needed
    if (!shader) shader = make_unique<Shader>(diffuse_vert_glsl, diffuse_frag_glsl);
    if (!texture) texture = make_unique<Texture>(image::loadBMP("stars.bmp"));
    if (!mesh) mesh = make_unique<Mesh>("cube.obj");
}

bool Ground::update(Scene &scene, float dt) {

    // Generate modelMatrix from position, rotation and scale
    generateModelMatrix();

    return true;
}


void Ground::render(Scene &scene) {
    shader->use();

    // Set up light
    shader->setUniform("LightDirection", scene.lightDirection);

    // use cameras
    for(int i=0; i<2;i++){
        shader->setUniform("ProjectionMatrix", scene.cameras[i]->projectionMatrix);
        shader->setUniform("ViewMatrix", scene.cameras[i]->viewMatrix);
    }

    // render mesh
    shader->setUniform("ModelMatrix", modelMatrix);
    shader->setUniform("Texture", *texture);
    mesh->render();
}