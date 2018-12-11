#include <glm/gtc/random.hpp>
#include "scene.h"
#include "missile.h"
#include "tree.h"
#include "wall.h"
#include "explosion.h"
#include "particle.h"

#include <shaders/diffuse_vert_glsl.h>
#include <shaders/tunneler_frag_glsl.h>

using namespace std;
using namespace glm;
using namespace ppgso;

// shared resources
unique_ptr<Mesh> Missile::mesh;
unique_ptr<Shader> Missile::shader;
unique_ptr<Texture> Missile::texture;

Missile::Missile() {
  // Set default speed
  speed = {0.0f, 3.0f, 0.0f};
  rotMomentum = {0.0f, 0.0f, linearRand(-PI/4.0f, PI/4.0f)};

  // Initialize static resources if needed
  if (!shader) shader = make_unique<Shader>(diffuse_vert_glsl, tunneler_frag_glsl);
  if (!texture) texture = make_unique<Texture>(image::loadBMP("missile.bmp"));
  if (!mesh) mesh = make_unique<Mesh>("missile.obj");

  ifstream mtl("my_material.mtl", std::ifstream::binary);
  tinyobj::LoadMtl(this->material_map, this->material, mtl);

  ambient = vec3(material.data()->ambient[0], material.data()->ambient[1], material.data()->ambient[2]);
  diffuse = vec4(material.data()->diffuse[0], material.data()->diffuse[1], material.data()->diffuse[2], 1.0f);
  specular = vec3(material.data()->specular[0], material.data()->specular[1], material.data()->specular[2]);
  shininess = material.data()->shininess * 128;
}


Missile::Missile(float x, float y) {
  speed = {x, y, 0.0f};
//  rotMomentum = {0.0f, 0.0f, linearRand(-PI/4.0f, PI/4.0f)};
//cout << x << " " << y << endl;

  if(x > 0){
    rotation.y = -PI/2.0f;
  }
  if(x < 0){
    rotation.y = PI/2.0f;
  }
  if(y > 0){
    rotation.y = 0.0f;
  }
  if(y < 0){
    rotation.y = -PI;
  }
  // Initialize static resources if needed
  if (!shader) shader = make_unique<Shader>(diffuse_vert_glsl, tunneler_frag_glsl);
  if (!texture) texture = make_unique<Texture>(image::loadBMP("missile.bmp"));
  if (!mesh) mesh = make_unique<Mesh>("missile.obj");

}


bool Missile::update(Scene &scene, float dt) {
  // Increase age
  age += dt;


  // colision detection
  int pos_x = (int)round(position.x);
  int pos_y = (int)round(position.y);
// cout << pos_x << " " << pos_y << endl;

  // missile vs wall
  if (scene.object_map[pos_x][pos_y] == 2) {
      explode(scene, position, scale);
      return false;
  }

  // missile vs tree
  if (scene.object_map[pos_x][pos_y] == 1) {
    scene.object_map[pos_x][pos_y] = 0;
    explode(scene, position, scale);
    return false;
  }

  // Move the projectile
  position += speed * dt;

  // Die after 5s
  if (age > 5.0f) return false;

  generateModelMatrix();
  return true;
}

void Missile::explode(Scene &scene, vec3 explosionPosition, vec3 explosionScale) {
  // Generate explosion
  for(int i = 0; i < 10; i++) {
//              float random_size = linearRand(0.1f, 0.3f);
    float random_x = linearRand(-5.0f, 5.0f);
    float random_y = linearRand(-5.0f, 5.0f);
    float random_z = linearRand(-3.0f, 3.0f);
    auto particle = make_unique<Particle>(position, vec3{random_x, random_y, random_z},
                                          vec3{0.1f, 0.1f, 0.1f}, 0.5f, -101.0f);
    scene.objects.push_back(move(particle));
  }
  auto explosion = make_unique<Explosion>();
  explosion->position = explosionPosition;
  explosion->scale = explosionScale;
  explosion->speed = vec3{2,2,0};
  scene.objects.push_back(move(explosion));
}

void Missile::render(Scene &scene, int player_number) {
  shader->use();

  // Set up light
  shader->setUniform("MaterialAmbient", {ambient.x, ambient.y, ambient.z});
  shader->setUniform("MaterialDiffuse", {diffuse.x, diffuse.y, diffuse.z, 1.0f});
  shader->setUniform("MaterialSpecular", {specular.x, specular.y, specular.z});
  shader->setUniform("MaterialShininess", shininess);

  shader->setUniform("LightDirection", scene.lightDirection);
  shader->setUniform("LightColor", scene.lightColor);

  shader->setUniform("LightDirection2", scene.lightDirection2);
  shader->setUniform("LightColor2", scene.lightColor2);

  shader->setUniform("AmbientLightColor", scene.AmbientLightColor);

  shader->setUniform("MaterialAmbient", ambient);
  shader->setUniform("MaterialDiffuse", diffuse);
  shader->setUniform("MaterialSpecular", specular);
  shader->setUniform("MaterialShininess", shininess);
//

    // use cameras
        shader->setUniform("ProjectionMatrix", scene.cameras[player_number]->projectionMatrix);
        shader->setUniform("ViewMatrix", scene.cameras[player_number]->viewMatrix);

  // render mesh
  shader->setUniform("CameraPosition", scene.cameras[player_number]->position);

  shader->setUniform("ModelMatrix", modelMatrix);
  shader->setUniform("Texture", *texture);
  mesh->render();
}

void Missile::destroy() {
  // This will destroy the projectile on Update
  age = 100.0f;
}
