#pragma once
#include <memory>

#include <ppgso/ppgso.h>

#include "scene.h"
#include "object.h"

class Ground final : public Object {
private:
    // Static resources (Shared between instances)
    static std::unique_ptr<ppgso::Mesh> mesh;
    static std::unique_ptr<ppgso::Shader> shader;
    static std::unique_ptr<ppgso::Texture> texture;

    std::map<std::string, int> material_map;
    std::vector<tinyobj::material_t> material;

    glm::vec3 ambient{0.2f,0.2f,0.2f};
    glm::vec4 diffuse{0.8f,0.8f,0.8f,1.0f};
    glm::vec3 specular{0.2f,0.2f,0.2f};
    float shininess = 0.1f;

public:

    Ground(int size_x, int size_y);

    bool update(Scene &scene, float dt) override;

    void render(Scene &scene, int player_number) override;

private:
};

