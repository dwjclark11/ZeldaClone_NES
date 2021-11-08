#include "../../ECS/ECS.h"
#include "../../Components/SpriteComponent.h"
#include "../../Components/TransformComponent.h"
#include "../../Components/SaveComponent.h" 
#include "../../Events/EventManager.h"
#include "../../Events/KeyPressedEvent.h"
#include "../../Game/LevelLoader.h"
#include "../../States/MenuState.h"
#include "../../Game/Game.h"
#include <Windows.h>

class SaveSelectKeyboardSystem : public System
{
private:
	class Game& game;
public:
	
	SaveSelectKeyboardSystem()
		: game(*Game::Instance())
	{
		RequiredComponent<SpriteComponent>();
		RequiredComponent<TransformComponent>();
		RequiredComponent<SaveComponent>();
	}
	
	void SubscribeToEvents(std::unique_ptr<EventManager>& eventManager)
	{
		eventManager->SubscribeToEvent<KeyPressedEvent>(this, &SaveSelectKeyboardSystem::OnKeyPressed);
	}
	
	void OnKeyPressed(KeyPressedEvent& event)
	{
		for (auto entity : GetSystemEntities())
		{
			if (entity.HasTag("save_selector"))
			{
				auto& transform = entity.GetComponent<TransformComponent>();

				switch (event.symbol)
				{
				case SDLK_UP:
					transform.position.y -= 200;
					if (transform.position.y < 185) transform.position.y = 585;
					break;

				case SDLK_DOWN:
					transform.position.y += 200;
					if (transform.position.y > 585) transform.position.y = 185;
					break;

				case SDLK_RETURN:
				{
					if (transform.position.y == 185)
					{
						LevelLoader loader;

						if (game.GetPlayerNum() == 1)
						{
							loader.SavePlayerDataToLuaTable("1", game.GetAssetManager(), game.GetRenderer());
						}
						else if (game.GetPlayerNum() == 2)
						{
							loader.SavePlayerDataToLuaTable("2", game.GetAssetManager(), game.GetRenderer());
						}
						else if (game.GetPlayerNum() == 3)
						{
							loader.SavePlayerDataToLuaTable("3", game.GetAssetManager(), game.GetRenderer());
						}

						//Entity saving = Registry::Instance()->CreateEntity();
						//saving.AddComponent<SpriteComponent>("save_gui", 96, 16, 1, true, 0, 0);
						//saving.AddComponent<TransformComponent>(glm::vec2(400, 100), glm::vec2(2, 2), 0.0);
						//saving.AddComponent<AnimationComponent>(2, 5);
						//saving.AddComponent<SaveComponent>();
						Sleep(1000);
						game.GetStateMachine()->PopState();
					}
					else if (transform.position.y == 385)
					{
						State::exitToMain = true;
						//game.GetStateMachine()->ClearStates();
						//game.GetStateMachine()->PushState(new MenuState());
					}
					else if (transform.position.y == 585)
					{
						game.GetGameRunning() = false;
					}
					break;
				}
				default:
					break;
				}
			}
		}
	}
};