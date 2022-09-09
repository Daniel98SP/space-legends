#include "Math.h"

float Math::getDistance(glm::vec2 position1, glm::vec2 position2) {
	return sqrt(pow(position1.x - position2.x, 2) + pow(position1.y - position2.y, 2));
}

glm::vec2 Math::getVector(glm::vec2 position1, glm::vec2 position2) {
	if (position1.x == position2.x && position1.y == position2.y) {
		return glm::vec2(0.f, 0.f);
	}
	else {
		return glm::normalize(glm::vec2(position2.x - position1.x, position2.y - position1.y));
	}
}

float Math::getAngleBetweenVectors(glm::vec2 vector1, glm::vec2 vector2) {
	float dot = vector1.x * vector2.x + vector1.y * vector2.y;
	float det = vector1.x * vector2.y - vector1.y * vector2.x;
	float test = glm::degrees(atan2(det, dot));
	return test;
}

glm::vec2 Math::getCorrectingVector(glm::vec2 targetVector, glm::vec2 currentVector, bool onlyCorrective) {
	float angle = getAngleBetweenVectors(targetVector, currentVector);
	float theta = 0.f;
	if (abs(angle) < 90.f && !onlyCorrective) {
		theta = glm::radians(-angle);
	}
	else {
		theta = angle < 0.f ? glm::radians(angle + 180.f) : glm::radians(angle - 180.f);
	}
	return glm::vec2(targetVector.x * cos(theta) - targetVector.y * sin(theta), targetVector.x * sin(theta) + targetVector.y * cos(theta));
}

float Math::getMagnitude(glm::vec2 vector) {
	return sqrt(pow(vector.x, 2) + pow(vector.y, 2));
}

glm::vec2 Math::getComponentsFromMagnitude(glm::vec2 vector, float magnitude) {
	vector = vector.x == 0.f && vector.y == 0.f ? glm::vec2(0.1f, 0.1f) : vector;
	glm::vec2 normalizedVector = glm::normalize(vector);
	return glm::vec2(normalizedVector.x * magnitude, normalizedVector.y * magnitude);
}

float Math::getAngleDifference(float angle1, float angle2) {
	float angle = angle2 - angle1;
	angle = angle < -180.f ? 360.f + angle : angle;
	angle = 180.f < angle ? -360.f + angle : angle;
	return angle;
}
