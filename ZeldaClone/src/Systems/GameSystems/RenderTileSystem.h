#pragma once

struct SDL_Renderer;
struct SDL_Rect;
class AssetManager;
#include "../../ECS/ECS.h"

class RenderTileSystem : public System
{
private:
	class Game& game;

public:
	RenderTileSystem();

	void Update(SDL_Renderer* renderer, std::unique_ptr<AssetManager>& assetManager, SDL_Rect& camera);
			
	void OnExit();

};