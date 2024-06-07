#include "States/NameState.h"
#include "States/GameStateMachine.h"
#include "ECS/ECS.h"
#include "Systems/GameSystems/RenderSystem.h"
#include "Systems/NameSystems/RenderNameStateTextSystem.h"
#include "Systems/NameSystems/RenderNameSystem.h"
#include "Components/RegisterNameComponent.h"
#include "Components/TransformComponent.h"
#include "Components/TextLabelComponent.h"
#include "Components/SpriteComponent.h"
#include "Components/KeyboardControlComponent.h"
#include "States/MenuState.h"
#include "AssetManager/AssetManager.h"
#include "Game/Game.h"
#include "Game/LevelLoader.h"
#include "Utilities/Camera.h"
#include "Utilities/GameData.h"
#include "inputs/InputManager.h"
#include "inputs/Gamepad.h"

#include <SDL.h>

const std::string NameState::nameID = "NAME";

NameState::NameState()
	: game( Game::Instance() )
	, reg( Registry::Instance() )
	, m_InputManager( InputManager::GetInstance() )
	, m_GameData( GameData::GetInstance() )
	, m_Slot{ 0 }
	, m_Row{ 0 }
	, m_Col{ 0 }
	, m_bEditor{ false }
	, m_bKeyDown{ false }
{
}

void NameState::Update( const float& deltaTime )
{
	reg.Update();
}

void NameState::Render()
{
	Registry::Instance().GetSystem<RenderNameSystem>().Update( game.GetRenderer(), game.GetAssetManager() );
	Registry::Instance().GetSystem<RenderNameStateTextSystem>().Update(
		game.GetRenderer(), game.GetAssetManager(), game.GetCamera().GetCameraRect() );
}

bool NameState::OnEnter()
{
	reg.AddSystem<RenderNameStateTextSystem>();
	reg.AddSystem<RenderNameSystem>();

	game.GetAssetManager()->AddTextures( game.GetRenderer(), "name-letters", "./assets/HUDSprites/Name_Letters.png" );
	game.GetAssetManager()->AddTextures( game.GetRenderer(), "box", "./assets/HUDSprites/box.png" );
	game.GetAssetManager()->AddFonts( "charriot-font-120", "./assets/Fonts/charriot.ttf", 120 );

	Entity letters = reg.CreateEntity();
	letters.AddComponent<SpriteComponent>( SpriteComponent{ .assetID = "name-letters",
															.width = 128,
															.height = 112,
															.layer = 0,
															.bIsFixed = true,
															.srcRect = SDL_Rect{ 0, 0, 128, 112 } } );

	letters.AddComponent<TransformComponent>( glm::vec2( 260, 200 ), glm::vec2( 4, 4 ), 0 );
	letters.AddComponent<RegisterNameComponent>();

	Entity box = reg.CreateEntity();
	box.AddComponent<SpriteComponent>( SpriteComponent{ .assetID = "box",
														.width = 16,
														.height = 16,
														.layer = 0,
														.bIsFixed = true,
														.srcRect = SDL_Rect{ 0, 0, 16, 16 } } );

	box.AddComponent<TransformComponent>( glm::vec2( 260, 200 ), glm::vec2( 4, 4 ), 0 );
	box.AddComponent<RegisterNameComponent>();
	box.AddComponent<KeyboardControlComponent>();
	box.AddComponent<TextLabelComponent>(
		glm::vec2( 300, 800 ), "", "charriot-font-120", SDL_Color{ 255, 255, 255, 255 }, true );
	box.Tag( "box" );

	return true;
}

bool NameState::OnExit()
{
	Registry::Instance().GetSystem<RenderNameStateTextSystem>().OnExit();

	return true;
}

void NameState::ProcessEvents( SDL_Event& event )
{
	auto& keyboard = m_InputManager.GetKeyboard();
	auto& gamepad = m_InputManager.GetGamepad();

	if ( m_GameData.GetPlayer1Name().size() == 0 )
		m_Slot = 1;
	else if ( m_GameData.GetPlayer2Name().size() == 0 )
		m_Slot = 2;
	else if ( m_GameData.GetPlayer1Name().size() == 0 )
		m_Slot = 3;

	auto entity = Registry::Instance().GetEntityByTag( "box" );

	const auto& sprite = entity.GetComponent<SpriteComponent>();
	auto& transform = entity.GetComponent<TransformComponent>();
	auto& text = entity.GetComponent<TextLabelComponent>();

	if ( keyboard.IsKeyJustPressed( KEY_W ) || gamepad.IsButtonJustPressed( GP_BTN_DPAD_UP ) )
	{

		transform.position.y -= sprite.height * transform.scale.y * 2;
		game.GetSoundPlayer().PlaySoundFX( "text_slow", 0, SoundChannel::TEXT );
		m_Row--;
		if ( transform.position.y < 200 )
		{
			transform.position.y = 584;
			m_Row = 3;
		}
	}
	else if ( keyboard.IsKeyJustPressed( KEY_S ) || gamepad.IsButtonJustPressed( GP_BTN_DPAD_DOWN ) )
	{
		transform.position.y += sprite.height * transform.scale.y * 2;
		game.GetSoundPlayer().PlaySoundFX( "text_slow", 0, SoundChannel::TEXT );
		m_Row++;
		if ( transform.position.y > 584 )
		{
			transform.position.y = 200;
			m_Row = 0;
		}
	}
	else if ( keyboard.IsKeyJustPressed( KEY_D ) || gamepad.IsButtonJustPressed( GP_BTN_DPAD_RIGHT ) )
	{
		transform.position.x += sprite.width * transform.scale.x;
		game.GetSoundPlayer().PlaySoundFX( "text_slow", 0, SoundChannel::TEXT );
		m_Col++;
		if ( transform.position.x > 708 )
		{
			transform.position.x = 260;
			m_Row = 0;
		}
	}
	else if ( keyboard.IsKeyJustPressed( KEY_A ) || gamepad.IsButtonJustPressed( GP_BTN_DPAD_LEFT ) )
	{
		transform.position.x -= sprite.width * transform.scale.x;
		game.GetSoundPlayer().PlaySoundFX( "text_slow", 0, SoundChannel::TEXT );
		m_Col--;
		if ( transform.position.x < 260 )
		{
			transform.position.x = 708;
			m_Row = 7;
		}
	}
	else if ( keyboard.IsKeyJustPressed( KEY_SPACE ) || gamepad.IsButtonJustPressed( GP_BTN_A ) )
	{
		if ( text.text.size() < 6 )
		{
			bool valid = true;
			// Make sure that the row and col have a valid letter based on the
			// name-letters sprite
			if ( m_Row == 3 && m_Col > 1 )
			{
				valid = false;
			}

			// Create a new char based on ASCII Upper characters and the letter position
			// on the sprite
			char newChar = ( m_Row * 8 ) + m_Col + 65; // 65 is 'A'

			if ( valid )
				text.text += newChar;
		}

		if ( transform.position.x == 452 && transform.position.y == 584 )
		{
			if ( text.text.size() > 0 )
				text.text.erase( text.text.size() - 1 );
		}

		if ( transform.position.x == 580 && transform.position.y == 584 )
		{
			LevelLoader loader;
			m_sName = text.text.c_str();

			// Reset the column and the row after name is entered
			m_Row = 0;
			m_Col = 0;
			loader.SavePlayerNameToLuaTable( std::to_string( m_Slot ), m_sName );
			game.GetStateMachine()->PushState( std::make_unique<MenuState>() );
		}
	}
}
