#pragma once

#include "../../ECS/ECS.h"

class AssetManager;
struct SDL_Renderer;

const int OFFSET = 720;

class RenderHUDSystem : public System
{
private:
	class Game& game;
public:
	bool pause;
	bool gamePlay;

	RenderHUDSystem();

	void Update(SDL_Renderer* renderer, std::unique_ptr<AssetManager>& assetManager);
	
	void OnExit();

};
