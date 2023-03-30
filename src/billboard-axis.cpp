/* billboard-axis.cpp
 * Implements axis billboard viewer
 * Base code: Bryn Mawr College, alinen, 2020
 * Modified by JL
 * 30 March 2023
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
    renderer.loadShader("simple-texture",
      "../shaders/simple-texture.vs",
      "../shaders/simple-texture.fs");

    Image img;
    img.load("../textures/tree.png", true);
    renderer.loadTexture("tree", img, 0);
    imgWidth = img.width();
    imgHeight = img.height();

    renderer.loadTexture("grass", "../textures/grass.png", 0);
    renderer.blendMode(agl::BLEND);
  }


  void mouseMotion(int x, int y, int dx, int dy) {
    if (dragging) {
      azimuth = (azimuth + dx) % 360;
      elevation = elevation + dy;
      // convert to degrees to radians
      float azimuthRad = azimuth * M_PI / 180.0f;
      float elevationRad = elevation * M_PI / 180.0f;
      // sin to squish to range [-90, 90] then convert back to radians
      elevationRad = (90 * sin(elevationRad)) * M_PI / 180.0f;
      // x-axis, out-of-screen direction
      eyePos = radius * vec3(sin(azimuthRad) * cos(elevationRad),
          sin(elevationRad), cos(azimuthRad) * cos(elevationRad));
    }
  }

  void mouseDown(int button, int mods) {
    cout << "Mouse DOWN" << endl;
    if (button == GLFW_MOUSE_BUTTON_LEFT && mods == 0) {
      dragging = true;
    }
  }

  void mouseUp(int button, int mods) {
    cout << "Mouse UP" << endl;
    if (button == GLFW_MOUSE_BUTTON_LEFT && mods == 0) {
      dragging = false;
    }
  }

  void scroll(float dx, float dy) {  }

  void draw() {
    renderer.beginShader("simple-texture");

    float aspect = ((float)width()) / height();
    renderer.perspective(glm::radians(60.0f), aspect, 0.1f, 50.0f);
    renderer.lookAt(eyePos, lookPos, up);

    // draw plane
    renderer.texture("Image", "grass");
    renderer.push();
    renderer.translate(vec3(0.0, -0.5, 0));
    renderer.scale(vec3(2.0f));
    renderer.plane();
    renderer.pop();

    // draw tree
    renderer.texture("Image", "tree");
    renderer.push();

    // normalized direction to eye
    vec3 n = normalize(eyePos - lookPos);
    // compute rotation
    float theta = atan2(n.x, n.z);
    renderer.rotate(vec3(0, theta, 0));

    // compute scale based on original image
    float ratio = (float) imgWidth / imgHeight;
    renderer.scale(vec3(ratio, 1, 1));

    renderer.translate(vec3(-0.5, -0.5, 0));
    renderer.quad(); // vertices span from (0,0,0) to (1,1,0)
    renderer.pop();

    renderer.endShader();
  }

 protected:
  vec3 eyePos = vec3(0, 0, 2);
  vec3 lookPos = vec3(0, 0, 0);
  vec3 up = vec3(0, 1, 0);
  int azimuth = 0;  // in degrees 0 to 360
  int elevation = 0;  // in degrees, no limit
  float radius = length(eyePos.z - lookPos.z);  // radius of viewing sphere
  bool dragging = false;  // if true, compute change in camPos
  int imgWidth;
  int imgHeight;
};

int main(int argc, char** argv) {
  Viewer viewer;
  viewer.run();
  return 0;
}
