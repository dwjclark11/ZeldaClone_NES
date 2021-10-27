#pragma once
#include "../../ECS/ECS.h"
#include "../../Events/EventManager.h"
#include "../../Events/KeyPressedEvent.h"
#include "../../Components/RigidBodyComponent.h"
#include "../../Components/SpriteComponent.h"
#include "../../Components/AIComponent.h"
#include "../../Components/KeyboardControlComponent.h"
#include "../../Logger/Logger.h"
#include "../../AssetManager/AssetManager.h"
#include <string>
#include <SDL.h>

class AISystem : public System
{
public:
	AISystem()
	{
		RequiredComponent<AIComponent>();
	}
	
	void Update()
	{
		for (auto entity : GetSystemEntities())
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
				if (stateMachine.GetEnemyPos() == Game::Instance()->GetPlayerPos())
				{
					stateMachine.GetEnemyStateMachine().Update(entity);
				}
				else
				{
					// Stop enemy movement
					rigidBody.velocity = glm::vec2(0);
				}
			}


			//stateMachine.GetEnemyStateMachine().Update(entity);
		}
	}
};