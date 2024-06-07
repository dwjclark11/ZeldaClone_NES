#pragma once
#include "ECS/ECS.h"

struct RenderEditorSystem : public System
{
	RenderEditorSystem();

	void Update( struct SDL_Renderer* renderer, std::unique_ptr<class AssetManager>& assetManager,
				 struct SDL_Rect& camera );
	void OnExit();
};