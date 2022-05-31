#version 420 core

out vec4 FragColor;

uniform sampler2D image;
uniform sampler2D bloomBlur;

in vec2 uv;

void main() {
	vec3 hdrColour = texture(image, uv).rgb;
	vec3 bloomColour = texture(bloomBlur, uv).rgb;
	hdrColour += bloomColour;

	vec3 reinhard = hdrColour / (hdrColour + vec3(1.0));
	FragColor = vec4(reinhard, 1.0);
}