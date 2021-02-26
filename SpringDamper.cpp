#include "SpringDamper.h"

SpringDamper::SpringDamper(Particle* p1, Particle* p2, float restLength) :
      ks(2000), kd(10), p1(p1), p2(p2), restLength(restLength) {}

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

      if (currLength > 1.2 * restLength) {
            glm::vec3 center = (p2->position + p1->position) / 2.0f;
            if (!p1->fixed) {
                  p1->position = center - 0.6f * restLength * direction;
            }
            if (!p2->fixed) {
                  p2->position = center + 0.6f * restLength * direction;
            }
      }
      else if (currLength < 0.5 * restLength) {
            glm::vec3 center = (p2->position + p1->position) / 2.0f;
            if (!p1->fixed) {
                  p1->position = center - restLength * direction / 4.0f;
            }
            if (!p2->fixed) {
                  p2->position = center + restLength * direction / 4.0f;
            }
      }

      glm::vec3 vClose = glm::dot(p2->velocity - p1->velocity, direction) * direction;
      glm::vec3 force = ks * deltaX * direction + kd * vClose;

      p1->applyForce(force);
      p2->applyForce(-force);
}