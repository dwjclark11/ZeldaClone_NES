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
			OnPlayerTriggerA(a, b);
		}
		if (b.BelongsToGroup("trigger") && a.HasTag("player"))
		{
			OnPlayerTriggerB(a, b);	
		}
	}

	void OnPlayerTriggerA( Entity trigger, Entity player)
	{
		auto& trig = trigger.GetComponent<TriggerBoxComponent>();
		auto& transform = player.GetComponent<TransformComponent>();
		auto& rigidbody = player.GetComponent<RigidBodyComponent>();
		Logger::Log("trig num: " + std::to_string(trig.triggerType));

		switch (trig.triggerType)
		{
		case NO_TRIGGER:
			Logger::Log("No Trig?");
			break;

		case SECRET_AREA:
			Logger::Log("Secret");
			Game::Instance()->GetSystem<SoundFXSystem>().PlaySoundFX(Game::Instance()->GetAssetManager(), "stairs", 0, 1);
			transform.position.x = trig.transportOffset.x;
			transform.position.y = trig.transportOffset.y;
			Game::Instance()->GetStateMachine()->PopState();
			Game::Instance()->GetStateMachine()->PushState(new DungeonState(trig.level));
			break;

		case ENTER_DUNGEON:
			Logger::Log("Dungeon");
			Game::Instance()->GetSystem<SoundFXSystem>().PlaySoundFX(Game::Instance()->GetAssetManager(), "stairs", 0, 1);
			transform.position.x = trig.transportOffset.x;
			transform.position.y = trig.transportOffset.y;
			Game::Instance()->GetStateMachine()->PopState();
			Game::Instance()->GetStateMachine()->PushState(new DungeonState(trig.level));
			break;

		case BURN_BUSHES:

			break;

		case PUSH_ROCKS:

			break;

		case CAMERA_RIGHT:

			break;

		case RETURN_WORLD:
			Logger::Err("In The Trigger");
			Game::Instance()->GetSystem<SoundFXSystem>().PlaySoundFX(Game::Instance()->GetAssetManager(), "stairs", 0, 1);
			transform.position = trig.transportOffset;
			Game::Instance()->GetLevelWidth() = 16;
			Game::Instance()->GetLevelHeight() = 8;
			Game::Instance()->GetStateMachine()->PopState();
			Game::Instance()->GetStateMachine()->PushState(new GameState(trig.cameraOffset));
			break;
		default:
			
			break;
		}
	}
	void OnPlayerTriggerB(Entity player, Entity trigger)
	{
		auto& trig = trigger.GetComponent<TriggerBoxComponent>();
		auto& transform = player.GetComponent<TransformComponent>();
		auto& rigidbody = player.GetComponent<RigidBodyComponent>();
		Logger::Log("trig num: " + std::to_string(trig.triggerType));
		switch (trig.triggerType)
		{
		case NO_TRIGGER:
			break;

		case SECRET_AREA:

			Game::Instance()->GetSystem<SoundFXSystem>().PlaySoundFX(Game::Instance()->GetAssetManager(), "stairs", 0, 1);
			transform.position.x = trig.transportOffset.x;
			transform.position.y = trig.transportOffset.y;
			Game::Instance()->GetStateMachine()->PopState();
			Game::Instance()->GetStateMachine()->PushState(new DungeonState(trig.level));
			break;

		case ENTER_DUNGEON:

			break;

		case BURN_BUSHES:

			break;

		case PUSH_ROCKS:

			break;

		case CAMERA_RIGHT:

			break;

		case RETURN_WORLD:
			Game::Instance()->GetSystem<SoundFXSystem>().PlaySoundFX(Game::Instance()->GetAssetManager(), "stairs", 0, 1);
			transform.position = trig.transportOffset;

			Game::Instance()->GetStateMachine()->PopState();
			Game::Instance()->GetStateMachine()->PushState(new GameState(trig.cameraOffset));
			break;
		default:

			break;
		}
	}
};
