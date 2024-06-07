#include "States/GameState.h"
#include "States/MenuState.h"
#include "States/GameOverState.h"
#include "States/GameStateMachine.h"
#include "Game/Game.h"
#include "AssetManager/AssetManager.h"
#include "Systems/GameSystems/CollisionSystem.h"
#include "Systems/GameSystems/RenderSystem.h"
#include "Systems/GameSystems/DamageSystem.h"
#include "Systems/GameSystems/RenderHUDSystem.h"
#include "Systems/GameSystems/MovementSystem.h"
#include "Systems/GameSystems/CaptionSystem.h"
#include "Systems/GameSystems/RenderHealthSystem.h"
#include "Systems/GameSystems/RenderTileSystem.h"
#include "Systems/GameSystems/RenderGameOverSystem.h"
#include "Systems/GameSystems/HealthSystem.h"
#include "Systems/GameSystems/TriggerSystem.h"
#include "Systems/CameraMovementSystem.h"
#include "Systems/GameSystems/CollectItemSystem.h"
#include "Systems/GameSystems/ProjectileEmitterSystem.h"
#include "Systems/GameSystems/ProjectileLifeCycleSystem.h"
#include "Systems/GameSystems/AISystem.h"
#include "Systems/MenuSystems/RenderMainMenuSystem.h"
#include "Components/HealthComponent.h"
#include "Events/KeyPressedEvent.h"
#include "Events/EventManager.h"
#include "Events/TriggerEvent.h"
#include "Game/LevelLoader.h"
#include "Utilities/Utility.h"
#include "StateMachines/NewPlayerStates.h"
#include "Systems/SoundFXSystem.h"

#include "Utilities/GameData.h"
#include "Utilities/Camera.h"
#include "inputs/InputManager.h"
#include "inputs/Gamepad.h"
#include "Game/Player.h"

void GameState::UpdatePlayerKeys()
{
	const auto& player = m_Game.GetPlayer();
	player->UpdatePlayer( m_Game, m_InputManager );
}

void GameState::UpdatePauseContol()
{
	const auto& keyboard = m_InputManager.GetKeyboard();
	const auto& gamepad = m_InputManager.GetGamepad();
	auto& camera = m_Game.GetCamera();
	auto& player = m_Game.GetPlayer();

	// Set to paused
	if ( keyboard.IsKeyJustReleased( KEY_P ) || gamepad.IsButtonJustReleased( GP_BTN_Y ) )
	{
		camera.SetFadeFinished( false );
		camera.StartFadeOut( true );
		camera.StartFadeIn( false );
		m_InputManager.SetPaused( true );
		m_bPaused = true;
	}
	else if ( keyboard.IsKeyJustPressed( KEY_C ) || gamepad.IsButtonHeld( GP_BTN_BACK ) )
	{
		if ( !m_Game.IsDebugging() )
			m_Game.SetDebug( true );
		else
			m_Game.SetDebug( false );
	}

	if ( keyboard.IsKeyJustReleased( KEY_SPACE ) || keyboard.IsKeyJustReleased( KEY_RSHIFT ) ||
		 gamepad.IsButtonJustReleased( GP_BTN_A ) || gamepad.IsButtonJustReleased( GP_BTN_X ) )
	{
		player->SetAttacking( false );
		m_InputManager.SetAttack( false );
	}
}

GameState::GameState()
	: GameState( glm::vec2{ 0.f } )
{
}

