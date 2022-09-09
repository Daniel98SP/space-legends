#include "MapElements.h"
#include <random>
#include "../Math.h"

using namespace Math;

void MapElements::init(PlayerRegistrationData playerRegistrationData, MatchInitialData matchInitialData) {
	this->playerRegistrationData = playerRegistrationData;
	this->matchInitialData = matchInitialData;

	for (int i = 0; i < STATIONS_PER_TEAM * 2; i++) {
		this->stationsScales[i] = 1.f;
	}

	std::random_device randomDevice;
	std::mt19937 gen(randomDevice());
	std::uniform_real_distribution<> angleDist(-180.f, 180.f);
	std::uniform_real_distribution<> rotationDist(-0.01f, 0.01f);
	for (int i = 0; i < NUMBER_OF_ASTEROIDS; i++) {
		this->asteroidsAngles[i] = angleDist(gen);
		this->asteroidsVelocities[i] = rotationDist(gen);
	}

	this->backgroundTexture.loadFromFile("assets/map/map_tile_6.png", TEXTURE_PIXEL_FORMAT_RGBA);
	this->blueShipTexture.loadFromFile("assets/ship/blue_ship_2.png", TEXTURE_PIXEL_FORMAT_RGBA);
	this->orangeShipTexture.loadFromFile("assets/ship/orange_ship_2.png", TEXTURE_PIXEL_FORMAT_RGBA);
	this->blueShieldTexture.loadFromFile("assets/player/blue_shield.png", TEXTURE_PIXEL_FORMAT_RGBA);
	this->orangeShieldTexture.loadFromFile("assets/player/orange_shield.png", TEXTURE_PIXEL_FORMAT_RGBA);
	this->exhaustTexture.loadFromFile("assets/ship/exhaust.png", TEXTURE_PIXEL_FORMAT_RGBA);
	this->blueHealthBarContainerTexture.loadFromFile("assets/hud/blue_health_bar_container.png", TEXTURE_PIXEL_FORMAT_RGBA);
	this->orangeHealthBarContainerTexture.loadFromFile("assets/hud/orange_health_bar_container.png", TEXTURE_PIXEL_FORMAT_RGBA);
	this->healthBarTexture.loadFromFile("assets/hud/health_bar.png", TEXTURE_PIXEL_FORMAT_RGBA);
	this->bluePlanetTexture.loadFromFile("assets/map/blue_planet.png", TEXTURE_PIXEL_FORMAT_RGBA);
	this->orangePlanetTexture.loadFromFile("assets/map/orange_planet.png", TEXTURE_PIXEL_FORMAT_RGBA);
	this->blueStationTexture.loadFromFile("assets/map/blue_station.png", TEXTURE_PIXEL_FORMAT_RGBA);
	this->orangeStationTexture.loadFromFile("assets/map/orange_station.png", TEXTURE_PIXEL_FORMAT_RGBA);
	this->asteroidTexture.loadFromFile("assets/map/asteroid.png", TEXTURE_PIXEL_FORMAT_RGBA);
	this->bulletTexture.loadFromFile("assets/ship/bullet.png", TEXTURE_PIXEL_FORMAT_RGBA);

	this->backgroundSprite = Sprite::createSprite(glm::ivec2(BACKGROUND_TILE_SIZE_X * TILE_SIZE_MULTIPLIER, BACKGROUND_TILE_SIZE_Y * TILE_SIZE_MULTIPLIER), glm::vec2(1.f, 1.f), &this->backgroundTexture);
	this->blueShipSprite = Sprite::createSprite(glm::ivec2(1020, 916), glm::vec2(1.f, 1.f), &this->blueShipTexture, "center");
	this->orangeShipSprite = Sprite::createSprite(glm::ivec2(1020, 916), glm::vec2(1.f, 1.f), &this->orangeShipTexture, "center");
	this->blueShieldSprite = Sprite::createSprite(glm::ivec2(2000, 2000), glm::vec2(1.f, 1.f), &this->blueShieldTexture, "center");
	this->orangeShieldSprite = Sprite::createSprite(glm::ivec2(2000, 2000), glm::vec2(1.f, 1.f), &this->orangeShieldTexture, "center");
	this->exhaustSprite = Sprite::createSprite(glm::ivec2(120, 800), glm::vec2(1.f, 1.f), &this->exhaustTexture, "center");
	this->blueHealthBarContainerSprite = Sprite::createSprite(glm::ivec2(1760, 260), glm::vec2(1.f, 1.f), &this->blueHealthBarContainerTexture, "corner");
	this->orangeHealthBarContainerSprite = Sprite::createSprite(glm::ivec2(1760, 260), glm::vec2(1.f, 1.f), &this->orangeHealthBarContainerTexture, "corner");
	this->healthBarSprite = Sprite::createSprite(glm::ivec2(1730, 230), glm::vec2(1.f, 1.f), &this->healthBarTexture, "corner");
	this->bluePlanetSprite = Sprite::createSprite(glm::ivec2(5104, 5120), glm::vec2(1.f, 1.f), &this->bluePlanetTexture, "center");
	this->orangePlanetSprite = Sprite::createSprite(glm::ivec2(5104, 5120), glm::vec2(1.f, 1.f), &this->orangePlanetTexture, "center");
	this->blueStationSprite = Sprite::createSprite(glm::ivec2(2388, 3198), glm::vec2(1.f, 1.f), &this->blueStationTexture, "center");
	this->orangeStationSprite = Sprite::createSprite(glm::ivec2(2388, 3198), glm::vec2(1.f, 1.f), &this->orangeStationTexture, "center");
	this->asteroidSprite = Sprite::createSprite(glm::ivec2(2510, 2300), glm::vec2(1.f, 1.f), &this->asteroidTexture, "center");
	this->bulletSprite = Sprite::createSprite(glm::ivec2(138, 651), glm::vec2(1.f, 1.f), &this->bulletTexture, "center");
}

