#pragma once
#include "../../ECS/ECS.h"

struct SDL_Renderer;
struct SDL_Rect;
class AssetManager;


class RenderTileSystem : public System
{
private:
	class Game& game;

public:
	RenderTileSystem();

	void Update(SDL_Renderer* renderer, std::unique_ptr<AssetManager>& assetManager, SDL_Rect& camera);
			
	void OnExit();

};