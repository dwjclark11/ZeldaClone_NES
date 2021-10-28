#pragma once
#define SDL_MAIN_HANDLED
#include <SDL.h>
#include <SDL_image.h>
#include <imgui/imgui.h>
#include <imgui/imgui_sdl.h>
#include <imgui/imgui_impl_sdl.h>
#include <sol/sol.hpp>
#include <iostream>
#include "../ECS/ECS.h"
#include "../Logger/Logger.h"
#include "../Events/EventManager.h"
#include "../Events/GamePadButtonPressedEvent.h"
#include "../States/GameStateMachine.h"
#include "../Systems/GameSystems/RenderSystem.h"
#include "../Systems/RenderTitleSystem.h"
#include "../Systems/MusicPlayerSystem.h"
#include "../Systems/RenderCollisionSystem.h"
#include "../Systems/GameSystems/AnimationSystem.h"
#include "../Systems/RemoveEntitySystem.h"
#include "../Systems/GameSystems/AttackSystem.h"
#include "../StateMachines/EnemyStateMachine.h"
#include "../StateMachines/PlayerStateMachine.h"

// Forward Declarations
class AssetManager;

// Define constants
const int FPS = 60;
const int MILLISECONDS_PER_FRAME = 1000 / FPS;

struct GameItems
{
	bool woodSword;
	bool steelSword;
	bool magicSword;
	bool woodBoomerang;
	bool magicBoomerang;
	bool magicRod;
	bool bombs;
	bool flute;
	bool food;
	bool raft;
	bool ladder;
	bool blueRing;
	bool redRing;
	bool bow;
	bool magicBow;
	bool shield;
	bool magicShield;
	bool map;
	bool powerBraclet;
	bool masterKey;
	bool bluePotion;
	bool redPotion;
	bool candle;
};

class Game
{
public:
	static int gameScale;
	static int tilePixels;
	static int windowWidth;
	static int windowHeight;
	static bool isDebug;
	
	// Constructor / Destructor
	Game();
	~Game();
	
	// Creation of the singleton Game
	static Game* Instance()
	{
		if (pInstance == nullptr)
		{
			pInstance = new Game();
		}
		return pInstance;
	}
	
	// Function Declarations
	void Initialize();
	void Update();
	void ProcessEvents();
	void Draw();
	void Shutdown();
	void Run();
	
	// Getters and Setters of objects to be used through-out the game
	SDL_Renderer* GetRenderer() const { return mRenderer; }
	SDL_Window* GetWindow() { return mWindow;  }
	SDL_Rect& GetCamera();
	
	void SetCameraY(int change);
	void SetCameraX(int change);
	
	SDL_Rect& GetMouseBox();
	SDL_Event& GetEvent();
	
	const bool HasSword() 
	{ 
		if (mGameItems.woodSword || mGameItems.steelSword || mGameItems.magicSword)
			return true;
		
		return false;
	}
	bool& GetAttack();
	bool& IsRunning() { return mIsRunning; }
	bool& GetCameraMoving(){ return cameraMoving;}
	bool& GetGameRunning(){ return mIsRunning;}
	bool& GetplayerCreated() { return playerCreated; }
	int& GetLevelWidth() { return mLevelWidth; }
	int& GetLevelHeight() { return mLevelHeight; }
	GameStateMachine* GetStateMachine() { return gameStateMachine; }
	
	std::unique_ptr<AssetManager>& GetAssetManager() { return assetManager; } 
	std::unique_ptr<EventManager>& GetEventManager() { return eventManager; }
	
	GameItems& GetGameItems() { return mGameItems; }
	unsigned& GetPlayerNum();
	glm::vec2& GetPlayerPos() { return mPlayerPos; }
	double& GetDeltaTime();
	sol::state& GetLuaState() { return lua; }
	bool& GetPlayerItem() { return playerItem; }
	bool& GetPlayerDead() { return playerDead; }

	template<typename TSystem> TSystem& GetSystem();

	int milliSecondsPreviousFrame;

	EnemyStateMachine& GetEnemyStateMachine() { return esm; }
	PlayerStateMachine& GetPlayerStateMachine() { return psm; }

private:
	int milliSecondsPerFrame;
	bool mIsRunning;
	bool attack;
	bool keydown;
	bool cameraMoving;
	bool playerCreated;
	bool playerItem = false;
	bool playerDead = false;
	unsigned gamePlayerNum;
	
	SDL_Window* mWindow;
	SDL_Renderer* mRenderer;
	GameStateMachine* gameStateMachine;
	static Game* pInstance;
	double deltaTime;

	SDL_Rect camera;
	SDL_Rect mouseBox;
	SDL_Event sdlEvent;

	std::unique_ptr<AssetManager> assetManager;
	std::unique_ptr<EventManager> eventManager;
	GameItems mGameItems;
	
	glm::vec2 mPlayerPos;
	int mLevelWidth;
	int mLevelHeight;

	EnemyStateMachine esm;
	PlayerStateMachine psm;

	sol::state lua;
};


template<typename TSystem>
TSystem& Game::GetSystem()
{
	return Registry::Instance()->GetSystem<TSystem>();
}
