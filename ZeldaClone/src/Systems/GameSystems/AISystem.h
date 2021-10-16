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
		int i = 0;
		for (auto entity : GetSystemEntities())
		{
			auto& stateMachine = entity.GetComponent<AIComponent>();
			auto& rigidBody = entity.GetComponent<RigidBodyComponent>();
			if (entity.HasTag("player"))
				Game::Instance()->GetPlayerStateMachine().Update(entity);
			else
			{
				if (stateMachine.GetEnemyPos() == Game::Instance()->GetPlayerPos())
				{
					i++;
					//Logger::Err("The enemy and the player are in the same sceen -- " + std::to_string(i));
					stateMachine.GetEnemyStateMachine()->Update(entity);
				}
				else
				{
					rigidBody.velocity = glm::vec2(0);
				}
			}
		}
		i = 0;
	}
};