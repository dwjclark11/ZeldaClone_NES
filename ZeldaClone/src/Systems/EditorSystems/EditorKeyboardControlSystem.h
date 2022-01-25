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
	class Game& game;
public:
	EditorKeyboardControlSystem()
		: game(*Game::Instance())
	{
		// These are magic numbers That should not be used!
		levelWidth = MouseControlSystem::CanvasWidth;
		levelHeight = MouseControlSystem::CanvasHeight;
	}

	void SubscribeToEvents(std::unique_ptr<EventManager>& eventManager)
	{
		eventManager->SubscribeToEvent<KeyPressedEvent>(this, &EditorKeyboardControlSystem::OnKeyPressed); // use the callback function
	}

	void OnKeyPressed(KeyPressedEvent& event)
	{
		switch (event.symbol)
		{
		// Move the camera UP
		case SDLK_w:
			game.SetCameraY(-16); // Why 16? Should be based on the sprite_size*scale
			break;
		// Move the camera RIGHT
		case SDLK_d:
			game.SetCameraX(16);
			break;
		// Move the camera DOWN
		case SDLK_s:
			game.SetCameraY(16);
			break;
		// Move the camera LEFT
		case SDLK_a:
			game.SetCameraX(-16);
			break;
		// Toggle the Collision
		case SDLK_c:
			MouseControlSystem::isCollision = !MouseControlSystem::isCollision; // Also changed with the editor ImGui!!
			break;
		// Change the layer of the Object/Tile/Enemy etc!
		case SDLK_l:
			MouseControlSystem::layer++;
			break;

		case SDLK_k:
			MouseControlSystem::layer--;
			break;
		// Toggle the trigger
		case SDLK_t:
			MouseControlSystem::isTrigger = !MouseControlSystem::isTrigger;
			break;
		// I don't think this works yet!
		case SDLK_F1:
			FileDialogs::SaveFile();
			break;
		// Change the type of trigger --> This will be removed when drop-down list is funcitonal
		case SDLK_m:
		/*	if(MouseControlSystem::triggerType == SECRET_AREA || MouseControlSystem::triggerType == ENTER_DUNGEON)
				MouseControlSystem::triggerLevelNum++;*/
			break;
		// Same as above
		case SDLK_n:
			/*if (MouseControlSystem::triggerType == SECRET_AREA || MouseControlSystem::triggerType == ENTER_DUNGEON)
				MouseControlSystem::triggerLevelNum--;
			if (MouseControlSystem::triggerLevelNum-- < 0)
				MouseControlSystem::triggerLevelNum = 0;*/
			break;
		// This will also be removed with the implementation of the drop-list stated above
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
				MouseControlSystem::triggerType = COLLECT_ITEM;
				break;
			case 6:
				MouseControlSystem::triggerType = BOMB_SECRET;
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

		// These are magic numbers That should not be used!
		levelWidth = MouseControlSystem::CanvasWidth;
		levelHeight = MouseControlSystem::CanvasHeight;


		// Clamps for Game Camera and Image Src Rec!! --> This needs to be investigated further
		if (game.GetCamera().x < -1000) game.GetCamera().x = -1000;
		if (game.GetCamera().y < -1000) game.GetCamera().y = -1000;
		if (game.GetCamera().x > (levelWidth - game.GetCamera().w ) + 1000) game.GetCamera().x = (levelWidth - game.GetCamera().w) + 1000;
		if (game.GetCamera().y > (levelHeight - game.GetCamera().h) + 1000) game.GetCamera().y = (levelHeight - game.GetCamera().h) + 1000;

		if (MouseControlSystem::layer < 0) MouseControlSystem::layer = 0;
		if (MouseControlSystem::layer > 10) MouseControlSystem::layer = 10;
	}

	void Update()
	{

	}

};