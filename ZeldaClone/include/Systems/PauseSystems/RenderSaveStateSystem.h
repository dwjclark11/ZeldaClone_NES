#pragma once
#include "ECS/ECS.h"

struct RenderSaveStateSystem : public System
{
	RenderSaveStateSystem();

	void Update( struct SDL_Renderer* renderer, std::unique_ptr<class AssetManager>& assetManager );
	void OnExit();
};