#include "Cloth.h"

Cloth::Cloth(int width, int height, glm::vec3 offset, Land* land) : land(land) {
	// model matrix and color
	model = glm::translate(offset) * glm::mat4(1.0f);
	color = glm::vec3(1.0f, 0.1f, 0.1f); 

	for (int i = 0; i < height; ++i) {
		for (int j = 0; j < width; ++j) {
			glm::vec3 pos = glm::vec3(-0.1 * width / 2, 0.1 * height / 2, 0) +
				glm::vec3(i) * glm::vec3(0, -0.1, 0) +
				glm::vec3(j) * glm::vec3(0.1, 0, 0);
			Particle* particle = new Particle(pos, glm::vec3(0.1, 0.1, 0.1), 0.1);
			particles.push_back(particle);
			positions.push_back(pos);
			if (i == 0) {
				particle->setFix();
			}
		}
	}

	for (int i = 0; i < height - 1; ++i) {
		for (int j = 0; j < width - 1; ++j) {
			int index1 = i * width + j;
			int index2 = (i + 1) * width + j;
			int index3 = i * width + j + 1;
			Particle* p1 = particles[index1];
			Particle* p2 = particles[index2];
			Particle* p3 = particles[index3];
			Triangle* triangle = new Triangle(p1, p2, p3);

			indices.push_back(index1);
			indices.push_back(index2);
			indices.push_back(index3);
			triangles.push_back(triangle);
		}
	}

	for (int i = 1; i < height; ++i) {
		for (int j = 0; j < width - 1; ++j) {
			int index1 = i * width + j;
			int index2 = i * width + j + 1;
			int index3 = (i - 1) * width + j + 1;
			Particle* p1 = particles[index1];
			Particle* p2 = particles[index2];
			Particle* p3 = particles[index3];
			Triangle* triangle = new Triangle(p1, p2, p3);

			indices.push_back(index1);
			indices.push_back(index2);
			indices.push_back(index3);
			triangles.push_back(triangle);
		}
	}

	// horizontal spring damper
	for (int i = 0; i < height; ++i) {
		for (int j = 0; j < width - 1; ++j) {
			int index1 = i * width + j;
			int index2 = i * width + j + 1;
			Particle* p1 = particles[index1];
			Particle* p2 = particles[index2];
			SpringDamper* springDamper = new SpringDamper(p1, p2, 0.1);

			springDampers.push_back(springDamper);
		}
	}

	// vertical spring damper
	for (int i = 0; i < height - 1; ++i) {
		for (int j = 0; j < width; ++j) {
			int index1 = i * width + j;
			int index2 = (i + 1) * width + j;
			Particle* p1 = particles[index1];
			Particle* p2 = particles[index2];
			SpringDamper* springDamper = new SpringDamper(p1, p2, 0.1);

			springDampers.push_back(springDamper);
		}
	}

	// diagonal spring damper
	for (int i = 0; i < height - 1; ++i) {
		for (int j = 0; j < width - 1; ++j) {
			int index1 = i * width + j;
			int index2 = (i + 1) * width + j + 1;
			Particle* p1 = particles[index1];
			Particle* p2 = particles[index2];
			SpringDamper* springDamper = new SpringDamper(p1, p2, 0.1 * glm::sqrt(2));

			springDampers.push_back(springDamper);
		}
	}

	// diagonal spring damper
	for (int i = 1; i < height; ++i) {
		for (int j = 0; j < width - 1; ++j) {
			int index1 = i * width + j;
			int index2 = (i - 1) * width + j + 1;
			Particle* p1 = particles[index1];
			Particle* p2 = particles[index2];
			SpringDamper* springDamper = new SpringDamper(p1, p2, 0.1 * glm::sqrt(2));

			springDampers.push_back(springDamper);
		}
	}

	updateNormal();
	updateAcceleration();

	for (auto particle : particles) {
		normals.push_back(particle->normal);
	}
	
	// Generate a vertex array (VAO) and two vertex buffer objects (VBO).
	glGenVertexArrays(1, &VAO);
	glGenBuffers(1, &VBO_positions);
	glGenBuffers(1, &VBO_normals);

	// Bind to the VAO.
	glBindVertexArray(VAO);

	// Bind to the first VBO - We will use it to store the vertices
	glBindBuffer(GL_ARRAY_BUFFER, VBO_positions);
	glBufferData(GL_ARRAY_BUFFER, sizeof(glm::vec3) * positions.size(), positions.data(), GL_STATIC_DRAW);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(GLfloat), 0);

	// Bind to the second VBO - We will use it to store the normals
	glBindBuffer(GL_ARRAY_BUFFER, VBO_normals);
	glBufferData(GL_ARRAY_BUFFER, sizeof(glm::vec3)* normals.size(), normals.data(), GL_STATIC_DRAW);
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(GLfloat), 0);

	// Generate EBO, bind the EBO to the bound VAO and send the data
	glGenBuffers(1, &EBO);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(unsigned int) * indices.size(), indices.data(), GL_STATIC_DRAW);

	// Unbind the VBOs.
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0);
}

