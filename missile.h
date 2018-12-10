#pragma once
#include <ppgso/ppgso.h>

#include "object.h"

/*!
 * Object representing a rocket projectile that will accelerate from the ship one created
 */
class Missile final : public Object {
private:
  static std::unique_ptr<ppgso::Shader> shader;
  static std::unique_ptr<ppgso::Mesh> mesh;
  static std::unique_ptr<ppgso::Texture> texture;

    std::map<std::string, int> material_map;
    std::vector<tinyobj::material_t> material;

    glm::vec3 ambient{0.2f,0.2f,0.2f};
    glm::vec4 diffuse{0.8f,0.8f,0.8f,1.0f};
    glm::vec3 specular{0.2f,0.2f,0.2f};
    float shininess = 0.1f;

  float age{0.0f};
  glm::vec3 speed;
  glm::vec3 rotMomentum;
public:
  /*
   * Create new projectile
   */
  Missile();
  Missile(float x, float y);

  /*!
   * Update projectile position
   * @param scene Scene to update
   * @param dt Time delta
   * @return true to delete the object
   */
  bool update(Scene &scene, float dt) override;

  void explode(Scene &scene, glm::vec3 explosionPosition, glm::vec3 explosionScale);

  /*!
   * Render projectile
   * @param scene Scene to render in
   */
  void render(Scene &scene, int player_number) override;

  /*!
   * Destroy the projectile
   */
  void destroy();
};

