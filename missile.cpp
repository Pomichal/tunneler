#include <glm/gtc/random.hpp>
#include "scene.h"
#include "missile.h"
#include "tree.h"
#include "wall.h"
#include "explosion.h"

#include <shaders/diffuse_vert_glsl.h>
#include <shaders/diffuse_frag_glsl.h>

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
  if (!shader) shader = make_unique<Shader>(diffuse_vert_glsl, diffuse_frag_glsl);
  if (!texture) texture = make_unique<Texture>(image::loadBMP("missile.bmp"));
  if (!mesh) mesh = make_unique<Mesh>("missile.obj");
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
  if (!shader) shader = make_unique<Shader>(diffuse_vert_glsl, diffuse_frag_glsl);
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
  auto explosion = make_unique<Explosion>();
  explosion->position = explosionPosition;
  explosion->scale = explosionScale;
  explosion->speed = vec3{2,2,0};
  scene.objects.push_back(move(explosion));
}

void Missile::render(Scene &scene) {
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

void Missile::destroy() {
  // This will destroy the projectile on Update
  age = 100.0f;
}
