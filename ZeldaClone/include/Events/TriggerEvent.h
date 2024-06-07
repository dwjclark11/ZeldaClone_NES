#pragma once
#include "ECS/ECS.h"
#include "Events/Event.h"

struct TriggerEvent : public Event
{
	Entity a{};
	Entity b{};
};