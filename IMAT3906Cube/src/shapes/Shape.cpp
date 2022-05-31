#include "shapes/Shape.h"

void Shape::extractVerts(std::vector<float> vertexData) {
	Vertex V;
	for (int i = 0; i < vertexData.size(); i += 8) {
		V.pos = glm::vec3(vertexData[i], vertexData[i + 1], vertexData[i + 2]);
		V.normal = glm::vec3(vertexData[i + 3], vertexData[i + 4], vertexData[i + 5]);
		V.uv = glm::vec2(vertexData[i + 6], vertexData[i + 7]);

		V.tan = glm::vec3(0.0f);
		V.biTan = glm::vec3(0.0f);

		vertices.push_back(V);
	}
}

void Shape::computeTanAndBiTan(std::vector<unsigned int> indicesData) {
	for (int i = 0; i < indicesData.size(); i += 3) {
		Vertex& a = vertices.at(indicesData[i]);
		Vertex& b = vertices.at(indicesData[i + 1]);
		Vertex& c = vertices.at(indicesData[i + 2]);

		glm::vec3 delta1 = b.pos - a.pos;
		glm::vec3 delta2 = c.pos - a.pos;

		glm::vec2 dUV1 = b.uv - a.uv;
		glm::vec2 dUV2 = c.uv - a.uv;

		float r = 1.0f / (dUV1.x * dUV2.y - dUV1.y * dUV2.x);

		glm::vec3 tangent = (delta1 * dUV2.y - delta2 * dUV1.y) * r;
		glm::vec3 biTangent = (delta2 * dUV1.x - delta1 * dUV2.x) * r;

		a.tan = a.tan + tangent;
		a.biTan = a.biTan + biTangent;

		b.tan = b.tan + tangent;
		b.biTan = b.biTan + biTangent;

		c.tan = c.tan + tangent;
		c.biTan = c.biTan + biTangent;
	}

	for (Vertex& V : vertices) {
		V.tan = glm::normalize(V.tan);
		V.biTan = glm::normalize(V.biTan);
	}
}

void Shape::calculateTanAndBiTan(std::vector<float> vertexData, std::vector<unsigned int> indicesData) {
	extractVerts(vertexData);
	computeTanAndBiTan(indicesData);
	unpackVertices();
}

void Shape::unpackVertices() {
	for (Vertex V : vertices) {
		int stride = 14;

		float vertexData[] = {
			V.pos.x, V.pos.y, V.pos.z,
			V.normal.x, V.normal.y, V.normal.z,
			V.uv.x, V.uv.y,
			V.tan.x, V.tan.y, V.tan.z,
			V.biTan.x, V.biTan.y, V.biTan.z
		};

		for (int i = 0; i < stride; i++) {
			updatedVertexData.push_back(vertexData[i]);
		}
	}
}

std::vector<float> Shape::getUpdatedVertexData() {
	return updatedVertexData;
}

void Shape::initGL(std::vector<float> updatedVerts, std::vector<unsigned int> indicesData) {
	int length = indicesData.size() * sizeof(GLint);
	unsigned int* indData = &indicesData[0];

	glEnable(GL_DEPTH_TEST);

	// Create VAO
	glGenVertexArrays(1, &VAO);
	glGenBuffers(1, &VBO);
	glGenBuffers(1, &EBO);

	glBindVertexArray(VAO);
	// fill VBO with vertex data
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, updatedVerts.size() * sizeof(GLfloat), updatedVerts.data(), GL_STATIC_DRAW);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, length, indData, GL_STATIC_DRAW);
	// position attribute
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, stride * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);
	// normal attribute
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, stride * sizeof(float), (void*)(3 * sizeof(float)));
	glEnableVertexAttribArray(1);
	// uv attribute
	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, stride * sizeof(float), (void*)(6 * sizeof(float)));
	glEnableVertexAttribArray(2);


	// tan attribute
	glVertexAttribPointer(3, 3, GL_FLOAT, GL_FALSE, stride * sizeof(float), (void*)(8 * sizeof(float)));
	glEnableVertexAttribArray(3);
	// bitan attribute
	glVertexAttribPointer(4, 3, GL_FLOAT, GL_FALSE, stride * sizeof(float), (void*)(11 * sizeof(float)));
	glEnableVertexAttribArray(4);
}
