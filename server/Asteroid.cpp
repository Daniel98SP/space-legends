#include "Asteroid.h"
#include <random>
#include "../Math.h"

using namespace Math;

void Asteroid::init(glm::vec2 position) {
	std::random_device randomDevice;
	std::mt19937 gen(randomDevice());

	std::uniform_real_distribution<> sizeDist(0.9f, 1.3f);
	this->scale = sizeDist(gen);
	// size.x = 2510 * scale.x  | size.y = 2300 * scale.y
	this->position = position;
}

bool Asteroid::isInsideRadius(glm::vec2 position) {
	return getDistance(this->position, position) < ((2510.f * this->scale) / 2) + 300.f;
}

glm::vec2 Asteroid::getPosition() {
	return this->position;
}
float Asteroid::getScale() {
	return this->scale;
}