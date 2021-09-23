#pragma once
#include "../ECS/ECS.h"
#include "../Components/TextLabelComponent.h"
#include "../Components/TransformComponent.h"
#include "../Components/SpriteComponent.h"
#include "../Components/EditorComponent.h"
#include "../AssetManager/AssetManager.h"
#include "MouseControlSystem.h"
#include <SDL.h>
#include <SDL_ttf.h>

class RenderEditorLabelSystem : public System
{
public:
	RenderEditorLabelSystem()
	{
		RequiredComponent<SpriteComponent>();
		RequiredComponent<TransformComponent>();
		RequiredComponent<EditorComponent>();
	}

	void Update(SDL_Renderer* renderer, const std::unique_ptr<AssetManager>& assetManager, SDL_Rect& camera)
	{
		SDL_Color white = { 255,255,255,255 };
		std::string collision = MouseControlSystem::isCollision ? "Enabled" : "Disabled";
		// ===================================================================================================================================
		// Create Text
		// ===================================================================================================================================
		std::string colliderWidth = "Collider Width:     " + std::to_string(MouseControlSystem::boxColliderWidth);
		std::string colliderHeight = "Collider Height:    " + std::to_string(MouseControlSystem::boxColliderHeight);
		std::string colliderOffsetX = "Collider Offset X:  " + std::to_string(MouseControlSystem::boxColliderOffsetX);
		std::string colliderOffsetY = "Collider Offset Y:  " + std::to_string(MouseControlSystem::boxColliderOffsetY);
		std::string tileScaleX = "Tile Scale X:       " + std::to_string(MouseControlSystem::tileScaleX);
		std::string tileScaleY = "Tile Scale Y:       " + std::to_string(MouseControlSystem::tileScaleY);
		std::string tilePositionText = "Mouse X: " + std::to_string(static_cast<int>(Game::Instance()->GetMouseBox().x + camera.x) / MouseControlSystem::gridSize)
			+ ", Mouse Y: " + std::to_string(static_cast<int>(Game::Instance()->GetMouseBox().y + camera.y) / MouseControlSystem::gridSize);

		std::string collisionText = "Collision: " + collision;
		std::string layerText = "Layer: " + std::to_string(MouseControlSystem::layer);

	/*	std::string triggerText = "Trigger: " + std::to_string(isTrigger);
		std::string trigType = "";
		std::string trigLevelNum = "Level Num: " + triggerLevelNum;*/

		//switch (triggerType)
		//{
		//case NO_TRIGGER:
		//	trigType = "NO_TRIGGER";
		//	break;

		//case SECRET_AREA:
		//	trigType = "SECRET_AREA";
		//	break;

		//case ENTER_DUNGEON:
		//	trigType = "ENTER_DUNGEON";
		//	break;

		//case BURN_BUSHES:
		//	trigType = "BURN_BUSHES";
		//	break;

		//case PUSH_ROCKS:
		//	trigType = "PUSH_ROCKS";
		//	break;

		//case COLLECT_ITEM:
		//	trigType = "CAMERA_RIGHT";
		//	break;

		//case TRAP:
		//	trigType = "TRAP";
		//	break;

		//case HIDDEN_SWITCH:
		//	trigType = "HIDDEN_SWITCH";
		//	break;

		//case HIDDEN_OBJECT:
		//	trigType = "HIDDEN_OBJECT";
		//	break;
		//}


		SDL_Surface* surface1 = TTF_RenderText_Blended(
			assetManager->GetFont("charriot-font-30"),
			colliderWidth.c_str(),
			white);

		SDL_Surface* surface2 = TTF_RenderText_Blended(
			assetManager->GetFont("charriot-font-30"),
			colliderHeight.c_str(),
			white);

		SDL_Surface* surface3 = TTF_RenderText_Blended(
			assetManager->GetFont("charriot-font-30"),
			colliderOffsetX.c_str(),
			white);

		SDL_Surface* surface4 = TTF_RenderText_Blended(
			assetManager->GetFont("charriot-font-30"),
			colliderOffsetY.c_str(),
			white);

		SDL_Surface* surface5 = TTF_RenderText_Blended(
			assetManager->GetFont("charriot-font-30"),
			tileScaleX.c_str(),
			white);

		SDL_Surface* surface6 = TTF_RenderText_Blended(
			assetManager->GetFont("charriot-font-30"),
			tileScaleY.c_str(),
			white);

		SDL_Surface* surface7 = TTF_RenderText_Blended(
			assetManager->GetFont("charriot-font-30"),
			tilePositionText.c_str(),
			white);

		SDL_Surface* surface8 = TTF_RenderText_Blended(
			assetManager->GetFont("charriot-font-30"),
			collisionText.c_str(),
			white);

		SDL_Surface* surface9 = TTF_RenderText_Blended(
			assetManager->GetFont("charriot-font-30"),
			layerText.c_str(),
			white);

		SDL_Texture* texture1 = SDL_CreateTextureFromSurface(renderer, surface1);
		SDL_Texture* texture2 = SDL_CreateTextureFromSurface(renderer, surface2);
		SDL_Texture* texture3 = SDL_CreateTextureFromSurface(renderer, surface3);
		SDL_Texture* texture4 = SDL_CreateTextureFromSurface(renderer, surface4);
		SDL_Texture* texture5 = SDL_CreateTextureFromSurface(renderer, surface5);
		SDL_Texture* texture6 = SDL_CreateTextureFromSurface(renderer, surface6);
		SDL_Texture* texture7 = SDL_CreateTextureFromSurface(renderer, surface7);
		SDL_Texture* texture8 = SDL_CreateTextureFromSurface(renderer, surface8);
		SDL_Texture* texture9 = SDL_CreateTextureFromSurface(renderer, surface9);


		// As soon as we create a texture we can free the surface
		SDL_FreeSurface(surface1);
		SDL_FreeSurface(surface2);
		SDL_FreeSurface(surface3);
		SDL_FreeSurface(surface4);
		SDL_FreeSurface(surface5);
		SDL_FreeSurface(surface6);
		SDL_FreeSurface(surface7);
		SDL_FreeSurface(surface8);
		SDL_FreeSurface(surface9);

		int labelWidth = 0;
		int labelHeight = 0;
		int mouseWidth = 0;
		int mouseHeight = 0;
		int collisionWidth = 0;
		int collisionHeight = 0;
		int layerWidth = 0;
		int layerHeight = 0;

		SDL_QueryTexture(texture1, NULL, NULL, &labelWidth, &labelHeight);
		SDL_QueryTexture(texture2, NULL, NULL, &labelWidth, &labelHeight);
		SDL_QueryTexture(texture3, NULL, NULL, &labelWidth, &labelHeight);
		SDL_QueryTexture(texture4, NULL, NULL, &labelWidth, &labelHeight);
		SDL_QueryTexture(texture5, NULL, NULL, &labelWidth, &labelHeight);
		SDL_QueryTexture(texture6, NULL, NULL, &labelWidth, &labelHeight);
		SDL_QueryTexture(texture7, NULL, NULL, &mouseWidth, &mouseHeight);
		SDL_QueryTexture(texture8, NULL, NULL, &collisionWidth, &collisionHeight);
		SDL_QueryTexture(texture9, NULL, NULL, &layerWidth, &layerHeight);



		SDL_Rect colliderWidthTextRect = {
			100,
			30,
			labelWidth,
			labelHeight
		};

		SDL_Rect colliderHeightTextRect = {
			100,
			60,
			labelWidth,
			labelHeight
		};

		SDL_Rect colliderOffsetXTextRect = {
			100,
			90,
			labelWidth,
			labelHeight
		};

		SDL_Rect colliderOffsetYTextRect = {
			100,
			120,
			labelWidth,
			labelHeight
		};

		SDL_Rect tileScaleXTextRect = {
			100,
			145,
			labelWidth,
			labelHeight
		};

		SDL_Rect tileScaleYTextRect = {
			100,
			175,
			labelWidth,
			labelHeight
		};

		SDL_Rect tilePositionTextRect = {
			100,
			205,
			mouseWidth,
			mouseHeight
		};

		SDL_Rect collisionTextRect = {
			labelWidth + 250,
			30,
			collisionWidth,
			collisionHeight
		};

		SDL_Rect layerTextRect = {
			labelWidth + 250,
			60,
			labelWidth,
			layerHeight
		};

		// ===================================================================================================================================
		// Render Everything
		// ===================================================================================================================================

		SDL_RenderCopy(renderer, texture1, NULL, &colliderWidthTextRect);
		SDL_RenderCopy(renderer, texture2, NULL, &colliderHeightTextRect);
		SDL_RenderCopy(renderer, texture3, NULL, &colliderOffsetXTextRect);
		SDL_RenderCopy(renderer, texture4, NULL, &colliderOffsetYTextRect);
		SDL_RenderCopy(renderer, texture5, NULL, &tileScaleXTextRect);
		SDL_RenderCopy(renderer, texture6, NULL, &tileScaleYTextRect);
		SDL_RenderCopy(renderer, texture7, NULL, &tilePositionTextRect);
		SDL_RenderCopy(renderer, texture8, NULL, &collisionTextRect);
		SDL_RenderCopy(renderer, texture9, NULL, &layerTextRect);

		// Destroy the Textures to Free up Memory
		SDL_DestroyTexture(texture1);
		SDL_DestroyTexture(texture2);
		SDL_DestroyTexture(texture3);
		SDL_DestroyTexture(texture4);
		SDL_DestroyTexture(texture5);
		SDL_DestroyTexture(texture6);
		SDL_DestroyTexture(texture7);
		SDL_DestroyTexture(texture8);
		SDL_DestroyTexture(texture9);
	}
};