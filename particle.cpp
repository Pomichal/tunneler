#include <glm/gtc/random.hpp>
#include "particle.h"
#include "missile.h"
#include "explosion.h"

#include <shaders/diffuse_vert_glsl.h>
#include <shaders/tunneler_vert_glsl.h>
#include <shaders/diffuse_frag_glsl.h>
#include <shaders/tunneler_frag_glsl.h>
#include <shaders/color_vert_glsl.h>
#include <shaders/color_frag_glsl.h>
#include <sstream>

using namespace std;
using namespace glm;
using namespace ppgso;

const vec3 GRAV = {0.0f,0.0f, -2.0f};

// Static resources
unique_ptr<Mesh> Particle::mesh;
unique_ptr<Texture> Particle::texture;
unique_ptr<Shader> Particle::shader;

Particle::Particle(glm::vec3 p, glm::vec3 s, glm::vec3 c, float sc, float b) {

//    scale.z *=  5.0f;
    if (!shader) shader = make_unique<Shader>(color_vert_glsl, color_frag_glsl);
    if (!texture) texture = make_unique<Texture>(image::loadBMP("textura1.bmp"));
    if (!mesh) mesh = make_unique<Mesh>("sphere.obj");
    position = p;
    speed = s;
    color = c;
    scale = sc;
    bumm = b;

    ifstream mtl("my_material.mtl", std::ifstream::binary);
    tinyobj::LoadMtl(this->material_map, this->material, mtl);

    ambient = vec3(material.data()->ambient[0], material.data()->ambient[1], material.data()->ambient[2]);
    diffuse = vec4(material.data()->diffuse[0], material.data()->diffuse[1], material.data()->diffuse[2], 1.0f);
    specular = vec3(material.data()->specular[0], material.data()->specular[1], material.data()->specular[2]);
    shininess = material.data()->shininess * 128;
}

bool Particle::update(Scene &scene, float dt) {

    modelMatrix = mat4(1);
    position -= speed * 0.9f * dt;
//      cout << position.y << endl;
    if(position.z > 0.5f && speed.z < 0 && GAME_SIZE > position.x  && position.x > 0 && GAME_SIZE > position.y
    && position.y > 0){
        speed.z = 0;
    }
    else{
        speed += GRAV * dt;
    }
//      speed += WIND * dTime;
    modelMatrix = translate(modelMatrix, position);
    modelMatrix = glm::scale(modelMatrix, scale * vec3{1, 1, 1});
//      cout << bumm << endl;
    if(bumm < 0.f && bumm > -100.0f){
        for(int i = 0; i < 100; i++) {
            float col_r = linearRand(0.0f, 1.0f);
            float col_g = linearRand(0.0f, 1.0f);
            float col_b = linearRand(0.0f, 1.0f);
//              float random_size = linearRand(0.1f, 0.3f);
            float random_x = linearRand(-2.0f, 2.0f);
            float random_y = linearRand(-2.0f, 2.0f);
            float random_z = linearRand(-5.0f, 5.0f);
            auto particle = make_unique<Particle>(position, vec3{random_x, random_y, random_z},
                                                  vec3{col_r, col_g, col_b}, 0.1f, -101.0f);
            scene.objects.push_back(move(particle));
        }
        return false;
    }
    if(bumm < -102.f){
        return false;
    }
//      if(naObrazovke.y < -0.2 && !broken){
//        float random1  = (float)(rand() % 1000)/(float)1000000;
//        float random2  = (float)(rand() % 1000)/(float)1000000;
//        scene.emplace_back(new Particle(vec3{0, -0.4, -10}, vec3{random1, random2, 0}, vec3{0, 1, 0}, 0.3f,true));
//        random1  = (float)(rand() % 1000)/(float)1000000+0.0005;
//        random2  = (float)(rand() % 1000)/(float)1000000+0.0005;
//        scene.emplace_back(new Particle(vec3{0, -0.4, -10}, vec3{random2, random1, 0}, vec3{0, 1, 0},0.3f,true));
//        random1  = (float)(rand() % 1000)/(float)1000000+0.0005;
//        random2  = (float)(rand() % 1000)/(float)1000000+0.0005;
//        scene.emplace_back(new Particle(vec3{0, -0.4, -10}, vec3{-random1, random2,0}, vec3{0, 1, 0},0.3f,true));
//        return false;
//      }
//      return !jeMimoObrazovky();
    bumm -= dt;
    return true;
}

void Particle::render(Scene &scene, int player_number) {
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

    // use cameras
//    for(int i=0; i<2;i++){
    shader->setUniform("ProjectionMatrix", scene.cameras[player_number]->projectionMatrix);
    shader->setUniform("ViewMatrix", scene.cameras[player_number]->viewMatrix);
//    }

    // render mesh
    shader->setUniform("ModelMatrix", modelMatrix);
//    shader->setUniform("Texture", *texture);
    shader->setUniform("OverallColor", color);



    mesh->render();

//    }

}

void Particle::onClick(Scene &scene) {
    cout << "Asteroid clicked!" << endl;
//    explode(scene, position, {10.0f, 10.0f, 10.0f}, 0 );
//    age = 10000;
}