Cloth::~Cloth()
{
	// Delete the VBOs and the VAO.
	glDeleteBuffers(1, &VBO_positions);
	glDeleteBuffers(1, &VBO_normals);
	glDeleteBuffers(1, &EBO);
	glDeleteVertexArrays(1, &VAO);

	for (auto particle : particles) {
		delete particle;
	}

	for (auto triangle : triangles) {
		delete triangle;
	}
}

////////////////////////////////////////////////////////////////////////////////

void Cloth::draw(const glm::mat4& viewProjMtx, GLuint shader)
{
	// actiavte the shader program 
	glUseProgram(shader);

	// get the locations and send the uniforms to the shader 
	glUniformMatrix4fv(glGetUniformLocation(shader, "viewProj"), 1, false, (float*)&viewProjMtx);
	glUniformMatrix4fv(glGetUniformLocation(shader, "model"), 1, GL_FALSE, (float*)&model);
	glUniform3fv(glGetUniformLocation(shader, "DiffuseColor"), 1, &color[0]);

	// Bind the VAO
	glBindVertexArray(VAO);

	// draw the points using triangles, indexed with the EBO
	glDrawElements(GL_TRIANGLES, indices.size(), GL_UNSIGNED_INT, 0);

	// Unbind the VAO and shader program
	glBindVertexArray(0);
	glUseProgram(0);
}

void Cloth::update() {
	for (auto particle : particles) {
            particle->applyAcceleration(particle->acceleration, 0.001);

		glm::vec3 pos = glm::vec3(model * glm::vec4(particle->position, 1));

		// collision bouncing
		if (land->collide(pos)) {
			pos[1] = land->top() + 0.01;
			particle->position = glm::vec3(glm::inverse(model) * glm::vec4(pos, 1));

			glm::vec3 v = glm::vec3(model * glm::vec4(particle->velocity, 0));
                  glm::vec3 vClose = glm::dot(glm::vec3(0, 1, 0), v) * glm::vec3(0, 1, 0);
                  glm::vec3 vTangent = v - vClose;

			if (vTangent != glm::vec3(0)) {
				float decreaseRatio = glm::min(0.5f * glm::length(vClose), 1.0f);
				vTangent = (1 - decreaseRatio) * vTangent;
			}
			else {
				vTangent = vTangent;
			}

                  particle->velocity = glm::vec3(glm::inverse(model) * glm::vec4(-0.1f * vClose + vTangent, 0));
		}
	}

	updateNormal();
	updateAcceleration();

	for (int i = 0; i < particles.size(); ++i) {
		positions[i] = particles[i]->position;
	}

	for (int i = 0; i < particles.size(); ++i) {
		normals[i] = particles[i]->normal;
	}

	// Bind to the VAO.
	glBindVertexArray(VAO);

	// Bind to the first VBO - We will use it to store the vertices
	glBindBuffer(GL_ARRAY_BUFFER, VBO_positions);
	glBufferData(GL_ARRAY_BUFFER, sizeof(glm::vec3) * positions.size(), positions.data(), GL_STATIC_DRAW);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(GLfloat), 0);

	// Bind to the second VBO - We will use it to store the normals
	glBindBuffer(GL_ARRAY_BUFFER, VBO_normals);
	glBufferData(GL_ARRAY_BUFFER, sizeof(glm::vec3)* normals.size(), normals.data(), GL_STATIC_DRAW);
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(GLfloat), 0);

	// Generate EBO, bind the EBO to the bound VAO and send the data
	glGenBuffers(1, &EBO);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(unsigned int) * indices.size(), indices.data(), GL_STATIC_DRAW);

	// Unbind the VBOs.
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0);
}

void Cloth::updateNormal() {
	for (auto particle : particles) {
		particle->normal = glm::vec3(0);
	}

	for (auto triangle : triangles) {
		triangle->updateNormal();
	}

	for (auto particle : particles) {
		particle->normal = glm::normalize(particle->normal);
	}
}

void Cloth::updateAcceleration() {
	for (auto particle : particles) {
		particle->acceleration = glm::vec3(glm::inverse(model) * glm::vec4(0, -9.8, 0, 0));
	}
	
	for (auto springDamper : springDampers) {
		springDamper->updateAcceleration();
	}
}
