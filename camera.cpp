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

float getPt(float n1 , float n2 , float perc )
{
//    return n1 + ((n2 - n1) * perc );
    return (1- perc) * n1 + perc * n2;
//    (1 - t) * v0 + t * v1;
}


vec3 bezierPoint( vec3* points, int numPoints, float t ) {
    vec3* tmp = new vec3[numPoints];
    memcpy(tmp, points, numPoints * sizeof(vec3));
    int i = numPoints - 1;
    while (i > 0) {
        for (int k = 0; k < i; k++) {
            tmp[k].x = getPt(tmp[k].x, tmp[k + 1].x, t);
            tmp[k].y = getPt(tmp[k].y, tmp[k + 1].y, t);
            tmp[k].z = getPt(tmp[k].z, tmp[k + 1].z, t);
        }
        i--;
    }
    vec3 answer = tmp[0];
    delete[] tmp;
//    cout << answer.x << " " << answer.y << " " << answer.z << endl << endl;
    return answer;
}


void Camera::update(float time,float tank_position_x, float tank_position_y, int plus, int minus,
        int move_L, int move_R, int move_U, int move_D, int rot_R, int rot_L) {


    if(frame_num != -1) {
        vec3* v1 = new vec3[2];
        v1[0] = frames[frame_num]->t_position;
        v1[1] = frames[frame_num]->s_position;

        position = bezierPoint(v1, 2, time_counter / frames[frame_num]->t);

        vec3* b1 = new vec3[2];
        b1[0] = frames[frame_num]->t_back;
        b1[1] = frames[frame_num]->s_back;
        back = bezierPoint(b1, 2, time_counter / frames[frame_num]->t);

        time_counter -= time;

        if(time > time_counter && frame_num == 0) {
            frame_num = 1;
            frames[1]->s_position = position;
            frames[1]->s_back = back;
            time_counter = frames[1]->t;
        }else if( time > time_counter) {
            frame_num = -1;
            time_counter = 0;
        }
    }
    else {
        if (time != 0) {
            position.x = tank_position_x;
            position.y = tank_position_y;
        }

        if (minus && position.z > -45.0f) {
//  if(minus){
            position.z -= 0.05f;
        } else if (plus && position.z < -10.0f) {
//  } else if (plus){
            position.z += 0.05f;
        }
        if (rot_R) {
            back.x -= 0.05f;
        } else if (rot_L) {
            back.x += 0.05f;
        }
        if (move_L) {
            position.x += 0.05f;
        } else if (move_R) {
            position.x -= 0.05f;
        }
        if (move_U) {
            position.y += 0.05f;
        } else if (move_D) {
            position.y -= 0.05f;
        }
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
