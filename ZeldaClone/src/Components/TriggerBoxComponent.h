#pragma once
#include <glm/glm.hpp>
#include "../Utilities/Timer.h"

class TriggerBoxComponent
{
public:

	enum class TriggerType
	{
		NO_TRIGGER = 0, SCENE_CHANGE = 1, TRANSPORT = 2, BURN_BUSHES = 3,
		PUSH_ROCKS = 4, COLLECT_ITEM = 5, BOMB_SECRET = 6, LOCKED_DOOR = 7,
		HIDDEN_OBJECT = 8, SHOP_ITEM = 9, LADDER = 10, RAFT = 11,
		MONEY_GAME = 12, TRAP_DOOR = 13, FAIRY_CIRCLE = 14,
	};

	TriggerType triggerType;
	glm::vec2 transportOffset, cameraOffset;

	Timer collectedTimer;

	bool active, collected, collider;
	std::string entityRemoveTag;

	TriggerBoxComponent(TriggerType trigger = TriggerType::NO_TRIGGER, glm::vec2 transport_offset = glm::vec2(0), glm::vec2 camera_offset = glm::vec2(0), bool collider = false, const std::string& entityRemoveTag = "")
		: triggerType{trigger}, transportOffset{ transport_offset }, cameraOffset{camera_offset}
		, collectedTimer{}, active{false}, collected{false}, collider{collider}, entityRemoveTag{entityRemoveTag}
	{
		
	}
};