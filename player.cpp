#include "player.h"
#include "scene.h"
//#include "asteroid.h"
#include "missile.h"
#include "explosion.h"
#include "tree.h"
#include "wall.h"


#include <shaders/diffuse_vert_glsl.h>
#include <shaders/diffuse_frag_glsl.h>

using namespace std;
using namespace glm;
using namespace ppgso;

// shared resources
unique_ptr<Mesh> Player::mesh;
unique_ptr<Texture> Player::texture;
unique_ptr<Shader> Player::shader;



Player::Player(int u, int d, int l, int r, int f) {
//   Scale the default model
//  scale.z *= TANK_WIDTH;
//  scale.x *= TANK_LENGHT;
//  scale *= 1f;
//  scale *= 3.f;
//  rotation.x = PI/2.f;

  up = u;
  down = d;
  left = l;
  right = r;
  fire = f;

  damage = 0;
  energy_level = TANK_ENERGY;

  rotation.z = PI/2.0f;
  rotation.y = -PI/2.0f;

  auto li = make_unique<Life>(vec3{1,0,0});
  li->position = position;
  life = move(li);

  auto en = make_unique<Life>(vec3{0,0,1});
  en->position = position;
  energy = move(en);

  // Initialize static resources if needed
  if (!shader) shader = make_unique<Shader>(diffuse_vert_glsl, diffuse_frag_glsl);
  if (!texture) texture = make_unique<Texture>(image::loadBMP("lena.bmp"));
//  if (!mesh) mesh = make_unique<Mesh>("sphere.obj");
  if (!mesh) mesh = make_unique<Mesh>("ultimate-tank2.obj");
}

