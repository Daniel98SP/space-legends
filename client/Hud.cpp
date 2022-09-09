#include "Hud.h"
#include "../Global.h"
#include "../openglframework/Text.h"

using namespace Global;

void Hud::init(PlayerRegistrationData playerRegistrationData) {
	if (playerRegistrationData.playerTeam == Team::blue) {
		this->healthBarContainerTexture.loadFromFile("assets/hud/blue_health_bar_container.png", TEXTURE_PIXEL_FORMAT_RGBA);
		this->shieldBarContainerTexture.loadFromFile("assets/hud/blue_shield_bar_container.png", TEXTURE_PIXEL_FORMAT_RGBA);
	}
	else {
		this->healthBarContainerTexture.loadFromFile("assets/hud/orange_health_bar_container.png", TEXTURE_PIXEL_FORMAT_RGBA);
		this->shieldBarContainerTexture.loadFromFile("assets/hud/orange_shield_bar_container.png", TEXTURE_PIXEL_FORMAT_RGBA);
	}

	this->healthBarTexture.loadFromFile("assets/hud/health_bar.png", TEXTURE_PIXEL_FORMAT_RGBA);
	this->shieldBarTexture.loadFromFile("assets/hud/shield_bar.png", TEXTURE_PIXEL_FORMAT_RGBA);
	this->coinTexture.loadFromFile("assets/hud/coin.png", TEXTURE_PIXEL_FORMAT_RGBA);
	this->storeHeaderTexture.loadFromFile("assets/hud/store_header.png", TEXTURE_PIXEL_FORMAT_RGBA);
	this->storeHeaderHoveredTexture.loadFromFile("assets/hud/store_header_hovered.png", TEXTURE_PIXEL_FORMAT_RGBA);
	this->storeBodyTexture.loadFromFile("assets/hud/store_body.png", TEXTURE_PIXEL_FORMAT_RGBA);
	this->upgradeTexture.loadFromFile("assets/hud/upgrade.png", TEXTURE_PIXEL_FORMAT_RGBA);
	this->upgradeAvailableTexture.loadFromFile("assets/hud/upgrade_available.png", TEXTURE_PIXEL_FORMAT_RGBA);
	this->upgradeNotAvailableTexture.loadFromFile("assets/hud/upgrade_not_available.png", TEXTURE_PIXEL_FORMAT_RGBA);

	this->healthBarContainerSprite = Sprite::createSprite(glm::ivec2(176, 26), glm::vec2(1.f, 1.f), &this->healthBarContainerTexture, "corner");
	this->healthBarSprite = Sprite::createSprite(glm::ivec2(173, 23), glm::vec2(1.f, 1.f), &this->healthBarTexture, "corner");
	this->shieldBarContainerSprite = Sprite::createSprite(glm::ivec2(176, 26), glm::vec2(1.f, 1.f), &this->shieldBarContainerTexture, "corner");
	this->shieldBarSprite = Sprite::createSprite(glm::ivec2(173, 23), glm::vec2(1.f, 1.f), &this->shieldBarTexture, "corner");
	this->coinSprite = Sprite::createSprite(glm::ivec2(26, 32), glm::vec2(1.f, 1.f), &this->coinTexture, "corner");
	this->storeHeaderSprite = Sprite::createSprite(glm::ivec2(300, 63), glm::vec2(1.f, 1.f), &this->storeHeaderTexture, "corner");
	this->storeHeaderHoveredSprite = Sprite::createSprite(glm::ivec2(300, 63), glm::vec2(1.f, 1.f), &this->storeHeaderHoveredTexture, "corner");
	this->storeBodySprite = Sprite::createSprite(glm::ivec2(1356, 810), glm::vec2(1.f, 1.f), &this->storeBodyTexture, "corner");
	this->upgradeSprite = Sprite::createSprite(glm::ivec2(150, 400), glm::vec2(1.f, 1.f), &this->upgradeTexture, "corner");
	this->upgradeAvailableSprite = Sprite::createSprite(glm::ivec2(150, 400), glm::vec2(1.f, 1.f), &this->upgradeAvailableTexture, "corner");
	this->upgradeNotAvailableSprite = Sprite::createSprite(glm::ivec2(150, 400), glm::vec2(1.f, 1.f), &this->upgradeNotAvailableTexture, "corner");

	this->playerRegistrationData = playerRegistrationData;
	this->storeOpen = false;
}

