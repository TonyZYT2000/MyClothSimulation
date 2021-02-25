#include "Triangle.h"

Triangle::Triangle(Particle* p1, Particle* p2, Particle* p3) :
      p1(p1), p2(p2), p3(p3) {
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