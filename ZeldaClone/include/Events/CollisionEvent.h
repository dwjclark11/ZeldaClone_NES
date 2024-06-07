#pragma once
#include "ECS/ECS.h"
#include "Events/EventManager.h"

struct CollisionEvent : public Event
{
	Entity a{};
	Entity b{};
};