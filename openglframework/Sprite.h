#pragma once

#include <vector>
#include <glm/glm.hpp>
#include "Texture.h"
#include "ShaderProgram.h"

// This class is derived from code seen earlier in TexturedQuad but it is also
// able to manage animations stored as a spritesheet. 

class Sprite
{

public:
	// Textured quads can only be created inside an OpenGL context
	static Sprite *createSprite(const glm::vec2 &quadSize, const glm::vec2 &sizeInSpritesheet, Texture *spritesheet, string origin = "corner");

	Sprite(const glm::vec2 &quadSize, const glm::vec2 &sizeInSpritesheet, Texture *spritesheet, string origin);

	void update(int deltaTime);
	void render() const;
	void free();

	void setNumberAnimations(int nAnimations);
	void setAnimationSpeed(int animId, int keyframesPerSec);
	void addKeyframe(int animId, const glm::vec2 &frame);
	void changeAnimation(int animId);
	int animation() const;
	
	void setPosition(const glm::vec2 &pos);
	void setRotation(float rotationAngle);
	void setScale(const glm::vec2& scale);

	float getRotation();
	glm::vec2 getScale();

private:
	struct AnimKeyframes {
		float millisecsPerKeyframe;
		vector<glm::vec2> keyframeDispl;
	};

	Texture *texture;
	GLuint vao;
	GLuint vbo;

	glm::vec2 size;
	glm::vec2 position;
	float rotationAngle;
	glm::vec2 scale;

	int currentAnimation, currentKeyframe;
	float timeAnimation;
	glm::vec2 textureCoordinateOffset;
	vector<AnimKeyframes> animations;

	int origin;
};

