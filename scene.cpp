#include "scene.h"
#include "player.h"


#include <shaders/diffuse_vert_glsl.h>
#include <shaders/diffuse_frag_glsl.h>

void Scene::update(float time, int player_number) {
  // Use iterator to update all objects so we can remove while iterating
//  camera->update(glm::vec3{0,0,0}, time);
  auto t = std::begin(tanks);
  while (t != std::end(tanks)) {
    // Update and remove from list if needed
    auto obj = t->get();


    if (!obj->update(*this, time))
      t = tanks.erase(t); // NOTE: no need to call destructors as we store shared pointers in the scene
    else
      ++t;
  }

  float new_pos_x = cameras[player_number]->position.x;
  float new_pos_y = cameras[player_number]->position.y;

  if(tanks[player_number]){
    new_pos_x = tanks[player_number]->position.x;
    new_pos_y = tanks[player_number]->position.y;
  }
  cameras[player_number]->update(time, new_pos_x, new_pos_y, keyboard[GLFW_KEY_1], keyboard[GLFW_KEY_2],keyboard[GLFW_KEY_I],keyboard[GLFW_KEY_O],
                  keyboard[GLFW_KEY_9],keyboard[GLFW_KEY_K],keyboard[GLFW_KEY_8],keyboard[GLFW_KEY_0]);
//  camera2->update(time, new_pos_x, new_pos_y, keyboard[GLFW_KEY_1], keyboard[GLFW_KEY_2],keyboard[GLFW_KEY_I],keyboard[GLFW_KEY_O],
//                  keyboard[GLFW_KEY_9],keyboard[GLFW_KEY_K],keyboard[GLFW_KEY_8],keyboard[GLFW_KEY_0]);
//    camera1->update(time, new_pos_x, new_pos_y, keyboard[GLFW_KEY_1], keyboard[GLFW_KEY_2],keyboard[GLFW_KEY_I],keyboard[GLFW_KEY_O],
//                   keyboard[GLFW_KEY_9],keyboard[GLFW_KEY_K],keyboard[GLFW_KEY_8],keyboard[GLFW_KEY_0]);

  auto i = std::begin(objects);

  while (i != std::end(objects)) {
    // Update and remove from list if needed
    auto obj = i->get();


    if (!obj->update(*this, time))
      i = objects.erase(i); // NOTE: no need to call destructors as we store shared pointers in the scene
    else
      ++i;
  }

  i = std::begin(missiles);

  while (i != std::end(missiles)) {
    // Update and remove from list if needed
    auto obj = i->get();

    if (!obj->update(*this, time))
      i = missiles.erase(i); // NOTE: no need to call destructors as we store shared pointers in the scene
    else
      ++i;
  }
  i = std::begin(walls);
  while (i != std::end(walls)) {
    // Update and remove from list if needed
    auto obj = i->get();

    !obj->update(*this, time);
      ++i;
  }
}

void Scene::render(int player_number) {
  // Simply render all objects
  for ( auto& obj : objects )
    obj->render(*this,player_number);
  for ( auto& obj : missiles )
    obj->render(*this,player_number);
  for ( auto& obj : walls )
    obj->render(*this,player_number);
  for ( auto& obj : tanks )
    obj->render(*this,player_number);
}

std::vector<Object*> Scene::intersect(const glm::vec3 &position, const glm::vec3 &direction) {
  std::vector<Object*> intersected = {};
  for(auto& object : objects) {
    // Collision with sphere of size object->scale.x
    auto oc = position - object->position;
    auto radius = object->scale.x;
    auto a = glm::dot(direction, direction);
    auto b = glm::dot(oc, direction);
    auto c = glm::dot(oc, oc) - radius * radius;
    auto dis = b * b - a * c;

    if (dis > 0) {
      auto e = sqrt(dis);
      auto t = (-b - e) / a;

      if ( t > 0 ) {
        intersected.push_back(object.get());
        continue;
      }

      t = (-b + e) / a;

      if ( t > 0 ) {
        intersected.push_back(object.get());
        continue;
      }
    }
  }

  return intersected;
}
