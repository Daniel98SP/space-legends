#include "Bot.h"
#include "../Math.h"
#include "../Global.h"
#include <vector>

using namespace std;
using namespace Math;

Bot::Node Bot::nodes[23] = {
	{glm::vec2(10000.f, 10000.f), {1, 8, -1, -1}},
	{glm::vec2(25000.f, 10000.f), {0, 2, 6, -1}},
	{glm::vec2(40000.f, 10000.f), {1, 3, 6, -1}},
	{glm::vec2(55000.f, 10000.f), {2, 4, 7, -1}},
	{glm::vec2(70000.f, 10000.f), {3, 5, 7, -1}},
	{glm::vec2(105000.f, 15000.f), {4, 14, -1, -1}},

	{glm::vec2(40000.f, 25000.f), {1, 2, 10, 11}},
	{glm::vec2(70000.f, 25000.f), {3, 4, 12, 13}},

	{glm::vec2(10000.f, 40000.f), {0, 9, 17, -1}},
	{glm::vec2(25000.f, 40000.f), {8, 10, 15, -1}},
	{glm::vec2(40000.f, 40000.f), {6, 9, 11, 15}},
	{glm::vec2(55000.f, 40000.f), {6, 10, 12, 16}},
	{glm::vec2(70000.f, 40000.f), {7, 11, 13, 16}},
	{glm::vec2(85000.f, 40000.f), {7, 12, 14, -1}},
	{glm::vec2(100000.f, 40000.f), {5, 13, 22, -1}},

	{glm::vec2(40000.f, 55000.f), {9, 10, 18, 19}},
	{glm::vec2(70000.f, 55000.f), {11, 12, 20, 21}},

	{glm::vec2(15000.f, 65000.f), {8, 18, -1, -1}},
	{glm::vec2(40000.f, 70000.f), {15, 17, 19, -1}},
	{glm::vec2(55000.f, 70000.f), {15, 18, 20, -1}},
	{glm::vec2(70000.f, 70000.f), {16, 19, 21, -1}},
	{glm::vec2(85000.f, 70000.f), {16, 20, 22, -1}},
	{glm::vec2(100000.f, 70000.f), {14, 21, -1, -1}}
};

void Bot::initClass() {
	Bot::nodes[0].position = glm::vec2(10000.f, 10000.f);
}

void Bot::init(Ship* ship, Team botTeam, Base* allyPlanet, Base* enemyPlanet, Base* allyStations, Base* enemyStations, Ship* allyShips, Ship* enemyShips) {
	this->ship = ship;
	this->team = botTeam;
	this->allyPlanet = allyPlanet;
	this->enemyPlanet = enemyPlanet;
	this->allyStations = allyStations;
	this->enemyStations = enemyStations;
	this->allyShips = allyShips;
	this->enemyShips = enemyShips;
	this->objective = nullptr;

	this->coins = 50;
	this->currentNodeId = botTeam == Team::blue ? 17 : 5;
}

void Bot::decide(int deltaTime) {
	if (this->ship->retrieveDestroyedNotice()) {
		this->currentNodeId = this->team == Team::blue ? 17 : 5;
		this->objective = nullptr;
	}

	if (!this->objective || this->objective->isDestroyed()) {
		setNewObjective();
	}

	if (15000.f < getDistance(this->ship->getPosition(), this->objective->getPosition()) && (this->ship->getDestination().x == NULL || getDistance(this->ship->getPosition(), this->ship->getDestination()) < 5000.f)) {
		if (!this->destinations.empty()) {
			this->ship->setDestination(glm::vec2(Bot::nodes[this->destinations.top()].position.x + (rand() % 5000) - 2500.f, Bot::nodes[this->destinations.top()].position.y + (rand() % 5000) - 2500.f));
			this->currentNodeId = this->destinations.top();
			this->destinations.pop();
		}
	}

	if (getDistance(this->ship->getPosition(), this->objective->getPosition()) < this->ship->getRange()) {
		this->ship->setTarget(this->objective, TargetType::station, this->objective->getPosition());
	}
	else {
		for (int i = 0; i < SHIPS_PER_TEAM; i++) {
			if (getDistance(this->ship->getPosition(), enemyShips[i].getPosition()) < this->ship->getRange()) {
				this->ship->setTarget(&enemyShips[i], TargetType::ship, enemyShips[i].getPosition());
			}
		}
	}

	bool shieldActivated = false;
	for (int i = 0; i < SHIPS_PER_TEAM; i++) {
		if (!shieldActivated && this->ship == enemyShips[i].getTarget() && getDistance(this->ship->getPosition(), enemyShips[i].getBulletPosition()) < 4000.f) {
			shieldActivated = true;
			break;
		}
	}
	for (int i = 0; i < STATIONS_PER_TEAM; i++) {
		if (!shieldActivated && this->ship == enemyStations[i].getBulletTarget() && getDistance(this->ship->getPosition(), enemyStations[i].getBulletPosition()) < 4000.f) {
			shieldActivated = true;
			break;
		}
	}
	this->ship->activateShield(deltaTime, shieldActivated);


	this->ship->manualShooting();

	this->coins += this->ship->retrieveCoins();

	vector<UpgradeType> availableUpgrades(0);

	if (engineUpgradePrices[this->ship->getLevel(UpgradeType::engine)] <= this->coins && this->ship->getLevel(UpgradeType::engine) < 4) {
		availableUpgrades.push_back(UpgradeType::engine);
	}
	if (rcsUpgradePrices[this->ship->getLevel(UpgradeType::rcs)] <= this->coins && this->ship->getLevel(UpgradeType::rcs) < 4) {
		availableUpgrades.push_back(UpgradeType::rcs);
	}
	if (weaponUpgradePrices[this->ship->getLevel(UpgradeType::weapon)] <= this->coins && this->ship->getLevel(UpgradeType::weapon) < 4) {
		availableUpgrades.push_back(UpgradeType::weapon);
	}
	if (shieldUpgradePrices[this->ship->getLevel(UpgradeType::shield)] <= this->coins && this->ship->getLevel(UpgradeType::shield) < 4) {
		availableUpgrades.push_back(UpgradeType::shield);
	}
	if (antiGravityUpgradePrices[this->ship->getLevel(UpgradeType::antiGravity)] <= this->coins && this->ship->getLevel(UpgradeType::antiGravity) < 4) {
		availableUpgrades.push_back(UpgradeType::antiGravity);
	}

	if (!availableUpgrades.empty()) {
		int randomUpgradeIndex = rand() % availableUpgrades.size();

		if (availableUpgrades[randomUpgradeIndex] == UpgradeType::engine) {
			this->coins -= engineUpgradePrices[this->ship->getLevel(UpgradeType::engine)];
			this->ship->upgradeShip(UpgradeType::engine);
		}
		else if (availableUpgrades[randomUpgradeIndex] == UpgradeType::rcs) {
			this->coins -= rcsUpgradePrices[this->ship->getLevel(UpgradeType::rcs)];
			this->ship->upgradeShip(UpgradeType::rcs);
		}
		else if (availableUpgrades[randomUpgradeIndex] == UpgradeType::weapon) {
			this->coins -= weaponUpgradePrices[this->ship->getLevel(UpgradeType::weapon)];
			this->ship->upgradeShip(UpgradeType::weapon);
		}
		else if (availableUpgrades[randomUpgradeIndex] == UpgradeType::shield) {
			this->coins -= shieldUpgradePrices[this->ship->getLevel(UpgradeType::shield)];
			this->ship->upgradeShip(UpgradeType::shield);
		}
		else if (availableUpgrades[randomUpgradeIndex] == UpgradeType::antiGravity) {
			this->coins -= antiGravityUpgradePrices[this->ship->getLevel(UpgradeType::antiGravity)];
			this->ship->upgradeShip(UpgradeType::antiGravity);
		}
	}
}

