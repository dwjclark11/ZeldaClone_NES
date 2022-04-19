#pragma once
#include <glm/glm.hpp>
#include "../Utilities/Timer.h"

class TriggerBoxComponent
{
public:

	enum class TriggerType
	{
		NO_TRIGGER = 0,
		SCENE_CHANGE = 1,
		TRANSPORT = 2,
		BURN_BUSHES = 3,
		PUSH_ROCKS = 4,
		COLLECT_ITEM = 5,
		BOMB_SECRET = 6,
		LOCKED_DOOR = 7,
		HIDDEN_OBJECT = 8,
		SHOP_ITEM = 9,
		LADDER = 10,
		RAFT = 11,
		MONEY_GAME = 12,
		TRAP_DOOR = 13,
	};

	TriggerType triggerType;
	glm::vec2 transportOffset;
	glm::vec2 cameraOffset;

	Timer collectedTimer;

	bool active;
	bool collected;
	bool collider;

	std::string entityRemoveTag;

	TriggerBoxComponent(TriggerType trigger = TriggerType::NO_TRIGGER, glm::vec2 offset = glm::vec2(0), glm::vec2 camera_offset = glm::vec2(0), bool collider = false, std::string entityRemoveTag = "")
	{
		this->triggerType = trigger;
		this->transportOffset = offset;
		this->cameraOffset = camera_offset;

		this->active = false;
		this->collected = false;
		this->collider = collider;
		this->entityRemoveTag = entityRemoveTag;
	}
};