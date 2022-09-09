#include <GL/glew.h>
#include <GL/glut.h>
#include <iostream>
#include <fstream>
#include <sstream>
#include "../Global.h"
#include "Match.h"

using namespace Global;

void Match::init() {
	this->blueShips[0].init(glm::vec2(11000.f, MAP_SIZE_Y - 15500.f), Team::blue, true);
	this->blueShips[1].init(glm::vec2(13000.f, MAP_SIZE_Y - 16000.f), Team::blue);
	this->blueShips[2].init(glm::vec2(15000.f, MAP_SIZE_Y - 15000.f), Team::blue);
	this->blueShips[3].init(glm::vec2(16000.f, MAP_SIZE_Y - 13000.f), Team::blue);
	this->blueShips[4].init(glm::vec2(15500.f, MAP_SIZE_Y - 11000.f), Team::blue);
	this->orangeShips[0].init(glm::vec2(MAP_SIZE_X - 15500.f, 11000.f), Team::orange);
	this->orangeShips[1].init(glm::vec2(MAP_SIZE_X - 16000.f, 13000.f), Team::orange);
	this->orangeShips[2].init(glm::vec2(MAP_SIZE_X - 15000.f, 15000.f), Team::orange);
	this->orangeShips[3].init(glm::vec2(MAP_SIZE_X - 13000.f, 16000.f), Team::orange);
	this->orangeShips[4].init(glm::vec2(MAP_SIZE_X - 11000.f, 15500.f), Team::orange);

	this->bluePlanet.init(Base::Type::planet, Team::blue, 17, bluePlanetPosition, this->blueShips, this->orangeShips, 5000.f);
	this->orangePlanet.init(Base::Type::planet, Team::orange, 5, orangePlanetPosition, this->blueShips, this->orangeShips, 5000.f);

	this->blueStations[0].init(Base::Type::station, Team::blue, 8, blueStationsPositions[0], this->blueShips, this->orangeShips, 1000.f);
	this->blueStations[1].init(Base::Type::station, Team::blue, 18, blueStationsPositions[1], this->blueShips, this->orangeShips, 1000.f);
	this->blueStations[2].init(Base::Type::station, Team::blue, 0, blueStationsPositions[2], this->blueShips, this->orangeShips, 500.f);
	this->blueStations[3].init(Base::Type::station, Team::blue, 10, blueStationsPositions[3], this->blueShips, this->orangeShips, 500.f);
	this->blueStations[4].init(Base::Type::station, Team::blue, 20, blueStationsPositions[4], this->blueShips, this->orangeShips, 500.f);
	this->orangeStations[0].init(Base::Type::station, Team::orange, 14, orangeStationsPositions[0], this->blueShips, this->orangeShips, 1000.f);
	this->orangeStations[1].init(Base::Type::station, Team::orange, 4, orangeStationsPositions[1], this->blueShips, this->orangeShips, 1000.f);
	this->orangeStations[2].init(Base::Type::station, Team::orange, 22, orangeStationsPositions[2], this->blueShips, this->orangeShips, 500.f);
	this->orangeStations[3].init(Base::Type::station, Team::orange, 12, orangeStationsPositions[3], this->blueShips, this->orangeShips, 500.f);
	this->orangeStations[4].init(Base::Type::station, Team::orange, 2, orangeStationsPositions[4], this->blueShips, this->orangeShips, 500.f);

	this->player.init(&this->blueShips[0], Team::blue, &this->bluePlanet, &this->orangePlanet, this->blueStations, this->orangeStations, this->blueShips, this->orangeShips);
	this->blueBots[0].init(&this->blueShips[1], Team::blue, &this->bluePlanet, &this->orangePlanet, this->blueStations, this->orangeStations, this->blueShips, this->orangeShips);
	this->blueBots[1].init(&this->blueShips[2], Team::blue, &this->bluePlanet, &this->orangePlanet, this->blueStations, this->orangeStations, this->blueShips, this->orangeShips);
	this->blueBots[2].init(&this->blueShips[3], Team::blue, &this->bluePlanet, &this->orangePlanet, this->blueStations, this->orangeStations, this->blueShips, this->orangeShips);
	this->blueBots[3].init(&this->blueShips[4], Team::blue, &this->bluePlanet, &this->orangePlanet, this->blueStations, this->orangeStations, this->blueShips, this->orangeShips);
	this->orangeBots[0].init(&this->orangeShips[0], Team::orange, &this->orangePlanet, &this->bluePlanet, this->orangeStations, this->blueStations, this->orangeShips, this->blueShips);
	this->orangeBots[1].init(&this->orangeShips[1], Team::orange, &this->orangePlanet, &this->bluePlanet, this->orangeStations, this->blueStations, this->orangeShips, this->blueShips);
	this->orangeBots[2].init(&this->orangeShips[2], Team::orange, &this->orangePlanet, &this->bluePlanet, this->orangeStations, this->blueStations, this->orangeShips, this->blueShips);
	this->orangeBots[3].init(&this->orangeShips[3], Team::orange, &this->orangePlanet, &this->bluePlanet, this->orangeStations, this->blueStations, this->orangeShips, this->blueShips);
	this->orangeBots[4].init(&this->orangeShips[4], Team::orange, &this->orangePlanet, &this->bluePlanet, this->orangeStations, this->blueStations, this->orangeShips, this->blueShips);

	int i = 0;
	ifstream is("server/data/asteroids_positions_2.txt");
	string line;
	while (getline(is, line)) {
		istringstream is(line);
		float positionX;
		float positionY;
		is >> positionX;
		is >> positionY;
		this->asteroids[i].init(glm::vec2(positionX + (rand() % 1000) -250.f, positionY + (rand() % 1000) - 250.f));
		i++;
	}

	this->destroyedBlueStations = 0;
	this->destroyedOrangeStations = 0;

	this->timeBlueVictory = -1;
	this->timeOrangeVictory = -1;
}

