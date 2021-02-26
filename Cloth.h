#ifndef _CLOTH_H_
#define _CLOTH_H_

#include "core.h"
#include "Land.h"
#include "Particle.h"
#include "Triangle.h"
#include "SpringDamper.h"

////////////////////////////////////////////////////////////////////////////////

class Cloth {
private:
	Land* land;
	glm::vec3 localWind;

	GLuint VAO;
	GLuint VBO_positions, VBO_normals, EBO;

	glm::mat4 model;
	glm::vec3 color;

	std::vector<Particle*> particles;
	std::vector<Triangle*> triangles;
	std::vector<SpringDamper*> springDampers;

	std::vector<glm::vec3> positions;
	std::vector<glm::vec3> normals;
	std::vector<unsigned int> indices;

	void updateNormal();
	void updateAcceleration();

public:
      Cloth(int width, int height, glm::vec3 offset, Land* land);
      ~Cloth();
      
	void draw(const glm::mat4& viewProjMtx, GLuint shader);
	void update();
	void blow(glm::vec3 wind);
};

////////////////////////////////////////////////////////////////////////////////

#endif
