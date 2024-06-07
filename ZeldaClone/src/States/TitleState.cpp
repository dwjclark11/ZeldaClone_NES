#include "States/TitleState.h"
#include "States/MenuState.h"
#include "States/GameStateMachine.h"
#include "Game/Game.h"
#include "AssetManager/AssetManager.h"

#include "ECS/ECS.h"
#include "Components/AnimationComponent.h"
#include "Components/RigidBodyComponent.h"
#include "Components/TransformComponent.h"
#include "Components/SpriteComponent.h"
#include "Components/BackgroundImageComponent.h"
#include "Systems/CameraMovementSystem.h"
#include "Systems/GameSystems/AnimationSystem.h"
#include "Systems/SoundFXSystem.h"
#include "Utilities/Camera.h"
#include "Utilities/Timer.h"
#include "inputs/InputManager.h"
#include "inputs/Gamepad.h"
#include "inputs/Keyboard.h"

void TitleState::UpdateCameraMovement( Camera& camera, const float& dt )
{
	// Camera data
	const auto& cameraPos = camera.GetCameraPos();
	const auto& cameraWidth = camera.GetCameraWidth();
	const auto& cameraHeight = camera.GetCameraHeight();
	int new_cam_x = cameraPos.x;
	int new_cam_y = cameraPos.y;

	// Where to place the Origin of the camera during the first entrance of the state
	if ( m_pCamMoveTimer->GetTicks() < 1000 )
		camera.SetCameraPosition( 0, 0 );

	if ( m_pCamMoveTimer->GetTicks() > 15000 && m_pCamMoveTimer->GetTicks() < 18000 )
	{
		new_cam_y += 100 * dt;
		std::this_thread::sleep_for( std::chrono::microseconds( 5 ) );

		if ( new_cam_y >= 960 )
		{
			new_cam_y = 960;
			camera.SetCameraMoving( true );
		}
	}

	if ( m_pCamMoveTimer->GetTicks() > 18000 )
	{
		new_cam_y += 100 * dt;

		if ( new_cam_y >= 6720 )
		{
			new_cam_y = 6720;
		}
	}

	if ( m_pCamMoveTimer->GetTicks() > 120000 )
	{
		m_pCamMoveTimer->Stop();
		new_cam_y = 0;
		m_bScrollFinished = true;
	}

	// Keep the camera rectange view inside the screen limits
	new_cam_x = new_cam_x < 0 ? 0 : new_cam_x; // If the camera.x is < 0 make it 0
	new_cam_y = new_cam_y < 0 ? 0 : new_cam_y;

	camera.SetCameraPosition( new_cam_x, new_cam_y );
}

void TitleState::AnimateTitle()
{
	if ( m_pCamMoveTimer->GetTicks() > 10000 && m_pCamMoveTimer->GetTicks() < 15000 )
	{
		auto entity = m_Registry.GetEntityByTag( "title" );
		auto& animation = entity.GetComponent<AnimationComponent>();

		if ( !m_bOffset )
		{
			animation.frameOffset = 768;
			animation.numFrames = 9;
		}
		// Logger::Err(std::to_string(animation.currentFrame));
		if ( animation.currentFrame == 8 && !m_bOffset )
		{
			animation.frameOffset = 3072;
			animation.numFrames = 3;
			m_bOffset = true;
		}
	}
}

TitleState::TitleState()
	: m_Game{ Game::Instance() }
	, m_Registry{ Registry::Instance() }
	, m_Timer{ 0 }
	, m_bScroll{ false }
	, m_bScrollFinished{ false }
	, m_bOffset{ false }
	, m_pCamMoveTimer{ std::make_unique<Timer>() }
{
}

