#pragma once
#include <glad/glad.h>
#include <glm/glm.hpp>
#include "Properties.h"

class FrameBuffer {
private:
	glm::vec2 m_screenSize;
	Properties m_props;
	unsigned int fboCol;
	unsigned int fboDepth;
	unsigned int fboColDepth;
	unsigned int fboBlur;
	unsigned int fboShadowMap;

	unsigned int colAttArray[2];
	unsigned int colourAttachment;
	unsigned int blurAttachment;
	unsigned int depthAttachment;
	unsigned int shadowAttachment;
public:
	FrameBuffer(glm::vec2 screenSize, Properties props) : m_screenSize(screenSize), m_props(props) {}
	void setFBOCol();
	void setFBOBlur();
	void setFBODepth();
	void setFBOShadowMap();
	void setFBOColAndDepth();

	inline unsigned int getFboCol() { return fboCol; }
	inline unsigned int getFboDepth() { return fboDepth; }
	inline unsigned int getFboColDepth() { return fboColDepth; }
	inline unsigned int getFboBlur() { return fboBlur; }
	inline unsigned int getFboShadowMap() { return fboShadowMap; }

	inline unsigned int getColAttArray(int i) { return colAttArray[i]; }
	inline unsigned int getColAttachment() { return colourAttachment; }
	inline unsigned int getBlurAttachment() { return blurAttachment; }
	inline unsigned int getDepthAttachment() { return depthAttachment; }
	inline unsigned int getShadowAttachment() { return shadowAttachment; }
};