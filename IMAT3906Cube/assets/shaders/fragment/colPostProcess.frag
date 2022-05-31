#version 410 core

out vec4 FragColor;

uniform sampler2D image;
uniform int colShader;
uniform bool hdr;

in vec2 uv;

void main() {
	vec3 tex = texture(image, uv).xyz;
	float greyscale = (tex.x + tex.y + tex.z) / 3;

	vec3 result = tex;

	if (colShader == 0) { //nothing
		result = tex;
	}
	if (colShader == 1) { //greyscale
		result = vec3(greyscale);
	}
	if (colShader == 2) { //inverted
		result = vec3(1.0-tex.x, 1.0-tex.y, 1.0-tex.z);
	}
	if (colShader == 3) { //night vision
		result = vec3(0.0, tex.y, 0.0);
	}

	if (hdr) {
		result = result / (result + vec3(1.0f));
	}

	FragColor = vec4(result, 1.0f);
}