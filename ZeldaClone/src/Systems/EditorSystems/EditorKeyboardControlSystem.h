#pragma once

#include "../ECS/ECS.h"
#include "../Events/EventManager.h"
#include "../Events/KeyPressedEvent.h"
#include "../Components/KeyboardControlComponent.h"
#include <SDL.h>
#include "../Logger/Logger.h"
#include <string>
#include "MouseControlSystem.h"
#include "../Utilities/FileManagerUtils.h"
#include "../Game/Game.h"

class EditorKeyboardControlSystem : public System
{
private:
	int levelWidth;
	int levelHeight;
public:
	EditorKeyboardControlSystem()
	{
		levelWidth = 16384;
		levelHeight = 8000;
	}

	void SubscribeToEvents(std::unique_ptr<EventManager>& eventManager)
	{
		eventManager->SubscribeToEvent<KeyPressedEvent>(this, &EditorKeyboardControlSystem::OnKeyPressed); // use the callback function
	}

	void OnKeyPressed(KeyPressedEvent& event) // Callback function
	{
		switch (event.symbol)
		{
		case SDLK_w:
			Game::Instance()->SetCameraY(-16);
			break;

		case SDLK_d:
			Game::Instance()->SetCameraX(16);
			break;

		case SDLK_s:
			Game::Instance()->SetCameraY(16);
			break;

		case SDLK_a:
			Game::Instance()->SetCameraX(-16);
			break;

		case SDLK_c:
			MouseControlSystem::isCollision = !MouseControlSystem::isCollision;
			break;

		case SDLK_l:
			MouseControlSystem::layer++;
			break;

		case SDLK_k:
			MouseControlSystem::layer--;
			break;

		case SDLK_t:
			MouseControlSystem::isTrigger = !MouseControlSystem::isTrigger;
			break;

		case SDLK_F1:
			FileDialogs::SaveFile();
			break;

		case SDLK_m:
			if(MouseControlSystem::triggerType == SECRET_AREA || MouseControlSystem::triggerType == ENTER_DUNGEON)
				MouseControlSystem::triggerLevelNum++;
			break;

		case SDLK_n:
			if (MouseControlSystem::triggerType == SECRET_AREA || MouseControlSystem::triggerType == ENTER_DUNGEON)
				MouseControlSystem::triggerLevelNum--;
			if (MouseControlSystem::triggerLevelNum-- < 0)
				MouseControlSystem::triggerLevelNum = 0;
			break;

		case SDLK_SPACE:
			MouseControlSystem::triggerNum += 1;
			if (MouseControlSystem::triggerNum > 8) MouseControlSystem::triggerNum = 0;

			switch (MouseControlSystem::triggerNum)
			{
			case 0:
				MouseControlSystem::triggerType = NO_TRIGGER;
				break;
			case 1:
				MouseControlSystem::triggerType = SECRET_AREA;
				break;
			case 2:
				MouseControlSystem::triggerType = ENTER_DUNGEON;
				break;
			case 3:
				MouseControlSystem::triggerType = BURN_BUSHES;
				break;
			case 4:
				MouseControlSystem::triggerType = PUSH_ROCKS;
				break;
			case 5:
				MouseControlSystem::triggerType = CAMERA_RIGHT;
				break;
			case 6:
				MouseControlSystem::triggerType = TRAP;
				break;
			case 7:
				MouseControlSystem::triggerType = HIDDEN_SWITCH;
				break;
			case 8:
				MouseControlSystem::triggerType = HIDDEN_OBJECT;
				break;
			}
		default:
			break;
		}

		// Clamps for Game Camera and Image Src Rec!!
		if (Game::Instance()->GetCamera().x < -100) Game::Instance()->GetCamera().x = -100;
		if (Game::Instance()->GetCamera().y < -100) Game::Instance()->GetCamera().y = -100;
		if (Game::Instance()->GetCamera().x > levelWidth - Game::Instance()->GetCamera().w) Game::Instance()->GetCamera().x = levelWidth - Game::Instance()->GetCamera().w;
		if (Game::Instance()->GetCamera().y > levelHeight - Game::Instance()->GetCamera().h) Game::Instance()->GetCamera().y = levelHeight - Game::Instance()->GetCamera().h;

		if (MouseControlSystem::layer < 0) MouseControlSystem::layer = 0;
		if (MouseControlSystem::layer > 10) MouseControlSystem::layer = 10;
	}

	void Update()
	{

	}

};