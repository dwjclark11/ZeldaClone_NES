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
	class GameData& gameData;

	bool CheckInventory(ItemComponent::SpecialItemType& item);
	void SetInventory(ItemComponent::SpecialItemType& item);
	void StopPlayerMovement(Entity& player, Entity& trigger);
	
	float angle;
	float heartOffset;
	void ChangeScene(Entity& player, Entity& trigger);
	void PushRocks(Entity& player, Entity& trigger);
	void RideRaft(Entity& player, Entity& trigger);
	void Transport(Entity& player, Entity& trigger);
	void UseLadder(Entity& player, Entity& trigger);
	void BombSecret(Entity& trigger);
	void UnlockDoor(Entity& trigger);
	void BuyShopItem(Entity& trigger);
	void EnterFairyCircle(Entity& player, Entity& trigger);
	void UpdateFairyCircle(Entity& player, Entity& trigger);
	void UpdateRaft(Entity& player, Entity& trigger);
	void UpdateMovingRocks(Entity& player, Entity& trigger);
	void UpdateLadder(Entity& player, Entity& trigger);
public:

	TriggerSystem();
	void SubscribeToEvents(std::unique_ptr<EventManager>& eventManager);
	void OnTrigger(class CollisionEvent& event);
	void OnEnterTrigger(Entity& player, Entity& trigger);
	void Update(const float& dt);

	void SecretTrigger(Entity& trigger, bool startup = false);
};