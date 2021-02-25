#ifndef _TRIANGLE_H_
#define _TRIANGLE_H_

#include "core.h"
#include "Particle.h"

////////////////////////////////////////////////////////////////////////////////

class Triangle {
private:
      Particle* p1;
      Particle* p2;
      Particle* p3;


public:
      Triangle(Particle* p1, Particle* p2, Particle* p3);
      ~Triangle();
      
      void updateNormal();
};

////////////////////////////////////////////////////////////////////////////////

#endif
