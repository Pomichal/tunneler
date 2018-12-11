#pragma once
#include <memory>

#include <ppgso/ppgso.h>

#include "scene.h"
#include "object.h"
//#include "player.h"

class Life final : public Object {
private:
    // Static resources (Shared between instances)
    static std::unique_ptr<ppgso::Mesh> mesh;
    static std::unique_ptr<ppgso::Shader> shader;
    static std::unique_ptr<ppgso::Texture> texture;

    glm::vec3 color;

public:

    Life(glm::vec3 c);


    bool update(Scene &scene, float dt) override;

    void render(Scene &scene, int player_number) override;

private:
};

