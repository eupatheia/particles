/* sparkle-trail.cpp
 * Uses particles to draw a moving star with a sparkle trail
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

struct Particle {
  glm::vec3 pos;
  glm::vec3 vel;
  glm::vec4 color;
  float rot;
  float size;
  int age;  // number of times updated, from 0 to lifespan - 1
};

class Viewer : public Window {
 public:
  Viewer() : Window() {  }

  void setup() {
    setWindowSize(1000, 1000);
    createConfetti(maxParticles);
    renderer.setDepthTest(false);
    renderer.blendMode(agl::ADD);
  }

  float randFloat() {
    return (float) rand() / RAND_MAX;
  }

  void createConfetti(int size) {
    renderer.loadTexture("particle", "../textures/star4.png", 0);
    for (int i = 0; i < size; i++) {
      Particle particle;
      particle.color = vec4(agl::randomUnitCube(), 1);
      particle.size = 0.25;
      particle.rot = 0.0;
      particle.pos = position;
      // set as negative of current start velocity plus a random mutation
      particle.vel = -vec3(-sin(elapsedTime()), cos(elapsedTime()), 0) + 
          vec3(randFloat(), randFloat(), randFloat());
      particle.age = rand() % lifespan;  // randomly initialize age
      mParticles.push_back(particle);
    }
  }

  void updateConfetti() {
    // update star position (move along a circle)
    position = vec3(cos(elapsedTime()), sin(elapsedTime()), 0);
    for (int i = 0; i < mParticles.size(); i++) {
      Particle particle = mParticles[i];
      if (particle.age == lifespan - 1) {
        // done with life, recycle by reinitializing
        particle.age = 0;
        particle.color.a = 1;
        particle.size = 0.25;
        particle.rot = 0.0;
        particle.pos = position;  // start at current star position
        // new sparkle velocity is negative of current star velocity
        // plus a random mutation
        particle.vel = -vec3(-sin(elapsedTime()), cos(elapsedTime()), 0) + 
            vec3(randFloat(), randFloat(), randFloat());
      } else {
        // update age, transparency, size, rotation, position
        particle.age++;
        particle.color.a = 1.0 - ((float) particle.age / (lifespan - 1));
        particle.size += 0.0005;
        particle.rot += 0.005;
        particle.pos += dt() * particle.vel;
      }
      mParticles[i] = particle;
    }
  }

  void drawConfetti() {
    vec3 cameraPos = renderer.cameraPosition();

    // sort particles in increasing closeness
    for (int i = 1; i < mParticles.size(); i++) {
      Particle particle1 = mParticles[i];
      Particle particle2 = mParticles[i - 1];
      float dSqr1 = length2(particle1.pos - cameraPos);
      float dSqr2 = length2(particle2.pos - cameraPos);
      if (dSqr2 < dSqr1) {
        mParticles[i] = particle2;
        mParticles[i - 1] = particle1;
      }
    }

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
    eyePos.z += dy;
  }

  void keyUp(int key, int mods) {  }

  void draw() {
    renderer.beginShader("sprite");

    float aspect = ((float)width()) / height();
    renderer.perspective(glm::radians(60.0f), aspect, 0.1f, 50.0f);

    renderer.lookAt(eyePos, lookPos, up);
    renderer.sprite(position, vec4(1.0f), 0.25f);
    updateConfetti();
    drawConfetti();
    renderer.endShader();
  }

 protected:
  vec3 eyePos = vec3(0, 0, 3);
  vec3 lookPos = vec3(0, 0, 0);
  vec3 up = vec3(0, 1, 0);
  vec3 position = vec3(1, 0, 0);
  std::vector<Particle> mParticles;  // object pool
  int maxParticles = 100;
  int lifespan = 150;  // frames to draw particle before recycling
};

int main(int argc, char** argv) {
  Viewer viewer;
  viewer.run();
  return 0;
}
