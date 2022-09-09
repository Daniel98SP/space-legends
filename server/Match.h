#pragma once

#include <glm/gtc/matrix_transform.hpp>
#include "Player.h"
#include "Bot.h"
#include "Ship.h"
#include "Base.h"
#include "Asteroid.h"

class Match {
public:
	void init();
	bool update(int matchElapsedTime, int deltaTime);
	void setPlayerInput(int playerId, glm::vec2 leftClickPosition, glm::vec2 rightClickPosition, bool spacePressed, bool shiftPressed, UpgradeType upgradeType);
	PlayerRegistrationData registerPlayer();
	MatchInitialData getMatchInitialData();
	MatchData getMatchData(int playerId);

private:
	void checkShipCollisions(Ship* ship);

	Player player;
	Bot blueBots[SHIPS_PER_TEAM - 1];
	Bot orangeBots[SHIPS_PER_TEAM];
	Ship blueShips[SHIPS_PER_TEAM];
	Ship orangeShips[SHIPS_PER_TEAM];
	Base bluePlanet;
	Base orangePlanet;
	Base blueStations[STATIONS_PER_TEAM];
	Base orangeStations[STATIONS_PER_TEAM];
	Asteroid asteroids[NUMBER_OF_ASTEROIDS];

	int destroyedBlueStations;
	int destroyedOrangeStations;

	int timeBlueVictory;
	int timeOrangeVictory;
};


