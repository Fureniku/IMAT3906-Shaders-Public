#pragma once
#include <glm/glm.hpp>
#include "rendering/Shader.h"

class Light {
protected:
	std::string m_name;
	glm::vec3 m_lightPos;

	float m_Kc = 1.0; //constant
	float m_Kl = 0.22; //linear
	float m_Ke = 0.22; //emission (?)

	float m_shine = 128;
	float m_specularStrength = 0.4;

public:
	Light(std::string name, glm::vec3 pos, float kc, float kl, float ke, float shine, float specStr) :
		m_name(name),
		m_lightPos(pos),
		m_Kc(kc),
		m_Kl(kl),
		m_Ke(ke),
		m_shine(shine),
		m_specularStrength(specStr)
	{}

	virtual void setUniforms(int id, Shader& shader) = 0;

	void setAttnValues(float kc, float kl, float ke);
	virtual void updateValue(Shader& shader, int id, std::string valName, float value); //!< Update a shader float value
	virtual void updateValue(Shader& shader, int id, std::string valName, glm::vec3 value); //!< update a shader vec3 value
	virtual void updateValue(Shader& shader, std::string valName, float value); //!< Update a shader float value
	virtual void updateValue(Shader& shader, std::string valName, glm::vec3 value); //!< update a shader vec3 value
	std::string conc(std::string name, int id, std::string value); //!< Create a shader-friendly string identifier with the ID
	std::string conc(std::string name, std::string value); //!< Create a shader-friendly string identifier

	inline void setPos(glm::vec3 pos) { m_lightPos = pos; }
	inline void setShine(float shine) { m_shine = shine; }
	inline void setSpecularStrength(float specStr) { m_specularStrength = specStr; }
	
	
};