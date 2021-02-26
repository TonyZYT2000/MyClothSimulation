#include "Triangle.h"

Triangle::Triangle(Particle* p1, Particle* p2, Particle* p3) :
      p1(p1), p2(p2), p3(p3), dragCo(1.28), fluidDensity(1.225) {
      updateNormal();
}

Triangle::~Triangle() {}

void Triangle::updateNormal() {
      glm::vec3 side1 = p2->position - p1->position;
      glm::vec3 side2 = p3->position - p1->position;
      glm::vec3 normal = glm::normalize(glm::cross(side1, side2));
      p1->normal += normal;
      p2->normal += normal;
      p3->normal += normal;
}

void Triangle::wind(glm::vec3 vWind) {
      glm::vec3 side1 = p2->position - p1->position;
      glm::vec3 side2 = p3->position - p1->position;
      float area = glm::length(glm::cross(side1, side2)) / 2.0f;

      glm::vec3 normal;
      if (area == 0) {
            normal = glm::vec3(0, 1, 0);
      }
      else {
            normal = glm::normalize(glm::cross(side1, side2));
      }

      glm::vec3 vTriangle = (p1->velocity + p2->velocity + p3->velocity) / 3.0f;
      glm::vec3 vClose = vTriangle - vWind;
      float crossArea;
      if (glm::length(vClose) == 0) {
            crossArea = 0;
      }
      else {
            crossArea = area * glm::dot(vClose, normal) / glm::length(vClose);
      }

      glm::vec3 force = - fluidDensity * dragCo * glm::dot(vClose, vClose) * crossArea * normal / 2.0f;
      glm::vec3 forceOnParticle = force / 3.0f;

      p1->applyForce(forceOnParticle);
      p2->applyForce(forceOnParticle);
      p3->applyForce(forceOnParticle);
}