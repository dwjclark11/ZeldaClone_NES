#pragma once

#include "../Logger/Logger.h"
#include <SDL.h>
#include <SDL_ttf.h>

#include "../ECS/ECS.h"
#include "../AssetManager/AssetManager.h"
#include "../Components/SpriteComponent.h"
#include "../Components/TransformComponent.h"
#include "../Components/KeyboardControlComponent.h"
#include "../Events/EventManager.h"
#include "../Events/KeyPressedEvent.h"
#include <map>
#include <sstream>
#include <vector>
#include <stack>
#include <fstream>

enum ButtonState
{
	BTN_IDLE = 0,
	BTN_HOVER,
	BTN_ACTIVE
};

class MenuButtonSystem : public System
{
private:

	
public:
	MenuButtonSystem()
	{
		RequiredComponent<SpriteComponent>();
		RequiredComponent<TransformComponent>();
		RequiredComponent<KeyboardControlComponent>();
	}

	void SubscribeToEvents(std::unique_ptr<EventManager>& eventManager)
	{
		eventManager->SubscribeToEvent<KeyPressedEvent>(this, &MenuButtonSystem::OnKeyPressed);
	}

	void OnKeyPressed(KeyPressedEvent& event)
	{

	}

};