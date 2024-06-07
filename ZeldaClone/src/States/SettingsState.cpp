#include "States/SettingsState.h"
#include "States/MenuState.h"
#include "States/GameStateMachine.h"
#include "Components/TransformComponent.h"
#include "Components/SpriteComponent.h"
#include "Components/TextLabelComponent.h"
#include "Components/SettingsComponent.h"
#include "Components/GamePadComponent.h"
#include "Components/GameComponent.h"

#include "Components/KeyboardControlComponent.h"
#include "Systems/RenderTextSystem.h"
#include "Systems/GameSystems/RenderSystem.h"

#include "Game/Game.h"
#include "AssetManager/AssetManager.h"
#include "Logger/Logger.h"

#include "inputs/InputManager.h"
#include "inputs/Keyboard.h"
#include "inputs/Gamepad.h"
#include "Utilities/Camera.h"
#include "Utilities/GameData.h"

#include <string>

SettingsState::SettingsState()
	: m_InputManager( InputManager::GetInstance() )
	, m_Game( Game::Instance() )
	, m_GameData( GameData::GetInstance() )
{
}

void SettingsState::Update( const float& deltaTime )
{
	Registry::Instance().Update();
}

void SettingsState::Render()
{
	SDL_Rect KeyRect = { 350, 215, 300, 500 };
	SDL_Rect KeyRectOutline = { 350, 215, 300, 500 };

	SDL_Rect BtnRect = { 650, 215, 300, 500 };
	SDL_Rect BtnRectOutline = { 650, 215, 300, 500 };

	SDL_SetRenderDrawColor( Game::Instance().GetRenderer(), 70, 70, 70, 70 );
	SDL_RenderFillRect( Game::Instance().GetRenderer(), &KeyRect );
	SDL_RenderDrawRect( Game::Instance().GetRenderer(), &KeyRect );
	SDL_RenderFillRect( Game::Instance().GetRenderer(), &BtnRect );
	SDL_RenderDrawRect( Game::Instance().GetRenderer(), &BtnRect );

	SDL_SetRenderDrawColor( Game::Instance().GetRenderer(), 100, 100, 100, 150 );
	SDL_RenderDrawRect( Game::Instance().GetRenderer(), &KeyRectOutline );
	SDL_RenderDrawRect( Game::Instance().GetRenderer(), &BtnRectOutline );

	Registry::Instance().GetSystem<RenderTextSystem>().Update();
	Registry::Instance().GetSystem<RenderSystem>().Update();
}

