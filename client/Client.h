#pragma once

#include <glm/gtc/matrix_transform.hpp>
#include "../server/Server.h"
#include "Camera.h"
#include "Menu.h"
#include "Hud.h"
#include "MapElements.h"
#include "PlayerElements.h"

class Client {

public:	
	static Client&instance()	{
		static Client C;
		return C;
	}

	enum Scene { menuScene, matchScene, endScene };
	
	void init(Server* server);
	void update(int deltaTime);
	void render(int deltaTime);
	
	void keyPressed(int key);
	void keyReleased(int key);
	void specialKeyPressed(int key);
	void specialKeyReleased(int key);
	void mouseClick(int state, int button, int x, int y);
	void mouseMove(int x, int y);

private:
	void initMenu();
	void initMatch();

	Server* server;

	int currentTime;
	int matchEndTime;

	glm::vec2 leftClickPosition;
	glm::vec2 rightClickPosition;
	glm::vec2 cursorPosition;
	bool spacePressed;
	bool shiftPressed;

	float zoomLevel;
	float targetZoomLevel;
	Scene scene;

	Camera camera;
	Menu menu;
	Hud hud;
	MapElements mapElements;
	PlayerElements playerElements;

	PlayerRegistrationData playerRegistrationData;
	MatchInitialData matchInitialData;
	MatchData matchData;
};


