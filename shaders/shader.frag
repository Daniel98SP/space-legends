#version 330

uniform sampler2D tex;
uniform int textRendering = 0;
uniform int endScene = 0;
uniform vec3 textColor;

in vec2 textureCoordinateFragment;

out vec4 outColor;

void main() {
	if (textRendering == 1) {
		vec4 sampled = vec4(1.0, 1.0, 1.0, texture(tex, textureCoordinateFragment).r);
		outColor = vec4(textColor, 1.0) * sampled;
	}
	else {
		outColor = texture(tex, textureCoordinateFragment);
	}
	if (endScene == 1) {
		outColor.w -= 0.8;
	}
}