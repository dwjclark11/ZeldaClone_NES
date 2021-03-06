#pragma once
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
#include <glm/glm.hpp>

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

	enum class Action
	{
		MOVE_UP = 0,
		MOVE_DOWN,
		MOVE_LEFT,
		MOVE_RIGHT,
		ATTACK,
		USE_ITEM,
		PAUSE,
		SELECT,
		CANCEL,
	};

	enum class ItemType
	{
		EMPTY = 0,
		BOOMERANG,
		BOMB,
		POTION_BLUE,
		POTION_RED,
		CANDLE,
		MAGIC_CANDLE,
		WOOD_BOW,
		MAGIC_BOW,
		FOOD,
		WHISTLE,
		MAGIC_ROD
	};

	static ItemType mSelectedItem;

	// Change these from statics and created getters also make them const!!
	// ==================================================
	static int gameScale;
	static int tilePixels;
	static int windowWidth;
	static int windowHeight;
	// ==================================================

	static bool isDebug;

	~Game();
	// Creation of the singleton Game
	static Game& Instance();
	void Shutdown();
	void Run();

	// Getters and Setters of objects to be used through-out the game
	SDL_Renderer* GetRenderer() const { return mRenderer; }
	SDL_Window* GetWindow() { return mWindow; }
	SDL_Rect& GetCamera();

	void SetCameraY(int change);
	void SetCameraX(int change);

	SDL_Rect& GetMouseBox();
	SDL_Event& GetEvent();

	const const bool HasSword() const;
	const bool PlayerHold() const;

	bool& GetAttack();
	
	inline const bool& IsRunning() const { return mIsRunning; }
	inline void SetGameRunning(bool running) { mIsRunning = running; }

	inline const bool& GetCameraMoving() const { return cameraMoving; }
	inline void SetCameraMoving(bool moving) { cameraMoving = moving; }

	
	inline const bool& GetPlayerCreated() const { return playerCreated; }
	inline void SetPlayerCreated(bool created) { playerCreated = created; }

	inline const bool& FadeInStarted()  const { return startFadeIn; }
	inline void StartFadeIn(bool fade) { startFadeIn = fade; }
	
	inline const bool& FadeOutStarted() const { return startFadeOut; }
	inline void StartFadeOut(bool fade) { startFadeOut = fade; }
	
	inline const bool& FadeFinished() const { return fadeFinished; }
	inline void SetFadeFinished(bool finished) { fadeFinished = finished; }

	inline const int& GetLevelWidth()  const { return mLevelWidth; }
	inline const int& GetLevelHeight() const { return mLevelHeight; }

	void FadeScreen();

	inline std::unique_ptr<AssetManager>& GetAssetManager() { return assetManager; }
	inline std::unique_ptr<EventManager>& GetEventManager() { return eventManager; }
	inline std::unique_ptr<GameStateMachine>& GetStateMachine() { return gameStateMachine; }


	inline GameItems& GetGameItems() { return mGameItems; }
	
	const unsigned& GetPlayerNum() const;
	inline void SetPlayerNum(unsigned num) { gamePlayerNum = num; }

	inline const glm::vec2& GetPlayerPos() const { return mPlayerPos; }
	inline void SetPlayerPos(glm::vec2 pos) { mPlayerPos = pos; }

	inline sol::state& GetLuaState() { return lua; }

	inline const bool& GetPlayerItem() const { return playerItem; }
	inline void SetPlayerItem(bool item) { playerItem = item; }

	inline const bool& GetPlayerDead() const { return playerDead; }
	inline void SetPlayerDead(bool dead) { playerDead = dead; }

	inline const bool& GetPlayerOnStairs() const { return onStairs; }
	inline void SetPlayerOnStairs(bool stairs) { onStairs = stairs; }

	inline const bool& GetStairsFinished() const { return stairsFinished; }
	inline void SetStairsFinished(bool finished) { stairsFinished = finished; }

	inline void SetOnRaft(bool raft) { onRaft = raft; }
	inline const bool& GetRaft() const { return onRaft; }
	
	inline void SetFadeAlpha(Uint8 alpha) { fadeAlpha = alpha; }
	inline const Uint8& GetFadeAlpha() const { return fadeAlpha; }
	
	inline const float& GetDeltaTime() const { return deltaTime; }

	void AddGameSecrets(const std::string& locationID, bool found);
	bool HasSecret(const std::string& locationID);
	bool IsSecretFound(const std::string& locationID);
	void SetSecretFound(const std::string& locationID, bool found);
	std::map<std::string, bool> GetGameSecrets() { return gameSecrets; }
	
	StateMachine& GetPlayerStateMachine() { return psm; }

	void AddKeyToMap(Action action, SDL_Keycode key);
	void ChangeKeyBinding(Action action, SDL_Keycode key);
	const std::map<Action, SDL_Keycode>& GetKeyBindings() const { return mMappedKeys; }

	void AddBtnToMap(Action action, SDL_GameControllerButton button);
	void ChangeBtnBinding(Action action, SDL_GameControllerButton button);
	const std::map<Action, SDL_GameControllerButton>& GetBtnBindings() const { return mMappedButtons; }

private:

	bool gamePadButtonDown = false;

	std::map<Action, SDL_Keycode> mMappedKeys;
	std::map<Action, SDL_GameControllerButton> mMappedButtons;
	std::map<std::string, bool> gameSecrets;
	int milliSecondsPreviousFrame;
	int milliSecondsPerFrame;
	bool mIsRunning;
	bool attack;
	bool keydown;
	bool cameraMoving;
	bool playerCreated;
	bool playerItem = false;
	bool playerDead = false;

	bool onStairs = false;
	bool onRaft = false;
	bool stairsFinished = false;

	bool startFadeIn = false;
	bool startFadeOut = false;
	bool fadeFinished = true;

	unsigned gamePlayerNum;

	SDL_Window* mWindow;
	SDL_Renderer* mRenderer;
	float deltaTime;

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

	// Private Function Declarations
	void Initialize();
	void Update();
	void ProcessEvents();
	void Draw();
	void CreateDefaultBtnBindings();
	void CreateDefaultKeyBindings();
};

