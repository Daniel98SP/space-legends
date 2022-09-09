#include "Server.h"

#include <GL/glew.h>
#include <GL/glut.h>
#include <iostream>
#include <fstream>
#include <sstream>
#include "../Global.h"

using namespace Global;

void Server::init() {
	srand(time(NULL));
	Bot::initClass();
	this->match.init();
	this->matchActive = false;
	this->matchReset = true;
	this->matchElapsedTime = 0;
	this->matchEndTime = -1;
}

void Server::update(int deltaTime) {
	this->matchElapsedTime += 1000 < deltaTime ? 17 : deltaTime;

	if (this->matchActive) {
		this->matchActive = this->match.update(this->matchElapsedTime, deltaTime);

		if (!this->matchActive) {
			this->matchEndTime = this->matchElapsedTime;
			this->matchReset = false;
		}
	}
	else if (!this->matchReset && 3000 < this->matchElapsedTime - this->matchEndTime) {
		this->match = Match();
		this->match.init();
		this->matchReset = true;
		this->matchEndTime = -1;
	}
}

void Server::startMatch() {
	this->matchActive = true;
	this->matchElapsedTime = 0;
}

PlayerRegistrationData Server::registerPlayer() {
	return this->match.registerPlayer();
}

MatchInitialData Server::getMatchInitialData() {
	return this->match.getMatchInitialData();
}

MatchData Server::getMatchData(int playerId) {
	return this->match.getMatchData(playerId);
}

void Server::setPlayerInput(int playerId, glm::vec2 leftClickPosition, glm::vec2 rightClickPosition, bool spacePressed, bool shiftPressed, UpgradeType upgradeType) {
	this->match.setPlayerInput(playerId, leftClickPosition, rightClickPosition, spacePressed, shiftPressed, upgradeType);
}