bool SettingsState::OnEnter()
{

	Game::Instance().GetAssetManager()->AddSoundFX( "bomb_drop", "./assets/sounds/Bomb_Drop.wav" );
	Game::Instance().GetAssetManager()->AddSoundFX( "get_item", "./assets/sounds/Get_Item.wav" );

	std::string attackKey = std::string( SDL_GetKeyName( m_InputManager.GetKeyCode( InputManager::Action::ATTACK ) ) );
	std::string moveUpKey = std::string( SDL_GetKeyName( m_InputManager.GetKeyCode( InputManager::Action::MOVE_UP ) ) );
	std::string moveDownKey =
		std::string( SDL_GetKeyName( m_InputManager.GetKeyCode( InputManager::Action::MOVE_DOWN ) ) );
	std::string moveLeftKey =
		std::string( SDL_GetKeyName( m_InputManager.GetKeyCode( InputManager::Action::MOVE_LEFT ) ) );
	std::string moveRightKey =
		std::string( SDL_GetKeyName( m_InputManager.GetKeyCode( InputManager::Action::MOVE_RIGHT ) ) );
	std::string useItemKey =
		std::string( SDL_GetKeyName( m_InputManager.GetKeyCode( InputManager::Action::USE_ITEM ) ) );

	std::string attackBtn = std::string(
		SDL_GameControllerGetStringForButton( m_InputManager.GetBtnCode( InputManager::Action::ATTACK ) ) );
	std::string moveUpBtn = std::string(
		SDL_GameControllerGetStringForButton( m_InputManager.GetBtnCode( InputManager::Action::MOVE_UP ) ) );
	std::string moveDownBtn = std::string(
		SDL_GameControllerGetStringForButton( m_InputManager.GetBtnCode( InputManager::Action::MOVE_DOWN ) ) );
	std::string moveLeftBtn = std::string(
		SDL_GameControllerGetStringForButton( m_InputManager.GetBtnCode( InputManager::Action::MOVE_LEFT ) ) );
	std::string moveRightBtn = std::string(
		SDL_GameControllerGetStringForButton( m_InputManager.GetBtnCode( InputManager::Action::MOVE_RIGHT ) ) );
	std::string useItemBtn = std::string(
		SDL_GameControllerGetStringForButton( m_InputManager.GetBtnCode( InputManager::Action::USE_ITEM ) ) );

	auto action = Registry::Instance().CreateEntity();
	action.AddComponent<TextLabelComponent>(
		glm::vec2( 100, 150 ), "ACTION", "charriot-font-60", SDL_Color{ 255, 255, 255, 255 } );
	action.AddComponent<SettingsComponent>();
	auto key = Registry::Instance().CreateEntity();
	key.AddComponent<TextLabelComponent>(
		glm::vec2( 400, 150 ), "KEY", "charriot-font-60", SDL_Color{ 255, 255, 255, 255 } );
	key.AddComponent<SettingsComponent>();
	auto btn = Registry::Instance().CreateEntity();
	btn.AddComponent<TextLabelComponent>(
		glm::vec2( 700, 150 ), "BUTTON", "charriot-font-60", SDL_Color{ 255, 255, 255, 255 } );
	btn.AddComponent<SettingsComponent>();

	auto upACT = Registry::Instance().CreateEntity();
	upACT.AddComponent<TextLabelComponent>(
		glm::vec2( 400, 225 ), moveUpKey, "charriot-font-40", SDL_Color{ 255, 255, 255, 255 } );
	upACT.AddComponent<KeyboardControlComponent>();
	upACT.AddComponent<SettingsComponent>( 0, SettingsInput::KEY );

	auto downACT = Registry::Instance().CreateEntity();
	downACT.AddComponent<TextLabelComponent>(
		glm::vec2( 400, 300 ), moveDownKey, "charriot-font-40", SDL_Color{ 255, 255, 255, 255 } );
	downACT.AddComponent<KeyboardControlComponent>();
	downACT.AddComponent<SettingsComponent>( 1, SettingsInput::KEY );

	auto leftACT = Registry::Instance().CreateEntity();
	leftACT.AddComponent<TextLabelComponent>(
		glm::vec2( 400, 375 ), moveLeftKey, "charriot-font-40", SDL_Color{ 255, 255, 255, 255 } );
	leftACT.AddComponent<KeyboardControlComponent>();
	leftACT.AddComponent<SettingsComponent>( 2, SettingsInput::KEY );

	auto rightACT = Registry::Instance().CreateEntity();
	rightACT.AddComponent<TextLabelComponent>(
		glm::vec2( 400, 450 ), moveRightKey, "charriot-font-40", SDL_Color{ 255, 255, 255, 255 } );
	rightACT.AddComponent<KeyboardControlComponent>();
	rightACT.AddComponent<SettingsComponent>( 3, SettingsInput::KEY );

	auto attackACT = Registry::Instance().CreateEntity();
	attackACT.AddComponent<TextLabelComponent>(
		glm::vec2( 400, 525 ), attackKey, "charriot-font-40", SDL_Color{ 255, 255, 255, 255 } );
	attackACT.AddComponent<KeyboardControlComponent>();
	attackACT.AddComponent<SettingsComponent>( 4, SettingsInput::KEY );

	auto useItemACT = Registry::Instance().CreateEntity();
	useItemACT.AddComponent<TextLabelComponent>(
		glm::vec2( 400, 600 ), useItemKey, "charriot-font-40", SDL_Color{ 255, 255, 255, 255 } );
	useItemACT.AddComponent<KeyboardControlComponent>();
	useItemACT.AddComponent<SettingsComponent>( 5, SettingsInput::KEY );

	auto upACTBtn = Registry::Instance().CreateEntity();
	upACTBtn.AddComponent<TextLabelComponent>(
		glm::vec2( 700, 225 ), moveUpBtn, "charriot-font-40", SDL_Color{ 255, 255, 255, 255 } );
	upACTBtn.AddComponent<GamePadComponent>();
	upACTBtn.AddComponent<SettingsComponent>( 0, SettingsInput::BUTTON );

	auto downACTBtn = Registry::Instance().CreateEntity();
	downACTBtn.AddComponent<TextLabelComponent>(
		glm::vec2( 700, 300 ), moveDownBtn, "charriot-font-40", SDL_Color{ 255, 255, 255, 255 } );
	downACTBtn.AddComponent<GamePadComponent>();
	downACTBtn.AddComponent<SettingsComponent>( 1, SettingsInput::BUTTON );

	auto leftACTBtn = Registry::Instance().CreateEntity();
	leftACTBtn.AddComponent<TextLabelComponent>(
		glm::vec2( 700, 375 ), moveLeftBtn, "charriot-font-40", SDL_Color{ 255, 255, 255, 255 } );
	leftACTBtn.AddComponent<GamePadComponent>();
	leftACTBtn.AddComponent<SettingsComponent>( 2, SettingsInput::BUTTON );

	auto rightACTBtn = Registry::Instance().CreateEntity();
	rightACTBtn.AddComponent<TextLabelComponent>(
		glm::vec2( 700, 450 ), moveRightBtn, "charriot-font-40", SDL_Color{ 255, 255, 255, 255 } );
	rightACTBtn.AddComponent<GamePadComponent>();
	rightACTBtn.AddComponent<SettingsComponent>( 3, SettingsInput::BUTTON );

	auto attackACTBtn = Registry::Instance().CreateEntity();
	attackACTBtn.AddComponent<TextLabelComponent>(
		glm::vec2( 700, 525 ), attackBtn, "charriot-font-40", SDL_Color{ 255, 255, 255, 255 } );
	attackACTBtn.AddComponent<GamePadComponent>();
	attackACTBtn.AddComponent<SettingsComponent>( 4, SettingsInput::BUTTON );

	auto useItemACTBtn = Registry::Instance().CreateEntity();
	useItemACTBtn.AddComponent<TextLabelComponent>(
		glm::vec2( 700, 600 ), useItemBtn, "charriot-font-40", SDL_Color{ 255, 255, 255, 255 } );
	useItemACTBtn.AddComponent<GamePadComponent>();
	useItemACTBtn.AddComponent<SettingsComponent>( 5, SettingsInput::BUTTON );

	auto moveUpWord = Registry::Instance().CreateEntity();
	moveUpWord.AddComponent<TextLabelComponent>(
		glm::vec2( 100, 225 ), "MOVE UP", "charriot-font-40", SDL_Color{ 255, 255, 255, 255 } );
	moveUpWord.AddComponent<SettingsComponent>();

	auto moveDownWord = Registry::Instance().CreateEntity();
	moveDownWord.AddComponent<TextLabelComponent>(
		glm::vec2( 100, 300 ), "MOVE DOWN", "charriot-font-40", SDL_Color{ 255, 255, 255, 255 } );
	moveDownWord.AddComponent<SettingsComponent>();

	auto moveLeftWord = Registry::Instance().CreateEntity();
	moveLeftWord.AddComponent<TextLabelComponent>(
		glm::vec2( 100, 375 ), "MOVE LEFT", "charriot-font-40", SDL_Color{ 255, 255, 255, 255 } );
	moveLeftWord.AddComponent<SettingsComponent>();

	auto moveRightWord = Registry::Instance().CreateEntity();
	moveRightWord.AddComponent<TextLabelComponent>(
		glm::vec2( 100, 450 ), "MOVE RIGHT", "charriot-font-40", SDL_Color{ 255, 255, 255, 255 } );
	moveRightWord.AddComponent<SettingsComponent>();

	auto attackWord = Registry::Instance().CreateEntity();
	attackWord.AddComponent<TextLabelComponent>(
		glm::vec2( 100, 525 ), "ATTACK", "charriot-font-40", SDL_Color{ 255, 255, 255, 255 } );
	attackWord.AddComponent<SettingsComponent>();

	auto useItemWord = Registry::Instance().CreateEntity();
	useItemWord.AddComponent<TextLabelComponent>(
		glm::vec2( 100, 600 ), "USE ITEM", "charriot-font-40", SDL_Color{ 255, 255, 255, 255 } );
	useItemWord.AddComponent<SettingsComponent>();

	auto selector = Registry::Instance().CreateEntity();
	selector.AddComponent<SpriteComponent>( SpriteComponent{ .assetID = "hearts",
															 .width = 16,
															 .height = 16,
															 .layer = 0,
															 .bIsFixed = true,
															 .srcRect = SDL_Rect{ 0, 0, 16, 16 } } );

	selector.AddComponent<TransformComponent>( glm::vec2( 50, 225 ), glm::vec2( 4, 4 ), 0 );
	selector.AddComponent<SettingsComponent>();
	selector.AddComponent<GameComponent>();
	selector.AddComponent<GamePadComponent>();
	selector.AddComponent<KeyboardControlComponent>();
	selector.AddComponent<TextLabelComponent>( glm::vec2( 200, 800 ),
											   "Choose an Action to Change!",
											   "charriot-font-40",
											   SDL_Color{ 255, 255, 255, 255 },
											   true );
	selector.Tag( "settings_selector" );

	return true;
}

