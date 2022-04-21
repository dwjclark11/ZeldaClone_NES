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

	bool CheckInventory(ItemComponent::SpecialItemType& item);
	void SetInventory(ItemComponent::SpecialItemType& item);
	void StopPlayerMovement(Entity& player, Entity& trigger);
	
	float angle;
	float heartOffset;

public:

	TriggerSystem();
	void SubscribeToEvents(std::unique_ptr<EventManager>& eventManager);
	void OnTrigger(class CollisionEvent& event);
	void OnEnterTrigger(Entity& player, Entity& trigger);
	void Update(const float& dt);

	void SecretTrigger(Entity& trigger, bool startup = false);
};