#ifndef _CUBE_H_
#define _CUBE_H_

#include "core.h"

////////////////////////////////////////////////////////////////////////////////

class Cube
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
	Cube(glm::vec3 cubeMin=glm::vec3(-1,-1,-1), glm::vec3 cubeMax=glm::vec3(1, 1, 1));
	~Cube();

	void draw(const glm::mat4& viewProjMtx, GLuint shader);
	void update();

	void spin(float deg);
};

////////////////////////////////////////////////////////////////////////////////

#endif
