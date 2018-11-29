#include <glm/gtc/random.hpp>
#include <ppgso/ppgso.h>

#include "base.h"
#include "wall.h"
#include "player.h"


using namespace std;
using namespace glm;
using namespace ppgso;

Base::Base(){
};

void Base::add_wall(Scene &scene, vec3 pos) {
    auto obj = make_unique<Wall>();
    obj->position = pos;
    scene.objects.push_back(move(obj));
}

bool Base::in_base(vec3 pos) {
    return (abs(pos.x - position.x) <= (BASE_X / 2.0f) +2) && (abs(pos.y - position.y) <= (BASE_Y / 2.0f));
}

bool Base::update(Scene &scene, float dt) {
    // Accumulate time
    time += dt;

    for (auto &obj : scene.objects) {
        // Ignore self in scene
        if (obj.get() == this) continue;

        // We only need to collide with asteroids and projectiles, ignore other objects
        auto tank = dynamic_cast<Player*>(obj.get()); //dynamic_pointer_cast<Projectile>(obj);
        if (!tank) continue;

        if(in_base(tank->position)){
//            cout << "in" << endl;
        }
    }
    // check tank and heal
    return true;
}

void Base::render(Scene &scene) {
    // Generator will not be rendered
}

