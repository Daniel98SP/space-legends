#include "Ship.h"
#include "../Global.h"
#include "../Math.h"
#include "Base.h"

using namespace Math;
using namespace Global;

void Ship::init(glm::vec2 position, Team team, bool isPlayer) {

	this->attributes.engineLevel = 0;
	this->attributes.rcsLevel = 0;
	this->attributes.weaponsLevel = 0;
	this->attributes.shieldLevel = 0;
	this->attributes.antiGravityLevel = 0;
	
	this->spawnPosition = position;
	
	this->physics.position = this->spawnPosition;
	this->physics.velocity = glm::vec2(0.f, 0.f);
	this->physics.steerVelocity = 0.f;
	this->physics.angle = 0.f;

	this->status.health = this->attributes.maxHealthLevels[this->attributes.shieldLevel];
	this->status.shield = this->attributes.maxShieldLevels[this->attributes.shieldLevel];
	this->status.enginesActivated = false;
	this->status.shieldActivated = false;

	this->destination = glm::vec2(NULL, NULL);
	this->target = nullptr;
	this->targetType = TargetType::none;
	this->targetPosition = glm::vec2(NULL, NULL);
	this->bullet = nullptr;
	this->timeSinceBulletShot = 99999;
	this->accumulatedCoins = 0;
	this->wasDestroyed = false;
	this->isPlayer = isPlayer;
	this->team = team;
}

void Ship::update(int deltaTime) {
	float distanceToTarget = getDistance(this->destination, this->physics.position);
	if (this->destination.x != NULL) {
		controlShip(deltaTime);
	}
	else {
		stopShip(deltaTime);
	}

	if (distanceToTarget < 200.f && getMagnitude(this->physics.velocity) < 2.f) {
		this->destination = glm::vec2(NULL, NULL);
	}

	updatePositionAndAngle(deltaTime);
	automaticShooting(deltaTime);
	if (!isTargetInRange(this->target, this->targetType, this->targetPosition)) {
		this->target = nullptr;
		this->targetType = TargetType::none;
	}

	if (this->targetType == TargetType::station || this->targetType == TargetType::planet) {
		Base* castedTarget = (Base*)this->target;
		if (castedTarget->isDestroyed()) {
			this->target = nullptr;
			this->targetType = TargetType::none;
		}
	}
}

ImpactType Ship::damage(float damage, bool isBulletDamage) {
	if (!this->status.shieldActivated || !isBulletDamage) {
		this->status.health -= damage;

		if (this->status.health <= 0.f) {
			this->physics.position = this->spawnPosition;
			this->physics.velocity = glm::vec2(0.f, 0.f);
			this->physics.steerVelocity = 0.f;
			this->physics.angle = 0.f;
			this->status.health = this->attributes.maxHealthLevels[this->attributes.shieldLevel];
			this->status.shield = this->attributes.maxShieldLevels[this->attributes.shieldLevel];
			this->status.enginesActivated = false;
			this->destination = glm::vec2(NULL, NULL);
			this->target = nullptr;
			this->targetType = TargetType::none;
			this->timeSinceBulletShot = 99999;
			this->wasDestroyed = true;
			return ImpactType::destroyed;
		}
		else {
			return ImpactType::hit;
		}
	}
	else {
		this->status.shield = max(0.f, this->status.shield - damage * 10);
	}

	return ImpactType::stopped;
}

void Ship::activateShield(int deltaTime, bool shieldActivated) {
	if (shieldActivated && this->status.shieldActivated && 0.f < this->status.shield) {
		this->status.shieldActivated = true;
		this->status.shield = max(0.f, this->status.shield - deltaTime);
	}
	else if (shieldActivated && this->attributes.maxShieldLevels[this->attributes.shieldLevel] * 0.5 < this->status.shield) {
		this->status.shieldActivated = true;
		this->status.shield = max(0.f, this->status.shield - deltaTime);
	}
	else {
		this->status.shieldActivated = false;
		this->status.shield = min(this->status.shield + this->attributes.shieldRecoverySpeedLevels[this->attributes.shieldLevel] * deltaTime, this->attributes.maxShieldLevels[this->attributes.shieldLevel]);
	}
}