bool SettingsState::OnExit()
{
	Registry::Instance().GetSystem<RenderTextSystem>().OnExitSettings();
	Game::Instance().GetAssetManager()->RemoveSoundFX( "bomb_drop" );
	Game::Instance().GetAssetManager()->RemoveSoundFX( "get_item" );

	return true;
}

void SettingsState::ProcessEvents( SDL_Event& event )
{
	//	auto selector = Registry::Instance().GetEntityByTag("settings_selector");
	//	auto& selectTransform = selector.GetComponent<TransformComponent>();
	//
	//
	//	if (!SettingsState::mEnterKey)
	//	{
	//		if (event.symbol == SDLK_RETURN)
	//		{
	//			SettingsState::mEnterKey = true;
	//			m_Game.GetSoundPlayer().PlaySoundFX("bomb_drop", 0, SoundChannel::ANY);
	//			break;
	//
	//		}
	//		else if (event.symbol == SDLK_UP)
	//		{
	//			selectTransform.position.y -= 75;
	//			SettingsState::mActionIndex--;
	//
	//			if (selectTransform.position.y < 225)
	//			{
	//				selectTransform.position.y = 600;
	//				SettingsState::mActionIndex = 5;
	//			}
	//			m_Game.GetSoundPlayer().PlaySoundFX("text_slow", 0, SoundChannel::TEXT);
	//		}
	//		else if (event.symbol == SDLK_DOWN)
	//		{
	//			selectTransform.position.y += 75;
	//			SettingsState::mActionIndex++;
	//
	//			if (selectTransform.position.y > 600)
	//			{
	//				selectTransform.position.y = 225;
	//				SettingsState::mActionIndex = 0;
	//			}
	//			m_Game.GetSoundPlayer().PlaySoundFX("text_slow", 0, SoundChannel::TEXT);
	//		}
	//	}
	//	else
	//	{
	//		m_InputManager.ChangeKeyBinding(static_cast<InputManager::Action>(SettingsState::mActionIndex),
	//event.symbol); 		std::string keyPressed = std::string(SDL_GetKeyName(event.symbol)); 		if
	//(entity.HasComponent<TextLabelComponent>())
	//		{
	//
	//			if (settings.index == SettingsState::mActionIndex && settings.input == SettingsInput::KEY)
	//			{
	//				Logger::Log(keyPressed);
	//				auto& text = entity.GetComponent<TextLabelComponent>();
	//				text.text = keyPressed;
	//				m_Game.GetSoundPlayer().PlaySoundFX("get_item", 0, SoundChannel::ANY);
	//				SettingsState::mEnterKey = false;
	//				break;
	//			}
	//		}
	//	}
	//}
	//		else
	//		continue;
	//	}
	//
	//	auto& selectText = selector.GetComponent<TextLabelComponent>();
	//	if (SettingsState::mEnterKey)
	//	{
	//		switch (SettingsState::mActionIndex)
	//		{
	//		case 0: selectText.text = "Changing Move Up Key"; break;
	//		case 1: selectText.text = "Changing Move Down Key"; break;
	//		case 2: selectText.text = "Changing Move Left Key"; break;
	//		case 3: selectText.text = "Changing Move Right Key"; break;
	//		case 4: selectText.text = "Changing Attack Key"; break;
	//		case 5: selectText.text = "Changing Use Item Key"; break;
	//		default: break;
	//		}
	//	}
	//	else
	//		selectText.text = "Choose an Action to Change!";

	if ( m_InputManager.GetKeyboard().IsKeyJustPressed( KEY_BACKSPACE ) ||
		 m_InputManager.GetGamepad().IsButtonJustPressed( GP_BTN_B ) )
	{
		Game::Instance().GetStateMachine()->PushState( std::make_unique<MenuState>() );
	}
}

void SettingsState::OnKeyDown( SDL_Event* event )
{
}

void SettingsState::OnKeyUp( SDL_Event* event )
{
}

void SettingsState::OnBtnDown( SDL_Event* event )
{
}

void SettingsState::OnBtnUp( SDL_Event* event )
{
}
