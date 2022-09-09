#pragma once

#include "Match.h"

class Server {
public:	
	static Server &instance()	{
		static Server S;	
		return S;
	}
	
	void init();
	void update(int deltaTime);
	void startMatch();
	PlayerRegistrationData registerPlayer();
	MatchInitialData getMatchInitialData();
	MatchData getMatchData(int playerId);
	void setPlayerInput(int playerId, glm::vec2 leftClickPosition, glm::vec2 rightClickPosition, bool spacePressed, bool shiftPressed, UpgradeType upgradeType);

private:
	int matchElapsedTime;
	int matchEndTime;
	Match match;
	bool matchActive;
	bool matchReset;
};


