/* explosion.cpp
 * Uses sprite sheet to create explosion effect
 * Base code: Bryn Mawr College, alinen, 2020
 * Modified by JL
 * 29 March 2023
 */

#include <cmath>
#include <string>
#include <vector>
#include "agl/window.h"

using namespace std;
using namespace glm;
using namespace agl;

class Viewer : public Window {
 public:
  Viewer() : Window() {  }

  void setup() {
    setWindowSize(1000, 1000);
    renderer.loadShader("billboard-animated", 
        "../shaders/billboard-animated.vs", 
        "../shaders/billboard-animated.fs");

    renderer.loadTexture("explosion", 
        "../textures/explosion2.png", 0);
    renderer.setDepthTest(false);
    renderer.blendMode(agl::ADD);
  }

  void mouseMotion(int x, int y, int dx, int dy) {  }

  void mouseDown(int button, int mods) {  }

  void mouseUp(int button, int mods) {  }

  void scroll(float dx, float dy) {
    eyePos.z += dy;
  }

  void draw() {
    renderer.beginShader("billboard-animated");
    renderer.texture("image", "explosion");

    // 30 fps => each frame 1/30 long, e.g. when time = 1s, we play frame 30
    frameCont = frameCont + (dt() * fps);
    while (frameCont > numRows * numCols) {
      // keep within 0 to (numRows * numCols) range
      frameCont -= numRows * numCols;
    }
    // calculate actual frame number
    int frame = (int) frameCont;
    renderer.setUniform("Frame", frame);
    renderer.setUniform("Rows", numRows);
    renderer.setUniform("Cols", numCols);

    float aspect = ((float)width()) / height();
    renderer.perspective(glm::radians(60.0f), aspect, 0.1f, 50.0f);
    renderer.lookAt(eyePos, lookPos, up);
    renderer.sprite(vec3(0.0), vec4(1.0f), 1.0);

    renderer.endShader();
  }

 protected:
  vec3 eyePos = vec3(0, 0, 2);
  vec3 lookPos = vec3(0, 0, 0);
  vec3 up = vec3(0, 1, 0);
  // frame continuum: decimal to keep accurate count between frames
  float frameCont = 0;
  int numRows = 8;
  int numCols = 8;
  float fps = 30;  // frames per second
};

int main(int argc, char** argv) {
  Viewer viewer;
  viewer.run();
  return 0;
}
