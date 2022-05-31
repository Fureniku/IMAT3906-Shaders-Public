#pragma once
#include <vector>
#include "rendering/Texture.h"
#include "rendering/Shader.h"
#include "rendering/Material.h"

struct Vertex {
	glm::vec3 pos;
	glm::vec3 normal;
	glm::vec2 uv;
	glm::vec3 tan;
	glm::vec3 biTan;
};

class Shape {
protected:
	std::vector<Vertex> vertices;
	std::vector<float> updatedVertexData;;
	unsigned int VBO, EBO, VAO;
	int stride = 14;
	Material m_material;

	void extractVerts(std::vector<float> vertexData);
	void computeTanAndBiTan(std::vector<unsigned int> indicesData);
public:
	virtual void draw(Shader& shader) = 0;
	virtual std::vector<float> getRawVertices() = 0;
	virtual std::vector<unsigned int> getRawIndices() = 0;

	inline Texture getTexture() { return m_material.getTexture(); }
	inline Material getMaterial() { return m_material; }
	void calculateTanAndBiTan(std::vector<float> vertexData, std::vector<unsigned int> indicesData);
	void unpackVertices();
	std::vector<float> getUpdatedVertexData();
	void initGL(std::vector<float> updatedVerts, std::vector<unsigned int> indicesData);
};