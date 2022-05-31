#include "shapes/Cube.h"
#include <GLFW/glfw3.h>

Cube::Cube(glm::vec3 col, float cubeSize, glm::vec3 pos, glm::vec3 scale) {
	init(col, cubeSize, pos, scale, m_spin, m_spinAxis);
}

Cube::Cube(Material mat, float cubeSize, glm::vec3 pos, glm::vec3 scale) {
	init(mat, cubeSize, pos, scale, m_spin, m_spinAxis);
}

Cube::Cube(Material mat, float cubeSize, glm::vec3 pos, glm::vec3 scale, bool spin) {
	init(mat, cubeSize, pos, scale, spin, m_spinAxis);
}

Cube::Cube(Material mat, float cubeSize, glm::vec3 pos, glm::vec3 scale, bool spin, glm::vec3 spinAxis) {
	init(mat, cubeSize, pos, scale, spin, spinAxis);
}

void Cube::init(Material mat, float cubeSize, glm::vec3 pos, glm::vec3 scale, bool spin, glm::vec3 spinAxis) {
	m_material = mat;
	hasMaterial = true;
	init(cubeSize, pos, scale, spin, spinAxis);
}

void Cube::init(glm::vec3 col, float cubeSize, glm::vec3 pos, glm::vec3 scale, bool spin, glm::vec3 spinAxis) {
	m_col = col;
	init(cubeSize, pos, scale, spin, spinAxis);
}

void Cube::init(float cubeSize, glm::vec3 pos, glm::vec3 scale, bool spin, glm::vec3 spinAxis) {
	m_size = cubeSize;
	m_cubePos = pos;
	m_scale = scale;
	m_spin = spin;
	m_spinAxis = spinAxis;

	calculateTanAndBiTan(getRawVertices(), getRawIndices());
	std::vector<float> updatedCubeVerts = getUpdatedVertexData();
	
	initGL(updatedCubeVerts, getRawIndices());
}

std::vector<float> Cube::getRawVertices() {
	std::vector<float> cubeVertices {
		//back
		-m_size, -m_size, -m_size,  0.0f,  0.0f, -1.0f, 0.0f, 0.0f, //0
		 m_size, -m_size, -m_size,  0.0f,  0.0f, -1.0f, 1.0f, 0.0f,
		 m_size,  m_size, -m_size,  0.0f,  0.0f, -1.0f, 1.0f, 1.0f,
		-m_size,  m_size, -m_size,  0.0f,  0.0f, -1.0f, 0.0f, 1.0f,

		//front
		-m_size, -m_size,  m_size,  0.0f,  0.0f,  1.0f, 0.0f, 0.0f, //4
		 m_size, -m_size,  m_size,  0.0f,  0.0f,  1.0f, 1.0f, 0.0f,
		 m_size,  m_size,  m_size,  0.0f,  0.0f,  1.0f, 1.0f, 1.0f,
		-m_size,  m_size,  m_size,  0.0f,  0.0f,  1.0f, 0.0f, 1.0f,

		//left
		-m_size,  m_size,  m_size, -1.0f,  0.0f,  0.0f, 1.0f, 0.0f, //8
		-m_size,  m_size, -m_size, -1.0f,  0.0f,  0.0f, 1.0f, 1.0f,
		-m_size, -m_size, -m_size, -1.0f,  0.0f,  0.0f, 0.0f, 1.0f,
		-m_size, -m_size,  m_size, -1.0f,  0.0f,  0.0f, 0.0f, 0.0f,

		//right
		m_size,  m_size,  m_size,  1.0f,  0.0f,  0.0f, 1.0f, 0.0f,  //12
		m_size,  m_size, -m_size,  1.0f,  0.0f,  0.0f, 1.0f, 1.0f,
		m_size, -m_size, -m_size,  1.0f,  0.0f,  0.0f, 0.0f, 1.0f,
		m_size, -m_size,  m_size,  1.0f,  0.0f,  0.0f, 0.0f, 0.0f,

		//bottom
		-m_size, -m_size, -m_size,  0.0f, -1.0f,  0.0f, 0.0f, 1.0f, //16
		 m_size, -m_size, -m_size,  0.0f, -1.0f,  0.0f, 1.0f, 1.0f,
		 m_size, -m_size,  m_size,  0.0f, -1.0f,  0.0f, 1.0f, 0.0f,
		-m_size, -m_size,  m_size,  0.0f, -1.0f,  0.0f, 0.0f, 0.0f,

		//top	
		-m_size,  m_size, -m_size,  0.0f,  1.0f,  0.0f, 0.0f, 1.0f, //20
		 m_size,  m_size, -m_size,  0.0f,  1.0f,  0.0f, 1.0f, 1.0f,
		 m_size,  m_size,  m_size,  0.0f,  1.0f,  0.0f, 1.0f, 0.0f,
		-m_size,  m_size,  m_size,  0.0f,  1.0f,  0.0f, 0.0f, 0.0f,
	};

	return cubeVertices;
}

std::vector<unsigned int> Cube::getRawIndices() {
	std::vector<unsigned int> cubeIndices {
		1,2,3,
		1,3,0,

		5,6,7,
		5,7,4,

		11,8,9,
		11,9,10,

		15,12,13,
		15,13,14,

		19,18,17,
		19,17,16,

		23,22,21,
		23,21,20
	};
	return cubeIndices;
}

void Cube::draw(Shader& shader) {
	glm::mat4 model = glm::mat4(1.0f);

	model = glm::translate(model, m_cubePos);
	if (m_spin) { model = glm::rotate(model, (float)(glfwGetTime()), m_spinAxis); }
	model = glm::scale(model, m_scale);

	shader.use();
	shader.setMat4("model", model);

	if (hasMaterial) {
		m_material.draw(shader);
	}
	else {
		shader.use();
		shader.setVec3("lightColour", m_col);
	}
	

	glBindVertexArray(VAO);  // bind and draw cube
	glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_INT, 0);

	model = glm::mat4(1.0f);
}