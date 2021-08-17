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

		// ===================================================================================================================================
		// Create Text
		// ===================================================================================================================================
		std::string colliderWidth = "Collider Width:     " + std::to_string(MouseControlSystem::boxColliderWidth);
		std::string colliderHeight = "Collider Height:    " + std::to_string(MouseControlSystem::boxColliderHeight);
		std::string colliderOffsetX = "Collider Offset X:  " + std::to_string(MouseControlSystem::boxColliderOffsetX);
		std::string colliderOffsetY = "Collider Offset Y:  " + std::to_string(MouseControlSystem::boxColliderOffsetY);
		std::string tileScaleX = "Tile Scale X:       " + std::to_string(MouseControlSystem::tileScaleX);
		std::string tileScaleY = "Tile Scale Y:       " + std::to_string(MouseControlSystem::tileScaleY);

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

		SDL_Texture* texture1 = SDL_CreateTextureFromSurface(renderer, surface1);
		SDL_Texture* texture2 = SDL_CreateTextureFromSurface(renderer, surface2);
		SDL_Texture* texture3 = SDL_CreateTextureFromSurface(renderer, surface3);
		SDL_Texture* texture4 = SDL_CreateTextureFromSurface(renderer, surface4);
		SDL_Texture* texture5 = SDL_CreateTextureFromSurface(renderer, surface5);
		SDL_Texture* texture6 = SDL_CreateTextureFromSurface(renderer, surface6);


		// As soon as we create a texture we can free the surface
		SDL_FreeSurface(surface1);
		SDL_FreeSurface(surface2);
		SDL_FreeSurface(surface3);
		SDL_FreeSurface(surface4);
		SDL_FreeSurface(surface5);
		SDL_FreeSurface(surface6);

		int labelWidth = 0;
		int labelHeight = 0;

		SDL_QueryTexture(texture1, NULL, NULL, &labelWidth, &labelHeight);
		SDL_QueryTexture(texture2, NULL, NULL, &labelWidth, &labelHeight);
		SDL_QueryTexture(texture3, NULL, NULL, &labelWidth, &labelHeight);
		SDL_QueryTexture(texture4, NULL, NULL, &labelWidth, &labelHeight);
		SDL_QueryTexture(texture5, NULL, NULL, &labelWidth, &labelHeight);
		SDL_QueryTexture(texture6, NULL, NULL, &labelWidth, &labelHeight);



		SDL_Rect colliderWidthTextRect = {
			10,
			10,
			labelWidth,
			labelHeight
		};

		SDL_Rect colliderHeightTextRect = {
			10,
			35,
			labelWidth,
			labelHeight
		};

		SDL_Rect colliderOffsetXTextRect = {
			10,
			60,
			labelWidth,
			labelHeight
		};

		SDL_Rect colliderOffsetYTextRect = {
			10,
			85,
			labelWidth,
			labelHeight
		};

		SDL_Rect tileScaleXTextRect = {
			10,
			110,
			labelWidth,
			labelHeight
		};

		SDL_Rect tileScaleYTextRect = {
			10,
			135,
			labelWidth,
			labelHeight
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

		// Destroy the Textures to Free up Memory
		SDL_DestroyTexture(texture1);
		SDL_DestroyTexture(texture2);
		SDL_DestroyTexture(texture3);
		SDL_DestroyTexture(texture4);
		SDL_DestroyTexture(texture5);
		SDL_DestroyTexture(texture6);
	}

};