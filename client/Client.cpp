#include <GL/glew.h>
#include <GL/glut.h>
#include <iostream>
#include <fstream>
#include <sstream>
#include "../Global.h"
#include "../Math.h"
#include "Client.h"
#include "../openglframework/Texture.h"
#include "../openglframework/Sprite.h"
#include "../openglframework/Text.h"

using namespace Global;
using namespace Math;

void Client::init(Server* server) {
	this->server = server;
	srand(time(NULL));
	this->currentTime = 0;
	initMenu();
	//soundEngine->play2D("assets/audio/main_theme.mp3", true);
}

void Client::initMenu() {
	this->menu.init();
	this->leftClickPosition = glm::vec2(-1.f, -1.f);
	this->zoomLevel = 1.f * 1980 / RESOLUTION.x;
	this->targetZoomLevel = 1.f * 1980 / RESOLUTION.x;

	this->scene = Scene::menuScene;
}
void Client::initMatch() {
	this->playerRegistrationData = server->registerPlayer();
	this->server->startMatch();
	this->matchInitialData = server->getMatchInitialData();
	this->matchData = server->getMatchData(this->playerRegistrationData.playerId);
	this->hud.init(this->playerRegistrationData);
	this->mapElements.init(this->playerRegistrationData, this->matchInitialData);
	this->playerElements.init(this->playerRegistrationData, this->matchInitialData);

	this->leftClickPosition = glm::vec2(-1.f, -1.f);
	this->rightClickPosition = glm::vec2(-1.f, -1.f);
	this->spacePressed = false;
	this->shiftPressed = false;
	this->zoomLevel = 17.f * 1980 / RESOLUTION.x;
	this->targetZoomLevel = 17.f * 1980 / RESOLUTION.x;

	this->matchEndTime = -1;

	this->scene = Scene::matchScene;
}

void Client::update(int deltaTime) {
	// Debugging purposes
	if (100 < deltaTime) {
		deltaTime = 17;
	}
	this->currentTime += deltaTime;

	if (this->scene == Scene::menuScene) {
		if (this->leftClickPosition.x != -1.f && this->menu.isPlayButtonHovered(this->leftClickPosition)) {
			initMatch();
		}
		else {
			this->menu.update(deltaTime, this->leftClickPosition);
			this->leftClickPosition = glm::vec2(-1.f, -1.f);
		}
	}
	else if (this->scene == Scene::matchScene) {
		if (this->matchData.matchFinished) {
			this->matchEndTime = this->matchEndTime == -1.f ? this->currentTime : this->matchEndTime;
			if (10000 < this->currentTime - this->matchEndTime) {
				initMenu();
			}
		}
		else {
			UpgradeType upgradeBought = this->hud.processLeftClick(this->camera.getCameraLimits(this->matchData.shipsPositions[this->playerRegistrationData.playerShipId], zoomLevel), leftClickPosition);
			this->leftClickPosition = this->hud.isStoreOpen() || this->hud.isStoreButtonClicked(this->camera.getCameraLimits(this->matchData.shipsPositions[this->playerRegistrationData.playerShipId], zoomLevel), this->leftClickPosition) ? glm::vec2(-1.f) : leftClickPosition;
			this->server->setPlayerInput(this->playerRegistrationData.playerId, this->leftClickPosition, this->rightClickPosition, this->spacePressed, this->shiftPressed, upgradeBought);
			this->matchData = this->server->getMatchData(this->playerRegistrationData.playerId);

			this->mapElements.update(deltaTime, this->matchData);

			this->leftClickPosition = glm::vec2(-1.f, -1.f);
			this->rightClickPosition = glm::vec2(-1.f, -1.f);
			this->spacePressed = false;

			if (this->targetZoomLevel < this->zoomLevel) {
				this->zoomLevel -= 0.01 * deltaTime;
			}
			if (this->zoomLevel < this->targetZoomLevel) {
				this->zoomLevel += 0.01 * deltaTime;
			}
		}
	}
}