bool Match::update(int matchElapsedTime, int deltaTime) {
	if (this->bluePlanet.isDestroyed() && this->timeBlueVictory == -1 && this->timeOrangeVictory == -1) {
		this->timeOrangeVictory = matchElapsedTime;
	}
	else if (this->orangePlanet.isDestroyed() && this->timeBlueVictory == -1 && this->timeOrangeVictory == -1) {
		this->timeBlueVictory = matchElapsedTime;
	}

	if (this->timeBlueVictory == -1 && this->timeOrangeVictory == -1) {
		for (int i = 0; i < SHIPS_PER_TEAM; i++) {
			checkShipCollisions(&this->blueShips[i]);
			checkShipCollisions(&this->orangeShips[i]);

			this->blueShips[i].applyPlanetAcceleration(deltaTime, this->orangePlanet.getGravityAcceleration(this->blueShips[i].getPosition()));
			this->orangeShips[i].applyPlanetAcceleration(deltaTime, this->bluePlanet.getGravityAcceleration(this->orangeShips[i].getPosition()));

			this->blueShips[i].update(deltaTime);
			this->orangeShips[i].update(deltaTime);
		}

		for (int i = 0; i < size(this->blueBots); i++) {
			this->blueBots[i].decide(deltaTime);
		}
		for (int i = 0; i < size(this->orangeBots); i++) {
			this->orangeBots[i].decide(deltaTime);
		}
		this->player.decide(deltaTime);

		this->bluePlanet.update(deltaTime);
		this->orangePlanet.update(deltaTime);
		for (int i = 0; i < STATIONS_PER_TEAM; i++) {
			bool orangeShipDestroyed = this->blueStations[i].update(deltaTime);
			if (orangeShipDestroyed) {
				for (int i = 0; i < SHIPS_PER_TEAM; i++) {
					this->blueShips[i].addCoins(200 / SHIPS_PER_TEAM);
				}
			}

			bool blueShipDestroyed = this->orangeStations[i].update(deltaTime);
			if (blueShipDestroyed) {
				for (int i = 0; i < SHIPS_PER_TEAM; i++) {
					this->orangeShips[i].addCoins(200 / SHIPS_PER_TEAM);
				}
			}
		}

		int currentDestroyedBlueStations = 0;
		int currentDestroyedOrangeStations = 0;
		for (int i = 0; i < STATIONS_PER_TEAM; i++) {
			if (this->blueStations[i].isDestroyed()) {
				currentDestroyedBlueStations++;
			}
			if (this->orangeStations[i].isDestroyed()) {
				currentDestroyedOrangeStations++;
			}
		}
		for (int i = 0; i < SHIPS_PER_TEAM; i++) {
			this->blueShips[i].addCoins((1000 * (currentDestroyedOrangeStations - this->destroyedOrangeStations)) / SHIPS_PER_TEAM);
			this->orangeShips[i].addCoins((1000 * (currentDestroyedBlueStations - this->destroyedBlueStations)) / SHIPS_PER_TEAM);
		}
		this->destroyedBlueStations = currentDestroyedBlueStations;
		this->destroyedOrangeStations = currentDestroyedOrangeStations;

		return true;
	}
	else {
		if (this->timeBlueVictory != -1 && matchElapsedTime - this->timeBlueVictory < 10000 || this->timeOrangeVictory != -1 && matchElapsedTime - this->timeOrangeVictory < 10000) {
			return true;
		}
		else {
			return false;
		}
	}
}

void Match::checkShipCollisions(Ship* ship) {
	for (int i = 0; i < size(this->asteroids); i++) {
		if (this->asteroids[i].isInsideRadius(ship->getPosition())) {
			ship->damage(99999.f, false);
			for (int i = 0; i < SHIPS_PER_TEAM; i++) {
				if (ship->getTeam() == Team::blue) {
					this->orangeShips[i].addCoins(200 / SHIPS_PER_TEAM);
				}
				else {
					this->blueShips[i].addCoins(200 / SHIPS_PER_TEAM);
				}
			}
		}
	}
	if (this->bluePlanet.isInsideRadius(ship->getPosition()) || this->orangePlanet.isInsideRadius(ship->getPosition())) {
		ship->damage(99999.f, false);
		for (int i = 0; i < SHIPS_PER_TEAM; i++) {
			if (ship->getTeam() == Team::blue) {
				this->orangeShips[i].addCoins(200 / SHIPS_PER_TEAM);
			}
			else {
				this->blueShips[i].addCoins(200 / SHIPS_PER_TEAM);
			}
		}
	}
}

