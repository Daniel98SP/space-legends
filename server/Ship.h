#pragma once

#include "Bullet.h"

class Ship {
public:
	void init(glm::vec2 position, Team team, bool isPlayer = false);
	void update(int deltaTime);

	void upgradeShip(UpgradeType type);
	void applyPlanetAcceleration(int deltaTime, glm::vec2 planetAcceleration);
	bool setTarget(void* target, TargetType targetType, glm::vec2 targetPosition);
	void setDestination(glm::vec2 destinationPosition);
	void manualShooting();
	ImpactType damage(float damage, bool isBulletDamage);
	void activateShield(int deltaTime, bool shieldActivated);
	void addCoins(int coinsAmount);
	int retrieveCoins();
	bool retrieveDestroyedNotice();

	bool isInsideRadius(glm::vec2 position);
	bool isEngineActivated();
	bool isShieldActivated();
	glm::vec2 getPosition();
	glm::vec2 getBulletPosition();
	glm::vec2 getBulletVelocity();
	float getAngle();
	glm::vec2 getDestination();
	void* getTarget();
	TargetType getTargetType();
	glm::vec2 getTargetPosition();
	float getTargetAngle();
	float getRange();
	float getHealthRatio();
	float getShieldRatio();
	int getLevel(UpgradeType type);
	Team getTeam();

private:
	void controlShip(int deltaTime);
	void stopShip(int deltaTime);
	void automaticShooting(int deltaTime);

	void accelerateShipSteer(int deltaTime, float targetAngle);
	void accelerateShip(int deltaTime, glm::vec2 direction, float acceleration);
	void updatePositionAndAngle(int deltaTime);

	bool isTargetInRange(void* target, TargetType targetType, glm::vec2 targetPosition);

	struct Attributes {
		float maxSpeedLevels[5] = { 2.f, 2.5f, 3.f, 3.5f, 4.f };
		float enginesAccelerationLevels[5] = { 0.008f, 0.016f, 0.024f, 0.032f, 0.040f };
		float rcsAccelerationLevels[5] = { 0.0005f, 0.0015f, 0.0025f, 0.0035f, 0.0045f };
		float targetRangeLevels[5] = { 5000.f, 7500.f, 10000.f, 12500.f, 15000.f };
		float shotPowerLevels[5] = { 5.f, 10.f, 15.f, 20.f, 25.f };
		float bulletCooldownLevels[5] = { 3000.f, 2500.f, 2000.f, 1500.f, 1000.f };
		float maxShieldLevels[5] = { 5000.f, 6250.f, 7500.f, 8750.f, 10000.f };
		float shieldRecoverySpeedLevels[5] = { 0.25f, 0.3125f, 0.375f, 0.4375f, 0.5f };
		float maxHealthLevels[5] = { 100.f, 200.f, 300.f, 400.f, 500.f };
		float antiGravityLevels[5] = { 1.f, 0.5f, 0.25f, 0.125f, 0.f };

		int engineLevel;
		int rcsLevel;
		int weaponsLevel;
		int shieldLevel;
		int antiGravityLevel;
	} attributes;

	struct Physics {
		glm::vec2 position;
		glm::vec2 velocity;
		float angle;
		float steerVelocity;
	} physics;

	struct Status {
		float health;
		float shield;
		bool enginesActivated;
		bool shieldActivated;
	} status;

	glm::vec2 spawnPosition;
	glm::vec2 destination;

	void* target;
	TargetType targetType;
	glm::vec2 targetPosition;

	Bullet* bullet;
	int timeSinceBulletShot;

	int accumulatedCoins;
	bool wasDestroyed;

	bool isPlayer;
	Team team;
};
