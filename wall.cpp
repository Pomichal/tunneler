#include <glm/gtc/random.hpp>
#include "wall.h"
#include "missile.h"
#include "explosion.h"

#include <shaders/diffuse_vert_glsl.h>
#include <shaders/tunneler_vert_glsl.h>
#include <shaders/tunneler_frag_glsl.h>
#include <sstream>

using namespace std;
using namespace glm;
using namespace ppgso;

// Static resources
unique_ptr<Mesh> Wall::mesh;
unique_ptr<Texture> Wall::texture;
unique_ptr<Shader> Wall::shader;

Wall::Wall() {

    scale.z *=  5.0f;

    // Initialize static resources if needed
    if (!shader) shader = make_unique<Shader>(diffuse_vert_glsl, tunneler_frag_glsl);
    if (!texture) texture = make_unique<Texture>(image::loadBMP("asteroid.bmp"));
    if (!mesh) mesh = make_unique<Mesh>("cube.obj");

    ifstream mtl("my_material.mtl", std::ifstream::binary);
    tinyobj::LoadMtl(this->material_map, this->material, mtl);

    ambient = vec3(material.data()->ambient[0], material.data()->ambient[1], material.data()->ambient[2]);
    diffuse = vec4(material.data()->diffuse[0], material.data()->diffuse[1], material.data()->diffuse[2], 1.0f);
    specular = vec3(material.data()->specular[0], material.data()->specular[1], material.data()->specular[2]);
    shininess = material.data()->shininess * 128;
}

bool Wall::update(Scene &scene, float dt) {

    // Generate modelMatrix from position, rotation and scale
    generateModelMatrix();

    return true;
}

std::string SetLightUniform(const char* propertyName, size_t lightIndex) {
    std::ostringstream ss;
    ss << "allLights[" << lightIndex << "]." << propertyName;
    std::string uniformName = ss.str();

//    cout << uniformName.c_str() << endl;
    return uniformName;
}

void Wall::render(Scene &scene, int player_number) {
//    for(int i = 0; i <2; i++){
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

//
//    for(size_t i = 0; i < scene.gLights.size(); ++i){
//        shader->setUniform(SetLightUniform("position", i), scene.gLights[i].position);
//        shader->setUniform(SetLightUniform("intensities", i), scene.gLights[i].intensities);
//        shader->setUniform(SetLightUniform("attenuation", i), scene.gLights[i].attenuation);
//        shader->setUniform(SetLightUniform("ambientCoefficient", i), scene.gLights[i].ambientCoefficient);
//        shader->setUniform(SetLightUniform("coneAngle", i), scene.gLights[i].coneAngle);
//        shader->setUniform(SetLightUniform("coneDirection", i), scene.gLights[i].coneDirection);
//
//    }


    // use cameras
//    for(int i=0; i<2;i++){
        shader->setUniform("ProjectionMatrix", scene.cameras[player_number]->projectionMatrix);
        shader->setUniform("ViewMatrix", scene.cameras[player_number]->viewMatrix);
//    }

    // render mesh
//    shader->setUniform("CameraPosition", scene.cameras[player_number]->position);
//
    shader->setUniform("ModelMatrix", modelMatrix);
    shader->setUniform("Texture", *texture);


    mesh->render();

//    }

}

void Wall::onClick(Scene &scene) {
    cout << "Asteroid clicked!" << endl;
//    explode(scene, position, {10.0f, 10.0f, 10.0f}, 0 );
//    age = 10000;
}