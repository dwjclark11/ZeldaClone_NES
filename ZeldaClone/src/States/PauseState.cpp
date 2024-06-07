#include "States/PauseState.h"
#include "Game/Game.h"
#include "States/GameState.h"
#include "States/SaveGameState.h"
#include "States/GameStateMachine.h"
#include "Systems/PauseSystems/RenderPauseSystem.h"
#include "Systems/GameSystems/RenderHUDSystem.h"
#include "Systems/GameSystems/TriggerSystem.h"
#include "Systems/GameSystems/CollectItemSystem.h"
#include "Systems/GameSystems/MovementSystem.h"
#include "Systems/GameSystems/ProjectileEmitterSystem.h"
#include "Systems/GameSystems/DamageSystem.h"
#include "Systems/RenderTextSystem.h"

#include "Game/LevelLoader.h"
#include "Components/KeyboardControlComponent.h"
#include "Components/GamePadComponent.h"
#include "Components/TransformComponent.h"
#include "Components/SpriteComponent.h"
#include "Components/PauseComponent.h"
#include "Utilities/Camera.h"
#include "inputs/InputManager.h"
#include "inputs/Keyboard.h"
#include "inputs/Gamepad.h"

#include <SDL_ttf.h>

void PauseState::UpdateSelectedItemSprite()
{
	if ( m_SelectedItem == m_GameData.GetSelectedItem() )
		return;

	const auto& selectedItem = m_Registry.GetEntityByTag( "selectedItem" );
	auto& sprite = selectedItem.GetComponent<SpriteComponent>();

	switch ( m_GameData.GetSelectedItem() )
	{
	case GameData::ItemType::BOOMERANG:
		sprite.srcRect.x = 0;
		sprite.srcRect.y = 0;
		sprite.srcRect.x += sprite.width * 0;
		sprite.srcRect.y += sprite.height * 7;
		break;
	case GameData::ItemType::BOMB:
		sprite.srcRect.x = 0;
		sprite.srcRect.y = 0;
		sprite.srcRect.x += sprite.width * 4;
		sprite.srcRect.y += sprite.height * 7;
		break;

	case GameData::ItemType::BOW:
		sprite.srcRect.x = 0;
		sprite.srcRect.y = 0;
		sprite.srcRect.x += sprite.width * 4;
		break;

	case GameData::ItemType::CANDLE:
		sprite.srcRect.x = 0;
		sprite.srcRect.y = 0;
		sprite.srcRect.y += sprite.height * 3;
		break;

	case GameData::ItemType::FOOD:
		sprite.srcRect.x = 0;
		sprite.srcRect.y = 0;
		sprite.srcRect.x += sprite.width * 7;
		sprite.srcRect.y += sprite.height * 1;
		break;

	case GameData::ItemType::POTION_RED:
		sprite.srcRect.x = 0;
		sprite.srcRect.y = 0;
		sprite.srcRect.x += sprite.width * 2;
		sprite.srcRect.y += sprite.height * 2;
		break;

	case GameData::ItemType::MAGIC_ROD:
		sprite.srcRect.x = 0;
		sprite.srcRect.y = 0;
		sprite.srcRect.x += sprite.width * 4;
		sprite.srcRect.y += sprite.height * 5;
		break;
	}
}

PauseState::PauseState()
	: m_Game( Game::Instance() )
	, m_Registry( Registry::Instance() )
	, m_GameData( GameData::GetInstance() )
	, m_InputManager( InputManager::GetInstance() )
	, m_SelectedItem{ GameData::ItemType::EMPTY }
{
}