bool Ship::setTarget(void* target, TargetType targetType, glm::vec2 targetPosition) {
	if (isTargetInRange(target, targetType, targetPosition)) {
		if (this->target == target) {
			this->target = nullptr;
			this->targetType = TargetType::none;
			this->targetPosition = glm::vec2(NULL, NULL);
		}
		else {
			this->target = target;
			this->targetType = targetType;
			this->targetPosition = targetPosition;
		}
	}
	
	return this->targetType != TargetType::none;
}

void Ship::setDestination(glm::vec2 destinationPosition) {
	if (destinationPosition.x != -1) {
		this->destination = destinationPosition;
	}
}

void Ship::controlShip(int deltaTime) {
	this->status.enginesActivated = false;

	glm::vec2 shipToTargetVector = getVector(this->physics.position, this->destination);
	glm::vec2 courseCorrectionVector = getCorrectingVector(shipToTargetVector, this->physics.velocity, false);

	float velocityAngleToTarget = abs(getAngleBetweenVectors(shipToTargetVector, this->physics.velocity));
	float angleWithCorrectionVector = getAngleDifference(this->physics.angle, getAngleBetweenVectors(glm::vec2(0.f, -1.f), courseCorrectionVector));
	float angleWithTarget = getAngleDifference(this->physics.angle, getAngleBetweenVectors(glm::vec2(0.f, -1.f), shipToTargetVector));

	float distanceToTarget = getDistance(this->destination, this->physics.position);
	float stoppingDistance = (pow(getMagnitude(this->physics.velocity), 2) / (2 * this->attributes.rcsAccelerationLevels[this->attributes.rcsLevel]));
	float steerStoppingAngle = pow(this->physics.steerVelocity, 2) / (2 * this->attributes.rcsAccelerationLevels[this->attributes.rcsLevel] * 0.1);

	if (abs(angleWithTarget) <= steerStoppingAngle && angleWithTarget < 0.f == this->physics.steerVelocity < 0.f) {
		float shipToTargetVectorAngle = getAngleBetweenVectors(glm::vec2(0.f, -1.f), shipToTargetVector);
		float shipToTargetVectorOppositeAngle = shipToTargetVectorAngle < 0 ? 180.f + shipToTargetVectorAngle : -180.f + shipToTargetVectorAngle;
		accelerateShipSteer(deltaTime, shipToTargetVectorOppositeAngle);
	}
	else {
		accelerateShipSteer(deltaTime, getAngleBetweenVectors(glm::vec2(0.f, -1.f), shipToTargetVector));
	}

	if (distanceToTarget <= stoppingDistance && velocityAngleToTarget < 10.f) {
		accelerateShip(deltaTime, -getVector(this->physics.position, this->destination), this->attributes.rcsAccelerationLevels[this->attributes.rcsLevel]);
	}
	else {
		if (10.f < velocityAngleToTarget) {
			glm::vec2 courseCorrectionOnlyVector = getCorrectingVector(shipToTargetVector, this->physics.velocity, false);
			accelerateShip(deltaTime, courseCorrectionOnlyVector, this->attributes.rcsAccelerationLevels[this->attributes.rcsLevel]);
		}
		if (abs(angleWithTarget) < 15.f) {
			accelerateShip(deltaTime, courseCorrectionVector, this->attributes.enginesAccelerationLevels[this->attributes.engineLevel]);
			this->status.enginesActivated = true;
		}
	}
}

void Ship::stopShip(int deltaTime) {
	if (0.05 < getMagnitude(this->physics.velocity)) {
		accelerateShip(deltaTime, -this->physics.velocity, this->attributes.rcsAccelerationLevels[this->attributes.rcsLevel]);
	}

	if (0.05 < this->physics.steerVelocity) {
		accelerateShipSteer(deltaTime, -this->physics.steerVelocity);
	}
	else {
		this->physics.steerVelocity = 0.f;
	}
}