void MapElements::update(int deltaTime, MatchData matchData) {
	for (int i = 0; i < NUMBER_OF_ASTEROIDS; i++) {
		this->asteroidsAngles[i] += this->asteroidsVelocities[i] * deltaTime;
	}

	for (int i = 0; i < STATIONS_PER_TEAM; i++) {
		if (matchData.basesHealthRatio[i+1] <= 0.f) {
			this->stationsScales[i] = max(0.f, this->stationsScales[i] - 0.001f * deltaTime);
		}
		if (matchData.basesHealthRatio[i+7] <= 0.f) {
			this->stationsScales[i+5] = max(0.f, this->stationsScales[i+5] - 0.001f * deltaTime);
		}
	}
}

void MapElements::renderMapBackground() {
	for (int i = 0; i < NUMBER_OF_TILES_Y; i++) {
		for (int j = 0; j < NUMBER_OF_TILES_X; j++) {
			this->backgroundSprite->setPosition(glm::vec2(BACKGROUND_TILE_SIZE_X * TILE_SIZE_MULTIPLIER * j, BACKGROUND_TILE_SIZE_Y * TILE_SIZE_MULTIPLIER * i));
			this->backgroundSprite->render();
		}
	}
}

void MapElements::renderBullets(MatchData matchData) {
	for (int i = 0; i < size(matchData.bulletsPositions); i++) {
		if (matchData.bulletsPositions[i].x != -1.f) {
			this->bulletSprite->setPosition(matchData.bulletsPositions[i]);
			this->bulletSprite->setRotation(getAngleBetweenVectors(glm::vec2(0.f, -1.f), matchData.bulletsVelocities[i]));
			this->bulletSprite->render();
		}
	}
}

void MapElements::renderShips(MatchData matchData, bool renderHealthBar) {
	for (int i = 0; i < SHIPS_PER_TEAM * 2; i++) {
		if (matchData.shipsEnginesActivated[i]) {
			float positionX = matchData.shipsPositions[i].x - (matchData.shipsPositions[i].y + 520.f - matchData.shipsPositions[i].y) * sin(glm::radians(matchData.shipsAngles[i]));
			float positionY = matchData.shipsPositions[i].y + (matchData.shipsPositions[i].y + 520.f - matchData.shipsPositions[i].y) * cos(glm::radians(matchData.shipsAngles[i]));
			this->exhaustSprite->setPosition(glm::vec2(positionX, positionY));
			this->exhaustSprite->setRotation(matchData.shipsAngles[i]);
			this->exhaustSprite->render();
		}
		if (i < 5) {
			if (matchData.shipsShieldsActivated[i]) {
				this->blueShieldSprite->setPosition(matchData.shipsPositions[i]);
				this->blueShieldSprite->render();
			}
			this->blueShipSprite->setPosition(matchData.shipsPositions[i]);
			this->blueShipSprite->setRotation(matchData.shipsAngles[i]);
			this->blueShipSprite->render();
			if (i != this->playerRegistrationData.playerShipId && renderHealthBar) {
				this->blueHealthBarContainerSprite->setPosition(glm::vec2(matchData.shipsPositions[i].x - 880, matchData.shipsPositions[i].y - 130 - 1000));
				this->blueHealthBarContainerSprite->setScale(glm::vec2(1.f));
				this->blueHealthBarContainerSprite->render();

				this->healthBarSprite->setPosition(glm::vec2(matchData.shipsPositions[i].x - 865, matchData.shipsPositions[i].y - 115 - 1000));
				this->healthBarSprite->setScale(glm::vec2(matchData.shipsHealthRatio[i], 1.f));
				this->healthBarSprite->render();
			}
		}
		else {
			if (matchData.shipsShieldsActivated[i]) {
				this->orangeShieldSprite->setPosition(matchData.shipsPositions[i]);
				this->orangeShieldSprite->render();
			}
			this->orangeShipSprite->setPosition(matchData.shipsPositions[i]);
			this->orangeShipSprite->setRotation(matchData.shipsAngles[i]);
			this->orangeShipSprite->render();
			if (i != this->playerRegistrationData.playerShipId && renderHealthBar) {
				this->orangeHealthBarContainerSprite->setPosition(glm::vec2(matchData.shipsPositions[i].x - 880, matchData.shipsPositions[i].y - 130 - 1000));
				this->orangeHealthBarContainerSprite->setScale(glm::vec2(1.f));
				this->orangeHealthBarContainerSprite->render();

				this->healthBarSprite->setPosition(glm::vec2(matchData.shipsPositions[i].x - 865, matchData.shipsPositions[i].y - 115 - 1000));
				this->healthBarSprite->setScale(glm::vec2(matchData.shipsHealthRatio[i], 1.f));
				this->healthBarSprite->render();
			}
		}
	}
}

