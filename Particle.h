#ifndef _PARTICLE_H_
#define _PARTICLE_H_

#include "core.h"

////////////////////////////////////////////////////////////////////////////////

class Particle {
private:
      float mass;
      bool fixed;
      glm::vec3 normal;
      glm::vec3 position;
      glm::vec3 velocity;
      glm::vec3 acceleration;

      friend class Cloth;
      friend class Triangle;
      friend class SpringDamper;

public:
      Particle(glm::vec3 pos, glm::vec3 v, float m);
      ~Particle();
      
      void setFix();
      void setFree();
      void move(float step);
      void clearAcceleration();
      void applyForce(glm::vec3 force);
      void applyAcceleration(glm::vec3 a);
};

////////////////////////////////////////////////////////////////////////////////

#endif
