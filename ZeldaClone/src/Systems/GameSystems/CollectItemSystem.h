#pragma once
#include "../../ECS/ECS.h"
#include "../../Components/HealthComponent.h"
#include "../../Components/BoxColliderComponent.h"
#include "../../Components/AnimationComponent.h"
#include "../../Components/TransformComponent.h"
#include "../../Components/ProjectileComponent.h"
#include "../../Components/RupeeTypeComponent.h"
#include "../../Components/ItemComponent.h"
#include "../../Components/SpriteComponent.h"
#include "../../Events/EventManager.h"
#include "../../Events/CollisionEvent.h"
#include "../../Game/Game.h"
#include "../SoundFXSystem.h"
#include "../../States/GameState.h"

class CollectItemSystem : public System
{
private:

public:
	bool yellowCollected;
	bool blueCollected;
	
	/*
		This is a system that is used to collect basic items in the field,
		Such as rupees, bombs, etc. Those particular items do not require any
		animation change while obtaining.
	*/
	
	CollectItemSystem()
	{
		RequiredComponent<BoxColliderComponent>();
		//RequiredComponent<ItemComponent>();
		yellowCollected = false;
		blueCollected = false;
	}

	void SubscribeToEvents(std::unique_ptr<EventManager>& eventManager)
	{
		eventManager->SubscribeToEvent<CollisionEvent>(this, &CollectItemSystem::OnCollision);
	}

	void OnCollision(CollisionEvent& event)
	{
		Entity a = event.a;
		Entity b = event.b;
		
		if ((a.BelongsToGroup("money") || a.BelongsToGroup("items") && b.HasTag("player"))) OnPlayerGetsItem(a, b);
		if ((b.BelongsToGroup("money") || b.BelongsToGroup("items") && a.HasTag("player"))) OnPlayerGetsItem(b, a);
	}

	void OnPlayerGetsItem(Entity& item, Entity& player)
	{
		if (item.BelongsToGroup("money") && player.HasTag("player"))
		{
			auto& type = item.GetComponent<RupeeTypeComponent>();

			if (type.type == YELLOW)
			{
				GameState::totalRupees += 1;
				Game::Instance()->GetSystem<SoundFXSystem>().PlaySoundFX(Game::Instance()->GetAssetManager(), "get_rupee", 0, 1);
				Registry::Instance()->RemoveEntityFromSystems(item);
			}
			if (type.type == BLUE)
			{
				GameState::totalRupees += 5;
				Game::Instance()->GetSystem<SoundFXSystem>().PlaySoundFX(Game::Instance()->GetAssetManager(), "get_rupee", 0, 1);
				Registry::Instance()->RemoveEntityFromSystems(item);
			}
		}
		else if (item.BelongsToGroup("items"))
		{

			auto& type = item.GetComponent<ItemComponent>();

			if (type.type == BOMBS)
			{
				GameState::totalBombs += 3;
				Game::Instance()->GetSystem<SoundFXSystem>().PlaySoundFX(Game::Instance()->GetAssetManager(), "get_item", 0, 1);
				item.Kill();
			}
			else if (type.type == HEARTS)
			{
				auto& health = player.GetComponent<HealthComponent>();
				health.healthPercentage += 2;
				Game::Instance()->GetSystem<SoundFXSystem>().PlaySoundFX(Game::Instance()->GetAssetManager(), "get_item", 0, 1);
				Registry::Instance()->RemoveEntityFromSystems(item);
			}
		}
	}

	void Update()
	{

	}
};