GameState::GameState( glm::vec2 cameraOffset )
	: m_Game( Game::Instance() )
	, m_Registry( Registry::Instance() )
	, m_InputManager( InputManager::GetInstance() )
	, m_CameraOffset( cameraOffset )
	, m_GameData( GameData::GetInstance() )
	, m_HudRect{ 0, 0, m_Game.GetWindowWidth(), 256 }
	, m_bPaused{ false }
	, m_TriggerSystem{ m_Registry.GetSystem<TriggerSystem>() }
	, m_CollectItemSystem{ m_Registry.GetSystem<CollectItemSystem>() }
	, m_AnimationSystem{ m_Registry.GetSystem<AnimationSystem>() }
	, m_MovementSystem{ m_Registry.GetSystem<MovementSystem>() }
	, m_ProjectileEmitterSystem{ m_Registry.GetSystem<ProjectileEmitterSystem>() }
	, m_CameraMovementSystem{ m_Registry.GetSystem<CameraMovementSystem>() }
	, m_ProjectileLifeCycleSystem{ m_Registry.GetSystem<ProjectileLifeCycleSystem>() }
	, m_AISystem{ m_Registry.GetSystem<AISystem>() }
	, m_CaptionSystem{ m_Registry.GetSystem<CaptionSystem>() }
	, m_CollisionSystem{ m_Registry.GetSystem<CollisionSystem>() }
	, m_DamageSystem{ m_Registry.GetSystem<DamageSystem>() }
	, m_HealthSystem{ m_Registry.GetSystem<HealthSystem>() }
	, m_RenderSystem{ m_Registry.GetSystem<RenderSystem>() }
	, m_RenderTileSystem{ m_Registry.GetSystem<RenderTileSystem>() }
	, m_RenderHUDSystem{ m_Registry.GetSystem<RenderHUDSystem>() }
	, m_RenderCollisionSystem{ m_Registry.GetSystem<RenderCollisionSystem>() }
{
	m_Game.GetCamera().SetCameraPosition( m_CameraOffset.x, m_CameraOffset.y );
}

void GameState::Update( const float& deltaTime )
{
	auto& camera = m_Game.GetCamera();
	if ( m_InputManager.IsPaused() && camera.GetFadeAlpha() == 0 )
	{
		camera.SetFadeFinished( true );
		camera.StartFadeOut( false );
		m_Game.GetStateMachine()->PushState( std::make_unique<PauseState>(), false );
	}

	// Check to see if level music has been paused
	if ( !m_InputManager.IsPaused() && !camera.FadeOutStarted() && m_bPaused )
	{
		// Turn music volume up
		Mix_VolumeMusic( 10 );
		camera.StartFadeIn( true );
		m_bPaused = false;
	}

	// Update the registry values
	m_Registry.Update();

	m_CollisionSystem.Update( deltaTime );
	m_TriggerSystem.Update( deltaTime );
	m_CollectItemSystem.Update();
	m_MovementSystem.Update( deltaTime );
	m_CollisionSystem.Update( deltaTime );
	m_ProjectileEmitterSystem.Update();
	m_CameraMovementSystem.UpdatePlayerCam( m_Game.GetCamera(), deltaTime );
	m_ProjectileLifeCycleSystem.Update();
	m_AISystem.Update();
	m_CaptionSystem.Update( deltaTime );

	// Update the registry values
	m_Registry.Update();

	// Update all Game systems
	m_Game.GetPlayer()->UpdateStateMachine();
}

void GameState::Render()
{
	auto& camera = m_Game.GetCamera();
	m_RenderTileSystem.Update();
	camera.UpdateScreenFlash();
	m_AnimationSystem.Update();
	m_RenderSystem.Update();

	// Render all HUD objects
	SDL_SetRenderDrawColor( m_Game.GetRenderer(), 0, 0, 0, 255 );
	SDL_RenderFillRect( m_Game.GetRenderer(), &m_HudRect );
	SDL_RenderDrawRect( m_Game.GetRenderer(), &m_HudRect );
	SDL_SetRenderDrawColor( m_Game.GetRenderer(), 0, 0, 0, 255 );

	camera.UpdateCurtain();

	m_RenderHUDSystem.Update( m_Game.GetRenderer(), m_Game.GetAssetManager() );
	m_HealthSystem.Update();

	// If the m_Game is in debug mode, render the collision system
	if ( m_Game.IsDebugging() )
	{
		m_RenderCollisionSystem.Update( m_Game.GetRenderer(), camera.GetCameraRect() );
	}
}

