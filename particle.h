#pragma once
#include <memory>

#include <ppgso/ppgso.h>

#include "scene.h"
#include "object.h"

class Particle final : public Object {
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

    glm::mat4 modelMatrix;
    glm::vec3 position;
    float scale;
    glm::vec3 speed;
    glm::vec3 color;
    float bumm;

public:
    Particle(glm::vec3 p, glm::vec3 s, glm::vec3 c, float sc, float b);

    bool update(Scene &scene, float dt) override;

    void render(Scene &scene, int player_number) override;

    void onClick(Scene &scene) override;

private:
};

