#pragma once
#pragma once
#include "../../ECS/ECS.h"
#include "../../Events/EventManager.h"
#include "../../Events/KeyPressedEvent.h"
#include "../../Components/TransformComponent.h"
#include "../../Components/SpriteComponent.h"
#include "../../Components/KeyboardControlComponent.h"
#include "RenderGameOverTextSystem.h"
#include "../../Logger/Logger.h"
#include "../../AssetManager/AssetManager.h"
#include "../Systems/SoundFXSystem.h"
#include "../Systems/GameSystems/HealthSystem.h"
#include "../../States/MenuState.h"
#include "../../States/GameState.h"
#include "GamePadSystem.h"
#include <string>
#include <SDL.h>


class GameOverKeyboardControlSystem : public System
{

public:

	GameOverKeyboardControlSystem()
	{
		RequiredComponent<TransformComponent>();
		RequiredComponent<SpriteComponent>();
	}

	void SubscribeToEvents(std::unique_ptr<EventManager>& eventManager)
	{
		eventManager->SubscribeToEvent<KeyPressedEvent>(this, &GameOverKeyboardControlSystem::OnKeyPressed); // Callback Function
	}

	// Callback function
	void OnKeyPressed(KeyPressedEvent& event)
	{
		for (auto entity : GetSystemEntities())
		{
			auto& sprite = entity.GetComponent<SpriteComponent>();
			auto& transform = entity.GetComponent<TransformComponent>();

			if (entity.HasTag("gameOverSelector"))
			{
				switch (event.symbol)
				{
				case SDLK_UP:
					transform.position.y -= 100;
					Game::Instance()->GetSystem<SoundFXSystem>().PlaySoundFX(Game::Instance()->GetAssetManager(), "text_slow", 0, 1);
					if (transform.position.y < 475) transform.position.y = 675;
					break;

				case SDLK_DOWN:
					transform.position.y += 100;
					Game::Instance()->GetSystem<SoundFXSystem>().PlaySoundFX(Game::Instance()->GetAssetManager(), "text_slow", 0, 1);
					if (transform.position.y > 675) transform.position.y = 475;
					break;

				case SDLK_SPACE:
				{
					if (transform.position.y == 475)
					{
						Game::Instance()->GetPlayerNum() = 1;
						Game::Instance()->GetStateMachine()->PopState();
						Game::Instance()->GetStateMachine()->PushState(new GameState());
					}
					else if (transform.position.y == 575)
					{
						Game::Instance()->GetPlayerNum();
						Game::Instance()->GetStateMachine()->PopState();
						Game::Instance()->GetStateMachine()->PushState(new GameState());
					}
					else if (transform.position.y == 675)
					{
						
						Game::Instance()->GetStateMachine()->PopState();
						Game::Instance()->GetStateMachine()->PushState(new MenuState());
					}
					break;
				}
				default:
					break;
				}
			}
		}
	}


	void Update()
	{
		for (auto entity : GetSystemEntities())
		{
			auto& sprite = entity.GetComponent<SpriteComponent>();
			auto& transform = entity.GetComponent<TransformComponent>();

			if (entity.HasTag("gameOverSelector"))
			{
				if (GamePadSystem::upPressed && !GamePadSystem::buttonDirDown)
				{
					transform.position.y -= 100;
					Game::Instance()->GetSystem<SoundFXSystem>().PlaySoundFX(Game::Instance()->GetAssetManager(), "text_slow", 0, 1);
					if (transform.position.y < 475) transform.position.y = 675;
					GamePadSystem::buttonDirDown = true;
				}

				if (GamePadSystem::downPressed && !GamePadSystem::buttonDirDown)
				{
					transform.position.y += 100;
					Game::Instance()->GetSystem<SoundFXSystem>().PlaySoundFX(Game::Instance()->GetAssetManager(), "text_slow", 0, 1);
					if (transform.position.y > 675) transform.position.y = 475;
					GamePadSystem::buttonDirDown = true;
				}

				if (GamePadSystem::aPressed && !GamePadSystem::buttonDown)
				{
					if (transform.position.y == 475)
					{
						Game::Instance()->GetPlayerDead() = false;
						GameState::firstEntered = false;
						Game::Instance()->GetStateMachine()->PopState();
						Game::Instance()->GetStateMachine()->PushState(new GameState());
					
					}
					else if (transform.position.y == 575)
					{
						Game::Instance()->GetStateMachine()->PopState();
						Game::Instance()->GetStateMachine()->PushState(new GameState());
					}
					else if (transform.position.y == 675)
					{

						Game::Instance()->GetStateMachine()->PopState();
						Game::Instance()->GetStateMachine()->PushState(new MenuState());
					}
					GamePadSystem::buttonDown = true;
				}
			}
		}
	}
};