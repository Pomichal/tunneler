#pragma once
#include <ppgso/ppgso.h>

#include "object.h"
#include "scene.h"

/*!
 * Simple object representing the player
 * Reads keyboard status and manipulates its own position
 * On Update checks collisions with Asteroid objects in the scene
 */
class Player final : public Object {
private:
  // Static resources (Shared between instances)
  static std::unique_ptr<ppgso::Mesh> mesh;
  static std::unique_ptr<ppgso::Shader> shader;
  static std::unique_ptr<ppgso::Texture> texture;

  // Delay fire and fire rate
  float fireDelay{0.0f};
  float fireRate{0.2f};
  glm::vec3 fireOffset{0.0f,0.0f,0.0f};
  glm::vec3 direction{0.0f,TANK_LENGHT / 2,0.0f};
  glm::vec3 speed{0.0f,0.0f,0.0f};


  // controll
  int up;
  int down;
  int left;
  int right;
  int fire;
public:
  /*!
   * Create a new player
   */
  Player(int u, int d, int l, int r, int f);

  /*!
   * Update player position considering keyboard inputs
   * @param scene Scene to update
   * @param dt Time delta
   * @return true to delete the object
   */
  bool update(Scene &scene, float dt) override;

  /*!
   * Render player
   * @param scene Scene to render in
   */
  void render(Scene &scene) override;


  /*!
   * Player click event
   * @param scene
   */
  void onClick(Scene &scene) override ;
};

