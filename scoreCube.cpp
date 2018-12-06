#include <glm/gtc/random.hpp>
#include "scoreCube.h"
#include "missile.h"
#include "explosion.h"

#include <shaders/diffuse_vert_glsl.h>
#include <shaders/diffuse_frag_glsl.h>

using namespace std;
using namespace glm;
using namespace ppgso;

// Static resources
//unique_ptr<Mesh> scoreCube::mesh;
//unique_ptr<Texture> scoreCube::texture;
//unique_ptr<Shader> scoreCube::shader;

scoreCube::scoreCube(int type) {

    if(type == 1) {
        rotation.x = PI / 2.0f;
        // Initialize static resources if needed
        if (!shader) shader = make_unique<Shader>(diffuse_vert_glsl, diffuse_frag_glsl);
        if (!texture) texture = make_unique<Texture>(image::loadBMP("textura1.bmp"));
        if (!mesh) mesh = make_unique<Mesh>("scorecube.obj");
    }
    else{
//        scale.x *= 5;
//        scale.y *= 5;
        rotation = vec3{-PI/2.0f, PI/2.0f, PI/2.0f};
//        scale.x *= -1;
//        rotation.y = PI/2.0f;
        if (!shader) shader = make_unique<Shader>(diffuse_vert_glsl, diffuse_frag_glsl);
        if (!texture) texture = make_unique<Texture>(image::loadBMP("info.bmp"));
        if (!mesh) mesh = make_unique<Mesh>("scorecube2.obj");
    }
}

bool scoreCube::update(Scene &scene, float dt) {

    // Generate modelMatrix from position, rotation and scale
    generateModelMatrix();

    return true;
}

void scoreCube::render(Scene &scene, int player_number) {
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

void scoreCube::onClick(Scene &scene) {
    cout << "Asteroid clicked!" << endl;
//    explode(scene, position, {10.0f, 10.0f, 10.0f}, 0 );
//    age = 10000;
}