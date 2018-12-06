#include "scene.h"
#include "player.h"


#include <shaders/diffuse_vert_glsl.h>
#include <shaders/diffuse_frag_glsl.h>

bool Scene::update(float time, int player_number) {
  // Use iterator to update all objects so we can remove while iterating
//  camera->update(glm::vec3{0,0,0}, time);
  auto t = std::begin(tanks);
  while (t != std::end(tanks)) {
    // Update and remove from list if needed
    auto obj = t->get();


    if (!obj->update(*this, time)) {
      t = tanks.erase(t); // NOTE: no need to call destructors as we store shared pointers in the scene
      if(player_number == 1 && scores[0] < 6) scores[0]++;
      if(player_number == 0 && scores[1] < 6) scores[1]++;
      return false;
    }
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

    obj->update(*this, time);
      ++i;
  }

    i = std::begin(trees);
    while (i != std::end(trees)) {
        // Update and remove from list if needed
        auto obj = i->get();

        if(!obj->update(*this, time)){
            i = trees.erase(i);
        } else
        ++i;
    }

  return true;

}

void Scene::render(int player_number) {
  // Simply render all objects
  for ( auto& obj : objects )
    obj->render(*this,player_number);
  for ( auto& obj : missiles )
      if(glm::distance(cameras[player_number]->position * glm::vec3{1,1,0}, obj->position) < 18)
        obj->render(*this,player_number);
    for ( auto& obj : walls )
      if(glm::distance(cameras[player_number]->position * glm::vec3{1,1,0}, obj->position) < 18)
    obj->render(*this,player_number);
  for ( auto& obj : tanks )
    obj->render(*this,player_number);
  for ( auto& obj : trees ) {
      if(glm::distance(cameras[player_number]->position * glm::vec3{1,1,0}, obj->position) < 18)
          obj->render(*this, player_number);
  }
}

void Scene::updateMain(float time, int player_number) {

    cameras[player_number]->update(time, cameras[player_number]->position.x, cameras[player_number]->position.y, keyboard[GLFW_KEY_1], keyboard[GLFW_KEY_2],keyboard[GLFW_KEY_I],keyboard[GLFW_KEY_O],
                                   keyboard[GLFW_KEY_9],keyboard[GLFW_KEY_K],keyboard[GLFW_KEY_8],keyboard[GLFW_KEY_0]);

//    auto i = std::begin(menu_objects);

    for(int i = 0; i <3; i++){
        menu_objects[i]->update(*this, time);
        if(i == 0 || i == 1){
          switch(scores[i]) {
              case 1:
                menu_objects[i]->rotation = glm::vec3{ppgso::PI,0,0};
                break;
              case 2:
                menu_objects[i]->rotation = glm::vec3{ppgso::PI,0,-ppgso::PI/2.0f};
                break;
              case 3:
                menu_objects[i]->rotation = glm::vec3{ppgso::PI,0,ppgso::PI};
                break;
              case 4:
                menu_objects[i]->rotation = glm::vec3{ppgso::PI,0,ppgso::PI/2.0f};
                break;
              case 5:
                menu_objects[i]->rotation = glm::vec3{ppgso::PI/2.0f,0,ppgso::PI};
                break;
              default:
                menu_objects[i]->rotation = glm::vec3{ppgso::PI/2.0f,0,0};
                  break;
          }
        }
    }

//  while (i != std::end(menu_objects)) {
//     Update and remove from list if needed
//    auto obj = i->get();
//
//
//    if (!obj->update(*this, time))
//      i = menu_objects.erase(i); // NOTE: no need to call destructors as we store shared pointers in the scene
//    else
//      ++i;
//  }

}

void Scene::renderMain(int player_number) {

  for ( auto& obj : menu_objects )
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
