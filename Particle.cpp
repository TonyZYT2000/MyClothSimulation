#include "Particle.h"

Particle::Particle(glm::vec3 pos, glm::vec3 v, float m) : mass(m), fixed(false),
      normal(glm::vec3(0)), position(pos), velocity(v), acceleration(glm::vec3(0)) {}

Particle::~Particle() {}

void Particle::applyForce(glm::vec3 force, float step) {
      if (!fixed) {
            glm::vec3 acceleration = force / mass;
            applyAcceleration(acceleration, step);
      }
}

void Particle::applyAcceleration(glm::vec3 acceleration, float step) {
      if (!fixed) {
            velocity = velocity + step * acceleration;
            position = position + step * velocity;
      }
}

void Particle::setFix() {
      fixed = true;
}

void Particle::setFree() {
      fixed = false;
}