// Example gl_scene
// - Introduces the concept of a dynamic scene of objects
// - Uses abstract object interface for Update and Render steps
// - Creates a simple game scene with Player, Asteroid and Space objects
// - Contains a generator object that does not render but adds Asteroids to the scene
// - Some objects use shared resources and all object deallocations are handled automatically
// - Controls: LEFT, RIGHT, "R" to reset, SPACE to fire

#include <iostream>
#include <map>
#include <list>

#include <ppgso/ppgso.h>

#include "camera.h"
#include "scene.h"
#include "wall.h"
#include "player.h"
#include "tree.h"
#include "base.h"
#include "ground.h"
//#include "space.h"

using namespace std;
using namespace glm;
using namespace ppgso;



/*!
 * Custom windows for our simple game
 */
class SceneWindow : public Window {
private:
  Scene scene;
  bool animate = true;


  /*!
   * Reset and initialize the game scene
   * Creating unique smart pointers to objects that are stored in the scene object list
   */
  void initScene() {
    scene.objects.clear();
    scene.walls.clear();
    scene.trees.clear();
    scene.missiles.clear();
    scene.tanks.clear();

      for(int i = GAME_SIZE - 1; i >= 0;i--) {
          for (int j = GAME_SIZE - 1; j >= 0; j--)
              scene.object_map[i][j] = 0;
      }


    // Create camera1
    auto camera1 = make_unique<Camera>(60.0f, 1.0f, 0.1f, 100.0f,0,0);

    int init_x = linearRand(BASE_Y,GAME_SIZE - BASE_Y);
    int init_y = linearRand(BASE_Y,GAME_SIZE - BASE_Y);
//
    vec3 init_pos = vec3{init_x,init_y,0};
//
    camera1->position.z = -30.0f;
    camera1->position.x = init_x;
    camera1->position.y = init_y;
    scene.cameras.push_back(move(camera1));

    auto obj = make_unique<Ground>(GAME_SIZE,GAME_SIZE);
    obj->position = vec3{GAME_SIZE/2.0f - 0.5f,GAME_SIZE/2.0f - 0.5f,1};
//    obj->rotation = vec3{PI/2.0f,0,0};
    scene.objects.push_back(move(obj));


      // Add base1 to scene

    auto base1 = make_unique<Base>();
    base1->position = init_pos;
    for(int i = 0; i < BASE_Y; i++){
          add_wall(vec3{init_pos.x -7, init_pos.y -7 + i,0});
          add_wall(vec3{init_pos.x + 7, init_pos.y -7 + i,0});

          if(i == 1 || i == 2 || i == 3){
              add_wall(vec3{init_pos.x - 3 - i, init_pos.y + 7,0});
              add_wall(vec3{init_pos.x - 3 - i, init_pos.y - 7,0});
              add_wall(vec3{init_pos.x + 3 + i, init_pos.y + 7,0});
              add_wall(vec3{init_pos.x + 3 + i, init_pos.y - 7,0});
          }
      }

      //     Add player1 to the scene
      auto player1 = make_unique<Player>(GLFW_KEY_UP, GLFW_KEY_DOWN, GLFW_KEY_LEFT, GLFW_KEY_RIGHT, GLFW_KEY_KP_ENTER);
      player1->position = init_pos;
      scene.tanks.push_back(move(player1));


      // Add base2 to scene

      init_pos.x = GAME_SIZE - init_pos.x;
      init_pos.y = GAME_SIZE - init_pos.y;

      // Create camera2
      auto camera2 = make_unique<Camera>(60.0f, 1.0f, 0.1f, 100.0f, SIZE + 10, 0);

      camera2->position.z = -30.0f;
      camera2->position.x = init_x;
      camera2->position.y = init_y;
      scene.cameras.push_back(move(camera2));

      // create base2
      auto base2 = make_unique<Base>();
      base2->position = init_pos;
      for(int i = 0; i < BASE_Y; i++){
          add_wall(vec3{init_pos.x -7, init_pos.y -7 + i,0});
          add_wall(vec3{init_pos.x + 7, init_pos.y -7 + i,0});

          if(i == 1 || i == 2 || i == 3){
              add_wall(vec3{init_pos.x - 3 - i, init_pos.y + 7,0});
              add_wall(vec3{init_pos.x - 3 - i, init_pos.y - 7,0});
              add_wall(vec3{init_pos.x + 3 + i, init_pos.y + 7,0});
              add_wall(vec3{init_pos.x + 3 + i, init_pos.y - 7,0});
          }
      }
//                    add_tree(vec3{init_x + 10,init_y,0});


      //     Add player2 to the scene
      auto player2 = make_unique<Player>(GLFW_KEY_W, GLFW_KEY_S, GLFW_KEY_A, GLFW_KEY_D, GLFW_KEY_SPACE);
      player2->position = init_pos;
      scene.tanks.push_back(move(player2));
//
    for(int i = 0; i < GAME_SIZE; i++)
        for(int j = 0; j< GAME_SIZE;j++){
            if(i==0 || i == GAME_SIZE - 1 || j == 0 || j == GAME_SIZE - 1){
                add_wall(vec3{i,j,0});
            }
            else if(base1->in_base(vec3{i,j,0}) || base2->in_base(vec3{i,j,0})){
                continue;
            }
            else{
//            else if(abs(i-init_x) < 15){
//              add_tree(vec3{i,j,0});
            }
        }

      scene.objects.push_back(move(base1));
      scene.objects.push_back(move(base2));

        for(int i = GAME_SIZE - 1; i >= 0;i--) {
          for (int j = GAME_SIZE - 1; j >= 0; j--)
             cout << scene.object_map[i][j];
          cout << endl;
        }
        cout << endl;

  }

public:
  /*!
   * Construct custom game window
   */
    SceneWindow() : Window{"tunneler", SIZE*2 + 10, SIZE} {
    //hideCursor();
    glfwSetInputMode(window, GLFW_STICKY_KEYS, 1);
//    glfwSetScrollCallback(window, scroll_callback);

    // Initialize OpenGL state
    // Enable Z-buffer
      glEnable(GL_DEPTH_TEST);
    glDepthFunc(GL_LEQUAL);

    // Enable polygon culling
    glEnable(GL_CULL_FACE);
    glFrontFace(GL_CCW);
    glCullFace(GL_BACK);

//      glViewport(SIZE/2, SIZE/2, SIZE/2, SIZE/2);

      initScene();
  }

