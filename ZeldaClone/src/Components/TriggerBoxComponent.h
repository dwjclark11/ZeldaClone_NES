#pragma once
#include <glm/glm.hpp>
#include "../Utilities/Timer.h"

enum class TriggerType
{
	NO_TRIGGER = 0, SCENE_CHANGE = 1, TRANSPORT = 2, BURN_BUSHES = 3,
	PUSH_ROCKS = 4, COLLECT_ITEM = 5, BOMB_SECRET = 6, LOCKED_DOOR = 7,
	HIDDEN_OBJECT = 8, SHOP_ITEM = 9, LADDER = 10, RAFT = 11,
	MONEY_GAME = 12, TRAP_DOOR = 13, FAIRY_CIRCLE = 14,
};

struct TriggerBoxComponent
{
	TriggerType triggerType{ TriggerType::NO_TRIGGER };
	glm::vec2 transportOffset{ 0.f }, cameraOffset{ 0.f };
	std::string entityRemoveTag{ "" };
	bool collider{ false }, active{ false }, collected{ false };
	
	Timer collectedTimer{ };
};