void Client::render(int deltaTime) {
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	shader.use();
	glm::mat4 projection = glm::ortho(0.0, (double)RESOLUTION.x, (double)RESOLUTION.y, 0.0);
	shader.setUniformMatrix4f("projection", projection);

	if (this->scene == Scene::menuScene) {
		this->menu.render(this->cursorPosition);
	}
	else if (this->scene == Scene::matchScene) {
		shader.setUniformMatrix4f("projection", this->camera.getProjection(this->matchData.shipsPositions[this->playerRegistrationData.playerShipId], this->zoomLevel));

		if (this->matchData.matchFinished) {
			shader.setUniformInt("endScene", 1);
		}

		this->mapElements.renderMapBackground();
		this->playerElements.renderStationRange(this->matchData);
		this->playerElements.renderPlayerDestination(this->matchData);
		this->mapElements.renderBullets(this->matchData);
		this->mapElements.renderShips(this->matchData, !this->matchData.matchFinished);
		this->mapElements.renderPlanets(this->matchData, deltaTime, !this->matchData.matchFinished);
		this->mapElements.renderStations(this->matchData, !this->matchData.matchFinished);
		this->mapElements.renderAsteroids();
		this->playerElements.renderPlayerSelection(this->matchData, this->mapElements.getPlanetsAngle());

		if (!this->matchData.matchFinished) {
			this->hud.render(this->camera.getCameraLimits(this->matchData.shipsPositions[this->playerRegistrationData.playerShipId], zoomLevel), cursorPosition, this->matchData);
		}
		else {
			shader.setUniformInt("endScene", 0);
			glm::vec4 cameraLimits = this->camera.getCameraLimits(this->matchData.shipsPositions[this->playerRegistrationData.playerShipId], zoomLevel);
			float screenSizeX = cameraLimits[1] - cameraLimits[0];
			float screenSizeY = cameraLimits[3] - cameraLimits[2];
			float scale = screenSizeX / 1080;
			glm::vec2 victoryTextSize = Text::getTextSize("victory") * scale;
			glm::vec2 defeatTextSize = Text::getTextSize("defeat") * scale;
			if (this->matchData.basesHealthRatio[0] <= 0.f) {
				if (this->playerRegistrationData.playerTeam == Team::blue) {
					Text::renderText("defeat", glm::vec2(cameraLimits[0] + screenSizeX / 2 - defeatTextSize.x / 2, cameraLimits[2] + screenSizeY / 2 - defeatTextSize.y / 2), scale, glm::vec3(0.f, 0.f, 1.f));
				}
				else {
					Text::renderText("victory", glm::vec2(cameraLimits[0] + screenSizeX / 2 - victoryTextSize.x / 2, cameraLimits[2] + screenSizeY / 2 - victoryTextSize.y / 2), scale, glm::vec3(1.f, 0.f, 0.f));
				}
			}
			else if (this->matchData.basesHealthRatio[6] <= 0.f) {
				if (this->playerRegistrationData.playerTeam == Team::blue) {
					Text::renderText("victory", glm::vec2(cameraLimits[0] + screenSizeX / 2 - victoryTextSize.x / 2, cameraLimits[2] + screenSizeY / 2 - victoryTextSize.y / 2), scale, glm::vec3(0.f, 0.f, 1.f));
				}
				else {
					Text::renderText("defeat", glm::vec2(cameraLimits[0] + screenSizeX / 2 - defeatTextSize.x / 2, cameraLimits[2] + screenSizeY / 2 - defeatTextSize.y / 2), scale, glm::vec3(1.f, 0.f, 0.f));
				}
			}
		}
	}
}

void Client::keyPressed(int key) {
	if (key == 27) {
		exit(0);
	}
	else if (key == 32) {
		this->spacePressed = true;
	}
}

void Client::keyReleased(int key) {
}

void Client::specialKeyPressed(int key) {
	if (key == 112) {
		this->shiftPressed = true;
	}
}

void Client::specialKeyReleased(int key) {
	if (key == 112) {
		this->shiftPressed = false;
	}
}

void Client::mouseMove(int x, int y) {
	this->cursorPosition = this->scene == Scene::matchScene ? camera.screenToWorldCoordinates(glm::vec2(x, y), this->matchData.shipsPositions[this->playerRegistrationData.playerShipId], this->zoomLevel) : glm::vec2(x, y);
}

void Client::mouseClick(int state, int button, int x, int y) {
	// Left click button pressed
	if (button == 0 && state == 0) {
		this->leftClickPosition = this->scene == Scene::matchScene ? camera.screenToWorldCoordinates(glm::vec2(x, y), this->matchData.shipsPositions[this->playerRegistrationData.playerShipId], this->zoomLevel) : glm::vec2(x, y);
	}
	// Right click button pressed
	if (button == 2 && state == 0) {
		this->rightClickPosition = this->scene == Scene::matchScene ? camera.screenToWorldCoordinates(glm::vec2(x, y), this->matchData.shipsPositions[this->playerRegistrationData.playerShipId], this->zoomLevel) : glm::vec2(x, y);
	}
	// Wheel up
	if (button == 3 && state == 0) {
		this->targetZoomLevel = max(10.f * 1980 / RESOLUTION.x, this->targetZoomLevel - 0.5f);
	}
	// Wheel down
	if (button == 4 && state == 0) {
		this->targetZoomLevel = min(17.f * 1980 / RESOLUTION.x, this->targetZoomLevel + 0.5f);
	}
}





