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
private:
	Game& game;
public:

	GameOverKeyboardControlSystem()
		: game(*Game::Instance())
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
					game.GetSystem<SoundFXSystem>().PlaySoundFX(game.GetAssetManager(), "text_slow", 0, 1);
					if (transform.position.y < 475) transform.position.y = 675;
					break;

				case SDLK_DOWN:
					transform.position.y += 100;
					game.GetSystem<SoundFXSystem>().PlaySoundFX(game.GetAssetManager(), "text_slow", 0, 1);
					if (transform.position.y > 675) transform.position.y = 475;
					break;

				case SDLK_SPACE:
				{
					if (transform.position.y == 475)
					{
						game.GetplayerCreated() = false;
						game.GetPlayerDead() = false;
						HealthSystem::numHearts = 3;
						game.GetStateMachine()->PopState();
						game.GetStateMachine()->PushState(new GameState());
					}
					else if (transform.position.y == 575)
					{
						game.GetPlayerNum();
						game.GetStateMachine()->PopState();
						game.GetStateMachine()->PushState(new GameState());
					}
					else if (transform.position.y == 675)
					{
						
						game.GetStateMachine()->PopState();
						game.GetStateMachine()->PushState(new MenuState());
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
					game.GetSystem<SoundFXSystem>().PlaySoundFX(game.GetAssetManager(), "text_slow", 0, 1);
					if (transform.position.y < 475) transform.position.y = 675;
					GamePadSystem::buttonDirDown = true;
				}

				if (GamePadSystem::downPressed && !GamePadSystem::buttonDirDown)
				{
					transform.position.y += 100;
					game.GetSystem<SoundFXSystem>().PlaySoundFX(game.GetAssetManager(), "text_slow", 0, 1);
					if (transform.position.y > 675) transform.position.y = 475;
					GamePadSystem::buttonDirDown = true;
				}

				if (GamePadSystem::aPressed && !GamePadSystem::buttonDown)
				{
					if (transform.position.y == 475)
					{
						game.GetPlayerDead() = false;
						GameState::firstEntered = false;
						game.GetStateMachine()->PopState();
						game.GetStateMachine()->PushState(new GameState());
					
					}
					else if (transform.position.y == 575)
					{
						game.GetStateMachine()->PopState();
						game.GetStateMachine()->PushState(new GameState());
					}
					else if (transform.position.y == 675)
					{

						game.GetStateMachine()->PopState();
						game.GetStateMachine()->PushState(new MenuState());
					}
					GamePadSystem::buttonDown = true;
				}
			}
		}
	}
};