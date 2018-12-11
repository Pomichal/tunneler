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
#include "scoreCube.h"

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
  bool game = false;





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


        int init_x = linearRand(BASE_Y,GAME_SIZE - BASE_Y);
        int init_y = linearRand(BASE_Y,GAME_SIZE - BASE_Y);
//
        vec3 init_pos = vec3{init_x,init_y,0};


      for(int i = GAME_SIZE - 1; i >= 0;i--) {
          for (int j = GAME_SIZE - 1; j >= 0; j--)
              scene.object_map[i][j] = 0;
      }

        auto frame1 = make_unique<KeyFrame>();
        frame1->t = 5.0f;
        frame1->t_back = vec3{-1,-1,-1};
        frame1->t_position = vec3{0,0,-20};

        auto frame2 = make_unique<KeyFrame>();
        frame2->t = 5.0f;
        frame2->t_back = vec3{0,0,-1};
        frame2->s_back = vec3{-1,-1,-1};
        frame2->s_position = vec3{0,0,-20};


        // Create camera1
    auto camera1 = make_unique<Camera>(60.0f, 1.0f, 0.1f, 100.0f,SIZE + 10,0);


//

    camera1->position.z = -30.0f;
    camera1->position.x = init_x;
    camera1->position.y = init_y;

    camera1->back = {0,0,-1};

    camera1->frames.push_back(move(frame1));
    camera1->frames.push_back(move(frame2));
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

        auto frame3 = make_unique<KeyFrame>();
        frame3->t = 5.0f;
        frame3->t_back = vec3{1,1,-1};
        frame3->t_position = vec3{GAME_SIZE,GAME_SIZE,-20};

        auto frame4 = make_unique<KeyFrame>();
        frame4->t = 5.0f;
        frame4->s_back = vec3{1,1,-1};
        frame4->t_back = vec3{0,0,-1};
        frame4->s_position = vec3{GAME_SIZE,GAME_SIZE,-20};

      // Create camera2
      auto camera2 = make_unique<Camera>(60.0f, 1.0f, 0.1f, 100.0f, 0, 0);

      camera2->position.z = -30.0f;
      camera2->position.x = init_x;
      camera2->position.y = init_y;

        camera2->back = {0,0,-1};

        camera2->frames.push_back(move(frame3));
        camera2->frames.push_back(move(frame4));

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

    int type = linearRand(0,1);
    cout << type << endl;
    int swap = -1;
    for(int i = 0; i < GAME_SIZE; i++) {
        for (int j = 0; j < GAME_SIZE; j++) {
            if (i == 0 || i == GAME_SIZE - 1 || j == 0 || j == GAME_SIZE - 1) {
                add_wall(vec3{i, j, 0});
            } else if (base1->in_base(vec3{i, j, 0}) || base2->in_base(vec3{i, j, 0})) {
                continue;
            } else {
//            else if(abs(i-init_x) < 15){
                if (swap > 0) {
                    add_tree(vec3{i, j, 0}, 0);
                }
            }
            swap = linearRand(-2,1);
        }
        swap = linearRand(-2,1);
    }

      scene.objects.push_back(move(base1));
      scene.objects.push_back(move(base2));

//        for(int i = GAME_SIZE - 1; i >= 0;i--) {
//          for (int j = GAME_SIZE - 1; j >= 0; j--)
//             cout << scene.object_map[i][j];
//          cout << endl;
//        }
//        cout << endl;


  }

//  void showMain(){
//      scene.cameras[0]->position = vec3{0,0,-15};
//      scene.cameras[1]->position = vec3{0,0,-15};
//  }

