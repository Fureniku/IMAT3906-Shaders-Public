#version 420 core

out vec4 FragColor;

uniform sampler2D inFocusTex;
uniform sampler2D outFocusTex;
uniform sampler2D depthTex;

in vec2 uv;

float minDist = 1.0;
float maxDist = 3.0;

void main() {
	vec3 focusCol = texture(inFocusTex, uv).rgb;
	
	FragColor = vec4(focusCol, 1.0);
}