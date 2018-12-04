#include <glm/gtc/random.hpp>
#include "tree.h"
#include "missile.h"
#include "explosion.h"

#include <shaders/diffuse_vert_glsl.h>
#include <shaders/diffuse_frag_glsl.h>

using namespace std;
using namespace glm;
using namespace ppgso;

// Static resources
unique_ptr<Mesh> Tree::mesh;
unique_ptr<Texture> Tree::texture;
unique_ptr<Shader> Tree::shader;

Tree::Tree() {
    // Set random scale speed and rotation
    hit = false;
    scale.z *=  5.f;
//    rotation.x = -PI/2.f;
//    speed = {linearRand(-2.0f, 2.0f), linearRand(-5.0f, -10.0f), 0.0f};
//    rotation = ballRand(PI);
//    rotMomentum = ballRand(PI);

    // Initialize static resources if needed
    if (!shader) shader = make_unique<Shader>(diffuse_vert_glsl, diffuse_frag_glsl);
    if (!texture) texture = make_unique<Texture>(image::loadBMP("tree.bmp"));
    if (!mesh) mesh = make_unique<Mesh>("cube.obj");
}

bool Tree::update(Scene &scene, float dt) {

    int pos_x = (int)round(position.x);
    int pos_y = (int)round(position.y);
    if(scene.object_map[pos_x][pos_y] != 1){
        return false;
    }
    generateModelMatrix();

    return true;
}

void Tree::render(Scene &scene, int player_number) {
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

void Tree::onClick(Scene &scene) {
    cout << "Asteroid clicked!" << endl;
//    explode(scene, position, {10.0f, 10.0f, 10.0f}, 0 );
//    age = 10000;
}

void Tree::destroy() {
    hit = true;
}