void Hud::render(glm::vec4 cameraLimits, glm::vec2 cursorPosition, MatchData matchData) {
	float screenSizeX = cameraLimits[1] - cameraLimits[0];
	float screenSizeY = cameraLimits[3] - cameraLimits[2];

	float scale = screenSizeX / 1920;

	this->healthBarContainerSprite->setPosition(glm::vec2(cameraLimits[1] - screenSizeX / 8, cameraLimits[3] - screenSizeY / 8));
	this->healthBarContainerSprite->setScale(glm::vec2(scale));
	this->healthBarContainerSprite->render();

	this->healthBarSprite->setPosition(glm::vec2((cameraLimits[1] - screenSizeX / 8) + 20, (cameraLimits[3] - screenSizeY / 8) + 20));
	this->healthBarSprite->setScale(glm::vec2(scale * matchData.shipsHealthRatio[this->playerRegistrationData.playerShipId], scale));
	this->healthBarSprite->render();

	this->shieldBarContainerSprite->setPosition(glm::vec2(cameraLimits[1] - screenSizeX / 8, cameraLimits[3] - screenSizeY / 11));
	this->shieldBarContainerSprite->setScale(glm::vec2(scale));
	this->shieldBarContainerSprite->render();

	this->shieldBarSprite->setPosition(glm::vec2((cameraLimits[1] - screenSizeX / 8) + 20, (cameraLimits[3] - screenSizeY / 11) + 20));
	this->shieldBarSprite->setScale(glm::vec2(scale * matchData.playerShieldRatio, scale));
	this->shieldBarSprite->render();

	this->coinSprite->setPosition(glm::vec2(glm::vec2(cameraLimits[1] - screenSizeX / 8, cameraLimits[3] - screenSizeY / 6.3)));
	this->coinSprite->setScale(glm::vec2(scale));
	this->coinSprite->render();
	Text::renderText(to_string(matchData.playerCoins), (glm::vec2(cameraLimits[1] - (screenSizeX / 8) + (screenSizeX / 50), cameraLimits[3] - (screenSizeY / 6.3) + (screenSizeY / 200))), 0.15 * scale, glm::vec3(1.f, 0.84f, 0.f));

	if (this->storeOpen) {
		glm::vec2 storeBodyPosition(cameraLimits[0] + screenSizeX / 3.3 - 300 * scale, cameraLimits[2] + screenSizeY / 8);

		this->storeBodySprite->setPosition(glm::vec2(storeBodyPosition));
		this->storeBodySprite->setScale(glm::vec2(scale));
		this->storeBodySprite->render();


		// ENGINE
		if (storeBodyPosition.x + 18 * scale < cursorPosition.x && cursorPosition.x < storeBodyPosition.x + 168 * scale && storeBodyPosition.y + 17 * scale < cursorPosition.y && cursorPosition.y < storeBodyPosition.y + 417 * scale) {
			if (matchData.playerEngineLevel < 4 && engineUpgradePrices[matchData.playerEngineLevel] <= matchData.playerCoins) {
				this->upgradeAvailableSprite->setPosition(glm::vec2(storeBodyPosition.x + 18 * scale, storeBodyPosition.y + 17 * scale));
				this->upgradeAvailableSprite->setScale(glm::vec2(scale));
				this->upgradeAvailableSprite->render();
			}
			else {
				this->upgradeNotAvailableSprite->setPosition(glm::vec2(storeBodyPosition.x + 18 * scale, storeBodyPosition.y + 17 * scale));
				this->upgradeNotAvailableSprite->setScale(glm::vec2(scale));
				this->upgradeNotAvailableSprite->render();
			}
		}
		else {
			this->upgradeSprite->setPosition(glm::vec2(storeBodyPosition.x + 18 * scale, storeBodyPosition.y + 17 * scale));
			this->upgradeSprite->setScale(glm::vec2(scale));
			this->upgradeSprite->render();
		}
		Text::renderText("engine", glm::vec2(storeBodyPosition.x + (18 + 10) * scale, storeBodyPosition.y + (17 + 310) * scale), 0.15 * scale, glm::vec3(1.f, 1.0f, 1.f));
		if (matchData.playerEngineLevel < 4) {
			glm::vec3 color = matchData.playerCoins < engineUpgradePrices[matchData.playerEngineLevel] ? glm::vec3(1.f, 0.f, 0.f) : glm::vec3(0.f, 1.f, 0.f);
			Text::renderText(to_string(engineUpgradePrices[matchData.playerEngineLevel]), glm::vec2(storeBodyPosition.x + (18 + 10) * scale, storeBodyPosition.y + (17 + 363) * scale), 0.15 * scale, color);
		}
		else {
			Text::renderText("FULL", glm::vec2(storeBodyPosition.x + (18 + 32) * scale, storeBodyPosition.y + (17 + 363) * scale), 0.15 * scale, glm::vec3(1.f, 0.84f, 0.f));
		}


		// RCS
		if (storeBodyPosition.x + 185 * scale < cursorPosition.x && cursorPosition.x < storeBodyPosition.x + 335 * scale && storeBodyPosition.y + 17 * scale < cursorPosition.y && cursorPosition.y < storeBodyPosition.y + 417 * scale) {
			if (matchData.playerRcsLevel < 4 && rcsUpgradePrices[matchData.playerRcsLevel] <= matchData.playerCoins) {
				this->upgradeAvailableSprite->setPosition(glm::vec2(storeBodyPosition.x + 185 * scale, storeBodyPosition.y + 17 * scale));
				this->upgradeAvailableSprite->setScale(glm::vec2(scale));
				this->upgradeAvailableSprite->render();
			}
			else {
				this->upgradeNotAvailableSprite->setPosition(glm::vec2(storeBodyPosition.x + 185 * scale, storeBodyPosition.y + 17 * scale));
				this->upgradeNotAvailableSprite->setScale(glm::vec2(scale));
				this->upgradeNotAvailableSprite->render();
			}
		}
		else {
			this->upgradeSprite->setPosition(glm::vec2(storeBodyPosition.x + 185 * scale, storeBodyPosition.y + 17 * scale));
			this->upgradeSprite->setScale(glm::vec2(scale));
			this->upgradeSprite->render();
		}
		Text::renderText("rcs", glm::vec2(storeBodyPosition.x + (185 + 10) * scale, storeBodyPosition.y + (17 + 310) * scale), 0.15 * scale, glm::vec3(1.f, 1.0f, 1.f));
		if (matchData.playerRcsLevel < 4) {
			glm::vec3 color = matchData.playerCoins < rcsUpgradePrices[matchData.playerRcsLevel] ? glm::vec3(1.f, 0.f, 0.f) : glm::vec3(0.f, 1.f, 0.f);
			Text::renderText(to_string(rcsUpgradePrices[matchData.playerRcsLevel]), glm::vec2(storeBodyPosition.x + (185 + 10) * scale, storeBodyPosition.y + (17 + 363) * scale), 0.15 * scale, color);
		}
		else {
			Text::renderText("FULL", glm::vec2(storeBodyPosition.x + (185 + 32) * scale, storeBodyPosition.y + (17 + 363) * scale), 0.15 * scale, glm::vec3(1.f, 0.84f, 0.f));
		}


		// WEAPON
		if (storeBodyPosition.x + 352 * scale < cursorPosition.x && cursorPosition.x < storeBodyPosition.x + 502 * scale && storeBodyPosition.y + 17 * scale < cursorPosition.y && cursorPosition.y < storeBodyPosition.y + 417 * scale) {
			if (matchData.playerWeaponsLevel < 4 && weaponUpgradePrices[matchData.playerWeaponsLevel] <= matchData.playerCoins) {
				this->upgradeAvailableSprite->setPosition(glm::vec2(storeBodyPosition.x + 352 * scale, storeBodyPosition.y + 17 * scale));
				this->upgradeAvailableSprite->setScale(glm::vec2(scale));
				this->upgradeAvailableSprite->render();
			}
			else {
				this->upgradeNotAvailableSprite->setPosition(glm::vec2(storeBodyPosition.x + 352 * scale, storeBodyPosition.y + 17 * scale));
				this->upgradeNotAvailableSprite->setScale(glm::vec2(scale));
				this->upgradeNotAvailableSprite->render();
			}
		}
		else {
			this->upgradeSprite->setPosition(glm::vec2(storeBodyPosition.x + 352 * scale, storeBodyPosition.y + 17 * scale));
			this->upgradeSprite->setScale(glm::vec2(scale));
			this->upgradeSprite->render();
		}
		Text::renderText("weapon", glm::vec2(storeBodyPosition.x + (352 + 10) * scale, storeBodyPosition.y + (17 + 310) * scale), 0.15 * scale, glm::vec3(1.f, 1.0f, 1.f));
		if (matchData.playerWeaponsLevel < 4) {
			glm::vec3 color = matchData.playerCoins < weaponUpgradePrices[matchData.playerWeaponsLevel] ? glm::vec3(1.f, 0.f, 0.f) : glm::vec3(0.f, 1.f, 0.f);
			Text::renderText(to_string(weaponUpgradePrices[matchData.playerWeaponsLevel]), glm::vec2(storeBodyPosition.x + (352 + 10) * scale, storeBodyPosition.y + (17 + 363) * scale), 0.15 * scale, color);
		}
		else {
			Text::renderText("FULL", glm::vec2(storeBodyPosition.x + (352 + 32) * scale, storeBodyPosition.y + (17 + 363) * scale), 0.15 * scale, glm::vec3(1.f, 0.84f, 0.f));
		}


		// SHIELD
		if (storeBodyPosition.x + 519 * scale < cursorPosition.x && cursorPosition.x < storeBodyPosition.x + 669 * scale && storeBodyPosition.y + 17 * scale < cursorPosition.y && cursorPosition.y < storeBodyPosition.y + 417 * scale) {
			if (matchData.playerShieldLevel < 4 && shieldUpgradePrices[matchData.playerShieldLevel] <= matchData.playerCoins) {
				this->upgradeAvailableSprite->setPosition(glm::vec2(storeBodyPosition.x + 519 * scale, storeBodyPosition.y + 17 * scale));
				this->upgradeAvailableSprite->setScale(glm::vec2(scale));
				this->upgradeAvailableSprite->render();
			}
			else {
				this->upgradeNotAvailableSprite->setPosition(glm::vec2(storeBodyPosition.x + 519 * scale, storeBodyPosition.y + 17 * scale));
				this->upgradeNotAvailableSprite->setScale(glm::vec2(scale));
				this->upgradeNotAvailableSprite->render();
			}
		}
		else {
			this->upgradeSprite->setPosition(glm::vec2(storeBodyPosition.x + 519 * scale, storeBodyPosition.y + 17 * scale));
			this->upgradeSprite->setScale(glm::vec2(scale));
			this->upgradeSprite->render();
		}
		Text::renderText("shield", glm::vec2(storeBodyPosition.x + (519 + 10) * scale, storeBodyPosition.y + (17 + 310) * scale), 0.15 * scale, glm::vec3(1.f, 1.0f, 1.f));
		if (matchData.playerShieldLevel < 4) {
			glm::vec3 color = matchData.playerCoins < shieldUpgradePrices[matchData.playerShieldLevel] ? glm::vec3(1.f, 0.f, 0.f) : glm::vec3(0.f, 1.f, 0.f);
			Text::renderText(to_string(shieldUpgradePrices[matchData.playerShieldLevel]), glm::vec2(storeBodyPosition.x + (519 + 10) * scale, storeBodyPosition.y + (17 + 363) * scale), 0.15 * scale, color);
		}
		else {
			Text::renderText("FULL", glm::vec2(storeBodyPosition.x + (519 + 32) * scale, storeBodyPosition.y + (17 + 363) * scale), 0.15 * scale, glm::vec3(1.f, 0.84f, 0.f));
		}


		// ANTI GRAVITY
		if (storeBodyPosition.x + 686 * scale < cursorPosition.x && cursorPosition.x < storeBodyPosition.x + 836 * scale && storeBodyPosition.y + 17 * scale < cursorPosition.y && cursorPosition.y < storeBodyPosition.y + 417 * scale) {
			if (matchData.playerAntiGravityLevel < 4 && antiGravityUpgradePrices[matchData.playerAntiGravityLevel] <= matchData.playerCoins) {
				this->upgradeAvailableSprite->setPosition(glm::vec2(storeBodyPosition.x + 686 * scale, storeBodyPosition.y + 17 * scale));
				this->upgradeAvailableSprite->setScale(glm::vec2(scale));
				this->upgradeAvailableSprite->render();
			}
			else {
				this->upgradeNotAvailableSprite->setPosition(glm::vec2(storeBodyPosition.x + 686 * scale, storeBodyPosition.y + 17 * scale));
				this->upgradeNotAvailableSprite->setScale(glm::vec2(scale));
				this->upgradeNotAvailableSprite->render();
			}
		}
		else {
			this->upgradeSprite->setPosition(glm::vec2(storeBodyPosition.x + 686 * scale, storeBodyPosition.y + 17 * scale));
			this->upgradeSprite->setScale(glm::vec2(scale));
			this->upgradeSprite->render();
		}
		Text::renderText("anti-g", glm::vec2(storeBodyPosition.x + (686 + 10) * scale, storeBodyPosition.y + (17 + 310) * scale), 0.15 * scale, glm::vec3(1.f, 1.0f, 1.f));
		if (matchData.playerAntiGravityLevel < 4) {
			glm::vec3 color = matchData.playerCoins < antiGravityUpgradePrices[matchData.playerAntiGravityLevel] ? glm::vec3(1.f, 0.f, 0.f) : glm::vec3(0.f, 1.f, 0.f);
			Text::renderText(to_string(antiGravityUpgradePrices[matchData.playerAntiGravityLevel]), glm::vec2(storeBodyPosition.x + (686 + 10) * scale, storeBodyPosition.y + (17 + 363) * scale), 0.15 * scale, color);
		}
		else {
			Text::renderText("FULL", glm::vec2(storeBodyPosition.x + (686 + 32) * scale, storeBodyPosition.y + (17 + 363) * scale), 0.15 * scale, glm::vec3(1.f, 0.84f, 0.f));
		}
	}

	if (cameraLimits[1] - screenSizeX / 3.3 < cursorPosition.x && cursorPosition.x < cameraLimits[1] - screenSizeX / 3.3 + 300 * scale && cameraLimits[3] - screenSizeY / 8 < cursorPosition.y && cursorPosition.y < cameraLimits[3] - screenSizeY / 8 + 63 * scale) {
		this->storeHeaderHoveredSprite->setPosition(glm::vec2(cameraLimits[1] - screenSizeX / 3.3, cameraLimits[3] - screenSizeY / 8));
		this->storeHeaderHoveredSprite->setScale(glm::vec2(scale));
		this->storeHeaderHoveredSprite->render();
	}
	else {
		this->storeHeaderSprite->setPosition(glm::vec2(cameraLimits[1] - screenSizeX / 3.3, cameraLimits[3] - screenSizeY / 8));
		this->storeHeaderSprite->setScale(glm::vec2(scale));
		this->storeHeaderSprite->render();
	}
}

