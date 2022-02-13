#pragma once
#include "../ECS/ECS.h"

class RenderTextSystem : public System
{
private:
	class Game& game;
public: 
	RenderTextSystem();
	
	void Update(struct SDL_Renderer* renderer, std::unique_ptr<class AssetManager>& assetManager, const struct SDL_Rect& camera);
	void OnExit();
	void OnExitSettings();
};