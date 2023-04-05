/* snow.cpp
 * Uses particles to create falling snow effiect
 * Base code: Bryn Mawr College, alinen, 2020
 * Modified by JL
 * 5 April 2023
 */

#include <cmath>
#include <string>
#include <vector>
#include "agl/window.h"

using namespace std;
using namespace glm;
using namespace agl;

struct Particle {
  glm::vec3 pos;
  glm::vec3 vel;
  glm::vec4 color;
  float rot;
  float size;
  float mass;
  int age;  // number of times updated, from 0 to lifespan - 1
};

class Viewer : public Window {
 public:
  Viewer() : Window() {  }

  void setup() {
    setWindowSize(1000, 1000);
    createParticles(maxParticles);
    renderer.setDepthTest(false);
    renderer.blendMode(agl::ADD);
  }

  // return a random float in range [start, end)
  float randFloat(float start, float end) {
    float num = (float) rand() / RAND_MAX;
    num = (num * (end - start)) + start;
    return num;
  }

  void createParticles(int size) {
    renderer.loadTexture("particle", "../textures/particle.png", 0);
    for (int i = 0; i < size; i++) {
      Particle particle;
      particle.color = vec4(1, 1, 1, 1);
      particle.size = randFloat(0.01, 0.05);
      // start at "top"
      particle.pos = vec3(randFloat(-1.5, 1.5), 1.5, 0);
      // random movement in x direction, starting velocity.y = 0
      particle.vel = {randFloat(-0.1, 0.1), 0, 0};
      particle.mass = mass;
      particle.age = rand() % lifespan;  // randomly initialize age
      mParticles.push_back(particle);
    }
  }

  void updateParticles() {
    for (int i = 0; i < mParticles.size(); i++) {
      Particle particle = mParticles[i];
      if (particle.age == lifespan - 1) {
        // done with life, recycle by reinitializing
        particle.age = 0;
        // restart at top, restart velocity
        particle.pos = vec3(randFloat(-1.5, 1.5), 1.5, 0);
        particle.vel = {randFloat(-0.1, 0.1), 0, 0};
      } else {
        // update age, position, velocity;
        particle.age++;
        particle.pos += dt() * particle.vel;
        if (particle.pos.y <= -1.5) {
          // stop moving once reached ground;
          particle.pos.y = -1.5;
          particle.vel = {0, 0, 0};
        } else {
          // update velocity based on force of gravity and mass
          particle.vel.y -= dt() * (gravity / particle.mass);
        }
      }
      mParticles[i] = particle;
    }
  }

  void drawParticles() {
    // no need to sort by depth b/c all particles at z=0

    // draw particles from farthest to nearest (back to front)
    renderer.texture("image", "particle");
    for (int i = 0; i < mParticles.size(); i++) {
      Particle particle = mParticles[i];
      renderer.sprite(particle.pos, particle.color, particle.size,
          particle.rot);
    }
  }

  void mouseMotion(int x, int y, int dx, int dy) {  }

  void mouseDown(int button, int mods) {  }

  void mouseUp(int button, int mods) {  }

  void scroll(float dx, float dy) {
    // eyePos.z += dy;
  }

  void keyUp(int key, int mods) {  }

  void draw() {
    renderer.beginShader("sprite");

    float aspect = ((float)width()) / height();
    renderer.perspective(glm::radians(60.0f), aspect, 0.1f, 50.0f);
    renderer.lookAt(eyePos, lookPos, up);

    updateParticles();
    drawParticles();
    renderer.endShader();
  }

 protected:
  vec3 eyePos = vec3(0, 0, 3);
  vec3 lookPos = vec3(0, 0, 0);
  vec3 up = vec3(0, 1, 0);
  vec3 position = vec3(1, 0, 0);
  std::vector<Particle> mParticles;  // object pool
  int maxParticles = 300;
  int lifespan = 750;  // frames to draw particle before recycling
  float gravity = 100;
  float mass = 100;
};

int main(int argc, char** argv) {
  Viewer viewer;
  viewer.run();
  return 0;
}
