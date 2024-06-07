#include "Game/Game.h"
#include "Game/Player.h"
#include "States/TitleState.h"
#include "Logger/Logger.h"
#include "Utilities/Timer.h"
#include "Utilities/Camera.h"
#include "Utilities/GameData.h"
#include "States/GameState.h"
#include "States/EditorState.h"
#include "States/GameStateMachine.h"
#include "Events/KeyPressedEvent.h"
#include "Systems/CameraMovementSystem.h"
#include "Systems/SoundFXSystem.h"
#include "inputs/InputManager.h"
#include "inputs/Gamepad.h"

#include <SDL_ttf.h>

// Define constants
constexpr int FPS = 60;
constexpr int MILLISECONDS_PER_FRAME = 1000 / FPS;

// Initialize all static variables
std::unique_ptr<Game> Game::m_pInstance = nullptr;

// Constructor
Game::Game()
	: m_bIsRunning{ true }
	, m_pWindow{ nullptr }
	, m_pRenderer{ nullptr }
	, m_pGameStateMachine{ nullptr }
	, m_MSPerFrame{ 0 }
	, m_DeltaTime{ 0.0f }
	, m_pCamera{ nullptr }
	, m_pPlayer{ nullptr }
	, m_InputManager{ InputManager::GetInstance() }
	, m_pAssetManager{ std::make_unique<AssetManager>() }
	, m_pEventManager{ std::make_unique<EventManager>() }
{
}

Game& Game::Instance()
{
	if ( !m_pInstance )
	{
		m_pInstance.reset( new Game() );
	}

	return *m_pInstance;
}

// Initialize SDL/IMGUI and variables
void Game::Initialize()
{
	SDL_SetMainReady();

	if ( SDL_Init( SDL_INIT_EVERYTHING ) != 0 )
	{
		Logger::Err( "Failed to initialize: " + *SDL_GetError() );
	}

	// In order to use the SDL extensions, we must initialize them
	if ( TTF_Init() != 0 )
	{
		Logger::Err( "Failed to initialize TTF" );
		return;
	}

	// Allocate channels and control the volume of the music
	if ( !( Mix_Init( MIX_INIT_MP3 ) & MIX_INIT_MP3 ) )
	{
		Logger::Err( "Failed to initialize MP3: " + std::string{ Mix_GetError() } );
		return;
	}

	Mix_AllocateChannels( 16 );
	Mix_Volume( 1, MIX_MAX_VOLUME / 4 );
	// Turn music volume down
	Mix_VolumeMusic( 10 );
	/* Use SDL to grab the displays Resolution */
	SDL_DisplayMode displayMode;
	SDL_GetCurrentDisplayMode( 0, &displayMode );

	m_GameScale = 4;
	m_TilePixels = 16;
	m_WindowWidth = 256 * m_GameScale;
	m_WindowHeight = 240 * m_GameScale;
	m_bDebug = true;

	// Create the window
	m_pWindow = SDL_CreateWindow( "Zelda Clone",
								  SDL_WINDOWPOS_CENTERED,
								  SDL_WINDOWPOS_CENTERED,
								  m_WindowWidth,
								  m_WindowHeight,
								  SDL_WINDOW_OPENGL );

	// Check to see if the window creation worked correctly
	if ( !m_pWindow )
	{
		Logger::Err( "Failed to Create Window: " + *SDL_GetError() );
	}

	// Create the Renderer
	m_pRenderer = SDL_CreateRenderer( m_pWindow, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC );

	// Check if the renderer was created correctly
	if ( !m_pRenderer )
	{
		Logger::Err( "Failed to create renderer: " + *SDL_GetError() );
		return;
	}

	// Enable Alpha Blending
	SDL_SetRenderDrawBlendMode( GetRenderer(), SDL_BLENDMODE_BLEND );

	// Initialize IMGUI context
	ImGui::CreateContext();
	ImGuiSDL::Initialize( m_pRenderer, m_WindowWidth, m_WindowHeight );

	// Add the required systems that are needed at the beginning
	Registry::Instance().AddSystem<RenderSystem>();
	Registry::Instance().AddSystem<RenderTitleSystem>();
	Registry::Instance().AddSystem<RenderCollisionSystem>();
	Registry::Instance().AddSystem<AnimationSystem>();
	Registry::Instance().AddSystem<CameraMovementSystem>();

	m_pCamera = std::make_unique<Camera>( 7168, 4448, m_WindowWidth, m_WindowHeight );
	m_pMusicPlayer = std::make_unique<MusicPlayer>( *m_pAssetManager );
	m_pSoundPlayer = std::make_unique<SoundFX>( *m_pAssetManager );

	// Initialize Mouse Box for Editor State
	m_MouseBox.w = 1;
	m_MouseBox.h = 1;

	// Add the needed sounds, textures, and fonts to the asset manager
	m_pAssetManager->AddSoundFX( "text_slow", "./assets/sounds/Text_Slow.wav" );
	m_pAssetManager->AddTextures( m_pRenderer, "map", "./assets/Backgrounds/entire_worldmap_single_image.png" );

	// Create the finite Game State Machine
	m_pGameStateMachine = std::make_unique<GameStateMachine>();

	// Push the title screen
	m_pGameStateMachine->PushState( std::make_unique<TitleState>() );
}

