#include <glm/gtc/random.hpp>
#include "tree.h"
#include "missile.h"
#include "explosion.h"

#include <shaders/diffuse_vert_glsl.h>
#include <shaders/tunneler_frag_glsl.h>

using namespace std;
using namespace glm;
using namespace ppgso;

// Static resources
unique_ptr<Mesh> Tree::mesh;
unique_ptr<Texture> Tree::texture;
unique_ptr<Shader> Tree::shader;

Tree::Tree(int type) {
    // Set random scale speed and rotation
    hit = false;
//    scale.z *=  5.f;
    rotation.x = -PI/2.f;
//    speed = {linearRand(-2.0f, 2.0f), linearRand(-5.0f, -10.0f), 0.0f};
//    rotation = ballRand(PI);
//    rotMomentum = ballRand(PI);

    // Initialize static resources if needed
    if (!shader) shader = make_unique<Shader>(diffuse_vert_glsl, tunneler_frag_glsl);
    if (!mesh) mesh = make_unique<Mesh>("tree.obj");
    if(type == 0) {
        if (!texture) texture = make_unique<Texture>(image::loadBMP("tree2.bmp"));
    } else {
        if (!texture) texture = make_unique<Texture>(image::loadBMP("tree.bmp"));
    }

    ifstream mtl("my_material.mtl", std::ifstream::binary);
    tinyobj::LoadMtl(this->material_map, this->material, mtl);

    ambient = vec3(material.data()->ambient[0], material.data()->ambient[1], material.data()->ambient[2]);
    diffuse = vec4(material.data()->diffuse[0], material.data()->diffuse[1], material.data()->diffuse[2], 1.0f);
    specular = vec3(material.data()->specular[0], material.data()->specular[1], material.data()->specular[2]);
    shininess = material.data()->shininess * 128;
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
    shader->setUniform("MaterialAmbient", {ambient.x, ambient.y, ambient.z});
    shader->setUniform("MaterialDiffuse", {diffuse.x, diffuse.y, diffuse.z, 1.0f});
    shader->setUniform("MaterialSpecular", {specular.x, specular.y, specular.z});
    shader->setUniform("MaterialShininess", shininess);
//        shader->setUniform("numLights", 2);

//    // Set up light
    shader->setUniform("LightDirection", scene.lightDirection);
    shader->setUniform("LightColor", scene.lightColor);

    shader->setUniform("LightDirection2", scene.lightDirection2);
    shader->setUniform("LightColor2", scene.lightColor2);

    shader->setUniform("AmbientLightColor", scene.AmbientLightColor);

    shader->setUniform("MaterialAmbient", ambient);
    shader->setUniform("MaterialDiffuse", diffuse);
    shader->setUniform("MaterialSpecular", specular);
    shader->setUniform("MaterialShininess", shininess);
    // use cameras
//    for(int i=0; i<2;i++){
        shader->setUniform("ProjectionMatrix", scene.cameras[player_number]->projectionMatrix);
        shader->setUniform("ViewMatrix", scene.cameras[player_number]->viewMatrix);
//    }

    // render mesh
    shader->setUniform("ModelMatrix", modelMatrix);
    shader->setUniform("CameraPosition", scene.cameras[player_number]->position);
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