public:
  /*!
   * Construct custom game window
   */
    SceneWindow() : Window{"tunneler", SIZE*2 + 10, SIZE} {
    //hideCursor();
    glfwSetInputMode(window, GLFW_STICKY_KEYS, GLFW_FALSE);
//    glfwSetScrollCallback(window, scroll_callback);

    // Initialize OpenGL state
    // Enable Z-buffer
      glEnable(GL_DEPTH_TEST);
    glDepthFunc(GL_LEQUAL);


    glEnable(GL_LIGHTING);
//      glEnable(GL_BLEND);
//      glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    // Enable polygon culling
    glEnable(GL_CULL_FACE);
    glFrontFace(GL_CCW);
    glCullFace(GL_BACK);

//    if(game)
      auto score1 = make_unique<scoreCube>(1);
      score1->position = vec3{-2.5f,-3.5f,-1};
      auto score2 = make_unique<scoreCube>(1);
      score2->position = vec3{2.5f, -3.5f ,-1};

      auto scoreMain = make_unique<scoreCube>(2);
      scoreMain->position = vec3{0,0 ,0};

      scene.menu_objects.push_back(move(score1));
      scene.menu_objects.push_back(move(score2));
      scene.menu_objects.push_back(move(scoreMain));


      initScene();
//    else{

//    }
//    showMain();
  }

  void add_tree(vec3 pos, int type){
      auto obj = make_unique<Tree>(type);
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

    // exit game
    if (game && key == GLFW_KEY_M && action == GLFW_PRESS) {
      initScene();
      game = false;
    }

    // reset score
    if (!game && key == GLFW_KEY_R && action == GLFW_PRESS) {
        scene.scores[0] = 0;
        scene.scores[1] = 0;
//        scene.score2 = 0;
    }

    // Pause
    if (game && key == GLFW_KEY_P && action == GLFW_PRESS) {
      animate = !animate;
    }

    if (game && key == GLFW_KEY_Z && action == GLFW_PRESS) {
          scene.cameras[0]->frame_num = 0;
          scene.cameras[1]->frame_num = 0;
        scene.cameras[0]->frames[0]->s_position = scene.cameras[0]->position;
        scene.cameras[0]->frames[0]->s_back = scene.cameras[0]->back;
        scene.cameras[0]->time_counter = scene.cameras[0]->frames[0]->t;
        scene.cameras[1]->frames[0]->s_position = scene.cameras[1]->position;
        scene.cameras[1]->frames[0]->s_back = scene.cameras[1]->back;
        scene.cameras[1]->time_counter = scene.cameras[1]->frames[0]->t;

//        cout << scene.cameras[0]->position.x << " " << scene.cameras[0]->position.y << " " << endl;
//        cout << scene.cameras[0]->frames[0]->s_position.x << " " << scene.cameras[0]->frames[0]->s_position.y << " " << endl;
//        cout << scene.cameras[1]->position.x << " " << scene.cameras[1]->position.y << " " << endl;
//        cout << endl;
    }

    if (key == GLFW_KEY_ENTER && action == GLFW_PRESS) {
          game = true;
          initScene();
          scene.cameras[0]->position.z = -30;
          scene.cameras[1]->position.z = -30;
    }

      if (key == GLFW_KEY_V && action == GLFW_PRESS) {
          scene.lightDirection.x -= 1;
      }
      if (key == GLFW_KEY_B && action == GLFW_PRESS) {
          scene.lightDirection.x += 1;
      }
  }

  /*!
   * Handle cursor position changes
   * @param cursorX Mouse horizontal position in window coordinates
   * @param cursorY Mouse vertical position in window coordinates
   */
//  void onCursorPos(double cursorX, double cursorY) override {
//    scene.cursor.x = cursorX;
//    scene.cursor.y = cursorY;
//  }


  /*!
   * Handle cursor buttons
   * @param button Mouse button being manipulated
   * @param action Mouse bu
   * @param mods
   */
//  void onMouseButton(int button, int action, int mods) override {
//    if(button == GLFW_MOUSE_BUTTON_LEFT) {
//      scene.cursor.left = action == GLFW_PRESS;
//
//      if (scene.cursor.left) {
//        // Convert pixel coordinates to Screen coordinates
//        float u = (scene.cursor.x / width - 0.5f) * 2.0f;
//        float v = - (scene.cursor.y / height - 0.5f) * 2.0f;
//
//        // Get mouse pick vector in world coordinates
//        auto direction = scene.cameras[0]->cast(u, v);
//        auto position = scene.cameras[0]->position;
//
//        // Get all objects in scene intersected by ray
//        auto picked = scene.intersect(position, direction);
//
//        // Go through all objects that have been picked
//        for (auto &obj: picked) {
//          // Pass on the click event
//          obj->onClick(scene);
//        }
//      }
//    }
//    if(button == GLFW_MOUSE_BUTTON_RIGHT) {
//      scene.cursor.right = action == GLFW_PRESS;
//    }
//  }

  /*!
   * Window update implementation that will be called automatically from pollEvents
   */
  void onIdle() override {
    // Track time
    static auto time = (float) glfwGetTime();

    // Compute time delta
    float dt = animate ? (float) glfwGetTime() - time : 0;

    time = (float) glfwGetTime();

    if(game) {
        // Set gray background
        glClearColor(.3f, .3f, .3f, 0);
        // Clear depth and color buffers
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);


        // Update and render all objects
        glViewport(scene.cameras[0]->viewport_x, scene.cameras[0]->viewport_y, SIZE, SIZE);
        if(!scene.update(dt, 0)) game = false;
        scene.render(0);

        glViewport(scene.cameras[1]->viewport_x, scene.cameras[1]->viewport_y, SIZE, SIZE);
        if(!scene.update(dt, 1)) game = false;
        scene.render(1);
    }
    else{
        glClearColor(.8f, .8f, .8f, 0);
        // Clear depth and color buffers
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        scene.cameras[0]->position = vec3{0,0,-15};
        scene.cameras[1]->position = vec3{0,0,-15};

        glViewport(scene.cameras[0]->viewport_x, scene.cameras[0]->viewport_y, SIZE, SIZE);
            scene.updateMain(dt, 0);
            scene.renderMain(0);
        glViewport(scene.cameras[1]->viewport_x, scene.cameras[1]->viewport_y, SIZE, SIZE);
            scene.updateMain(dt, 1);
            scene.renderMain(1);
    }

  }
};

int main() {
  // Initialize our window
  SceneWindow window;

  // Main execution loop
  while (window.pollEvents()) {}

  return EXIT_SUCCESS;
}