void PauseState::Update( const float& deltaTime )
{
	m_Registry.Update();

	if ( m_GameData.GetTotalBombs() > 0 && !m_GameData.HasItem( GameData::GameItems::BOMB ) )
	{
		Entity bombItem = m_Registry.CreateEntity();
		bombItem.AddComponent<SpriteComponent>( SpriteComponent{ .assetID = "items",
																 .width = 16,
																 .height = 16,
																 .layer = 0,
																 .bIsFixed = false,
																 .srcRect = SDL_Rect{ 64, 112, 16, 16 } } );

		bombItem.AddComponent<TransformComponent>( glm::vec2( 486, 190 ), glm::vec2( 4, 4 ), 0.0 );
		bombItem.AddComponent<PauseComponent>();
		bombItem.Tag( "bombItem" );
		bombItem.Group( "pause" );

		m_GameData.AddItem( GameData::GameItems::BOMB );
	}

	if ( m_Game.GetCamera().GetFadeAlpha() == 0 && !m_Game.GetCamera().FadeFinished() )
	{
		m_InputManager.SetPaused( false );
		m_Game.GetCamera().SetFadeFinished( true );
		m_Game.GetCamera().StartFadeOut( false );
		m_Game.GetStateMachine()->PopState();
		return;
	}

	if ( m_GameData.GetTotalBombs() == 0 && m_GameData.HasItem( GameData::GameItems::BOMB ) )
		m_GameData.RemoveItem( GameData::GameItems::BOMB );

	UpdateSelectedItemSprite();
}

void PauseState::Render()
{
	m_Registry.GetSystem<RenderPauseSystem>().Update( m_Game.GetRenderer(), m_Game.GetAssetManager() );
	m_Registry.GetSystem<RenderHUDSystem>().Update( m_Game.GetRenderer(), m_Game.GetAssetManager() );
	m_Registry.GetSystem<RenderTextSystem>().Update();
}

