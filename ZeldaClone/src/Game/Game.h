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
#include "../Systems/SoundFXSystem.h"
#include "../Systems/RenderCollisionSystem.h"
#include "../Systems/GameSystems/AnimationSystem.h"
#include "../StateMachines/NewStateMachine.h"
#include <glm/glm.hpp>
#include "../inputs/Keyboard.h"

// Forward Declarations
class AssetManager;
class Camera;
class Player;

// Define constants
const int FPS = 60;
const int MILLISECONDS_PER_FRAME = 1000 / FPS;

class Game
{
private:
	int milliSecondsPreviousFrame;
	int milliSecondsPerFrame;
	bool m_bIsRunning;
	class InputManager& m_InputManager;

	SDL_Window* m_pWindow;
	SDL_Renderer* m_pRenderer;
	float m_DeltaTime;

	SDL_Rect mouseBox;
	SDL_Event m_SdlEvent;

	std::unique_ptr<GameStateMachine> m_pGameStateMachine;
	std::unique_ptr<AssetManager> m_pAssetManager;
	std::unique_ptr<EventManager> m_pEventManager;
	std::unique_ptr<MusicPlayer> m_pMusicPlayer;
	std::unique_ptr<SoundFX> m_pSoundPlayer;

	std::unique_ptr<Camera> m_Camera;
	std::unique_ptr<Player> m_pPlayer;

	static std::unique_ptr<Game> mInstance;
	sol::state m_LuaState;

	int m_GameScale, m_TilePixels, m_WindowWidth, m_WindowHeight;
	bool m_bDebug;

	// Constructor
	Game();

	// Private Function Declarations
	void Initialize();
	void Update();
	void ProcessEvents();
	void Draw();
public:
	~Game();
	// Creation of the singleton Game
	static Game& Instance();
	void Shutdown();
	void Run();

	// Getters and Setters of objects to be used through-out the game
	SDL_Renderer* GetRenderer() const { return m_pRenderer; }
	SDL_Window* GetWindow() { return m_pWindow; }
	Camera& GetCamera() { return *m_Camera; };
	SDL_Rect& GetMouseBox();
	SDL_Event& GetEvent();

	const bool PlayerHold() const;
	std::unique_ptr<Player>& CreateNewPlayer(Entity& player, Entity& sword, Entity& shield);
	std::unique_ptr<Player>& GetPlayer() { return m_pPlayer; };

	inline const bool& IsRunning() const { return m_bIsRunning; }
	inline void SetGameRunning(bool running) { m_bIsRunning = running; }

	inline const int GetWindowWidth() const { return m_WindowWidth; }
	inline const int GetWindowHeight() const { return m_WindowHeight; }

	inline std::unique_ptr<AssetManager>& GetAssetManager() { return m_pAssetManager; }
	inline std::unique_ptr<EventManager>& GetEventManager() { return m_pEventManager; }
	inline std::unique_ptr<GameStateMachine>& GetStateMachine() { return m_pGameStateMachine; }
	MusicPlayer& GetMusicPlayer() { return *m_pMusicPlayer; }
	SoundFX& GetSoundPlayer() { return *m_pSoundPlayer; }

	inline sol::state& GetLuaState() { return m_LuaState; }		
	inline const float& GetDeltaTime() const { return m_DeltaTime; }
	
	const bool IsDebugging() const { return m_bDebug; }
	void SetDebug(bool debug) { m_bDebug = debug; }

	const int GetGameScale() const { return m_GameScale; }
	void SetGameScale(int scale) { m_GameScale = scale; }
};