bool Player::update(Scene &scene, float dt) {
//    cout << position.x << " " << position.y << endl;
  // Fire delay increment
  fireDelay += dt;
  if(resistance_counter > 0){
    resistance_counter -= dt;
  } else {
    resistance_counter = 0;
    resistance = vec3{0,0,0};
  }
  speed = vec3{0, 0, 0};

  // Keyboard controls
  if (scene.keyboard[left]) {
    energy_level -= MOVE_LOSE;
    speed.x = SPEED * dt;
    rotation.x = PI / 2.f;
    direction.x = TANK_LENGHT / 2;
    direction.y = 0;
  } else if (scene.keyboard[right]) {
    energy_level -= MOVE_LOSE;
    speed.x = -SPEED * dt;
    rotation.x = -PI / 2.f;
    direction.x = -TANK_LENGHT / 2;
    direction.y = 0;
  } else if (scene.keyboard[up]) {
//      cout << scene.keyboard[up] << endl;
    energy_level -= MOVE_LOSE;
    speed.y = SPEED * dt;
    rotation.x = 0;
    direction.x = 0;
    direction.y = TANK_LENGHT / 2;
  } else if (scene.keyboard[down]) {
//      cout << "DOWN" << endl;
    energy_level -= MOVE_LOSE;
    speed.y = -SPEED * dt;
    rotation.x = PI;
    direction.x = 0;
    direction.y = -TANK_LENGHT / 2;
//  } else {
//    rotation.y = 0;
//    rotation.z = 0;
  }


  // Firing projectiles
  if (scene.keyboard[fire] && fireDelay > fireRate) {
    energy_level -= 1.0f;
    // Reset fire delay
    fireDelay = 0;

    auto missile = make_unique<Missile>(direction.x * SPEED, direction.y * SPEED);
    missile->position = position + glm::vec3(direction.x, direction.y, -1.5f); // + fireOffset;
    scene.missiles.push_back(move(missile));
  }

  if (energy_level <= 0) {
    // Explode
    auto explosion = make_unique<Explosion>();
    explosion->position = position;
    explosion->scale = scale * 3.0f;
    scene.objects.push_back(move(explosion));

    return false;
  }

  vec3 new_position = position + speed;
  // collision detection
  int pos_x = (int) round(new_position.x);
  int pos_y = (int) round(new_position.y);
//  cout << pos_x << " " << pos_y << endl;

//    cout << "old " << speed.x << " " << speed.y << " " << speed.z << endl;


  // tank vs tree
  for (int i = -TANK_WIDTH / 2; i <= TANK_WIDTH / 2; i++) {
    for (int j = -1; j < abs(direction.x) + abs(direction.y) + 1; j++) {
//      cout << j << endl;
      if (direction.x > 0 && (scene.object_map[pos_x + j][pos_y + (int) direction.y + i] == 1)) {
        energy_level -= TREE_LOSE;
        resistance = vec3{RESISTANCE,0,0};
        resistance_counter = 2;
        scene.object_map[pos_x + j][pos_y + (int) direction.y + i] = 0;
      }
      if (direction.x < 0 && (scene.object_map[pos_x - j][pos_y + (int) direction.y + i] == 1)) {
        energy_level -= TREE_LOSE;
        resistance = vec3{RESISTANCE,0,0};
        resistance_counter = 2;
        scene.object_map[pos_x - j][pos_y + (int) direction.y + i] = 0;
      }
      if (direction.y > 0 && (scene.object_map[pos_x + (int) direction.x + i][pos_y + j] == 1)) {
        energy_level -= TREE_LOSE;
        resistance = vec3{0,RESISTANCE,0};
        resistance_counter = 2;
        scene.object_map[pos_x + (int) direction.x + i][pos_y + j] = 0;
      }
      if (direction.y < 0 && (scene.object_map[pos_x + (int) direction.x + i][pos_y - j] == 1)) {
        energy_level -= TREE_LOSE;
        resistance = vec3{0,RESISTANCE,0};
        resistance_counter = 2;
        scene.object_map[pos_x + (int) direction.x + i][pos_y - j] = 0;
      }
    }
  }


  // tank vs wall
  for (int i = -TANK_WIDTH / 2; i <= TANK_WIDTH / 2; i++) {
    for (int j = -1; j < abs(direction.x) + abs(direction.y); j++) {

      if (direction.y == 0 && (scene.object_map[pos_x + (int) direction.x][pos_y + (int) direction.y + i] == 2)) {
//      position = old_pos;
        speed = vec3{0, 0, 0};
      }
      if (direction.x == 0 && (scene.object_map[pos_x + (int) direction.x + i][pos_y + (int) direction.y] == 2)) {
//      position = old_pos;
        speed = vec3{0, 0, 0};
      }
    }
  }
  position += speed - speed * resistance;


//  for (int i = GAME_SIZE - 1; i >= 0; i--) {
//    for (int j = GAME_SIZE - 1; j >= 0; j--)
//      cout << scene.object_map[j][i];
//    cout << endl;
//  }
//  cout << endl;


  // Hit detection
  for ( auto& obj : scene.missiles ) {


    if (distance(position, obj->position) < 3) {
      // Explode
      auto explosion = make_unique<Explosion>();
      explosion->position = position;
      explosion->scale = scale * 3.0f;
      scene.objects.push_back(move(explosion));

      auto missile = dynamic_cast<Missile*>(obj.get());

      missile->destroy();

      // Die
      damage++;
//      cout << damage;
      if(damage >= TANK_LIFE) {
          return false;
      }
    }
  }

  life->position = position + vec3{0,0,-3};
  life->scale.x = 2.0f * (TANK_LIFE - damage) / TANK_LIFE;
  life->update(scene,dt);

  energy->position = position + vec3{0,1,-3};
  energy->scale.x = 2.0f * energy_level / TANK_ENERGY;
  energy->update(scene,dt);

  generateModelMatrix();
  return true;
}

void Player::render(Scene &scene, int player_number) {
  shader->use();

  // Set up light
  shader->setUniform("LightDirection", scene.lightDirection);

  // use cameras
//  for(int i=0; i<2;i++){
      shader->setUniform("ProjectionMatrix", scene.cameras[player_number]->projectionMatrix);
      shader->setUniform("ViewMatrix", scene.cameras[player_number]->viewMatrix);
//  }


  // render mesh
  shader->setUniform("ModelMatrix", modelMatrix);
  shader->setUniform("Texture", *texture);
  mesh->render();

  life->render(scene,player_number);
  energy->render(scene,player_number);
}

void Player::onClick(Scene &scene) {
  cout << "Player has been clicked!" << endl;
}
