#include "Player.h"
#include "../Math.h"

using namespace Math;

void Player::init(Ship* playerShip, Team playerTeam, Base* allyPlanet, Base* enemyPlanet, Base* allyStations, Base* enemyStations, Ship* allyShips, Ship* enemyShips) {
	this->playerShip = playerShip;
	this->playerTeam = playerTeam;
	this->allyPlanet = allyPlanet;
	this->enemyPlanet = enemyPlanet;
	this->allyStations = allyStations;
	this->enemyStations = enemyStations;
	this->allyShips = allyShips;
	this->enemyShips = enemyShips;

	this->targetPosition = glm::vec2(NULL, NULL);
	this->targetType = TargetType::none;

	this->coins = 50;

	this->leftClickPosition = glm::vec2(-1.f, -1.f);
	this->rightClickPosition = glm::vec2(-1.f, -1.f);
	this->spacePressed = false;
	this->shiftPressed = false;
	this->upgradeType = UpgradeType::none;
}

void Player::decide(int deltaTime) {
	if (!this->playerShip->getTarget()) {
		this->targetPosition = glm::vec2(NULL, NULL);
		this->targetType = TargetType::none;
	}

	this->playerShip->setDestination(this->leftClickPosition);

	manageRightClick(this->rightClickPosition);

	if (this->spacePressed) {
		this->playerShip->manualShooting();
	}

	this->playerShip->activateShield(deltaTime, this->shiftPressed);

	this->upgradeShip(this->upgradeType);

	this->coins += this->playerShip->retrieveCoins();

	this->leftClickPosition = glm::vec2(-1.f, -1.f);
	this->rightClickPosition = glm::vec2(-1.f, -1.f);
	this->spacePressed = false;
	this->shiftPressed = false;
	this->upgradeType = UpgradeType::none;
}

void Player::setInput(glm::vec2 leftClickPosition, glm::vec2 rightClickPosition, bool spacePressed, bool shiftPressed, UpgradeType upgradeType) {
	this->leftClickPosition = leftClickPosition;
	this->rightClickPosition = rightClickPosition;
	this->spacePressed = spacePressed;
	this->shiftPressed = shiftPressed;
	this->upgradeType = upgradeType;
}

void Player::upgradeShip(UpgradeType type) {
	if (type == UpgradeType::engine && engineUpgradePrices[this->playerShip->getLevel(type)] <= this->coins && this->playerShip->getLevel(type) < 4) {
		this->coins -= engineUpgradePrices[this->playerShip->getLevel(type)];
		this->playerShip->upgradeShip(type);
	}
	else if (type == UpgradeType::rcs && rcsUpgradePrices[this->playerShip->getLevel(type)] <= this->coins && this->playerShip->getLevel(type) < 4) {
		this->coins -= rcsUpgradePrices[this->playerShip->getLevel(type)];
		this->playerShip->upgradeShip(type);
	}
	else if (type == UpgradeType::weapon && weaponUpgradePrices[this->playerShip->getLevel(type)] <= this->coins && this->playerShip->getLevel(type) < 4) {
		this->coins -= weaponUpgradePrices[this->playerShip->getLevel(type)];
		this->playerShip->upgradeShip(type);
	}
	else if (type == UpgradeType::shield && shieldUpgradePrices[this->playerShip->getLevel(type)] <= this->coins && this->playerShip->getLevel(type) < 4) {
		this->coins -= shieldUpgradePrices[this->playerShip->getLevel(type)];
		this->playerShip->upgradeShip(type);
	}
	else if (type == UpgradeType::antiGravity && antiGravityUpgradePrices[this->playerShip->getLevel(type)] <= this->coins && this->playerShip->getLevel(type) < 4) {
		this->coins -= antiGravityUpgradePrices[this->playerShip->getLevel(type)];
		this->playerShip->upgradeShip(type);
	}
}

void Player::manageRightClick(glm::vec2 rightClickPosition) {
	if (rightClickPosition.x != -1) {
		this->targetType = TargetType::none;

		if (this->enemyPlanet->isInsideRadius(rightClickPosition)) {
			bool targetSet = this->playerShip->setTarget(this->enemyPlanet, TargetType::planet, this->enemyPlanet->getPosition());
			if (targetSet) {
				this->targetType = TargetType::planet;
				this->targetPosition = this->enemyPlanet->getPosition();
			}
		}
		for (int i = 0; i < STATIONS_PER_TEAM; i++) {
			if (this->enemyStations[i].isInsideRadius(rightClickPosition) && !this->enemyStations[i].isDestroyed()) {
				bool targetSet = this->playerShip->setTarget(&this->enemyStations[i], TargetType::station, this->enemyStations[i].getPosition());
				if (targetSet) {
					this->targetType = TargetType::station;
					this->targetPosition = this->enemyStations[i].getPosition();
				}
				break;
			}
		}
		for (int i = 0; i < SHIPS_PER_TEAM; i++) {
			if (this->enemyShips[i].isInsideRadius(rightClickPosition)) {
				bool targetSet = this->playerShip->setTarget(&this->enemyShips[i], TargetType::ship, this->enemyShips[i].getPosition());
				if (targetSet) {
					this->targetType = TargetType::ship;
					this->targetPosition = this->enemyShips[i].getPosition();
				}
				break;
			}
		}
		if (this->targetType == TargetType::none) {
			this->playerShip->setTarget(nullptr, TargetType::none, glm::vec2(NULL, NULL));
		}
	}
}

Ship* Player::getShip() {
	return this->playerShip;
}

Team Player::getTeam() {
	return this->playerTeam;
}

int Player::getCoins() {
	return this->coins;
}