PlayerRegistrationData Match::registerPlayer() {
	PlayerRegistrationData playerRegistrationData{};

	playerRegistrationData.playerId = 0;
	playerRegistrationData.playerShipId = 0;
	playerRegistrationData.playerTeam = Team::blue;

	return playerRegistrationData;
}

MatchInitialData Match::getMatchInitialData() {
	MatchInitialData matchInitialData{};
	for (int i = 0; i < size(this->asteroids); i++) {
		matchInitialData.asteroidsPositions[i] = this->asteroids[i].getPosition();
		matchInitialData.asteroidsScales[i] = this->asteroids[i].getScale();
	}

	return matchInitialData;
}

MatchData Match::getMatchData(int playerId) {
	MatchData matchData{};

	// Match status info
	matchData.matchFinished = this->timeBlueVictory != -1 || this->timeOrangeVictory != -1;

	// Bases health info
	matchData.basesHealthRatio[0] = this->bluePlanet.getHealthRatio();
	matchData.basesHealthRatio[6] = this->orangePlanet.getHealthRatio();
	for (int i = 0; i < STATIONS_PER_TEAM; i++) {
		matchData.basesHealthRatio[i + 1] = this->blueStations[i].getHealthRatio();
		matchData.basesHealthRatio[i + 7] = this->orangeStations[i].getHealthRatio();
	}

	// Bullets info
	for (int i = 0; i < STATIONS_PER_TEAM; i++) {
		matchData.bulletsPositions[i] = this->blueStations[i].getBulletPosition();
		matchData.bulletsPositions[i+5] = this->orangeStations[i].getBulletPosition();

		matchData.bulletsVelocities[i] = this->blueStations[i].getBulletVelocity();
		matchData.bulletsVelocities[i+5] = this->orangeStations[i].getBulletVelocity();
	}
	for (int i = 0; i < SHIPS_PER_TEAM; i++) {
		matchData.bulletsPositions[i+10] = this->blueShips[i].getBulletPosition();
		matchData.bulletsPositions[i+15] = this->orangeShips[i].getBulletPosition();

		matchData.bulletsVelocities[i+10] = this->blueShips[i].getBulletVelocity();
		matchData.bulletsVelocities[i+15] = this->orangeShips[i].getBulletVelocity();
	}

	// Ships info
	for (int i = 0; i < SHIPS_PER_TEAM; i++) {
		matchData.shipsPositions[i] = this->blueShips[i].getPosition();
		matchData.shipsPositions[i+5] = this->orangeShips[i].getPosition();

		matchData.shipsAngles[i] = this->blueShips[i].getAngle();
		matchData.shipsAngles[i+5] = this->orangeShips[i].getAngle();

		matchData.shipsEnginesActivated[i] = this->blueShips[i].isEngineActivated();
		matchData.shipsEnginesActivated[i + 5] = this->orangeShips[i].isEngineActivated();

		matchData.shipsShieldsActivated[i] = this->blueShips[i].isShieldActivated();
		matchData.shipsShieldsActivated[i + 5] = this->orangeShips[i].isShieldActivated();

		matchData.shipsHealthRatio[i] = this->blueShips[i].getHealthRatio();
		matchData.shipsHealthRatio[i + 5] = this->orangeShips[i].getHealthRatio();
	}

	// Player info
	matchData.playerDestination = this->player.getShip()->getDestination();
	matchData.playerSelectionType = this->player.getShip()->getTargetType();
	matchData.playerSelectionPosition = this->player.getShip()->getTargetPosition();
	matchData.playerSelectionAngle = this->player.getShip()->getTargetAngle();
	matchData.playerShieldRatio = this->player.getShip()->getShieldRatio();
	matchData.playerEngineLevel = this->player.getShip()->getLevel(UpgradeType::engine);
	matchData.playerRcsLevel = this->player.getShip()->getLevel(UpgradeType::rcs);
	matchData.playerWeaponsLevel = this->player.getShip()->getLevel(UpgradeType::weapon);
	matchData.playerShieldLevel = this->player.getShip()->getLevel(UpgradeType::shield);
	matchData.playerAntiGravityLevel = this->player.getShip()->getLevel(UpgradeType::antiGravity);
	matchData.playerCoins = this->player.getCoins();

	return matchData;
}

void Match::setPlayerInput(int playerId, glm::vec2 leftClickPosition, glm::vec2 rightClickPosition, bool spacePressed, bool shiftPressed, UpgradeType upgradeType) {
	this->player.setInput(leftClickPosition, rightClickPosition, spacePressed, shiftPressed, upgradeType);
}