#include "Bullet.h"
#include <random>
#include "Ship.h"
#include "Base.h"
#include "../Math.h"

using namespace Math;

void Bullet::init(glm::vec2 position, void* target, TargetType targetType, float shotPower) {

	this->position = position;
	this->shotPower = shotPower;
	this->target = target;
	this->targetType = targetType;
}

ImpactType Bullet::update(int deltaTime) {
	if (this->targetType == TargetType::planet || this->targetType == TargetType::station) {
		Base* castedTarget = (Base*)this->target;
		if (castedTarget->isInsideRadius(this->position)) {
			ImpactType impactType = castedTarget->damage(this->shotPower);
			return impactType;
		}
		else {
			glm::vec2 velocityVector = getVector(this->position, castedTarget->getPosition());
			this->velocity = (velocityVector / getMagnitude(velocityVector)) * 9.f * 17.f;
		}
	}
	else {
		Ship* castedTarget = (Ship*)this->target;
		if (castedTarget->isInsideRadius(this->position)) {
			ImpactType impactType = castedTarget->damage(this->shotPower, true);
			return impactType;
		}
		else {
			glm::vec2 velocityVector = getVector(this->position, castedTarget->getPosition());
			this->velocity = (velocityVector / getMagnitude(velocityVector)) * 9.f * 17.f;
		}
	}

	this->position += this->velocity;
	return ImpactType::none;
}

glm::vec2 Bullet::getPosition() {
	return this->position;
}

glm::vec2 Bullet::getVelocity() {
	return this->velocity;
}

void* Bullet::getTarget() {
	return this->target;
}