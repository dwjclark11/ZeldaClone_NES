#pragma once
#include "../../ECS/ECS.h"
#include <SDL.h>
#include <algorithm>
#include <vector>


class Game;
class AIComponent;
class AssetManager;
struct TransformComponent;
struct PlayerComponent;
struct GameComponent;
struct SpriteComponent;


struct RenderSystem : public System
{
	RenderSystem()
	{
		// Entities polled must have these component
		RequiredComponent<TransformComponent>();
		RequiredComponent<SpriteComponent>();
		RequiredComponent<GameComponent>();
	}

	void Update(SDL_Renderer* renderer, std::unique_ptr<AssetManager>& assetManager, SDL_Rect& camera);

	void OnExit();

};