bool PauseState::OnEnter()
{
	// Turn music volume down while paused
	Mix_VolumeMusic( 3 );
	m_Game.GetEventManager().Reset();
	m_Game.GetCamera().StartFadeIn( true );
	// =============================================================================================================================
	// Add all necessary systems to the registry if they are not yet registered
	// =============================================================================================================================
	if ( !m_Registry.HasSystem<RenderPauseSystem>() )
		m_Registry.AddSystem<RenderPauseSystem>();
	// =============================================================================================================================

	m_Game.GetAssetManager()->AddTextures( m_Game.GetRenderer(), "pause_hud", "./assets/HUDSprites/pauseHud.png" );

	Entity pauseSelector = m_Registry.CreateEntity();
	pauseSelector.AddComponent<SpriteComponent>( SpriteComponent{ .assetID = "box",
																  .width = 16,
																  .height = 16,
																  .layer = 0,
																  .bIsFixed = false,
																  .srcRect = SDL_Rect{ 16, 0, 16, 16 } } );

	pauseSelector.AddComponent<TransformComponent>( glm::vec2( 386, 190 ), glm::vec2( 4, 4 ), 0.0 );
	pauseSelector.AddComponent<PauseComponent>();
	pauseSelector.AddComponent<KeyboardControlComponent>();
	pauseSelector.AddComponent<GamePadComponent>();
	pauseSelector.Tag( "pauseSelector" );
	pauseSelector.Group( "pause" );

	Entity selectedItem = m_Registry.CreateEntity();
	selectedItem.AddComponent<SpriteComponent>( SpriteComponent{ .assetID = "items",
																 .width = 16,
																 .height = 16,
																 .layer = 0,
																 .bIsFixed = false,
																 .srcRect = SDL_Rect{ 48, 16, 16, 16 } } );

	selectedItem.AddComponent<TransformComponent>( glm::vec2( 200, 185 ), glm::vec2( 6, 6 ), 0.0 );
	selectedItem.AddComponent<PauseComponent>();
	selectedItem.AddComponent<KeyboardControlComponent>();
	selectedItem.Tag( "selectedItem" );
	selectedItem.Group( "pause" );

	Entity hudHolder = m_Registry.CreateEntity();
	hudHolder.AddComponent<TransformComponent>( glm::vec2( 325, 100 ), glm::vec2( 5, 5 ), 0.0 );

	hudHolder.AddComponent<SpriteComponent>( SpriteComponent{ .assetID = "pause_hud",
															  .width = 96,
															  .height = 64,
															  .layer = 5,
															  .bIsFixed = true,
															  .srcRect = SDL_Rect{ 0, 0, 96, 64 } } );

	hudHolder.AddComponent<PauseComponent>();
	hudHolder.Group( "pause" );

	Entity hudHolder2 = m_Registry.CreateEntity();
	hudHolder2.AddComponent<TransformComponent>( glm::vec2( 144, 135 ), glm::vec2( 6, 6 ), 0.0 );
	hudHolder2.AddComponent<SpriteComponent>( SpriteComponent{ .assetID = "hud_holder",
															   .width = 32,
															   .height = 32,
															   .layer = 5,
															   .bIsFixed = true,
															   .srcRect = SDL_Rect{ 0, 0, 32, 32 } } );

	hudHolder2.AddComponent<PauseComponent>();
	hudHolder2.Group( "pause" );

	Entity triforce = m_Registry.CreateEntity();
	triforce.AddComponent<SpriteComponent>(
		SpriteComponent{ .assetID = "triforce",
						 .width = 96,
						 .height = 64,
						 .layer = 0,
						 .bIsFixed = false,
						 .srcRect = SDL_Rect{ m_GameData.GetTotalTriforce() * 96, 0, 96, 64 } } );

	triforce.AddComponent<TransformComponent>( glm::vec2( 325, 400 ), glm::vec2( 4, 4 ), 0.0 );
	triforce.AddComponent<PauseComponent>();

	// Top Row Items
	if ( m_GameData.HasItem( GameData::GameItems::BOOMERANG ) )
	{
		Entity boomerang = m_Registry.CreateEntity();
		boomerang.AddComponent<SpriteComponent>( SpriteComponent{ .assetID = "items",
																  .width = 16,
																  .height = 16,
																  .layer = 0,
																  .bIsFixed = false,
																  .srcRect = SDL_Rect{ 0, 112, 16, 16 } } );

		boomerang.AddComponent<TransformComponent>( glm::vec2( 386, 190 ), glm::vec2( 4, 4 ), 0.0 );
		boomerang.AddComponent<PauseComponent>();
		boomerang.Group( "pause" );
	}

	if ( m_GameData.GetTotalBombs() > 0 && m_GameData.HasItem( GameData::GameItems::BOMB ) )
	{
		Entity bombs = m_Registry.CreateEntity();
		bombs.AddComponent<SpriteComponent>( SpriteComponent{ .assetID = "items",
															  .width = 16,
															  .height = 16,
															  .layer = 0,
															  .bIsFixed = false,
															  .srcRect = SDL_Rect{ 64, 112, 16, 16 } } );

		bombs.AddComponent<TransformComponent>( glm::vec2( 486, 190 ), glm::vec2( 4, 4 ), 0.0 );
		bombs.AddComponent<PauseComponent>();
		bombs.Tag( "bombItem" );
		bombs.Group( "pause" );
	}

	if ( m_GameData.HasItem( GameData::GameItems::BOW ) )
	{
		Entity bow = m_Registry.CreateEntity();
		bow.AddComponent<SpriteComponent>( SpriteComponent{ .assetID = "items",
															.width = 16,
															.height = 16,
															.layer = 0,
															.bIsFixed = false,
															.srcRect = SDL_Rect{ 64, 0, 16, 16 } } );

		bow.AddComponent<TransformComponent>( glm::vec2( 586, 190 ), glm::vec2( 4, 4 ), 0.0 );
		bow.AddComponent<PauseComponent>();
		bow.Group( "pause" );
	}

	if ( m_GameData.HasItem( GameData::GameItems::CANDLE ) )
	{
		Entity candle = m_Registry.CreateEntity();
		candle.AddComponent<SpriteComponent>( SpriteComponent{ .assetID = "items",
															   .width = 16,
															   .height = 16,
															   .layer = 0,
															   .bIsFixed = false,
															   .srcRect = SDL_Rect{ 0, 48, 16, 16 } } );

		candle.AddComponent<TransformComponent>( glm::vec2( 686, 190 ), glm::vec2( 4, 4 ), 0.0 );
		candle.AddComponent<PauseComponent>();
		candle.Group( "pause" );
	}

	// Bottom Row Items
	if ( m_GameData.HasItem( GameData::GameItems::FLUTE ) )
	{
		Entity flute = m_Registry.CreateEntity();
		flute.AddComponent<SpriteComponent>( SpriteComponent{ .assetID = "items",
															  .width = 16,
															  .height = 16,
															  .layer = 0,
															  .bIsFixed = false,
															  .srcRect = SDL_Rect{ 80, 16, 16, 16 } } );

		flute.AddComponent<TransformComponent>( glm::vec2( 386, 260 ), glm::vec2( 4, 4 ), 0.0 );
		flute.AddComponent<PauseComponent>();
		flute.Group( "pause" );
	}

	if ( m_GameData.HasItem( GameData::GameItems::FOOD ) )
	{
		Entity meat = m_Registry.CreateEntity();
		meat.AddComponent<SpriteComponent>( SpriteComponent{ .assetID = "items",
															 .width = 16,
															 .height = 16,
															 .layer = 0,
															 .bIsFixed = false,
															 .srcRect = SDL_Rect{ 112, 16, 16, 16 } } );

		meat.AddComponent<TransformComponent>( glm::vec2( 486, 260 ), glm::vec2( 4, 4 ), 0.0 );
		meat.AddComponent<PauseComponent>();
		meat.Group( "pause" );
	}

	if ( m_GameData.HasItem( GameData::GameItems::RED_POTION ) )
	{
		Entity potion = m_Registry.CreateEntity();
		potion.AddComponent<SpriteComponent>( SpriteComponent{ .assetID = "items",
															   .width = 16,
															   .height = 16,
															   .layer = 0,
															   .bIsFixed = false,
															   .srcRect = SDL_Rect{ 32, 32, 16, 16 } } );

		potion.AddComponent<TransformComponent>( glm::vec2( 586, 260 ), glm::vec2( 4, 4 ), 0.0 );
		potion.AddComponent<PauseComponent>();
		potion.Group( "pause" );
	}

	if ( m_GameData.HasItem( GameData::GameItems::MAGIC_ROD ) )
	{
		Entity magicalRod = m_Registry.CreateEntity();
		magicalRod.AddComponent<SpriteComponent>( SpriteComponent{ .assetID = "items",
																   .width = 16,
																   .height = 16,
																   .layer = 0,
																   .bIsFixed = false,
																   .srcRect = SDL_Rect{ 64, 80, 16, 16 } } );

		magicalRod.AddComponent<TransformComponent>( glm::vec2( 686, 260 ), glm::vec2( 4, 4 ), 0.0 );
		magicalRod.AddComponent<PauseComponent>();
		magicalRod.Group( "pause" );
	}

	return false;
}

