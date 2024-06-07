#include "States/GameOverState.h"
#include "States/MenuState.h"
#include "States/GameStateMachine.h"

#include "ECS/ECS.h"
#include "Components/TextLabelComponent.h"
#include "Components/TransformComponent.h"
#include "Components/KeyboardControlComponent.h"
#include "Components/SpriteComponent.h"
#include "Systems/GameSystems/RenderGameOverSystem.h"
#include "Systems/GameSystems/RenderGameOverTextSystem.h"
#include "Systems/GameSystems/RenderSystem.h"
#include "AssetManager/AssetManager.h"
#include "Game/Game.h"
#include "Game/Player.h"
#include "Utilities/Camera.h"
#include "Utilities/GameData.h"
#include "inputs/InputManager.h"
#include "inputs/Keyboard.h"
#include "inputs/Gamepad.h"

const std::string GameOverState::m_sGameOverID = "GAMEOVER";

GameOverState::GameOverState()
	: m_Game( Game::Instance() )
	, m_GameData( GameData::GetInstance() )
	, m_InputManager( InputManager::GetInstance() )
{
}

void GameOverState::Update( const float& deltaTime )
{
	Registry::Instance().Update();
}

void GameOverState::Render()
{
	Registry::Instance().GetSystem<RenderGameOverTextSystem>().Update(
		m_Game.GetRenderer(), m_Game.GetAssetManager(), m_Game.GetCamera().GetCameraRect() );
	Registry::Instance().GetSystem<RenderGameOverSystem>().Update( m_Game.GetRenderer(), m_Game.GetAssetManager() );
}

bool GameOverState::OnEnter()
{
	if ( !Registry::Instance().HasSystem<RenderGameOverTextSystem>() )
		Registry::Instance().AddSystem<RenderGameOverTextSystem>();
	if ( !Registry::Instance().HasSystem<RenderGameOverSystem>() )
		Registry::Instance().AddSystem<RenderGameOverSystem>();

	m_Game.GetMusicPlayer().PlayMusic( "Main_Menu", -1 );

	m_Game.GetAssetManager()->AddTextures(
		m_Game.GetRenderer(), "game_over_words", "./assets/HUDSprites/game_over_words.png" );

	Entity game_over_text = Registry::Instance().CreateEntity();
	game_over_text.AddComponent<TransformComponent>( glm::vec2( 185, 50 ), glm::vec2( 4, 4 ), 0.0 );

	game_over_text.AddComponent<SpriteComponent>( SpriteComponent{ .assetID = "game_over_words",
																   .width = 176,
																   .height = 16,
																   .layer = 1,
																   .bIsFixed = true,
																   .srcRect = SDL_Rect{ 0, 0, 176, 16 } } );

	game_over_text.Group( "game_over" );

	Entity continue_text = Registry::Instance().CreateEntity();
	continue_text.AddComponent<TransformComponent>( glm::vec2( 320, 500 ), glm::vec2( 2, 2 ), 0.0 );
	continue_text.AddComponent<SpriteComponent>( SpriteComponent{ .assetID = "game_over_words",
																  .width = 128,
																  .height = 16,
																  .layer = 1,
																  .bIsFixed = true,
																  .srcRect = SDL_Rect{ 0, 16, 128, 16 } } );

	continue_text.Group( "game_over" );

	Entity retry_text = Registry::Instance().CreateEntity();
	retry_text.AddComponent<TransformComponent>( glm::vec2( 320, 600 ), glm::vec2( 2, 2 ), 0.0 );
	retry_text.AddComponent<SpriteComponent>( SpriteComponent{ .assetID = "game_over_words",
															   .width = 80,
															   .height = 16,
															   .layer = 1,
															   .bIsFixed = true,
															   .srcRect = SDL_Rect{ 0, 32, 80, 16 } } );

	retry_text.Group( "game_over" );

	Entity quit_text = Registry::Instance().CreateEntity();
	quit_text.AddComponent<TransformComponent>( glm::vec2( 320, 700 ), glm::vec2( 2, 2 ), 0.0 );
	quit_text.AddComponent<SpriteComponent>( SpriteComponent{ .assetID = "game_over_words",
															  .width = 64,
															  .height = 16,
															  .layer = 1,
															  .bIsFixed = true,
															  .srcRect = SDL_Rect{ 0, 48, 64, 16 } } );

	quit_text.Group( "game_over" );

	Entity selector = Registry::Instance().CreateEntity();
	selector.AddComponent<SpriteComponent>(
		SpriteComponent{ .assetID = "hearts", .width = 16, .height = 16, .layer = 0, .bIsFixed = false } );

	selector.AddComponent<TransformComponent>( glm::vec2( 260, 475 ), glm::vec2( 4, 4 ), 0.0 );
	selector.AddComponent<KeyboardControlComponent>();
	selector.Tag( "gameOverSelector" );
	selector.Group( "game_over" );

	return true;
}

bool GameOverState::OnExit()
{
	Registry::Instance().GetSystem<RenderGameOverSystem>().OnExit();
	Registry::Instance().GetSystem<RenderGameOverTextSystem>().OnExit();
	Registry::Instance().GetSystem<RenderTextSystem>().OnExit();
	m_Game.GetPlayer()->SetPlayerDead( false );
	return true;
}

void GameOverState::ProcessEvents( SDL_Event& event )
{
	const auto& selector = Registry::Instance().GetEntityByTag( "gameOverSelector" );
	auto& sprite = selector.GetComponent<SpriteComponent>();
	auto& transform = selector.GetComponent<TransformComponent>();
	auto& keyboard = m_InputManager.GetKeyboard();
	auto& gamepad = m_InputManager.GetGamepad();

	if ( keyboard.IsKeyJustPressed( KEY_W ) || gamepad.IsButtonJustPressed( GP_BTN_DPAD_UP ) )
	{
		transform.position.y -= 100;
		m_Game.GetSoundPlayer().PlaySoundFX( "text_slow", 0, SoundChannel::ANY );
		if ( transform.position.y < 475 )
			transform.position.y = 675;
	}
	else if ( keyboard.IsKeyJustPressed( KEY_S ) || gamepad.IsButtonJustPressed( GP_BTN_DPAD_DOWN ) )
	{
		transform.position.y += 100;
		m_Game.GetSoundPlayer().PlaySoundFX( "text_slow", 0, SoundChannel::ANY );
		if ( transform.position.y > 675 )
			transform.position.y = 475;
	}
	else if ( keyboard.IsKeyJustPressed( KEY_SPACE ) || gamepad.IsButtonJustPressed( GP_BTN_A ) )
	{
		if ( transform.position.y == 475 )
		{
			m_Game.GetPlayer()->SetPlayerCreated( false );
			m_Game.GetPlayer()->SetPlayerDead( false );
			m_Game.GetStateMachine()->PushState( std::make_unique<GameState>() );
		}
		else if ( transform.position.y == 575 )
		{
			m_GameData.PlayerNum();
			m_Game.GetStateMachine()->PushState( std::make_unique<GameState>() );
		}
		else if ( transform.position.y == 675 )
		{
			m_Game.GetStateMachine()->PushState( std::make_unique<MenuState>() );
		}
	}
}
