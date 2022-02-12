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
#include "../../Components/RigidBodyComponent.h"
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

	if (a.BelongsToGroup("items") && b.HasTag("player")) OnPlayerGetsItem(a, b);
	if (b.BelongsToGroup("items") && a.HasTag("player")) OnPlayerGetsItem(b, a);
	
	// Test for boomerang retrieving items
	if (a.BelongsToGroup("items") && b.BelongsToGroup("boomerang")) OnBoomerangGetsItem(a, b);
	if (b.BelongsToGroup("items") && a.BelongsToGroup("boomerang")) OnBoomerangGetsItem(b, a);
}

void CollectItemSystem::OnPlayerGetsItem(Entity& item, Entity& player)
{
	auto& type = item.GetComponent<ItemComponent>().type;

	if (type == YELLOW_RUPEE)
	{
		GameState::scrollRupees += 1;
		item.Kill();
	}
	else if (type == BLUE_RUPEE)
	{
		GameState::scrollRupees += 5;
		item.Kill();
	}
	else if (type == BOMBS)
	{
		GameState::totalBombs += 3;
		Registry::Instance().GetSystem<SoundFXSystem>().PlaySoundFX(game.GetAssetManager(), "get_item", 0, 1);
		item.Kill();
	}
	else if (type == HEARTS)
	{
		auto& health = player.GetComponent<HealthComponent>();
		health.healthPercentage += 2;

		// Clamp health to the maxHealth --> Create Variable?
		if (health.healthPercentage >= health.maxHearts * 2)
			health.healthPercentage = health.maxHearts * 2;

		Registry::Instance().GetSystem<SoundFXSystem>().PlaySoundFX(game.GetAssetManager(), "get_item", 0, 1);
		item.Kill();
	}
}

void CollectItemSystem::OnBoomerangGetsItem(Entity& item, Entity& boomerang)
{
	auto& itemTransform = item.GetComponent<TransformComponent>();
	auto& boomerangTransform = boomerang.GetComponent<TransformComponent>();
	auto& boomerangRigid = boomerang.GetComponent<RigidBodyComponent>();
	
	itemTransform.position = boomerangTransform.position;
}

void CollectItemSystem::Update()
{
	for (auto& entity : GetSystemEntities())
	{
		auto& trigger = entity.GetComponent<TriggerBoxComponent>();
		if (trigger.collected && trigger.collectedTimer.GetTicks() > 2000)
		{
			entity.Kill();
		}
		else
			continue;
	}
}
