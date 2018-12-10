#include <glm/gtc/random.hpp>
#include "ground.h"
#include "missile.h"
#include "explosion.h"

#include <shaders/diffuse_vert_glsl.h>
#include <shaders/tunneler_frag_glsl.h>

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


    // Initialize static resources if needed
    if (!shader) shader = make_unique<Shader>(diffuse_vert_glsl, tunneler_frag_glsl);
    if (!texture) texture = make_unique<Texture>(image::loadBMP("stars.bmp"));
    if (!mesh) mesh = make_unique<Mesh>("cube.obj");

    ifstream mtl("my_material.mtl", std::ifstream::binary);
    tinyobj::LoadMtl(this->material_map, this->material, mtl);

    ambient = vec3(material.data()->ambient[0], material.data()->ambient[1], material.data()->ambient[2]);
    diffuse = vec4(material.data()->diffuse[0], material.data()->diffuse[1], material.data()->diffuse[2], 1.0f);
    specular = vec3(material.data()->specular[0], material.data()->specular[1], material.data()->specular[2]);
    shininess = material.data()->shininess * 128;
}

bool Ground::update(Scene &scene, float dt) {

    // Generate modelMatrix from position, rotation and scale
    generateModelMatrix();

    return true;
}


void Ground::render(Scene &scene, int player_number) {
    shader->use();

    // Set up light
    shader->use();

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
//    shader->setUniform("LightDirection2", scene.lightDirection);
//    shader->setUniform("LightColor2", vec3{0,0,1});
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