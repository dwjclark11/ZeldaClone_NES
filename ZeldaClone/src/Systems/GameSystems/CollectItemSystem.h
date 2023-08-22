#pragma once
#include "../../ECS/ECS.h"

class CollisionEvent;
class EventManager;

class CollectItemSystem : public System
{
private:
	class Game& game;
	class GameData& m_GameData;

	class Registry& reg;

public:
	bool yellowCollected;
	bool blueCollected;
	
	/*
		This is a system that is used to collect basic items in the field,
		Such as rupees, bombs, etc. Those particular items do not require any
		animation change while obtaining.
	*/
	CollectItemSystem();

	void SubscribeToEvents(EventManager& eventManager);
	void OnCollision(CollisionEvent& event);
	void OnPlayerGetsItem(Entity& item, Entity& player);
	void OnBoomerangGetsItem(Entity& item, Entity& boomerang);
	void Update();

};

