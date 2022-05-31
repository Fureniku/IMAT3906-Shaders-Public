#pragma once
#include "Texture.h"

class Material {
private:
	float m_shine;
	float m_PXscale;
	Texture m_texture;

public:
	Material() {};
	Material(Texture tex, float shine, float PXscale = 0.0175f) :
		m_texture(tex),
		m_shine(shine),
		m_PXscale(PXscale) {}

	void init(Shader& shader);
	void draw(Shader& shader);
	inline Texture getTexture() { return m_texture; }
	inline float getShine() { return m_shine; }
};