#ifndef _LAND_H_
#define _LAND_H_

#include "core.h"

////////////////////////////////////////////////////////////////////////////////

class Land
{
private:
	GLuint VAO;
	GLuint VBO_positions, VBO_normals, EBO;

	glm::mat4 model;
	glm::vec3 color;

	// Cube Information
	std::vector<glm::vec3> positions;
	std::vector<glm::vec3> normals;
	std::vector<unsigned int> indices;

public:
	Land(float width, glm::vec3 offset);
	~Land();

	void draw(const glm::mat4& viewProjMtx, GLuint shader);
	void update();

	void spin(float deg);
};

////////////////////////////////////////////////////////////////////////////////

#endif
