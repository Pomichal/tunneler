#ifndef _PPGSO_SCENE_H
#define _PPGSO_SCENE_H

#include <memory>
#include <map>
#include <list>

#include "object.h"
#include "wall.h"
#include "tree.h"
#include "camera.h"


const int BASE_X = 11;
const int BASE_Y = 15;

const int TANK_LENGHT = 7;
const int TANK_WIDTH = 4;

const int TANK_LIFE = 5;
const int TANK_ENERGY = 1000;

const int SPEED = 10;


const unsigned int SIZE = 512;

const int GAME_SIZE = 100;



/*
 * Scene is an object that will aggregate all scene related data
 * Objects are stored in a list of objects
 * Keyboard and Mouse states are stored in a object_map and struct
 */
class Scene {
public:
    /*!
     * Update all objects in the scene
     * @param time
     */
    void update(float time, int player_number);

    /*!
     * Render all objects in the scene
     */
    void render(int player_number);

    /*!
     * Pick objects using a ray
     * @param position - Position in the scene to pick object from
     * @param direction - Direction to pick objects from
     * @return Objects - Vector of pointers to intersected objects
     */
    std::vector<Object*> intersect(const glm::vec3 &position, const glm::vec3 &direction);

    // Camera object
//    std::unique_ptr<Camera> camera1;
//    std::unique_ptr<Camera> camera2;
    std::vector< std::unique_ptr<Camera> > cameras;

    // All objects to be rendered in scene
    std::list< std::unique_ptr<Object> > objects;

//    std::unique_ptr<Player> tank1;
//    std::unique_ptr<Player> tank2;
    std::vector< std::unique_ptr<Object> > tanks;

    std::list< std::unique_ptr<Object> > missiles;
//    std::list< std::unique_ptr<Object> > tanks;
    std::list< std::unique_ptr<Object> > trees;
    std::list< std::unique_ptr<Object> > walls;

    std::array<std::array<int, GAME_SIZE>, GAME_SIZE> object_map = {0};

    // Keyboard state
    std::map< int, int > keyboard;

    // Lights, in this case using only simple directional diffuse lighting
    glm::vec3 lightDirection{-1.0f, -1.0f, -1.0f};

    // Store cursor state
    struct {
        double x, y;
        bool left, right;
    } cursor;
};

#endif // _PPGSO_SCENE_H
