#pragma once
#include <SDL.h>
#include <SDL_image.h>
#include <imgui.h>
#include <imgui_sdl.h>
#include <imgui_impl_sdl.h>
#include <sol/sol.hpp>

#include "ECS/ECS.h"
#include "Logger/Logger.h"
#include "Events/EventManager.h"
#include "Events/GamePadButtonPressedEvent.h"
#include "Systems/GameSystems/RenderSystem.h"
#include "Systems/RenderTitleSystem.h"
#include "Systems/MusicPlayerSystem.h"
#include "Systems/SoundFXSystem.h"
#include "Systems/RenderCollisionSystem.h"
#include "Systems/GameSystems/AnimationSystem.h"
#include "StateMachines/NewStateMachine.h"
#include "inputs/Keyboard.h"

#include <glm/glm.hpp>

// Forward Declarations
class AssetManager;
class Camera;
class Player;

class Game
{
  private:
	SDL_Window* m_pWindow;
	SDL_Renderer* m_pRenderer;
	SDL_Rect m_MouseBox;
	SDL_Event m_SdlEvent;

	std::unique_ptr<class GameStateMachine> m_pGameStateMachine;
	std::unique_ptr<AssetManager> m_pAssetManager;
	std::unique_ptr<EventManager> m_pEventManager;
	std::unique_ptr<MusicPlayer> m_pMusicPlayer;
	std::unique_ptr<SoundFX> m_pSoundPlayer;

	std::unique_ptr<Camera> m_pCamera;
	std::unique_ptr<Player> m_pPlayer;
	class InputManager& m_InputManager;
	sol::state m_LuaState;

	int m_MSPrevFrame, m_MSPerFrame;
	int m_GameScale, m_TilePixels, m_WindowWidth, m_WindowHeight;
	float m_DeltaTime;
	bool m_bIsRunning, m_bDebug;

	static std::unique_ptr<Game> m_pInstance;

  private:
	// Private Function Declarations
	void Initialize();
	void Update();
	void ProcessEvents();
	void Draw();

	// Constructor
	Game();
	Game( const Game& ) = delete;
	Game& operator=( const Game& ) = delete;

  public:
	~Game() = default;
	// Creation of the singleton Game
	static Game& Instance();
	void Shutdown();
	void Run();

	// Getters and Setters of objects to be used through-out the game
	inline SDL_Renderer* GetRenderer() const { return m_pRenderer; }
	inline SDL_Window* GetWindow() { return m_pWindow; }
	inline Camera& GetCamera() { return *m_pCamera; };
	SDL_Rect& GetMouseBox();
	SDL_Event& GetEvent();

	const bool PlayerHold() const;
	std::unique_ptr<Player>& CreateNewPlayer( Entity& player, Entity& sword, Entity& shield );
	std::unique_ptr<Player>& GetPlayer() { return m_pPlayer; };

	inline const bool IsRunning() const { return m_bIsRunning; }
	inline void SetGameRunning( bool running ) { m_bIsRunning = running; }

	inline const int GetWindowWidth() const { return m_WindowWidth; }
	inline const int GetWindowHeight() const { return m_WindowHeight; }

	inline std::unique_ptr<AssetManager>& GetAssetManager() { return m_pAssetManager; }
	inline EventManager& GetEventManager() { return *m_pEventManager; }
	inline std::unique_ptr<GameStateMachine>& GetStateMachine() { return m_pGameStateMachine; }
	MusicPlayer& GetMusicPlayer() { return *m_pMusicPlayer; }
	SoundFX& GetSoundPlayer() { return *m_pSoundPlayer; }

	inline sol::state& GetLuaState() { return m_LuaState; }
	inline const float& GetDeltaTime() const { return m_DeltaTime; }

	const bool IsDebugging() const { return m_bDebug; }
	void SetDebug( bool debug ) { m_bDebug = debug; }

	const int GetGameScale() const { return m_GameScale; }
	void SetGameScale( int scale ) { m_GameScale = scale; }
};
