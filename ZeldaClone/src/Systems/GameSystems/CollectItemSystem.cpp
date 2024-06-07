#include "Systems/GameSystems/CollectItemSystem.h"
#include "Components/HealthComponent.h"
#include "Components/BoxColliderComponent.h"
#include "Components/TriggerBoxComponent.h"
#include "Components/AnimationComponent.h"
#include "Components/TransformComponent.h"
#include "Components/ProjectileComponent.h"
#include "Components/PlayerComponent.h"
#include "Components/RupeeGameComponent.h"
#include "Components/ItemComponent.h"
#include "Components/SpriteComponent.h"
#include "Components/RigidBodyComponent.h"
#include "Events/EventManager.h"
#include "Events/CollisionEvent.h"
#include "Game/Game.h"
#include "Game/Player.h"
#include "Utilities/GameData.h"
#include "Systems/SoundFXSystem.h"
#include "States/GameState.h"

CollectItemSystem::CollectItemSystem()
	: m_Game( Game::Instance() )
	, m_GameData( GameData::GetInstance() )
	, m_Registry( Registry::Instance() )
{
	RequiredComponent<BoxColliderComponent>();
	RequiredComponent<TriggerBoxComponent>();
	RequiredComponent<ItemComponent>();
}

void CollectItemSystem::SubscribeToEvents( EventManager& eventManager )
{
	eventManager.SubscribeToEvent<CollisionEvent>( this, &CollectItemSystem::OnCollision );
}

void CollectItemSystem::OnCollision( CollisionEvent& event )
{
	Entity a = event.a;
	Entity b = event.b;

	if ( a.HasComponent<ItemComponent>() && b.HasComponent<PlayerComponent>() )
	{
		OnPlayerGetsItem( a );
		return;
	}
	else if ( b.HasComponent<ItemComponent>() && a.HasComponent<PlayerComponent>() )
	{
		OnPlayerGetsItem( b );
		return;
	}

	// Test for boomerang retrieving items
	if ( a.HasComponent<ItemComponent>() && b.HasTag( "boomerang" ) )
		OnBoomerangGetsItem( a, b );
	else if ( b.HasComponent<ItemComponent>() && a.HasTag( "boomerang" ) )
		OnBoomerangGetsItem( b, a );
}

void CollectItemSystem::OnPlayerGetsItem( Entity item )
{
	auto& itemComp = item.GetComponent<ItemComponent>();
	if ( itemComp.bCollected )
		return;

	bool bPlaySound{ true };
	switch ( itemComp.type )
	{
	case ItemCollectType::YELLOW_RUPEE: {
		m_GameData.AddRupees( 1 );
		bPlaySound = false;
		break;
	}
	case ItemCollectType::BLUE_RUPEE: {
		m_GameData.AddRupees( 5 );
		bPlaySound = false;
		break;
	}
	case ItemCollectType::BOMBS: {
		m_GameData.AddBombs( 3 );
		break;
	}
	case ItemCollectType::HEARTS: {
		auto& health = m_Game.GetPlayer()->GetPlayer().GetComponent<HealthComponent>();
		health.healthPercentage += 2;

		// Clamp health to the maxHealth --> Create Variable?
		if ( health.healthPercentage >= health.maxHearts * 2 )
			health.healthPercentage = health.maxHearts * 2;
		break;
	}
	case ItemCollectType::KEYS: {
		m_GameData.AddKeys( 1 );
		break;
	}
	case ItemCollectType::DEFAULT:
		if ( itemComp.special != SpecialItemType::NOT_SPECIAL )
			return;

	default: assert( false && "Item has not been defined!" ); break;
	}

	if ( bPlaySound )
		m_Game.GetSoundPlayer().PlaySoundFX( "get_item", 0, SoundChannel::ANY );

	itemComp.bCollected = true;
	item.Kill();
}

void CollectItemSystem::OnBoomerangGetsItem( Entity& item, Entity& boomerang )
{
	auto& itemTransform = item.GetComponent<TransformComponent>();
	auto& boomerangTransform = boomerang.GetComponent<TransformComponent>();
	auto& boomerangRigid = boomerang.GetComponent<RigidBodyComponent>();

	itemTransform.position = boomerangTransform.position;
}

void CollectItemSystem::Update()
{
	for ( auto& entity : GetSystemEntities() )
	{
		auto& trigger = entity.GetComponent<TriggerBoxComponent>();

		if ( !trigger.bCollected )
			continue;

		if ( trigger.bCollected && trigger.collectedTimer.GetTicks() > trigger.collectTime )
		{
			entity.Kill();
			m_Game.GetPlayer()->SetPlayerItem( false );
		}
	}
}
