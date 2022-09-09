#pragma once

#include "../openglframework/Texture.h"
#include "../openglframework/Sprite.h"
#include "../Global.h"

using namespace Global;

class PlayerElements {
public:
	void init(PlayerRegistrationData playerRegistrationData, MatchInitialData matchInitialData);
	void renderStationRange(MatchData matchData);
	void renderPlayerDestination(MatchData matchData);
	void renderPlayerSelection(MatchData matchData, float planetAngle);
private:
	PlayerRegistrationData playerRegistrationData;
	MatchInitialData matchInitialData;

	Texture blueRangeTexture;
	Texture orangeRangeTexture;
	Texture destinationTexture;
	Texture planetSelectionTexture;
	Texture stationSelectionTexture;
	Texture shipSelectionTexture;

	Sprite* blueRangeSprite;
	Sprite* orangeRangeSprite;
	Sprite* destinationSprite;
	Sprite* planetSelectionSprite;
	Sprite* stationSelectionSprite;
	Sprite* shipSelectionSprite;
};