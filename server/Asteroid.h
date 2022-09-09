#pragma once

#include <glm/glm.hpp>

class Asteroid {
public:
	void init(glm::vec2 position);
	bool isInsideRadius(glm::vec2 position);
	glm::vec2 getPosition();
	float getScale();

private:
	glm::vec2 position;
	float scale;
};
