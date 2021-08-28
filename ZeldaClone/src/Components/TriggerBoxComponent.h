#pragma once
#include <glm/glm.hpp>

enum TriggerType
{
	NO_TRIGGER = 0,
	SECRET_AREA = 1,
	ENTER_DUNGEON = 2,
	BURN_BUSHES = 3,
	PUSH_ROCKS = 4,
	CAMERA_RIGHT = 5,
	TRAP = 6,
	HIDDEN_SWITCH = 7,
	HIDDEN_OBJECT = 8,
	RETURN_WORLD = 9
};

struct TriggerBoxComponent
{
	TriggerType triggerType;
	glm::vec2 transportOffset;
	glm::vec2 cameraOffset;
	std::string level;

	TriggerBoxComponent(TriggerType trigger = NO_TRIGGER, glm::vec2 offset = glm::vec2(0), glm::vec2 camera_offset = glm::vec2(0), std::string level = "")
	{
		this->triggerType = trigger;
		this->transportOffset = offset;
		this->cameraOffset = camera_offset;
		this->level = level;
	}
};