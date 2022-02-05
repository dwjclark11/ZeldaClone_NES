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
#include "../StateMachines/NewStateMachine.h"

// Forward Declarations
class AssetManager;

// Define constants
const int FPS = 60;
const int MILLISECONDS_PER_FRAME = 1000 / FPS;



class Game
{
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

public:
	static int gameScale;
	static int tilePixels;
	static int windowWidth;
	static int windowHeight;
	static bool isDebug;

	~Game();

	
	// Creation of the singleton Game
	static Game& Instance();

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
	
	const bool HasSword();
	bool PlayerHold();
	
	bool& GetAttack();
	inline bool& IsRunning() { return mIsRunning; }
	inline bool& GetCameraMoving(){ return cameraMoving;}
	inline bool& GetGameRunning(){ return mIsRunning;}
	inline bool& GetplayerCreated() { return playerCreated; }
	
	inline bool& StartFadeIn() { return startFadeIn; }
	inline bool& StartFadeOut() { return startFadeOut; }
	inline bool& FadeFinished() { return fadeFinished; }

	inline int& GetLevelWidth() { return mLevelWidth; }
	inline int& GetLevelHeight() { return mLevelHeight; }
	
	void FadeScreen();

	inline std::unique_ptr<AssetManager>& GetAssetManager() { return assetManager; }
	inline std::unique_ptr<EventManager>& GetEventManager() { return eventManager; }
	inline std::unique_ptr<GameStateMachine>& GetStateMachine() { return gameStateMachine; }
	
	
	inline GameItems& GetGameItems() { return mGameItems; }
	unsigned& GetPlayerNum();
	inline glm::vec2& GetPlayerPos() { return mPlayerPos; }
	double& GetDeltaTime();
	inline sol::state& GetLuaState() { return lua; }
	inline bool& GetPlayerItem() { return playerItem; }
	inline bool& GetPlayerDead() { return playerDead; }
	inline bool GetPlayerOnStairs() { return onStairs; }
	inline void SetPlayerOnStairs(bool stairs) { onStairs = stairs; }

	inline bool GetStairsFinished() { return stairsFinished; }
	inline void SetStairsFinished(bool finished) { stairsFinished = finished; }

	inline void SetFadeAlpha(Uint8 alpha) { fadeAlpha = alpha; }
	inline Uint8 GetFadeAlpha() { return fadeAlpha; }
	inline const double& GetDeltaTime() const { return deltaTime; }

	void AddGameSecrets(const std::string& locationID, bool found);
	bool HasSecret(const std::string& locationID);
	bool IsSecretFound(const std::string& locationID);
	void SetSecretFound(const std::string& locationID, bool found);
	std::map<std::string, bool> GetGameSecrets() { return gameSecrets; }
	template<typename TSystem> TSystem& GetSystem();

	int milliSecondsPreviousFrame;
	
	StateMachine& GetPlayerStateMachine() { return psm; }
private:
	int milliSecondsPerFrame;
	bool mIsRunning;
	bool attack;
	bool keydown;
	bool cameraMoving;
	bool playerCreated;
	bool playerItem = false;
	bool playerDead = false;

	bool onStairs = false;
	bool stairsFinished = false;

	bool startFadeIn = false;
	bool startFadeOut = false;
	bool fadeFinished = true;
	
	unsigned gamePlayerNum;
	
	SDL_Window* mWindow;
	SDL_Renderer* mRenderer;
	double deltaTime;

	SDL_Rect camera;
	SDL_Rect mouseBox;
	SDL_Event sdlEvent;
	Uint8 fadeAlpha;
	
	std::unique_ptr<GameStateMachine> gameStateMachine;
	std::unique_ptr<AssetManager> assetManager;
	std::unique_ptr<EventManager> eventManager;
	
	static std::unique_ptr<Game> mInstance;
	
	
	GameItems mGameItems;
	
	glm::vec2 mPlayerPos;
	int mLevelWidth;
	int mLevelHeight;

	StateMachine psm;
	sol::state lua;

	// Constructor
	Game();

	std::map<std::string, bool> gameSecrets;
};

// Why is this needed?
template<typename TSystem>
TSystem& Game::GetSystem()
{
	return Registry::Instance().GetSystem<TSystem>();
}
