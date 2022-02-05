#include "CollectItemSystem.h"
#include "../../Components/HealthComponent.h"
#include "../../Components/BoxColliderComponent.h"
#include "../../Components/TriggerBoxComponent.h"
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

CollectItemSystem::CollectItemSystem()
	: game(Game::Instance()), reg(Registry::Instance())
{
	RequiredComponent<BoxColliderComponent>();
	RequiredComponent<TriggerBoxComponent>();

	yellowCollected = false;
	blueCollected = false;
}

void CollectItemSystem::SubscribeToEvents(std::unique_ptr<EventManager>& eventManager)
{
	eventManager->SubscribeToEvent<CollisionEvent>(this, &CollectItemSystem::OnCollision);
}

void CollectItemSystem::OnCollision(CollisionEvent& event)
{
	Entity a = event.a;
	Entity b = event.b;

	if ((a.BelongsToGroup("money") || a.BelongsToGroup("items") && b.HasTag("player"))) OnPlayerGetsItem(a, b);
	if ((b.BelongsToGroup("money") || b.BelongsToGroup("items") && a.HasTag("player"))) OnPlayerGetsItem(b, a);
}

void CollectItemSystem::OnPlayerGetsItem(Entity& item, Entity& player)
{
	if (item.BelongsToGroup("money") && player.HasTag("player"))
	{
		auto& type = item.GetComponent<RupeeTypeComponent>();

		if (type.type == YELLOW)
		{
			GameState::scrollRupees += 1;
			item.Kill();
		}
		if (type.type == BLUE)
		{
			GameState::scrollRupees += 5;
			item.Kill();
		}
	}
	else if (item.BelongsToGroup("items"))
	{

		auto& type = item.GetComponent<ItemComponent>();

		if (type.type == BOMBS)
		{
			GameState::totalBombs += 3;
			game.GetSystem<SoundFXSystem>().PlaySoundFX(game.GetAssetManager(), "get_item", 0, 1);
			item.Kill();
		}
		else if (type.type == HEARTS)
		{
			auto& health = player.GetComponent<HealthComponent>();
			health.healthPercentage += 2;

			// Clamp health to the maxHealth --> Create Variable?
			if (health.healthPercentage >= health.maxHearts * 2)
				health.healthPercentage = health.maxHearts * 2;

			game.GetSystem<SoundFXSystem>().PlaySoundFX(game.GetAssetManager(), "get_item", 0, 1);
			item.Kill();
		}
	}
}

void CollectItemSystem::Update()
{
	for (auto& entity : GetSystemEntities())
	{
		auto& trigger = entity.GetComponent<TriggerBoxComponent>();
		if (trigger.collected && trigger.collectedTimer.GetTicks() > 2000)
		{
			Logger::Log("Killed");
			entity.Kill();
		}
		else
			continue;
	}
}
