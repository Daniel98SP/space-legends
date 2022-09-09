#pragma once

#include <glm/gtc/matrix_transform.hpp>

namespace Math {
	float getDistance(glm::vec2 position1, glm::vec2 position2);
	float getMagnitude(glm::vec2 vector);
	float getAngleBetweenVectors(glm::vec2 vector1, glm::vec2 vector2);
	float getAngleDifference(float angle1, float angle2);

	glm::vec2 getVector(glm::vec2 position1, glm::vec2 position2);
	glm::vec2 getCorrectingVector(glm::vec2 targetVector, glm::vec2 currentVector, bool onlyCorrective);
	glm::vec2 getComponentsFromMagnitude(glm::vec2 vector, float magnitude);
}