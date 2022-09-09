#pragma once

#include "Ship.h"
#include "../Global.h"

using namespace Global;

class Base {
public:
	enum Type { none, planet, station };

	void init(Type type, Team team, int nodeId, glm::vec2 position, Ship* blueShips, Ship* orangeShips, float health);
	bool update(int deltaTime);

	ImpactType damage(float damage);

	Type getType();
	bool isInsideRadius(glm::vec2 position);
	glm::vec2 getGravityAcceleration(glm::vec2 shipPosition);
	glm::vec2 getPosition();
	glm::vec2 getBulletPosition();
	glm::vec2 getBulletVelocity();
	void* getBulletTarget();
	bool isDestroyed();
	int getNodeId();
	float getHealthRatio();

private:
	bool automaticShooting(int deltaTime);

	int nodeId;
	glm::vec2 position;
	float maxHealth;
	float health;
	Type type;
	
	Ship* enemyShips;
	Bullet* bullet;
	int timeSinceBulletShot;
};
