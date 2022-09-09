#version 330

uniform mat4 projection, modelview;
uniform vec2 textureCoordinateOffset = vec2(0.f);

in vec2 position;
in vec2 textureCoordinate;

out vec2 textureCoordinateFragment;

void main() {
	textureCoordinateFragment = textureCoordinate + textureCoordinateOffset;
	gl_Position = projection * modelview * vec4(position, 0.0, 1.0);
}