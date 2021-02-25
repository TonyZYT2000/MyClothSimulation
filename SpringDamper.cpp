#include "SpringDamper.h"

SpringDamper::SpringDamper(Particle* p1, Particle* p2, float restLength) :
      ks(5000), kd(20), p1(p1), p2(p2), restLength(restLength) {}

SpringDamper::~SpringDamper() {}

void SpringDamper::updateAcceleration() {
      float currLength = glm::length(p2->position - p1->position);
      float deltaX = currLength - restLength;
      glm::vec3 direction;

      if (currLength == 0) {
            direction = glm::vec3(0, 1, 0);
      }
      else {
            direction = glm::normalize(p2->position - p1->position);
      }

      glm::vec3 vClose = glm::dot(p2->velocity - p1->velocity, direction) * direction;
      glm::vec3 force = ks * deltaX * direction + kd * vClose;

      p1->acceleration += force / p1->mass;
      p2->acceleration += (-force) / p2->mass;
}