bool GameState::OnEnter()
{
	LevelLoader loader;
	auto& camera = m_Game.GetCamera();
	if ( !m_GameData.IsGameLoaded() )
	{
		// Always start the player and the camera from the beginning Location for now --> Create Constants for Special
		// CAM Locations
		if ( camera.GetCameraPos().x != 7168 && camera.GetCameraPos().x != 6720 )
		{
			camera.SetCameraPosition( 7168, 6720 );
		}
		camera.StartCurtainOpen();

		// Open the lua libraries into the m_Game
		m_Game.GetLuaState().open_libraries( sol::lib::base, sol::lib::math, sol::lib::os );

		// Make sure the m_Game is not in debug mode!!
		m_Game.SetDebug( false );

		loader.LoadAssetsFromLuaTable( m_Game.GetLuaState(), "game_state_assets" );
		loader.LoadHUDFromLuaTable( m_Game.GetLuaState(), "hud" );
		loader.LoadEnemiesFromLuaTable( m_Game.GetLuaState(), "overworld_enemies_2" );
		loader.LoadEntitiesFromLuaTable( m_Game.GetLuaState(), "over_world_entities" );
		loader.LoadColliders( "overworld_colliders_1" );
		loader.LoadTriggers( m_Game.GetLuaState(), "overworld_triggers" );

		m_Game.GetMusicPlayer().PlayMusic( "Overworld", -1 );
		// Load the overworld map
		loader.LoadMap( "map", 4096, 1344 );

		m_TriggerSystem.SubscribeToEvents( m_Game.GetEventManager() );
		m_CollectItemSystem.SubscribeToEvents( m_Game.GetEventManager() );
		m_ProjectileEmitterSystem.SubscribeKeyToEvents( m_Game.GetEventManager() );
		m_ProjectileEmitterSystem.SubscribeBtnToEvents( m_Game.GetEventManager() );
		m_DamageSystem.SubscribeToEvents( m_Game.GetEventManager() );
		m_GameData.SetGameLoaded( true );
	}

	if ( !m_Game.GetPlayer() )
	{
		loader.CreatePlayerEntityFromLuaTable( m_Game.GetLuaState(), "new_player_create" );
		// Load the player file based on the selected slot
		auto playerNum = GameData::GetInstance().PlayerNum();
		loader.LoadPlayerDataFromLuaTable( m_Game.GetLuaState(), "save" + std::to_string( playerNum ), playerNum );

		auto player = Registry::Instance().GetEntityByTag( "player" );
		// Reset the player health after pressing continue [Game Over]
		auto& playerHealth = player.GetComponent<HealthComponent>();

		if ( playerHealth.healthPercentage <= 0 )
		{
			playerHealth.healthPercentage = 2 * playerHealth.maxHearts;
		}

		m_Game.GetPlayer()->SetPlayerCreated( true );
		{
			m_Game.GetPlayer()->GetPlayerStateMachine().AddState( std::make_unique<IdleState>() );
			m_Game.GetPlayer()->GetPlayerStateMachine().ChangeState( player );
		}
	}

	// Read in all secrets!!
	loader.ReadInSecrets( m_Game.GetLuaState() );

	// Remove the menu/Game over system for it is not needed in the m_Game state
	m_Registry.RemoveSystem<RenderMainMenuSystem>();
	m_Game.GetAssetManager()->AddFonts( "game_font", "./assets/Fonts/prstart.ttf", 30 );
	m_Game.GetAssetManager()->AddTextures(
		m_Game.GetRenderer(), "simple_enemies", "./assets/EnemySprites/simple_enemies.png" );

	return true;
}

bool GameState::OnExit()
{
	m_Game.GetEventManager().Reset();
	m_RenderCollisionSystem.OnExit();
	m_RenderSystem.OnExit();
	m_RenderTileSystem.OnExit();
	m_GameData.SetGameLoaded( false );
	return true;
}

void GameState::ProcessEvents( SDL_Event& event )
{
	UpdatePlayerKeys();
	UpdatePauseContol();
}
