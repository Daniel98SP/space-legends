#include "Menu.h"
#include "../Global.h"
#include "../openglframework/Text.h"

using namespace Global;

void Menu::init() {
	this->backgroundTexture.loadFromFile("assets/map/map_tile_6.png", TEXTURE_PIXEL_FORMAT_RGBA);
	this->controlsPanelTexture.loadFromFile("assets/controls/controls_panel.png", TEXTURE_PIXEL_FORMAT_RGBA);

	this->backgroundPosition = glm::vec2(0.f, 0.f);
	this->backgroundSprite = Sprite::createSprite(glm::ivec2(4528, 2943), glm::vec2(1.f, 1.f), &this->backgroundTexture);
	this->backgroundSprite->setPosition(glm::vec2(0.f, 0.f));
	this->backgroundMovementDirection = -1;

	this->controlsPanelSprite = Sprite::createSprite(glm::ivec2(1356, 810), glm::vec2(1.f, 1.f), &this->controlsPanelTexture, "center");

	this->titleSize = Text::getTextSize("SPACE LEGENDS") * 0.8f;
	this->playSize = Text::getTextSize("PLAY") * 0.5f;
	this->controlsSize = Text::getTextSize("CONTROLS") * 0.5f;
	this->controlsPanelXSize = Text::getTextSize("X") * 0.3f;
	this->titlePosition = glm::vec2(RESOLUTION.x / 2, RESOLUTION.y / 4);
	this->playPosition = glm::vec2(RESOLUTION.x / 2, RESOLUTION.y / 1.5);
	this->controlsPosition = glm::vec2(RESOLUTION.x / 2, RESOLUTION.y / 1.25);
	this->controlsPanelXPosition = glm::vec2((RESOLUTION.x / 2) + (678 - 80), (RESOLUTION.y / 2) - (405 - 40));

	this->controlsPanelOpen = false;
}

void Menu::update(int deltaTime, glm::vec2 leftClickPosition) {
	if (this->backgroundMovementDirection < 0.f && this->backgroundPosition.x + 4528.f <= RESOLUTION.x) {
		this->backgroundMovementDirection *= -1.f;
	}
	else if (0.f < this->backgroundPosition.x) {
		this->backgroundMovementDirection *= -1.f;
	}

	this->backgroundPosition.x += 0.01f * this->backgroundMovementDirection * deltaTime;

	if (!this->controlsPanelOpen && isControlsButtonHovered(leftClickPosition)) {
		this->controlsPanelOpen = true;
	}
	else if (this->controlsPanelOpen && isControlsPanelCloseButtonHovered(leftClickPosition)) {
		this->controlsPanelOpen = false;
	}
}

void Menu::render(glm::vec2 cursorPosition) {
	this->backgroundSprite->setPosition(this->backgroundPosition);
	this->backgroundSprite->render();

	float scale = RESOLUTION.x / 1920;

	if (this->controlsPanelOpen) {
		this->controlsPanelSprite->setPosition(glm::vec2(RESOLUTION.x / 2, RESOLUTION.y / 2));
		this->controlsPanelSprite->setScale(glm::vec2(scale));
		this->controlsPanelSprite->render();

		if (isControlsPanelCloseButtonHovered(cursorPosition)) {
			Text::renderText("X", glm::vec2((RESOLUTION.x / 2) + (678 - 80) * scale, (RESOLUTION.y / 2) - (405 - 40) * scale), scale * 0.3, glm::vec3(0.f, 0.f, 1.f));
		}
		else {
			Text::renderText("X", glm::vec2((RESOLUTION.x / 2) + (678 - 80) * scale, (RESOLUTION.y / 2) - (405 - 40) * scale), scale * 0.3, glm::vec3(1.f, 1.f, 1.f));
		}
	}
	else {
		Text::renderText("SPACE LEGENDS", glm::vec2(this->titlePosition.x - (this->titleSize.x / 2) * scale, this->titlePosition.y - (this->titleSize.y / 2) * scale), scale * 0.8, glm::vec3(255.f));

		if (isPlayButtonHovered(cursorPosition)) {
			Text::renderText("PLAY", glm::vec2(this->playPosition.x - (this->playSize.x / 2) * scale, this->playPosition.y - (this->playSize.y / 2) * scale), scale * 0.5f, glm::vec3(0.f, 0.f, 255.f));
		}
		else {
			Text::renderText("PLAY", glm::vec2(this->playPosition.x - (this->playSize.x / 2) * scale, this->playPosition.y - (this->playSize.y / 2) * scale), scale * 0.5f, glm::vec3(255.f, 255.f, 255.f));
		}

		if (isControlsButtonHovered(cursorPosition)) {
			Text::renderText("CONTROLS", glm::vec2(this->controlsPosition.x - (this->controlsSize.x / 2) * scale, this->controlsPosition.y - (this->controlsSize.y / 2) * scale), scale * 0.5f, glm::vec3(0.f, 0.f, 1.f));
		}
		else {
			Text::renderText("CONTROLS", glm::vec2(this->controlsPosition.x - (this->controlsSize.x / 2) * scale, this->controlsPosition.y - (this->controlsSize.y / 2) * scale), scale * 0.5f, glm::vec3(1.f, 1.f, 1.f));
		}
	}
}

bool Menu::isPlayButtonHovered(glm::vec2 cursorPosition) {
	return this->playPosition.x - this->playSize.x / 2 <= cursorPosition.x && cursorPosition.x <= this->playPosition.x + this->playSize.x / 2 &&
		this->playPosition.y - this->playSize.y / 2 <= cursorPosition.y && cursorPosition.y <= this->playPosition.y + this->playSize.y / 2;
}

bool Menu::isControlsButtonHovered(glm::vec2 cursorPosition) {
	return this->controlsPosition.x - this->controlsSize.x / 2 <= cursorPosition.x && cursorPosition.x <= this->controlsPosition.x + this->controlsSize.x / 2 &&
		this->controlsPosition.y - this->controlsSize.y / 2 <= cursorPosition.y && cursorPosition.y <= this->controlsPosition.y + this->controlsSize.y / 2;
}

bool Menu::isControlsPanelCloseButtonHovered(glm::vec2 cursorPosition) {
	return this->controlsPanelXPosition.x <= cursorPosition.x && cursorPosition.x <= this->controlsPanelXPosition.x + this->controlsPanelXSize.x &&
		this->controlsPanelXPosition.y <= cursorPosition.y && cursorPosition.y <= this->controlsPanelXPosition.y + this->controlsPanelXSize.y;
}