#pragma once
#include "../../ECS/ECS.h"

const int OFFSET = 720;

class RenderHUDSystem : public System
{
private:
	class Game& game;
public:
	bool pause;
	bool gamePlay;

	RenderHUDSystem();

	void Update(struct SDL_Renderer* renderer, std::unique_ptr<class AssetManager>& assetManager);
	void OnExit();

};