void Bot::setNewObjective() {
	vector<Base*> aliveEnemySecondLayerStations(0);
	for (int i = 2; i < STATIONS_PER_TEAM; i++) {
		if (!this->enemyStations[i].isDestroyed()) {
			aliveEnemySecondLayerStations.push_back(&this->enemyStations[i]);
		}
	}
	vector<Base*> aliveEnemyFirstLayerStations(0);
	for (int i = 0; i < STATIONS_PER_TEAM - 3; i++) {
		if (!this->enemyStations[i].isDestroyed()) {
			aliveEnemyFirstLayerStations.push_back(&this->enemyStations[i]);
		}
	}

	if (0 < aliveEnemySecondLayerStations.size()) {
		this->objective = aliveEnemySecondLayerStations[rand() % aliveEnemySecondLayerStations.size()];
	}
	else if (0 < aliveEnemyFirstLayerStations.size()) {
		this->objective = aliveEnemyFirstLayerStations[rand() % aliveEnemyFirstLayerStations.size()];
	}
	else {
		this->objective = this->enemyPlanet;
	}

	calculateShortestPath();
	calculateDestinationStack();
}

void Bot::calculateShortestPath() {
	std::fill(begin(this->distances), end(this->distances), 999999.f);
	distances[this->currentNodeId] = 0.f;
	bool visited[23] = { false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false };

	int currentNode = this->currentNodeId;
	float currentDistance = 0.f;
	while (currentNode != -1) {
		visited[currentNode] = true;
		for (int adjacentNode : Bot::nodes[currentNode].adjacentNodes) {
			if (adjacentNode != -1) {
				distances[adjacentNode] = min(distances[adjacentNode], distances[currentNode] + getDistance(nodes[currentNode].position, nodes[adjacentNode].position));
			}
		}

		currentNode = -1;
		currentDistance = 999999.f;
		for (int i = 0; i < 23; i++) {
			if (!visited[i] && distances[i] < currentDistance) {
				currentNode = i;
				currentDistance = distances[i];
			}
		}
	}
}

void Bot::calculateDestinationStack() {
	this->destinations.push(this->objective->getNodeId());

	while (this->destinations.top() != this->currentNodeId) {
		int minimumDistanceNodeId = -1;
		float distance = 999999.f;
		for (int adjacentNodeId : Bot::nodes[this->destinations.top()].adjacentNodes) {
			if (adjacentNodeId != -1 && this->distances[adjacentNodeId] + getDistance(Bot::nodes[this->destinations.top()].position, Bot::nodes[adjacentNodeId].position) < distance) {
				minimumDistanceNodeId = adjacentNodeId;
				distance = this->distances[adjacentNodeId] + getDistance(Bot::nodes[this->destinations.top()].position, Bot::nodes[adjacentNodeId].position);
			}
		}
		this->destinations.push(minimumDistanceNodeId);
	}

	this->destinations.pop();

	if (!this->destinations.empty()) {
		this->ship->setDestination(glm::vec2(Bot::nodes[this->destinations.top()].position.x + (rand() % 5000) - 2500.f, Bot::nodes[this->destinations.top()].position.y + (rand() % 5000) - 2500.f));
		this->currentNodeId = this->destinations.top();
		this->destinations.pop();
	}
}

glm::vec2 Bot::getPosition() {
	return this->ship->getPosition();
}

Ship* Bot::getShip() {
	return this->ship;
}