  void add_tree(vec3 pos){
      auto obj = make_unique<Tree>();
      obj->position = pos;
      scene.object_map[(int)pos.x][(int)pos.y] = 1;
      scene.trees.push_back(move(obj));
  }

    void add_wall(vec3 pos){
        auto obj = make_unique<Wall>();
        obj->position = pos;
        scene.object_map[(int)pos.x][(int)pos.y] = 2;
        scene.walls.push_back(move(obj));
    }
  /*!
   * Handles pressed key when the window is focused
   * @param key Key code of the key being pressed/released
   * @param scanCode Scan code of the key being pressed/released
   * @param action Action indicating the key state change
   * @param mods Additional modifiers to consider
   */
  void onKey(int key, int scanCode, int action, int mods) override {
    scene.keyboard[key] = action;

    // Reset
    if (key == GLFW_KEY_R && action == GLFW_PRESS) {
      initScene();
    }

    // Pause
    if (key == GLFW_KEY_P && action == GLFW_PRESS) {
      animate = !animate;
    }
  }

  /*!
   * Handle cursor position changes
   * @param cursorX Mouse horizontal position in window coordinates
   * @param cursorY Mouse vertical position in window coordinates
   */
  void onCursorPos(double cursorX, double cursorY) override {
    scene.cursor.x = cursorX;
    scene.cursor.y = cursorY;
  }


  /*!
   * Handle cursor buttons
   * @param button Mouse button being manipulated
   * @param action Mouse bu
   * @param mods
   */
  void onMouseButton(int button, int action, int mods) override {
    if(button == GLFW_MOUSE_BUTTON_LEFT) {
      scene.cursor.left = action == GLFW_PRESS;

      if (scene.cursor.left) {
        // Convert pixel coordinates to Screen coordinates
        float u = (scene.cursor.x / width - 0.5f) * 2.0f;
        float v = - (scene.cursor.y / height - 0.5f) * 2.0f;

        // Get mouse pick vector in world coordinates
        auto direction = scene.cameras[0]->cast(u, v);
        auto position = scene.cameras[0]->position;

        // Get all objects in scene intersected by ray
        auto picked = scene.intersect(position, direction);

        // Go through all objects that have been picked
        for (auto &obj: picked) {
          // Pass on the click event
          obj->onClick(scene);
        }
      }
    }
    if(button == GLFW_MOUSE_BUTTON_RIGHT) {
      scene.cursor.right = action == GLFW_PRESS;
    }
  }

  /*!
   * Window update implementation that will be called automatically from pollEvents
   */
  void onIdle() override {
    // Track time
    static auto time = (float) glfwGetTime();

    // Compute time delta
    float dt = animate ? (float) glfwGetTime() - time : 0;

    time = (float) glfwGetTime();

    // Set gray background
    glClearColor(.5f, .5f, .5f, 0);
    // Clear depth and color buffers
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    // Update and render all objects
      glViewport(scene.cameras[0]->viewport_x, scene.cameras[0]->viewport_y, SIZE, SIZE);
      scene.update(dt,0);
    scene.render();

      glViewport(scene.cameras[1]->viewport_x, scene.cameras[1]->viewport_y, SIZE, SIZE);
      scene.update(dt,1);
      scene.render();
  }
};

int main() {
  // Initialize our window
  SceneWindow window;

  // Main execution loop
  while (window.pollEvents()) {}

  return EXIT_SUCCESS;
}


// TODO viewport, 2 camera