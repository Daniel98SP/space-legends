#pragma once

#include "openglframework/ShaderProgram.h"
#include <irrKlang.h>
using namespace irrklang;

#pragma comment(lib, "libs/irrklang/lib/irrKlang.lib")

#define TIME_PER_FRAME 1000.f / 60.f

#define BACKGROUND_TILE_SIZE_X 4528.f
#define BACKGROUND_TILE_SIZE_Y 2943.f
#define TILE_SIZE_MULTIPLIER 5
#define NUMBER_OF_TILES_X 6
#define NUMBER_OF_TILES_Y 6

#define MAP_SIZE_X 110000.f
#define MAP_SIZE_Y 80000.f

#define STATIONS_PER_TEAM 5
#define SHIPS_PER_TEAM 5
#define NUMBER_OF_ASTEROIDS 108

namespace Global {
	enum class Team { blue, orange };
	enum class TargetType { none, planet, station, ship };
	enum class ImpactType { none, stopped, hit, destroyed };
	enum class UpgradeType { none, engine, rcs, weapon, shield, antiGravity };

	struct MatchInitialData {
		glm::vec2 asteroidsPositions[108];
		float asteroidsScales[108];
	};

	struct PlayerRegistrationData {
		int playerId;
		int playerShipId;
		Team playerTeam;
	};

	struct MatchData {
		bool matchFinished;
		float basesHealthRatio[12];
		glm::vec2 bulletsPositions[20];
		glm::vec2 bulletsVelocities[20];
		glm::vec2 shipsPositions[10];
		float shipsAngles[10];
		bool shipsEnginesActivated[10];
		bool shipsShieldsActivated[10];
		float shipsHealthRatio[10];
		glm::vec2 playerDestination;
		TargetType playerSelectionType;
		glm::vec2 playerSelectionPosition;
		float playerSelectionAngle;
		float playerShieldRatio;
		int playerEngineLevel;
		int playerRcsLevel;
		int playerWeaponsLevel;
		int playerShieldLevel;
		int playerAntiGravityLevel;
		int playerCoins;
	};

	extern ShaderProgram shader;
	extern ISoundEngine* soundEngine;
	extern glm::vec2 RESOLUTION;

	extern glm::vec2 bluePlanetPosition;
	extern glm::vec2 orangePlanetPosition;
	extern glm::vec2 blueStationsPositions[5];
	extern glm::vec2 orangeStationsPositions[5];
	extern int engineUpgradePrices[4];
	extern int rcsUpgradePrices[4];
	extern int weaponUpgradePrices[4];
	extern int shieldUpgradePrices[4];
	extern int antiGravityUpgradePrices[4];
}