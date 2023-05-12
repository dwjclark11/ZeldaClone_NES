#include "CollectItemSystem.h"
#include "../../Components/HealthComponent.h"
#include "../../Components/BoxColliderComponent.h"
#include "../../Components/TriggerBoxComponent.h"
#include "../../Components/AnimationComponent.h"
#include "../../Components/TransformComponent.h"
#include "../../Components/ProjectileComponent.h"
#include "../../Components/PlayerComponent.h"
#include "../../Components/RupeeGameComponent.h"
#include "../../Components/ItemComponent.h"
#include "../../Components/SpriteComponent.h"
#include "../../Components/RigidBodyComponent.h"
#include "../../Events/EventManager.h"
#include "../../Events/CollisionEvent.h"
#include "../../Game/Game.h"
#include "../../Game/Player.h"
#include "../../Utilities/GameData.h"
#include "../SoundFXSystem.h"
#include "../../States/GameState.h"

CollectItemSystem::CollectItemSystem()
	: game(Game::Instance()), m_GameData(GameData::GetInstance()), reg(Registry::Instance())
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

	if (a.HasComponent<ItemComponent>() && b.HasComponent<PlayerComponent>()) OnPlayerGetsItem(a, b);
	if (b.HasComponent<ItemComponent>() && a.HasComponent<PlayerComponent>()) OnPlayerGetsItem(b, a);
	
	// Test for boomerang retrieving items
	if (a.HasComponent<ItemComponent>() && b.HasTag("boomerang")) OnBoomerangGetsItem(a, b);
	if (b.HasComponent<ItemComponent>() && a.HasTag("boomerang")) OnBoomerangGetsItem(b, a);
}

void CollectItemSystem::OnPlayerGetsItem(Entity& item, Entity& player)
{
	auto& type = item.GetComponent<ItemComponent>().type;

	if (type == ItemComponent::ItemCollectType::YELLOW_RUPEE)
	{
		m_GameData.AddRupees(1);
		item.Kill();
	}
	else if (type == ItemComponent::ItemCollectType::BLUE_RUPEE)
	{
		m_GameData.AddRupees(5);
		item.Kill();
	}
	else if (type == ItemComponent::ItemCollectType::BOMBS)
	{
		m_GameData.AddBombs(3);
		game.GetSoundPlayer().PlaySoundFX("get_item", 0, SoundChannel::COLLECT);
		item.Kill();
	}
	else if (type == ItemComponent::ItemCollectType::HEARTS)
	{
		auto& health = player.GetComponent<HealthComponent>();
		health.healthPercentage += 2;

		// Clamp health to the maxHealth --> Create Variable?
		if (health.healthPercentage >= health.maxHearts * 2)
			health.healthPercentage = health.maxHearts * 2;

		game.GetSoundPlayer().PlaySoundFX("get_item", 0, SoundChannel::COLLECT);
		item.Kill();
	}
	else if (type == ItemComponent::ItemCollectType::KEYS)
	{
		m_GameData.AddKeys(1);
		game.GetSoundPlayer().PlaySoundFX("get_item", 0, SoundChannel::COLLECT);
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
		int time = 2000;
		if (entity.HasComponent<ItemComponent>())
		{
			auto& item = entity.GetComponent<ItemComponent>();
			if (item.special == ItemComponent::SpecialItemType::TRIFORCE_PIECE)
			{
				time = 9000;
			}
		}
		if (trigger.collected && trigger.collectedTimer.GetTicks() > time)
		{
			entity.Kill();
			game.GetPlayer()->SetPlayerItem(false);
		}
		else
			continue;
	}
}