bool PauseState::OnExit()
{
	m_Registry.GetSystem<CollectItemSystem>().SubscribeToEvents( m_Game.GetEventManager() );
	m_Registry.GetSystem<TriggerSystem>().SubscribeToEvents( m_Game.GetEventManager() );
	m_Registry.GetSystem<ProjectileEmitterSystem>().SubscribeKeyToEvents( m_Game.GetEventManager() );
	m_Registry.GetSystem<ProjectileEmitterSystem>().SubscribeBtnToEvents( m_Game.GetEventManager() );
	m_Registry.GetSystem<DamageSystem>().SubscribeToEvents( m_Game.GetEventManager() );
	m_Registry.GetSystem<RenderPauseSystem>().OnExit();
	return true;
}

void PauseState::ProcessEvents( SDL_Event& event )
{
	const auto& selector = m_Registry.GetEntityByTag( "pauseSelector" );
	auto& sprite = selector.GetComponent<SpriteComponent>();
	auto& transform = selector.GetComponent<TransformComponent>();
	auto& keyboard = m_InputManager.GetKeyboard();
	auto& gamepad = m_InputManager.GetGamepad();

	if ( keyboard.IsKeyJustPressed( KEY_W ) || gamepad.IsButtonJustPressed( GP_BTN_DPAD_UP ) )
	{
		transform.position.y -= ( ( sprite.height * transform.scale.y ) + 6 );
		m_Game.GetSoundPlayer().PlaySoundFX( "text_slow", 0, SoundChannel::TEXT );
		if ( transform.position.y < 190 )
			transform.position.y = 260;
	}
	else if ( keyboard.IsKeyJustPressed( KEY_S ) || gamepad.IsButtonJustPressed( GP_BTN_DPAD_DOWN ) )
	{
		transform.position.y += ( ( sprite.height * transform.scale.y ) + 6 );
		m_Game.GetSoundPlayer().PlaySoundFX( "text_slow", 0, SoundChannel::TEXT );
		if ( transform.position.y > 260 )
			transform.position.y = 190;
	}
	else if ( keyboard.IsKeyJustPressed( KEY_A ) || gamepad.IsButtonJustPressed( GP_BTN_DPAD_LEFT ) )
	{
		transform.position.x -= 100;
		m_Game.GetSoundPlayer().PlaySoundFX( "text_slow", 0, SoundChannel::TEXT );
		if ( transform.position.x < 386 )
			transform.position.x = 686;
	}
	else if ( keyboard.IsKeyJustPressed( KEY_D ) || gamepad.IsButtonJustPressed( GP_BTN_DPAD_RIGHT ) )
	{
		transform.position.x += 100;
		m_Game.GetSoundPlayer().PlaySoundFX( "text_slow", 0, SoundChannel::TEXT );
		if ( transform.position.x > 686 )
			transform.position.x = 386;
	}
	else if ( keyboard.IsKeyJustPressed( KEY_SPACE ) || gamepad.IsButtonJustPressed( GP_BTN_A ) )
	{
		if ( transform.position.x == 386 && transform.position.y == 190 &&
			 m_GameData.HasItem( GameData::GameItems::BOOMERANG ) )
		{
			m_GameData.SetSelectedItem( GameData::ItemType::BOOMERANG );
		}
		else if ( ( transform.position.x == 486 && transform.position.y == 190 ) && m_GameData.GetTotalBombs() > 0 &&
				  m_GameData.HasItem( GameData::GameItems::BOMB ) )
		{
			m_GameData.SetSelectedItem( GameData::ItemType::BOMB );
		}
		else if ( transform.position.x == 586 && transform.position.y == 190 &&
				  m_GameData.HasItem( GameData::GameItems::BOW ) )
		{
			m_GameData.SetSelectedItem( GameData::ItemType::BOW );
		}
		else if ( transform.position.x == 686 && transform.position.y == 190 &&
				  m_GameData.HasItem( GameData::GameItems::CANDLE ) )
		{
			m_GameData.SetSelectedItem( GameData::ItemType::CANDLE );
		}
		else if ( transform.position.x == 386 && transform.position.y == 260 )
		{
		}
		else if ( transform.position.x == 486 && transform.position.y == 260 &&
				  m_GameData.HasItem( GameData::GameItems::FOOD ) )
		{
			m_GameData.SetSelectedItem( GameData::ItemType::FOOD );
		}
		else if ( transform.position.x == 586 && transform.position.y == 260 &&
				  m_GameData.HasItem( GameData::GameItems::RED_POTION ) )
		{
			m_GameData.SetSelectedItem( GameData::ItemType::POTION_RED );
		}
		else if ( transform.position.x == 686 && transform.position.y == 260 &&
				  m_GameData.HasItem( GameData::GameItems::MAGIC_ROD ) )
		{
			m_GameData.SetSelectedItem( GameData::ItemType::MAGIC_ROD );
		}
	}
	else if ( keyboard.IsKeyJustReleased( KEY_P ) || gamepad.IsButtonJustReleased( GP_BTN_Y ) )
	{
		m_Game.GetCamera().SetFadeFinished( false );
		m_Game.GetCamera().StartFadeOut( true );
	}
	else if ( keyboard.IsKeyJustReleased( KEY_M ) || gamepad.IsButtonJustReleased( GP_BTN_START ) )
	{
		// Open the save Screen!
		m_Game.GetStateMachine()->PushState( std::make_unique<SaveGameState>() );
	}
}
