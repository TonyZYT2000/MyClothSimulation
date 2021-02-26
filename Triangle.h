#ifndef _TRIANGLE_H_
#define _TRIANGLE_H_

#include "iostream"
#include "core.h"
#include "Particle.h"

////////////////////////////////////////////////////////////////////////////////

class Triangle {
private:
      Particle* p1;
      Particle* p2;
      Particle* p3;
      float dragCo;
      float fluidDensity;

public:
      Triangle(Particle* p1, Particle* p2, Particle* p3);
      ~Triangle();
      
      void updateNormal();
      void wind(glm::vec3 vWind);
};

////////////////////////////////////////////////////////////////////////////////

#endif
