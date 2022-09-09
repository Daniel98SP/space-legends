#include <GL/glew.h>
#include <GL/gl.h>
#include <glm/gtc/matrix_transform.hpp>
#include "Sprite.h"
#include "../Global.h"

using namespace Global;


Sprite *Sprite::createSprite(const glm::vec2 &quadSize, const glm::vec2 &sizeInSpritesheet, Texture *spritesheet, string origin)
{
	Sprite *quad = new Sprite(quadSize, sizeInSpritesheet, spritesheet, origin);

	return quad;
}


Sprite::Sprite(const glm::vec2 &quadSize, const glm::vec2 &sizeInSpritesheet, Texture *spritesheet, string origin) {
	float vertices[24] = {
		0.f,        0.f,			0.f,                 0.f,
		quadSize.x, 0.f,			sizeInSpritesheet.x, 0.f,
		quadSize.x, quadSize.y,		sizeInSpritesheet.x, sizeInSpritesheet.y,
		0.f,        0.f,			0.f,                 0.f,
		quadSize.x, quadSize.y,		sizeInSpritesheet.x, sizeInSpritesheet.y,
		0.f,        quadSize.y,		0.f,                 sizeInSpritesheet.y
	};
	
	if (origin == "center") {
		vertices[0] =  -quadSize.x / 2; vertices[1] =  -quadSize.y / 2;
		vertices[4] =   quadSize.x / 2; vertices[5] =  -quadSize.y / 2;
		vertices[8] =   quadSize.x / 2; vertices[9] =   quadSize.y / 2;
		vertices[12] = -quadSize.x / 2; vertices[13] = -quadSize.y / 2;
		vertices[16] =  quadSize.x / 2; vertices[17] =  quadSize.y / 2;
		vertices[20] = -quadSize.x / 2; vertices[21] =  quadSize.y / 2;

	}

	glGenBuffers(1, &vbo);
	glBindBuffer(GL_ARRAY_BUFFER, vbo);
	glBufferData(GL_ARRAY_BUFFER, 24 * sizeof(float), vertices, GL_STATIC_DRAW);

	glGenVertexArrays(1, &vao);
	glBindVertexArray(vao);	
	glEnableVertexAttribArray(shader.bindVertexAttribute("position", 2, 4 * sizeof(float), 0));
	glEnableVertexAttribArray(shader.bindVertexAttribute("textureCoordinate", 2, 4 * sizeof(float), (void*)(2 * sizeof(float))));

	this->texture = spritesheet;

	this->currentAnimation = 0;
	this->size = quadSize;
	this->position = glm::vec2(0.0f);
	this->rotationAngle = 0.0f;
	this->scale = glm::vec2(1.0f);
	this->origin = origin == "corner" ? 0 : 1;

	setNumberAnimations(1);
	addKeyframe(0, glm::vec2(0.0, 0.0));
	changeAnimation(0);
}

void Sprite::update(int deltaTime) {
	if (currentAnimation >= 0) {
		timeAnimation += deltaTime;
		while(timeAnimation > animations[currentAnimation].millisecsPerKeyframe) {
			timeAnimation -= animations[currentAnimation].millisecsPerKeyframe;
			currentKeyframe = (currentKeyframe + 1) % animations[currentAnimation].keyframeDispl.size();
		}
		textureCoordinateOffset = animations[currentAnimation].keyframeDispl[currentKeyframe];
	}
}

void Sprite::render() const {
	glm::mat4 modelview = glm::translate(glm::mat4(1.0f), glm::vec3(position.x, position.y, 0.f));
	modelview = glm::rotate(modelview, glm::radians(this->rotationAngle), glm::vec3(0.0f, 0.0f, 1.0f));
	modelview = glm::scale(modelview, glm::vec3(this->scale.x, this->scale.y, 1.0f));
	shader.setUniformMatrix4f("modelview", modelview);
	shader.setUniform2f("textureCoordinateOffset", textureCoordinateOffset.x, textureCoordinateOffset.y);
	texture->use();
	glBindVertexArray(vao);
	glBindBuffer(GL_ARRAY_BUFFER, vbo);
	glDrawArrays(GL_TRIANGLES, 0, 6);
	glDisable(GL_TEXTURE_2D);
}

void Sprite::free() {
	glDeleteBuffers(1, &vbo);
}

void Sprite::setNumberAnimations(int nAnimations) {
	animations.clear();
	animations.resize(nAnimations);
}

void Sprite::setAnimationSpeed(int animId, int keyframesPerSec) {
	if (animId < int(animations.size())) {
		animations[animId].millisecsPerKeyframe = 1000.f / keyframesPerSec;
	}
}

void Sprite::addKeyframe(int animId, const glm::vec2 &displacement) {
	if (animId < int(animations.size())) {
		animations[animId].keyframeDispl.push_back(displacement);
	}
}

void Sprite::changeAnimation(int animId) {
	if (animId < int(animations.size()))	{
		currentAnimation = animId;
		currentKeyframe = 0;
		timeAnimation = 0.f;
		textureCoordinateOffset = animations[animId].keyframeDispl[0];
	}
}

int Sprite::animation() const {
	return currentAnimation;
}

void Sprite::setPosition(const glm::vec2 &pos) {
	this->position = pos;
}

void Sprite::setRotation(float rotationAngle) {
	this->rotationAngle = rotationAngle;
}

void Sprite::setScale(const glm::vec2& scale) {
	this->scale = scale;
}

float Sprite::getRotation() {
	return this->rotationAngle;
}

glm::vec2 Sprite::getScale() {
	return this->scale;
}



