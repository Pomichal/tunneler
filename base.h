#pragma once
#include "object.h"
#include "scene.h"

class Base final : public Object {
public:

    Base();


    bool update(Scene &scene, float dt) override;

    bool in_base(glm::vec3 pos);


    void render(Scene &scene, int player_number) override;

    float time = 0.0f;
};
