#include "AISystem.h"
#include "../../Events/EventManager.h"
#include "../../Events/KeyPressedEvent.h"
#include "../../Components/RigidBodyComponent.h"
#include "../../Components/SpriteComponent.h"
#include "../../Components/AIComponent.h"
#include "../../Components/KeyboardControlComponent.h"
#include "../../Logger/Logger.h"
#include "../../AssetManager/AssetManager.h"
#include "../../Game/Game.h"
#include <string>
#include <SDL.h>

AISystem::AISystem()
	: game(*Game::Instance())
{
	RequiredComponent<AIComponent>();
}

void AISystem::Update()
{
	for (auto& entity : GetSystemEntities())
	{
		auto& stateMachine = entity.GetComponent<AIComponent>();
		auto& rigidBody = entity.GetComponent<RigidBodyComponent>();

		// If the AI state machine is nullptr, create one!
		if (!stateMachine.StateMachineCreated())
			stateMachine.CreateStateMachine();

		// If the AI state machine has been created, update the entity AI
		if (stateMachine.StateMachineCreated())
		{
			// If the enemy is in the same screen panel as the player --> Update 
			if (stateMachine.GetEnemyPos() == game.GetPlayerPos())
			{
				stateMachine.GetEnemyStateMachine().Update(entity);
			}
			else
			{
				// Stop enemy movement if not on the same panel is player
				rigidBody.velocity = glm::vec2(0);
			}
		}
	}
}
