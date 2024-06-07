#pragma once
#include "Utilities/Timer.h"
#include <glm/glm.hpp>

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
	FAIRY_CIRCLE = 14,
};

struct TriggerBoxComponent
{
	TriggerType triggerType{ TriggerType::NO_TRIGGER };
	glm::vec2 transportOffset{ 0.f }, cameraOffset{ 0.f };
	std::string entityRemoveTag{ "" };
	bool bCollider{ false }, bActive{ false }, bCollected{ false };
	int collectTime{ 2000 };
	Timer collectedTimer{};
};