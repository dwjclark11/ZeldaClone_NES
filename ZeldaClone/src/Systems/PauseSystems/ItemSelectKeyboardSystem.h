#pragma once
#include "../../ECS/ECS.h"
#include "../../Events/EventManager.h"
#include "../../Events/KeyPressedEvent.h"
#include "../../Components/TransformComponent.h"
#include "../../Components/SpriteComponent.h"
#include "../../Components/KeyboardControlComponent.h"
#include "../../Components/TextLabelComponent.h"
#include "../../Logger/Logger.h"
#include "../../AssetManager/AssetManager.h"
#include "../../Systems/SoundFXSystem.h"
#include "../../States/GameState.h"
#include "../../Game/LevelLoader.h"
#include "../../Game/Game.h"
#include <string>
#include <SDL.h>


class ItemSelectKeyboardControlSystem : public System
{
private:
	class Game& game;
public:

	ItemSelectKeyboardControlSystem()
		: game(Game::Instance())
	{
		RequiredComponent<TransformComponent>();
		RequiredComponent<SpriteComponent>();

	}

	void SubscribeToEvents(std::unique_ptr<EventManager>& eventManager)
	{
		eventManager->SubscribeToEvent<KeyPressedEvent>(this, &ItemSelectKeyboardControlSystem::OnKeyPressed); // Callback Function
	}

	// Callback function
	void OnKeyPressed(KeyPressedEvent& event);
};

