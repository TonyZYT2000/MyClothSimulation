#include "Particle.h"

Particle::Particle(glm::vec3 pos, glm::vec3 v, float m) : mass(m), fixed(false),
      normal(glm::vec3(0)), position(pos), velocity(v), acceleration(glm::vec3(0)) {}

Particle::~Particle() {}

void Particle::move(float step) {
      if (!fixed) {
            velocity = velocity + step * acceleration;
            position = position + step * velocity;
      }
}

void Particle::clearAcceleration() {
      acceleration = glm::vec3(0);
}

void Particle::applyForce(glm::vec3 force) {
      acceleration += force / mass;
}

void Particle::applyAcceleration(glm::vec3 a) {
      acceleration += a;
}

void Particle::setFix() {
      fixed = true;
}

void Particle::setFree() {
      fixed = false;
}