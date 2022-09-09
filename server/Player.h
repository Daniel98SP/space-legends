#pragma once

#include "Ship.h"
#include "Base.h"
#include "../Global.h"

using namespace Global;

class Player {
public:
	void init(Ship* playerShip, Team playerTeam, Base* allyPlanet, Base* enemyPlanet, Base* allyStations, Base* enemyStations, Ship* allyShips, Ship* enemyShips);
	void decide(int deltaTime);
	void setInput(glm::vec2 leftClickPosition, glm::vec2 rightClickPosition, bool spacePressed, bool shiftPressed, UpgradeType upgradeType);
	void upgradeShip(UpgradeType type);

	Ship* getShip();
	Team getTeam();
	int getCoins();
private:
	void manageRightClick(glm::vec2 rightClickPosition);

	Ship* playerShip;
	Team playerTeam;
	Base* allyPlanet;
	Base* enemyPlanet;
	Base* allyStations;
	Base* enemyStations;
	Ship* allyShips;
	Ship* enemyShips;

	TargetType targetType;
	glm::vec2 targetPosition;

	int coins;

	glm::vec2 leftClickPosition;
	glm::vec2 rightClickPosition;
	bool spacePressed;
	bool shiftPressed;
	UpgradeType upgradeType;
};