#include "PlayerElements.h"
#include "../Math.h"

using namespace Math;

void PlayerElements::init(PlayerRegistrationData playerRegistrationData, MatchInitialData matchInitialData) {
	this->playerRegistrationData = playerRegistrationData;
	this->matchInitialData = matchInitialData;

	this->blueRangeTexture.loadFromFile("assets/map/blue_station_range.png", TEXTURE_PIXEL_FORMAT_RGBA);
	this->orangeRangeTexture.loadFromFile("assets/map/orange_station_range.png", TEXTURE_PIXEL_FORMAT_RGBA);
	this->destinationTexture.loadFromFile("assets/ship/goal_position.png", TEXTURE_PIXEL_FORMAT_RGBA);
	this->planetSelectionTexture.loadFromFile("assets/player/planet_selection.png", TEXTURE_PIXEL_FORMAT_RGBA);
	this->stationSelectionTexture.loadFromFile("assets/player/station_selection.png", TEXTURE_PIXEL_FORMAT_RGBA);
	this->shipSelectionTexture.loadFromFile("assets/player/ship_selection.png", TEXTURE_PIXEL_FORMAT_RGBA);

	this->blueRangeSprite = Sprite::createSprite(glm::ivec2(20000, 20000), glm::vec2(1.f, 1.f), &this->blueRangeTexture, "center");
	this->orangeRangeSprite = Sprite::createSprite(glm::ivec2(20000, 20000), glm::vec2(1.f, 1.f), &this->orangeRangeTexture, "center");
	this->destinationSprite = Sprite::createSprite(glm::ivec2(370, 370), glm::vec2(1.f, 1.f), &this->destinationTexture, "center");
	this->planetSelectionSprite = Sprite::createSprite(glm::ivec2(5104.f, 5120.f), glm::vec2(1.f, 1.f), &this->planetSelectionTexture, "center");
	this->stationSelectionSprite = Sprite::createSprite(glm::ivec2(2388.f, 3198.f), glm::vec2(1.f, 1.f), &this->stationSelectionTexture, "center");
	this->shipSelectionSprite = Sprite::createSprite(glm::ivec2(1020.f, 916.f), glm::vec2(1.f, 1.f), &this->shipSelectionTexture, "center");
}

void PlayerElements::renderStationRange(MatchData matchData) {
	for (int i = 0; i < STATIONS_PER_TEAM; i++) {
		if (this->playerRegistrationData.playerTeam == Team::blue) {
			if (getDistance(matchData.shipsPositions[this->playerRegistrationData.playerShipId], orangeStationsPositions[i]) <= 10000.f && 0.f < matchData.basesHealthRatio[i + 7]) {
				this->orangeRangeSprite->setPosition(orangeStationsPositions[i]);
				this->orangeRangeSprite->render();
			}
		}
		else {
			if (getDistance(matchData.shipsPositions[this->playerRegistrationData.playerShipId], blueStationsPositions[i]) <= 10000.f && 0.f < matchData.basesHealthRatio[i + 1]) {
				this->blueRangeSprite->setPosition(blueStationsPositions[i]);
				this->blueRangeSprite->render();
			}
		}
	}
}

void PlayerElements::renderPlayerDestination(MatchData matchData) {
	if (matchData.playerDestination.x) {
		this->destinationSprite->setPosition(matchData.playerDestination);
		this->destinationSprite->render();
	}
}

void PlayerElements::renderPlayerSelection(MatchData matchData, float planetAngle) {
	if (matchData.playerSelectionType == TargetType::planet) {
		this->planetSelectionSprite->setPosition(matchData.playerSelectionPosition);
		this->planetSelectionSprite->setRotation(planetAngle);
		this->planetSelectionSprite->render();
	}
	else if (matchData.playerSelectionType == TargetType::station) {
		this->stationSelectionSprite->setPosition(matchData.playerSelectionPosition);
		this->stationSelectionSprite->render();
	}
	else if (matchData.playerSelectionType == TargetType::ship) {
		this->shipSelectionSprite->setPosition(matchData.playerSelectionPosition);
		this->shipSelectionSprite->setRotation(matchData.playerSelectionAngle);
		this->shipSelectionSprite->render();
	}
}