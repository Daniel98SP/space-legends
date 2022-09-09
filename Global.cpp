#include "Global.h"

ShaderProgram Global::shader;
ISoundEngine* Global::soundEngine;
glm::vec2 Global::RESOLUTION;

glm::vec2 Global::bluePlanetPosition(10000.f, 70000.f);
glm::vec2 Global::orangePlanetPosition(100000.f, 10000.f);
glm::vec2 Global::blueStationsPositions[5] = { glm::vec2(10000.f, 40000.f), glm::vec2(40000.f, 70000.f), glm::vec2(10000.f, 10000.f), glm::vec2(40000.f, 40000.f), glm::vec2(70000.f, 70000.f) };
glm::vec2 Global::orangeStationsPositions[5] = { glm::vec2(100000.f, 40000.f), glm::vec2(70000.f, 10000.f), glm::vec2(100000.f, 70000.f), glm::vec2(70000.f, 40000.f), glm::vec2(40000.f, 10000.f) };;

int Global::engineUpgradePrices[4] = { 50, 100, 150, 200 };
int Global::rcsUpgradePrices[4] = { 50, 100, 150, 200 };
int Global::weaponUpgradePrices[4] = { 50, 100, 150, 200 };
int Global::shieldUpgradePrices[4] = { 50, 100, 150, 200 };
int Global::antiGravityUpgradePrices[4] = { 50, 100, 150, 200 };