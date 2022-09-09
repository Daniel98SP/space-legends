#include "Camera.h"
#include "../Global.h"

using namespace Global;

void Camera::init() {
}

glm::mat4 Camera::getProjection(glm::vec2 shipPosition, float zoomLevel) {
	glm::vec4 cameraLimits = getCameraLimits(shipPosition, zoomLevel);
	return glm::ortho(cameraLimits[0], cameraLimits[1], cameraLimits[3], cameraLimits[2]);
}

glm::vec2 Camera::screenToWorldCoordinates(glm::vec2 screenCoordinates, glm::vec2 shipPosition, float zoomLevel) {
	glm::vec4 cameraLimits = getCameraLimits(shipPosition, zoomLevel);
	return glm::ivec2(cameraLimits[0] + screenCoordinates.x * zoomLevel, cameraLimits[2] + screenCoordinates.y * zoomLevel);
}

glm::vec4 Camera::getCameraLimits(glm::vec2 shipPosition, float zoomLevel) {
	float left = shipPosition.x - RESOLUTION.x / 2 * zoomLevel;
	float right = shipPosition.x + RESOLUTION.x / 2 * zoomLevel;
	float top = shipPosition.y - RESOLUTION.y / 2 * zoomLevel;
	float bottom = shipPosition.y + RESOLUTION.y / 2 * zoomLevel;

	if (left < 0.f) {
		left = 0.f;
		right = (float)RESOLUTION.x * zoomLevel;
	}
	else if (MAP_SIZE_X < right) {
		left = MAP_SIZE_X - RESOLUTION.x * zoomLevel;
		right = MAP_SIZE_X;
	}

	if (top < 0.f) {
		top = 0.f;
		bottom = (float)RESOLUTION.y * zoomLevel;
	}
	else if (MAP_SIZE_Y < bottom) {
		top = MAP_SIZE_Y - RESOLUTION.y * zoomLevel;
		bottom = MAP_SIZE_Y;
	}

	return glm::vec4(left, right, top, bottom);
}