#pragma once
#include <glm/glm.hpp>
#include "../Utilities/Timer.h"



class TriggerBoxComponent
{
public:

	enum class TriggerType
	{
		NO_TRIGGER = 0,
		SECRET_AREA = 1,
		TRANSPORT = 2,
		BURN_BUSHES = 3,
		PUSH_ROCKS = 4,
		COLLECT_ITEM = 5,
		BOMB_SECRET = 6,
		LOCKED_DOOR = 7,
		HIDDEN_OBJECT = 8,
		SHOP_ITEM = 9
	};

	TriggerType triggerType;
	glm::vec2 transportOffset;
	glm::vec2 cameraOffset;
	std::string levelMusic;
	std::string assetFile;
	std::string enemyFile;
	std::string colliderFile;
	std::string tileMapName;
	std::string tileImageName;
	std::string entityFileName;
	std::string triggerFile;

	Timer collectedTimer;

	int imageHeight;
	int imageWidth;
	bool active;
	bool collected;
	bool collider;

	TriggerBoxComponent(TriggerType trigger = TriggerType::NO_TRIGGER, glm::vec2 offset = glm::vec2(0), glm::vec2 camera_offset = glm::vec2(0),
		std::string levelMusic = "no_file", std::string assetFile = "no_file", std::string enemyFile = "no_file", std::string colliderFile = "no_file",
		std::string tileMapName = "no_file", std::string tileImageName = "no_file", std::string entityFileName = "no_file", int imageWidth = 0, int imageHeight = 0, std::string triggerFile = "no_file", bool collider = false)
	{
		this->triggerType = trigger;
		this->transportOffset = offset;
		this->cameraOffset = camera_offset;
		this->levelMusic = levelMusic;
		this->assetFile = assetFile;
		this->enemyFile = enemyFile;
		this->colliderFile = colliderFile;
		this->tileMapName = tileMapName;
		this->tileImageName = tileImageName;
		this->entityFileName = entityFileName;
		this->triggerFile = triggerFile;
		this->imageHeight = imageHeight;
		this->imageWidth = imageWidth;
		this->active = false;
		this->collected = false;
		this->collider = collider;
	}
};