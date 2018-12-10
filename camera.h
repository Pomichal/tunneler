#pragma once
#include <memory>

#include <glm/glm.hpp>
#include <ppgso/ppgso.h>


//#include "scene.h"

struct KeyFrame{
    glm::vec3 s_position;
    glm::vec3 t_position;
    glm::vec3 s_back;
    glm::vec3 t_back;
    float t;
};

/*!
 * Simple camera object that keeps track of viewMatrix and projectionMatrix
 * the projectionMatrix is by default constructed as perspective projection
 * the viewMatrix is generated from up, position and back vectors on update
 */
class Camera {
public:
    glm::vec3 up{0,1,0};
    glm::vec3 position{0,0,0};
    glm::vec3 back{0,0,-1};

    glm::mat4 viewMatrix;
    glm::mat4 projectionMatrix;

    std::vector< std::shared_ptr<KeyFrame> > frames;

    int frame_num = -1;
    float time_counter = 0;

    int viewport_x;
    int viewport_y;

    /*!
     * Create new Camera that will generate viewMatrix and projectionMatrix based on its position, up and back vectors
     * @param fow - Field of view in degrees
     * @param ratio - Viewport screen ratio (usually width/height of the render window)
     * @param near - Distance to the near frustum plane
     * @param far - Distance to the far frustum plane
     */
    Camera(float fow = 45.0f, float ratio = 1.0f, float near = 0.1f, float far = 10.0f, int v_x=0, int v_y=0);

    /*!
     * Update Camera viewMatrix based on up, position and back vectors
     */
    void update(float time, float tank_position_x, float tank_position_y, int plus, int minus,
                int move_L, int move_R, int move_U, int move_D, int rot_R, int rot_L);

    /*!
     * Get direction vector in world coordinates through camera projection plane
     * @param u - camera projection plane horizontal coordinate [-1,1]
     * @param v - camera projection plane vertical coordinate [-1,1]
     * @return Normalized vector from camera position to position on the camera projection plane
     */
    glm::vec3 cast(float u, float v);
};

