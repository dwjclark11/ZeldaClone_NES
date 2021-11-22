#pragma once
#include "../../ECS/ECS.h"

class AssetManager;
struct SDL_Renderer;
struct SDL_Rect;

struct RenderSystem : public System
{
	RenderSystem();

	void Update(SDL_Renderer* renderer, std::unique_ptr<AssetManager>& assetManager, SDL_Rect& camera);

	void OnExit();
};