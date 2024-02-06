#include "AISystem.h"
#include "../../Events/EventManager.h"
#include "../../Events/KeyPressedEvent.h"
#include "../../Components/RigidBodyComponent.h"
#include "../../Components/SpriteComponent.h"
#include "../../Components/AIComponent.h"
#include "../../Components/KeyboardControlComponent.h"
#include "../../Components/HealthComponent.h"
#include "../../Logger/Logger.h"
#include "../../AssetManager/AssetManager.h"
#include "../../Game/Game.h"
#include "../../Game/Player.h"
#include <string>
#include <SDL.h>

AISystem::AISystem()
	: m_Game(Game::Instance())
{
	RequiredComponent<AIComponent>();
}

void AISystem::Update()
{
	for (auto& entity : GetSystemEntities())
	{
		auto& stateMachine = entity.GetComponent<AIComponent>();
		// If the AI state machine is nullptr, create one!
		if (!stateMachine.StateMachineCreated() && !stateMachine.IsCreated())
		{
			stateMachine.CreateStateMachine();
			stateMachine.GetEnemyStateMachine().ChangeState(entity);
		}
			
		// If the AI state machine has been created, update the entity AI
		if (stateMachine.StateMachineCreated())
		{
			// If the enemy is in the same screen panel as the player --> Update 
			if (stateMachine.GetEnemyPos() == m_Game.GetPlayer()->GetPlayerPos())
			{
				stateMachine.GetEnemyStateMachine().GetCurrentState()->Update(entity);
			}
		}
	}
}
