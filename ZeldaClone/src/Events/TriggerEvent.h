#pragma once
#include "../ECS/ECS.h"
#include "../Events/Event.h"

class TriggerEvent : public Event
{
public:
	Entity a;
	Entity b;

	TriggerEvent() = default;
	TriggerEvent(Entity a, Entity b) : a(a), b(b) {}
};