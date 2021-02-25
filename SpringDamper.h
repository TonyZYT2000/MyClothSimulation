#ifndef _SPRING_DAMPER_
#define _SPRING_DAMPER_

#include "iostream"
#include "core.h"
#include "Particle.h"

////////////////////////////////////////////////////////////////////////////////

class SpringDamper {
private:
      float ks;
      float kd;
      Particle* p1;
      Particle* p2;
      float restLength;

public:
      SpringDamper(Particle* p1, Particle* p2, float restLength);
      ~SpringDamper();
      
      void updateAcceleration();
};

////////////////////////////////////////////////////////////////////////////////

#endif
