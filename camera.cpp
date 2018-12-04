#include <glm/glm.hpp>

#include "camera.h"
#include "scene.h"

using namespace std;
using namespace glm;
using namespace ppgso;

Camera::Camera(float fow, float ratio, float near, float far, int v_x, int v_y) {
  float fowInRad = (PI/180.0f) * fow;
  viewport_x = v_x;
  viewport_y = v_y;

  projectionMatrix = perspective(fowInRad, ratio, near, far);
}

void Camera::update(float time,float tank_position_x, float tank_position_y, int plus, int minus,
        int move_L, int move_R, int move_U, int move_D, int rot_R, int rot_L) {

    if(time != 0) {
        position.x = tank_position_x;
        position.y = tank_position_y;
    }

    if(minus && position.z > -45.0f){
//  if(minus){
    position.z -= 0.05f;
  } else if (plus && position.z < -10.0f){
//  } else if (plus){
    position.z += 0.05f;
  }
  if(rot_R){
    back.x -= 0.05f;
  }else if(rot_L){
    back.x += 0.05f;
  }
  if(move_L){
    position.x += 0.05f;
  }else if(move_R){
      position.x -= 0.05f;
  }
  if(move_U){
      position.y += 0.05f;
  }else if(move_D){
      position.y -= 0.05f;
  }
    viewMatrix = lookAt(position, position - back, up);
}

glm::vec3 Camera::cast(float u, float v) {
  // Create point in Screen coordinates
  glm::vec4 screenPosition{u,v,0.0f,1.0f};

  // Use inverse matrices to get the point in world coordinates
  auto invProjection = glm::inverse(projectionMatrix);
  auto invView = glm::inverse(viewMatrix);

  // Compute position on the camera plane
  auto planePosition = invView * invProjection * screenPosition;
  planePosition /= planePosition.w;

  // Create direction vector
  auto direction = glm::normalize(planePosition - vec4{position,1.0f});
  return vec3{direction};
}
