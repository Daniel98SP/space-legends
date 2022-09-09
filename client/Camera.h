#pragma once

#include <glm/gtc/matrix_transform.hpp>
#include "../openglframework/Texture.h"
#include "../openglframework/Sprite.h"

class Camera
{
public:
	void init();
	glm::mat4 getProjection(glm::vec2 shipPosition, float zoomLevel);
	glm::vec2 screenToWorldCoordinates(glm::vec2 screenCoordinates, glm::vec2 shipPosition, float zoomLevel);
	glm::vec4 getCameraLimits(glm::vec2 shipPosition, float zoomLevel);
};



