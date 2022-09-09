#pragma once

#include <glm/gtc/matrix_transform.hpp>
#include "../openglframework/Texture.h"
#include "../openglframework/Sprite.h"

class Menu {
public:
	void init();
	void update(int deltaTime, glm::vec2 leftClickPosition);
	void render(glm::vec2 cursorPosition);
	bool isPlayButtonHovered(glm::vec2 cursorPosition);
	bool isControlsButtonHovered(glm::vec2 cursorPosition);
	bool isControlsPanelCloseButtonHovered(glm::vec2 cursorPosition);

private:
	Texture backgroundTexture;
	Texture controlsPanelTexture;

	Sprite* backgroundSprite;
	Sprite* controlsPanelSprite;

	glm::vec2 titlePosition;
	glm::vec2 playPosition;
	glm::vec2 controlsPosition;
	glm::vec2 controlsPanelXPosition;
	glm::vec2 backgroundPosition;

	glm::vec2 titleSize;
	glm::vec2 playSize;
	glm::vec2 controlsSize;
	glm::vec2 controlsPanelXSize;

	float backgroundMovementDirection;

	bool controlsPanelOpen;
};


