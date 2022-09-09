#include "Base.h"
#include "../Math.h"

using namespace Math;

void Base::init(Type type, Team team, int nodeId, glm::vec2 position, Ship* blueShips, Ship* orangeShips, float health) {
	this->nodeId = nodeId;
	this->position = position;
	float healthBarOffset = 0.f;
	float healthBarSizeMultiplier = 0.f;

	if (team == Team::blue) {
		this->enemyShips = orangeShips;
	}
	else {
		this->enemyShips = blueShips;
	}

	this->type = type;
	this->maxHealth = health;
	this->health = health;
	this->timeSinceBulletShot = 99999;
}

bool Base::update(int deltaTime) {
	if (this->type == Base::Type::station) {
		bool enemyShipDestroyed = automaticShooting(deltaTime);
		return enemyShipDestroyed;
	}
	else {
		return false;
	}
}

ImpactType Base::damage(float damage) {
	this->health -= damage;

	return this->health <= 0 ? ImpactType::destroyed : ImpactType::hit;
}

bool Base::automaticShooting(int deltaTime) {
	this->timeSinceBulletShot += deltaTime;

	if (this->bullet) {
		ImpactType impactType = this->bullet->update(deltaTime);
		if (impactType == ImpactType::stopped || impactType == ImpactType::hit || impactType == ImpactType::destroyed) {
			delete this->bullet;
			this->bullet = nullptr;
			if (impactType == ImpactType::destroyed) {
				return true;
			}
		}
	}
	else if (0.f < this->health) {
		Ship* closestEnemyShip = nullptr;
		float closestEnemyShipDistance = 999999.f;
		for (int i = 0; i < SHIPS_PER_TEAM; i++) {
			float distanceToShip = getDistance(this->position, this->enemyShips[i].getPosition());
			if (distanceToShip <= 10000.f && distanceToShip < closestEnemyShipDistance) {
				closestEnemyShip = &this->enemyShips[i];
				closestEnemyShipDistance = distanceToShip;
			}
		}

		if (closestEnemyShip && 1000 < this->timeSinceBulletShot) {
			this->timeSinceBulletShot = 0;
			this->bullet = new Bullet();
			this->bullet->init(this->position, closestEnemyShip, TargetType::ship, 25.f);
		}
	}

	return false;
}

Base::Type Base::getType() {
	return this->type;
}

bool Base::isInsideRadius(glm::vec2 position) {
	int radius = this->type == Base::Type::planet ? 2560 : 1194;
	return getDistance(this->position, position) < radius;
}

glm::vec2 Base::getGravityAcceleration(glm::vec2 shipPosition) {
	if (this->type == Base::Type::planet && getDistance(this->position, shipPosition) < 30000.f && 0.f < this->health) {
		float accelerationMagnitude = 1000000 / pow(getDistance(this->position, shipPosition), 2);
		glm::vec2 accelerationDirection = glm::normalize(getVector(shipPosition, this->position));
		return accelerationDirection * accelerationMagnitude;
	}
	else {
		return glm::vec2(0.f, 0.f);
	}
}

glm::vec2 Base::getPosition() {
	return this->position;
}

glm::vec2 Base::getBulletPosition() {
	return this->bullet ? this->bullet->getPosition() : glm::vec2(-1.f);
}

glm::vec2 Base::getBulletVelocity() {
	return this->bullet ? this->bullet->getVelocity() : glm::vec2(-1.f);
}

void* Base::getBulletTarget() {
	return this->bullet ? this->bullet->getTarget() : nullptr;
}

bool Base::isDestroyed() {
	return this->health <= 0.f;
}

int Base::getNodeId() {
	return this->nodeId;
}

float Base::getHealthRatio() {
	return this->health / this->maxHealth;
}