#include "Systems/GameSystems/AISystem.h"
#include "Events/EventManager.h"
#include "Events/KeyPressedEvent.h"
#include "Components/RigidBodyComponent.h"
#include "Components/SpriteComponent.h"
#include "Components/AIComponent.h"
#include "Components/KeyboardControlComponent.h"
#include "Components/HealthComponent.h"
#include "Logger/Logger.h"
#include "AssetManager/AssetManager.h"
#include "Game/Game.h"
#include "Game/Player.h"

#include "StateMachines/NewEnemyStates.h"
#include "StateMachines/BossStates.h"

#include <memory>
#include <string>
#include <SDL.h>

AISystem::AISystem()
	: m_Game( Game::Instance() )
{
	RequiredComponent<AIComponent>();
}

void AISystem::Update()
{
	for ( auto& entity : GetSystemEntities() )
	{
		auto& ai = entity.GetComponent<AIComponent>();

		if ( !ai.esm )
		{
			ai.esm = std::make_shared<StateMachine>();

			if ( !ai.bIsBoss )
				ai.esm->AddState( std::make_unique<EnemyIdleState>() );
			else
				ai.esm->AddState( std::make_unique<BossIdleState>() );

			ai.esm->ChangeState( entity );
		}

		if ( ai.enemyPos == m_Game.GetPlayer()->GetPlayerPos() )
		{
			ai.esm->GetCurrentState()->Update( entity );
		}
	}
}
