#pragma once
#include "../ECS/ECS.h"
#include "../Components/TransformComponent.h"
#include "../Components/BoxColliderComponent.h"
#include "../Components/TriggerBoxComponent.h"
#include "../Components/HealthComponent.h"
#include "../Events/CollisionEvent.h"
#include "../Events/TriggerEvent.h"
#include "../Events/EventManager.h"
#include "../../Game/Game.h"
#include <cmath>
#include "../../Utilities/Timer.h"
#include "../../States/DungeonState.h"

class TriggerSystem : public System
{
public: 

	TriggerSystem()
	{
		RequiredComponent<TransformComponent>();
		RequiredComponent<BoxColliderComponent>();
	}

	void SubscribeToEvents(std::unique_ptr<EventManager>& eventManager)
	{
		eventManager->SubscribeToEvent<CollisionEvent>(this, &TriggerSystem::OnTrigger);
	}

	void OnTrigger(CollisionEvent& event)
	{
		Entity a = event.a;
		Entity b = event.b;
		
		// Check to see if the player has activated a trigger
		if (a.BelongsToGroup("trigger") && b.HasTag("player"))
		{
			OnPlayerTrigger(a, b);
		}
		if (b.BelongsToGroup("trigger") && a.HasTag("player"))
		{
			OnPlayerTrigger(b, a);	
		}
	}

	void OnPlayerTrigger( Entity trigger, Entity player)
	{
		auto& trig = trigger.GetComponent<TriggerBoxComponent>();
		auto& transform = player.GetComponent<TransformComponent>();
		auto& rigidbody = player.GetComponent<RigidBodyComponent>();
	
		switch (trig.triggerType)
		{
		case NO_TRIGGER:
			break;

		case SECRET_AREA:
			if (trig.levelNum == 1)
				Logger::Err("LevelNum: " + std::to_string(trig.levelNum));
			Game::Instance()->GetSystem<SoundFXSystem>().PlaySoundFX(Game::Instance()->GetAssetManager(), "stairs", 0, 1);
			transform.position.x = 480;
			transform.position.y = 900;
			Game::Instance()->GetStateMachine()->PopState();
			Game::Instance()->GetStateMachine()->PushState(new DungeonState());
			break;

		case ENTER_DUNGEON:
	
			break;

		case BURN_BUSHES:

			break;

		case PUSH_ROCKS:

			break;

		case CAMERA_RIGHT:

			break;
		default:
			
			break;
		}
	}
};