void Ship::upgradeShip(UpgradeType type) {
	if (type == UpgradeType::engine) {
		this->attributes.engineLevel += this->attributes.engineLevel < 4 ? 1 : 0;
	}
	else if (type == UpgradeType::rcs) {
		this->attributes.rcsLevel += this->attributes.rcsLevel < 4 ? 1 : 0;
	}
	else if (type == UpgradeType::weapon) {
		this->attributes.weaponsLevel += this->attributes.weaponsLevel < 4 ? 1 : 0;
	}
	else if (type == UpgradeType::shield) {
		this->attributes.shieldLevel += this->attributes.shieldLevel < 4 ? 1 : 0;
		this->status.health = this->status.health * this->attributes.maxHealthLevels[this->attributes.shieldLevel] / this->attributes.maxHealthLevels[this->attributes.shieldLevel - 1];
		this->status.shield = this->status.shield * this->attributes.maxShieldLevels[this->attributes.shieldLevel] / this->attributes.maxShieldLevels[this->attributes.shieldLevel - 1];
	}
	else if (type == UpgradeType::antiGravity) {
		this->attributes.antiGravityLevel += this->attributes.antiGravityLevel < 4 ? 1 : 0;
	}
}

void Ship::applyPlanetAcceleration(int deltaTime, glm::vec2 planetAcceleration) {
	this->physics.velocity += planetAcceleration * (float)deltaTime * this->attributes.antiGravityLevels[this->attributes.antiGravityLevel];
}

void Ship::automaticShooting(int deltaTime) {
	this->timeSinceBulletShot += deltaTime;

	if (this->bullet) {
		ImpactType impactType = this->bullet->update(deltaTime);
		if (impactType == ImpactType::stopped || impactType == ImpactType::hit || impactType == ImpactType::destroyed) {
			delete this->bullet;
			this->bullet = nullptr;
			if (impactType == ImpactType::hit || impactType == ImpactType::destroyed) {
				this->accumulatedCoins += this->targetType == TargetType::ship ? this->attributes.shotPowerLevels[this->attributes.weaponsLevel] : this->attributes.shotPowerLevels[this->attributes.weaponsLevel] / 5;
				if (impactType == ImpactType::destroyed) {
					this->accumulatedCoins += this->targetType == TargetType::ship ? 200 : 0;
					this->target = nullptr;
					this->targetType = TargetType::none;
				}
			}
		}
	}
	else if (this->target && this->targetType != TargetType::ship && this->attributes.bulletCooldownLevels[this->attributes.weaponsLevel] < this->timeSinceBulletShot) {
		this->timeSinceBulletShot = 0;
		this->bullet = new Bullet();
		this->bullet->init(this->physics.position, this->target, this->targetType, this->attributes.shotPowerLevels[this->attributes.weaponsLevel]);
	}
}

void Ship::manualShooting() {
	if (!this->bullet && this->target && this->targetType == TargetType::ship && this->attributes.bulletCooldownLevels[this->attributes.weaponsLevel] < this->timeSinceBulletShot) {
		this->timeSinceBulletShot = 0;
		this->bullet = new Bullet();
		this->bullet->init(this->physics.position, this->target, this->targetType, this->attributes.shotPowerLevels[this->attributes.weaponsLevel]);
	}
}

void Ship::addCoins(int coinsAmount) {
	this->accumulatedCoins += coinsAmount;
}

int Ship::retrieveCoins() {
	int coins = this->accumulatedCoins;
	this->accumulatedCoins = 0;
	return coins;
}

bool Ship::retrieveDestroyedNotice() {
	bool destroyedNotice = this->wasDestroyed;
	this->wasDestroyed = false;
	return destroyedNotice;
}

void Ship::accelerateShipSteer(int deltaTime, float targetAngle) {
	float angle = targetAngle - this->physics.angle;
	float direction = 180.f < abs(angle) ? (angle / abs(angle)) * -1 : (angle / abs(angle));
	direction = isnan(direction) ? 0.f : direction;
	this->physics.steerVelocity += direction * this->attributes.rcsAccelerationLevels[this->attributes.rcsLevel] * 0.1 * (float)deltaTime;
}

