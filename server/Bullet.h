#pragma once

#include "../Global.h"

using namespace Global;

class Bullet {
public:
	void init(glm::vec2 position, void* target, TargetType targetType, float shotPower);
	ImpactType update(int deltaTime);

	glm::vec2 getPosition();
	glm::vec2 getVelocity();
	void* getTarget();

private:
	glm::vec2 position;
	glm::vec2 velocity;
	float shotPower;

	void* target;
	TargetType targetType;
};
