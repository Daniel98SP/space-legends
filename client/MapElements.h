#pragma once

#include "../openglframework/Texture.h"
#include "../openglframework/Sprite.h"
#include "../Global.h"

using namespace Global;

class MapElements {
public:
	void init(PlayerRegistrationData playerRegistrationData, MatchInitialData matchInitialData);
	void update(int deltaTime, MatchData matchData);
	void renderMapBackground();
	void renderBullets(MatchData matchData);
	void renderShips(MatchData matchData, bool renderHealthBar);
	void renderPlanets(MatchData matchData, int deltaTime, bool renderHealthBar);
	void renderStations(MatchData matchData, bool renderHealthBar);
	void renderAsteroids();

	float getPlanetsAngle();
private:
	PlayerRegistrationData playerRegistrationData;
	MatchInitialData matchInitialData;

	Texture backgroundTexture;
	Texture bulletTexture;
	Texture blueShipTexture;
	Texture orangeShipTexture;
	Texture blueShieldTexture;
	Texture orangeShieldTexture;
	Texture exhaustTexture;
	Texture blueHealthBarContainerTexture;
	Texture orangeHealthBarContainerTexture;
	Texture healthBarTexture;
	Texture bluePlanetTexture;
	Texture orangePlanetTexture;
	Texture blueStationTexture;
	Texture orangeStationTexture;
	Texture asteroidTexture;

	Sprite* backgroundSprite;
	Sprite* bulletSprite;
	Sprite* blueShipSprite;
	Sprite* orangeShipSprite;
	Sprite* blueShieldSprite;
	Sprite* orangeShieldSprite;
	Sprite* exhaustSprite;
	Sprite* blueHealthBarContainerSprite;
	Sprite* orangeHealthBarContainerSprite;
	Sprite* healthBarSprite;
	Sprite* bluePlanetSprite;
	Sprite* orangePlanetSprite;
	Sprite* blueStationSprite;
	Sprite* orangeStationSprite;
	Sprite* asteroidSprite;

	float stationsScales[10];
	float asteroidsVelocities[108];
	float asteroidsAngles[108];
};