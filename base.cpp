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


bool Base::in_base(vec3 pos) {
    return (abs(pos.x - position.x) <= (BASE_X / 2.0f) +2) && (abs(pos.y - position.y) <= (BASE_Y / 2.0f));
}

bool Base::update(Scene &scene, float dt) {
    // Accumulate time
    time += dt;

    for (auto &obj : scene.tanks) {

        auto tank = dynamic_cast<Player*>(obj.get()); //dynamic_pointer_cast<Projectile>(obj);
        if (!tank) continue;

        if(in_base(tank->position) && tank->damage > 0){
            tank->damage -= 0.01f;
        }

        if(in_base(tank->position) && tank->energy_level < TANK_ENERGY) {
            tank->energy_level += 0.4f;
        }
    }

    return true;
}

void Base::render(Scene &scene, int player_number) {
    // Base will not be rendered
}

