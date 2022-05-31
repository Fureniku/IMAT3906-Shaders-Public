#include "shapes/Plane.h"

Plane::Plane(Material mat, float size, float level) {
	m_material = mat;
	m_planeSize = size;
	m_planeLevel = level;
	init();
}

void Plane::init() {
	calculateTanAndBiTan(getRawVertices(), getRawIndices());
	std::vector<float> updatedPlaneVerts = getUpdatedVertexData();

	initGL(updatedPlaneVerts, getRawIndices());
}

std::vector<float> Plane::getRawVertices() {
	std::vector<float> planeVertices {
			-m_planeSize, m_planeLevel, -m_planeSize, 0.0, 1.0, 0.0, 0.0f, 0.0f,
			 m_planeSize, m_planeLevel, -m_planeSize, 0.0, 1.0, 0.0, 1.0f, 0.0f,
			 m_planeSize, m_planeLevel,  m_planeSize, 0.0, 1.0, 0.0, 1.0f, 1.0f,
			-m_planeSize, m_planeLevel,  m_planeSize, 0.0, 1.0, 0.0, 0.0f, 1.0f,
	};
	return planeVertices;
}

std::vector<unsigned int> Plane::getRawIndices() {
	std::vector<unsigned int> planeIndices {
		3,2,1,
		3,1,0
	};
	return planeIndices;
}

void Plane::draw(Shader& shader) {
	glm::mat4 model = glm::mat4(1.0f);
	shader.use();
	shader.setMat4("model", model);

	m_material.draw(shader);
	glBindVertexArray(VAO);  // bind and draw floor
	glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
}