#pragma once
#pragma once
#include "../../ECS/ECS.h"
#include "../../Events/EventManager.h"
#include "../../Events/KeyPressedEvent.h"
#include "../../Components/RigidBodyComponent.h"
#include "../../Components/SpriteComponent.h"
#include "../../Components/KeyboardControlComponent.h"
#include "../../Logger/Logger.h"
#include "../../AssetManager/AssetManager.h"
#include <string>
#include <SDL.h>

class AttackSystem : public System
{
public:
	AttackSystem()
	{
		RequiredComponent<RigidBodyComponent>();
		RequiredComponent<KeyboardControlComponent>();
		RequiredComponent<SpriteComponent>();
	}

	void SubscribeToEvents(std::unique_ptr<EventManager>& eventManager)
	{
		eventManager->SubscribeToEvent<KeyPressedEvent>(this, &AttackSystem::OnKeyPressed); // Callback Function
	}
	
	
	/*
		IS THIS EVEN USED ANYWHERE????
	*/
	// Callback function
	void OnKeyPressed(KeyPressedEvent& event)
	{
		if (event.symbol == SDLK_SPACE)
		{
			Logger::Err("ATTACK");
			for (auto entity : GetSystemEntities())
			{
				auto& sprite = entity.GetComponent<SpriteComponent>();

				sprite.srcRect.y = sprite.height * 5;
			}
		}
	}

};