#pragma once
#include "object.h"
#include "scene.h"

/*!
 * Example generator of objects
 * Constructs a new object during Update and adds it into the scene
 * Does not render anything
 */
class Base final : public Object {
public:

    Base();
    /*!
     * Generate new asteroids over time
     * @param scene Scene to update
     * @param dt Time delta
     * @return true to delete the object
     */
    bool update(Scene &scene, float dt) override;

    void add_wall(Scene &scene, glm::vec3 pos);

    bool in_base(glm::vec3 pos);
    /*!
     * Render placeholder for generator
     * @param scene Scene to render in
     */
    void render(Scene &scene) override;

    float time = 0.0f;
};
