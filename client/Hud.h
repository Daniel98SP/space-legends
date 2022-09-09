#pragma once

#include "../openglframework/Texture.h"
#include "../openglframework/Sprite.h"
#include "../Global.h"

using namespace Global;

class Hud {
public:
	void init(PlayerRegistrationData playerRegistrationData);
	void render(glm::vec4 cameraLimits, glm::vec2 cursorPosition, MatchData matchData);

	bool isStoreOpen();
	bool isStoreButtonClicked(glm::vec4 cameraLimits, glm::vec2 leftClickPosition);

	UpgradeType processLeftClick(glm::vec4 cameraLimits, glm::vec2 leftClickPosition);

private:
	Texture healthBarContainerTexture;
	Texture healthBarTexture;
	Texture shieldBarContainerTexture;
	Texture shieldBarTexture;
	Texture coinTexture;
	Texture storeHeaderTexture;
	Texture storeHeaderHoveredTexture;
	Texture storeBodyTexture;
	Texture upgradeTexture;
	Texture upgradeAvailableTexture;
	Texture upgradeNotAvailableTexture;

	Sprite* healthBarContainerSprite;
	Sprite* healthBarSprite;
	Sprite* shieldBarContainerSprite;
	Sprite* shieldBarSprite;
	Sprite* coinSprite;
	Sprite* storeHeaderSprite;
	Sprite* storeHeaderHoveredSprite;
	Sprite* storeBodySprite;
	Sprite* upgradeSprite;
	Sprite* upgradeAvailableSprite;
	Sprite* upgradeNotAvailableSprite;

	PlayerRegistrationData playerRegistrationData;
	bool storeOpen;
};