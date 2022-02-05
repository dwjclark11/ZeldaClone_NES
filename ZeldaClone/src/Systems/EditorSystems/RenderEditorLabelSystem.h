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
		: game(Game::Instance())
	{
		RequiredComponent<SpriteComponent>();
		RequiredComponent<TransformComponent>();
		RequiredComponent<EditorComponent>();
	}

	void Update(SDL_Renderer* renderer, const std::unique_ptr<AssetManager>& assetManager, SDL_Rect& camera)
	{
		// Every Thing Here IS JUST A HACK TO KEEP IMGUI FROM GHOSTING!!! Not sure why it works  but if I take it out it causes ghosting
		// and other issues!!
		SDL_Color white = { 255,255,255,255 };
		
		std::string nesScreenSize = "L";

		SDL_Surface* surface10 = TTF_RenderText_Blended(
			assetManager->GetFont("charriot-font-30"),
			nesScreenSize.c_str(),
			white);

		SDL_Texture* texture10 = SDL_CreateTextureFromSurface(renderer, surface10);

		SDL_FreeSurface(surface10);

		int labelWidth = 0;
		int labelHeight = 0;
		
		int nesSizeWidth = 0;
		int nesSizeHeight = 0;

		SDL_QueryTexture(texture10, NULL, NULL, &nesSizeWidth, &nesSizeHeight);

		SDL_Rect nesSizeRect = {
			0,
			1050,
			nesSizeWidth,
			nesSizeHeight
		};

		SDL_RenderCopy(renderer, texture10, NULL, &nesSizeRect);

		SDL_DestroyTexture(texture10);
	}
};