void Ship::accelerateShip(int deltaTime, glm::vec2 direction, float acceleration) {
	this->physics.velocity += glm::normalize(direction) * acceleration * (float)deltaTime;
	this->physics.velocity = this->attributes.maxSpeedLevels[this->attributes.engineLevel] < getMagnitude(this->physics.velocity) ? this->physics.velocity * this->attributes.maxSpeedLevels[this->attributes.engineLevel] / getMagnitude(this->physics.velocity) : this->physics.velocity;
}

void Ship::updatePositionAndAngle(int deltaTime) {
	this->physics.position += this->physics.velocity * (float)deltaTime;
	this->physics.angle += this->physics.steerVelocity * (float)deltaTime;
	this->physics.angle = this->physics.angle < -180.f ? 360.f + this->physics.angle : this->physics.angle;
	this->physics.angle = 180.f < this->physics.angle ? -360.f + this->physics.angle : this->physics.angle;
}

bool Ship::isTargetInRange(void* target, TargetType targetType, glm::vec2 targetPosition) {
	glm::vec2 currentTargetPosition = targetPosition;

	if (targetType == TargetType::ship) {
		Ship* castedTarget = (Ship*)target;
		currentTargetPosition = castedTarget->getPosition();
	}
	else if (targetType == TargetType::none) {
		return true;
	}

	return getDistance(this->physics.position, targetPosition) < this->attributes.targetRangeLevels[this->attributes.weaponsLevel];
}

bool Ship::isInsideRadius(glm::vec2 position) {
	return getDistance(this->physics.position, position) < 916.f / 2;
}

bool Ship::isEngineActivated() {
	return this->status.enginesActivated;
}

bool Ship::isShieldActivated() {
	return this->status.shieldActivated;
}

glm::vec2 Ship::getPosition() {
	return this->physics.position;
}

glm::vec2 Ship::getBulletPosition() {
	return this->bullet ? this->bullet->getPosition() : glm::vec2(-1.f);
}

glm::vec2 Ship::getBulletVelocity() {
	return this->bullet ? this->bullet->getVelocity() : glm::vec2(-1.f);
}

float Ship::getAngle() {
	return this->physics.angle;
}

glm::vec2 Ship::getDestination() {
	return this->destination;
}

void* Ship::getTarget() {
	return this->target;
}

TargetType Ship::getTargetType() {
	return this->targetType;
}

glm::vec2 Ship::getTargetPosition() {
	if (this->targetType == TargetType::ship) {
		Ship* castedTarget = (Ship*)this->target;
		return castedTarget->getPosition();
	}
	else {
		return this->targetPosition;
	}
}

float Ship::getTargetAngle() {
	if (this->targetType == TargetType::ship) {
		Ship* castedTarget = (Ship*)this->target;
		return castedTarget->getAngle();
	}
	else {
		return 0.f;
	}
}

float Ship::getRange() {
	return this->attributes.targetRangeLevels[this->attributes.weaponsLevel];
}

float Ship::getHealthRatio() {
	return this->status.health / this->attributes.maxHealthLevels[this->attributes.shieldLevel];
}

float Ship::getShieldRatio() {
	return this->status.shield / this->attributes.maxShieldLevels[this->attributes.shieldLevel];
}

int Ship::getLevel(UpgradeType type) {
	if (type == UpgradeType::engine) {
		return this->attributes.engineLevel;
	}
	else if (type == UpgradeType::rcs) {
		return this->attributes.rcsLevel;
	}
	else if (type == UpgradeType::weapon) {
		return this->attributes.weaponsLevel;
	}
	else if (type == UpgradeType::shield) {
		return this->attributes.shieldLevel;
	}
	else if (type == UpgradeType::antiGravity) {
		return this->attributes.antiGravityLevel;
	}
}

Team Ship::getTeam() {
	return this->team;
}