UpgradeType Hud::processLeftClick(glm::vec4 cameraLimits, glm::vec2 leftClickPosition) {
	float screenSizeX = cameraLimits[1] - cameraLimits[0];
	float screenSizeY = cameraLimits[3] - cameraLimits[2];

	float scale = screenSizeX / 1920;

	if (isStoreButtonClicked(cameraLimits, leftClickPosition)) {
		this->storeOpen = this->storeOpen ? false : true;
	}

	if (this->storeOpen) {
		glm::vec2 storeBodyPosition(cameraLimits[0] + screenSizeX / 3.3 - 300 * scale, cameraLimits[2] + screenSizeY / 8);

		if (storeBodyPosition.x + 18 * scale < leftClickPosition.x && leftClickPosition.x < storeBodyPosition.x + 168 * scale && storeBodyPosition.y + 17 * scale < leftClickPosition.y && leftClickPosition.y < storeBodyPosition.y + 417 * scale) {
			return UpgradeType::engine;
		}
		else if (storeBodyPosition.x + 185 * scale < leftClickPosition.x && leftClickPosition.x < storeBodyPosition.x + 335 * scale && storeBodyPosition.y + 17 * scale < leftClickPosition.y && leftClickPosition.y < storeBodyPosition.y + 417 * scale) {
			return UpgradeType::rcs;
		}
		else if (storeBodyPosition.x + 352 * scale < leftClickPosition.x && leftClickPosition.x < storeBodyPosition.x + 502 * scale && storeBodyPosition.y + 17 * scale < leftClickPosition.y && leftClickPosition.y < storeBodyPosition.y + 417 * scale) {
			return UpgradeType::weapon;
		}
		else if (storeBodyPosition.x + 519 * scale < leftClickPosition.x && leftClickPosition.x < storeBodyPosition.x + 669 * scale && storeBodyPosition.y + 17 * scale < leftClickPosition.y && leftClickPosition.y < storeBodyPosition.y + 417 * scale) {
			return UpgradeType::shield;
		}
		else if (storeBodyPosition.x + 686 * scale < leftClickPosition.x && leftClickPosition.x < storeBodyPosition.x + 836 * scale && storeBodyPosition.y + 17 * scale < leftClickPosition.y && leftClickPosition.y < storeBodyPosition.y + 417 * scale) {
			return UpgradeType::antiGravity;
		}
		else {
			return UpgradeType::none;
		}
	}
	else {
		return UpgradeType::none;
	}
}

bool Hud::isStoreOpen() {
	return this->storeOpen;
}

bool Hud::isStoreButtonClicked(glm::vec4 cameraLimits, glm::vec2 leftClickPosition) {
	float screenSizeX = cameraLimits[1] - cameraLimits[0];
	float screenSizeY = cameraLimits[3] - cameraLimits[2];

	float scale = screenSizeX / 1920;

	return cameraLimits[1] - screenSizeX / 3.3 < leftClickPosition.x && leftClickPosition.x < cameraLimits[1] - screenSizeX / 3.3 + 300 * scale && cameraLimits[3] - screenSizeY / 8 < leftClickPosition.y && leftClickPosition.y < cameraLimits[3] - screenSizeY / 8 + 63 * scale;
}