void MapElements::renderPlanets(MatchData matchData, int deltaTime, bool renderHealthBar) {
	this->bluePlanetSprite->setPosition(bluePlanetPosition);
	this->bluePlanetSprite->setRotation(this->bluePlanetSprite->getRotation() + 0.002 * deltaTime);
	this->bluePlanetSprite->render();

	this->orangePlanetSprite->setPosition(orangePlanetPosition);
	this->orangePlanetSprite->setRotation(this->orangePlanetSprite->getRotation() + 0.002 * deltaTime);
	this->orangePlanetSprite->render();

	if (renderHealthBar) {
		this->blueHealthBarContainerSprite->setPosition(glm::vec2(bluePlanetPosition.x - 1760.f, bluePlanetPosition.y - 260.f - 3500.f));
		this->blueHealthBarContainerSprite->setScale(glm::vec2(2.f));
		this->blueHealthBarContainerSprite->render();
		this->healthBarSprite->setPosition(glm::vec2(bluePlanetPosition.x - 1730.f, bluePlanetPosition.y - 230.f - 3500.f));
		this->healthBarSprite->setScale(glm::vec2(matchData.basesHealthRatio[0] * 2.f, 2.f));
		this->healthBarSprite->render();

		this->orangeHealthBarContainerSprite->setPosition(glm::vec2(orangePlanetPosition.x - 1760.f, orangePlanetPosition.y - 260.f - 3500.f));
		this->orangeHealthBarContainerSprite->setScale(glm::vec2(2.f));
		this->orangeHealthBarContainerSprite->render();
		this->healthBarSprite->setPosition(glm::vec2(orangePlanetPosition.x - 1730.f, orangePlanetPosition.y - 230.f - 3500.f));
		this->healthBarSprite->setScale(glm::vec2(matchData.basesHealthRatio[6] * 2.f, 2.f));
		this->healthBarSprite->render();
	}	
}

void MapElements::renderStations(MatchData matchData, bool renderHealthBar) {
	for (int i = 1; i < STATIONS_PER_TEAM + 1; i++) {
		this->blueStationSprite->setPosition(blueStationsPositions[i-1]);
		this->blueStationSprite->setScale(glm::vec2(this->stationsScales[i - 1]));
		this->blueStationSprite->render();

		if (0.f < matchData.basesHealthRatio[i] && renderHealthBar) {
			this->blueHealthBarContainerSprite->setPosition(glm::vec2(blueStationsPositions[i - 1].x - 880.f, blueStationsPositions[i - 1].y - 130.f - 2000.f));
			this->blueHealthBarContainerSprite->setScale(glm::vec2(1.f));
			this->blueHealthBarContainerSprite->render();
			this->healthBarSprite->setPosition(glm::vec2(blueStationsPositions[i - 1].x - 865.f, blueStationsPositions[i - 1].y - 115.f - 2000.f));
			this->healthBarSprite->setScale(glm::vec2(matchData.basesHealthRatio[i], 1.f));
			this->healthBarSprite->render();
		}
	}
	for (int i = 7; i < STATIONS_PER_TEAM + 7; i++) {
		this->orangeStationSprite->setPosition(orangeStationsPositions[i-7]);
		this->orangeStationSprite->setScale(glm::vec2(this->stationsScales[i-2]));
		this->orangeStationSprite->render();

		if (0.f < matchData.basesHealthRatio[i] && renderHealthBar) {
			this->orangeHealthBarContainerSprite->setPosition(glm::vec2(orangeStationsPositions[i - 7].x - 880.f, orangeStationsPositions[i - 7].y - 130.f - 2000.f));
			this->orangeHealthBarContainerSprite->setScale(glm::vec2(1.f));
			this->orangeHealthBarContainerSprite->render();
			this->healthBarSprite->setPosition(glm::vec2(orangeStationsPositions[i - 7].x - 865.f, orangeStationsPositions[i - 7].y - 115.f - 2000.f));
			this->healthBarSprite->setScale(glm::vec2(matchData.basesHealthRatio[i], 1.f));
			this->healthBarSprite->render();
		}
	}
}

void MapElements::renderAsteroids() {
	for (int i = 0; i < NUMBER_OF_ASTEROIDS; i++) {
		this->asteroidSprite->setPosition(this->matchInitialData.asteroidsPositions[i]);
		this->asteroidSprite->setRotation(this->asteroidsAngles[i]);
		this->asteroidSprite->setScale(glm::vec2(this->matchInitialData.asteroidsScales[i]));
		this->asteroidSprite->render();
	}
}

float MapElements::getPlanetsAngle() {
	return this->bluePlanetSprite->getRotation();
}