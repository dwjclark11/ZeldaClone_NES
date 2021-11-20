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
private:
	Game& game;
public:
	RenderEditorLabelSystem()
		: game(*Game::Instance())
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
		std::string tilePositionText = "Mouse X: " + std::to_string(static_cast<int>(game.GetMouseBox().x + camera.x) / MouseControlSystem::gridSize)
			+ ", Mouse Y: " + std::to_string(static_cast<int>(game.GetMouseBox().y + camera.y) / MouseControlSystem::gridSize);

		std::string collisionText = "Collision: " + collision;
		std::string layerText = "Layer: " + std::to_string(MouseControlSystem::layer);
		std::string nesScreenSize = "Zelda Screen Size: (256 x 240) x GameScale";


		// Not implemented yet
		std::string intructions1 = "Move Screen: W = UP, S = DOWN, A = LEFT, D = RIGHT";
		std::string intructions2 = "Add Tile: Mouse Left Click";
		std::string intructions3 = "Remove Tile: Mouse Right Click";
		std::string intructions4 = "To open a previously made tilemap: ";
		std::string intructions5 = "- Load the tile/object image you want to use.";
		std::string intructions6 = "- Load the same image to the mouse.";
		std::string intructions7 = "- Open the correct tilemap file and proceed.";
		std::string intructions8 = "Hit ENTER to open the Selection Screen!";
		std::string intructions9 = "Set the game scale and the mouse box size";
		std::string intructions10 = "Add a box collider if needed and set it's size and offset";


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

		SDL_Surface* surface10 = TTF_RenderText_Blended(
			assetManager->GetFont("charriot-font-30"),
			nesScreenSize.c_str(),
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
		SDL_Texture* texture10 = SDL_CreateTextureFromSurface(renderer, surface10);


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
		SDL_FreeSurface(surface10);

		int labelWidth = 0;
		int labelHeight = 0;
		
		int mouseWidth = 0;
		int mouseHeight = 0;
		
		int collisionWidth = 0;
		int collisionHeight = 0;
		
		int layerWidth = 0;
		int layerHeight = 0;

		int nesSizeWidth = 0;
		int nesSizeHeight = 0;

		SDL_QueryTexture(texture1, NULL, NULL, &labelWidth, &labelHeight);
		SDL_QueryTexture(texture2, NULL, NULL, &labelWidth, &labelHeight);
		SDL_QueryTexture(texture3, NULL, NULL, &labelWidth, &labelHeight);
		SDL_QueryTexture(texture4, NULL, NULL, &labelWidth, &labelHeight);
		SDL_QueryTexture(texture5, NULL, NULL, &labelWidth, &labelHeight);
		SDL_QueryTexture(texture6, NULL, NULL, &labelWidth, &labelHeight);
		SDL_QueryTexture(texture7, NULL, NULL, &mouseWidth, &mouseHeight);
		SDL_QueryTexture(texture8, NULL, NULL, &collisionWidth, &collisionHeight);
		SDL_QueryTexture(texture9, NULL, NULL, &layerWidth, &layerHeight);
		SDL_QueryTexture(texture10, NULL, NULL, &nesSizeWidth, &nesSizeHeight);



		SDL_Rect colliderWidthTextRect = {
			20,
			400 + 30,
			labelWidth,
			labelHeight
		};

		SDL_Rect colliderHeightTextRect = {
			20,
			400 + 70,
			labelWidth,
			labelHeight
		};

		SDL_Rect colliderOffsetXTextRect = {
			20,
			400 + 110,
			labelWidth,
			labelHeight
		};

		SDL_Rect colliderOffsetYTextRect = {
			20,
			400 + 150,
			labelWidth,
			labelHeight
		};

		SDL_Rect tileScaleXTextRect = {
			20,
			400 + 195,
			labelWidth,
			labelHeight
		};

		SDL_Rect tileScaleYTextRect = {
			20,
			400 + 235,
			labelWidth,
			labelHeight
		};

		SDL_Rect tilePositionTextRect = {
			20,
			400 + 275,
			mouseWidth,
			mouseHeight
		};

		SDL_Rect collisionTextRect = {
			20,
			715,
			collisionWidth,
			collisionHeight
		};

		SDL_Rect layerTextRect = {
			20,
			755,
			labelWidth,
			layerHeight
		};

		SDL_Rect nesSizeRect = {
			labelWidth + 320,
			1000,
			nesSizeWidth,
			nesSizeHeight
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
		SDL_RenderCopy(renderer, texture10, NULL, &nesSizeRect);

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
		SDL_DestroyTexture(texture10);
	}
};