void Game::Update()
{
	// If we are too fast, waste some time until we reach the desired time per frame.
	int timeToWait = MILLISECONDS_PER_FRAME - ( SDL_GetTicks() - m_MSPerFrame );

	if ( timeToWait > 0 && timeToWait <= m_MSPerFrame )
	{
		SDL_Delay( timeToWait );
	}

	// The difference in ticks since the last frame, converted to seconds
	m_DeltaTime = ( SDL_GetTicks() - m_MSPrevFrame ) / 1000.0f;

	// Store the current time frame
	m_MSPrevFrame = SDL_GetTicks();

	m_pGameStateMachine->Update( m_DeltaTime );
	m_InputManager.GetKeyboard().Update();
	m_InputManager.GetGamepad().Update();

	GameData::GetInstance().UpdateGameData();
}

void Game::ProcessEvents()
{
	// Update the Game Controller
	SDL_GameControllerUpdate();

	while ( SDL_PollEvent( &m_SdlEvent ) )
	{
		// ImGui SDL input
		ImGui_ImplSDL2_ProcessEvent( &m_SdlEvent );
		ImGuiIO& io = ImGui::GetIO();

		int mouseX, mouseY;
		const int buttons = SDL_GetMouseState( &mouseX, &mouseY );

		// Set up mouse inpots for ImGui
		io.MousePos = ImVec2( static_cast<float>( mouseX ), static_cast<float>( mouseY ) );
		io.MouseDown[ 0 ] = buttons & SDL_BUTTON( SDL_BUTTON_LEFT );
		io.MouseDown[ 1 ] = buttons & SDL_BUTTON( SDL_BUTTON_RIGHT );

		// Handle Core SDL Events
		switch ( m_SdlEvent.type )
		{
		case SDL_QUIT: m_bIsRunning = false; break;

		case SDL_KEYDOWN:
			if ( m_SdlEvent.key.keysym.sym == SDLK_ESCAPE )
				m_bIsRunning = false;

			m_InputManager.GetKeyboard().OnKeyDown( m_SdlEvent.key.keysym.sym );
			m_pEventManager->EmitEvent<KeyPressedEvent>( KeyPressedEvent{ .symbol = m_SdlEvent.key.keysym.sym } );
			break;

		case SDL_KEYUP:
			m_InputManager.GetKeyboard().OnKeyUp( m_SdlEvent.key.keysym.sym );

			if ( m_pPlayer )
				m_pPlayer->UpdatePlayerColliders();

			break;

		case SDL_MOUSEMOTION: {
			/*int x, y;
			SDL_GetMouseState(&x, &y);*/
			break;
		}
		case SDL_CONTROLLERBUTTONDOWN: {
			m_InputManager.GetGamepad().OnButtonDown( m_SdlEvent.cbutton.button );
			m_pEventManager->EmitEvent<KeyPressedEvent>( KeyPressedEvent{ .symbol = m_SdlEvent.cbutton.button } );
			break;
		}

		case SDL_CONTROLLERBUTTONUP: {
			m_InputManager.GetGamepad().OnButtonUp( m_SdlEvent.cbutton.button );
			if ( m_pPlayer )
				m_pPlayer->UpdatePlayerColliders();
			break;
		}
		default: break;
		}
	}

	m_pGameStateMachine->ProcessEvents( m_SdlEvent );
}

void Game::Draw()
{
	SDL_SetRenderDrawColor( m_pRenderer, 0, 0, 0, 255 );
	SDL_RenderClear( m_pRenderer );

	m_pGameStateMachine->Render();

	SDL_RenderPresent( m_pRenderer );
}

void Game::Shutdown()
{
	// Shutdown ImGui
	ImGuiSDL::Deinitialize();
	ImGui::DestroyContext();

	// Shutdown SDL
	SDL_DestroyRenderer( m_pRenderer );
	SDL_DestroyWindow( m_pWindow );
	SDL_Quit();
}

void Game::Run()
{
	// Call the initialize function
	Initialize();

	// while game is running
	while ( m_bIsRunning )
	{
		ProcessEvents();
		Update();
		Draw();
	}
}

SDL_Rect& Game::GetMouseBox()
{
	return m_MouseBox;
}

SDL_Event& Game::GetEvent()
{
	return m_SdlEvent;
}

std::unique_ptr<Player>& Game::CreateNewPlayer( Entity& player, Entity& sword, Entity& shield )
{
	return m_pPlayer = std::make_unique<Player>( player, sword, shield );
}

const bool Game::PlayerHold() const
{
	if ( m_pPlayer->GetPlayerDead() || m_pPlayer->GetPlayerOnStairs() || m_pPlayer->GetPlayerItem() ||
		 m_pPlayer->PlayerHeld() || m_pCamera->FadeOutStarted() || m_pCamera->FadeInStarted() || m_pPlayer->GetRaft() ||
		 m_pCamera->IsCameraMoving() || m_InputManager.IsAttacking() )
		return true;
	else
		return false;
}
