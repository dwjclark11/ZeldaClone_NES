#pragma once
#include "../../ECS/ECS.h"
#include "../../Game/LevelLoader.h"

class EventManager;
class TriggerSystem : public System
{
private:
	sol::state lua;
	LevelLoader loader;
	class Game& game;

	bool CheckInventory(SpecialItemType& item);
	void SetInventory(SpecialItemType& item);

public:

	TriggerSystem();
	void SubscribeToEvents(std::unique_ptr<EventManager>& eventManager);
	void OnTrigger(class CollisionEvent& event);
	void OnEnterTrigger(Entity& player, Entity& trigger);


	void SecretTrigger(Entity& trigger, bool startup = false);
};