void TitleState::Update( const float& deltaTime )
{
	m_Registry.Update();
	m_Registry.GetSystem<AnimationSystem>().Update();

	if ( m_pCamMoveTimer->GetTicks() > 15000 )
	{
		if ( !m_bScroll )
		{
			Entity scrollSheet = m_Registry.CreateEntity();
			scrollSheet.AddComponent<SpriteComponent>( SpriteComponent{ .assetID = "title_scroll",
																		.width = 256,
																		.height = 1920,
																		.layer = 10,
																		.bIsFixed = false,
																		.srcRect = SDL_Rect{ 0, 0, 256, 1920 } } );

			scrollSheet.AddComponent<TransformComponent>( glm::vec2( 0, 0 ), glm::vec2( 4, 4 ) );
			scrollSheet.AddComponent<BackgroundImageComponent>( glm::vec2( 0, 0 ), glm::vec2( 4, 4 ) );
			scrollSheet.AddComponent<RigidBodyComponent>( glm::vec2( 0 ) );
			scrollSheet.AddComponent<AnimationComponent>(
				AnimationComponent{ .numFrames = 2, .frameSpeedRate = 6, .vertical = false } );

			scrollSheet.Tag( "title_scroll" );

			m_bScroll = true;
		}

		UpdateCameraMovement( m_Game.GetCamera(), deltaTime );

		if ( m_bScrollFinished )
		{
			m_Game.GetStateMachine()->PushState( std::make_unique<TitleState>() );
		}
	}
}

void TitleState::Render()
{
	AnimateTitle();
	m_Registry.GetSystem<RenderTitleSystem>().Update(
		m_Game.GetRenderer(), m_Game.GetAssetManager(), m_Game.GetCamera() );
}

bool TitleState::OnEnter()
{
	// Turn music volume down
	Mix_VolumeMusic( 10 );

	// Add Assets to the Asset Manager
	m_Game.GetAssetManager()->AddMusic( "Title", "./assets/Music/Title_Theme.mp3" );
	if ( !m_Game.GetAssetManager()->HasTexture( "TitleScreen" ) )
		m_Game.GetAssetManager()->AddTextures(
			m_Game.GetRenderer(), "TitleScreen", "./assets/Backgrounds/DemoZeldaTitleScreen.png" );
	if ( !m_Game.GetAssetManager()->HasTexture( "title_scroll" ) )
		m_Game.GetAssetManager()->AddTextures(
			m_Game.GetRenderer(), "title_scroll", "./assets/Backgrounds/titleScroll.png" );
	if ( !m_Game.GetAssetManager()->HasTexture( "waterfall" ) )
		m_Game.GetAssetManager()->AddTextures(
			m_Game.GetRenderer(), "waterfall", "./assets/Backgrounds/waterfall.png" );

	// Start the Title Screen Music
	m_Game.GetMusicPlayer().PlayMusic( "Title", -1 );

	// Create the Title Screen Entity
	Entity titleScreen = m_Registry.CreateEntity();
	titleScreen.AddComponent<SpriteComponent>( SpriteComponent{ .assetID = "TitleScreen",
																.width = 256,
																.height = 240,
																.layer = 0,
																.bIsFixed = false,
																.srcRect = SDL_Rect{ 0, 0, 256, 240 } } );

	titleScreen.AddComponent<BackgroundImageComponent>( glm::vec2( 0, 0 ), glm::vec2( 4, 4 ) );
	titleScreen.AddComponent<TransformComponent>( glm::vec2( 0, 0 ), glm::vec2( 4, 4 ) );
	titleScreen.AddComponent<AnimationComponent>(
		AnimationComponent{ .numFrames = 3, .frameSpeedRate = 4, .vertical = false } );

	titleScreen.AddComponent<RigidBodyComponent>( glm::vec2( 0 ) );
	titleScreen.Tag( "title" );

	// Reset the camera pos to zero
	m_Game.GetCamera().SetCameraPosition( 0, 0 );

	if ( !m_pCamMoveTimer->isStarted() )
		m_pCamMoveTimer->Start();

	return true;
}

bool TitleState::OnExit()
{
	// Remove Assets/Entities from the m_Game that are not needed anymore
	m_Registry.GetSystem<RenderTitleSystem>().OnExit();

	// Remove Unused Textures
	m_Game.GetAssetManager()->RemoveTexture( "TitleScreen" );
	m_Game.GetAssetManager()->RemoveTexture( "title_scroll" );
	m_Game.GetAssetManager()->RemoveTexture( "waterfall" );

	// Remove Unused Music
	m_Game.GetAssetManager()->RemoveMusic( "Title" );

	return true;
}

void TitleState::ProcessEvents( SDL_Event& event )
{
	if ( InputManager::GetInstance().GetKeyboard().IsKeyJustPressed( KEY_SPACE ) ||
		 InputManager::GetInstance().GetGamepad().IsButtonJustPressed( GP_BTN_A ) )
	{
		m_Game.GetStateMachine()->PushState( std::make_unique<MenuState>() );
	}
}
