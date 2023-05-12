#include "PauseState.h"
#include "../Game/Game.h"
#include <SDL_ttf.h>
#include "../States/GameState.h"
#include "../States/SaveGameState.h"
#include "../Systems/PauseSystems/RenderPauseSystem.h"
#include "../Systems/GameSystems/RenderHUDSystem.h"
#include "../Systems/RenderTextSystem.h"
#include "../Game/LevelLoader.h"
#include "../Components/KeyboardControlComponent.h"
#include "../Components/GamePadComponent.h"
#include "../Components/TransformComponent.h"
#include "../Components/SpriteComponent.h"
#include "../Components/PauseComponent.h"
#include "../Utilities/Camera.h"
#include "../inputs/InputManager.h"
#include "../inputs/Keyboard.h"
#include "../inputs/Gamepad.h"

const std::string PauseState::pauseID = "PAUSE";
bool PauseState::firstEnter = false;

void PauseState::UpdateSelectedItemSprite()
{
	if (m_SelectedItem == gameData.GetSelectedItem())
		return;

	const auto& selectedItem = reg.GetEntityByTag("selectedItem");
	auto& sprite = selectedItem.GetComponent<SpriteComponent>();

	switch (gameData.GetSelectedItem())
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
	: game(Game::Instance()), reg(Registry::Instance())
	, gameData(GameData::GetInstance())
	, m_InputManager(InputManager::GetInstance())
	, m_SelectedItem{GameData::ItemType::EMPTY }
{
}

void PauseState::Update(const float& deltaTime)
{
	game.GetEventManager()->Reset();
	reg.Update();
	

	if (gameData.GetTotalBombs() > 0 && !gameData.HasItem(GameData::GameItems::BOMB))
	{
		Entity bombItem = reg.CreateEntity();
		bombItem.AddComponent<SpriteComponent>("items", 16, 16, 0, false, 64, 112);
		bombItem.AddComponent<TransformComponent>(glm::vec2(486, 190), glm::vec2(4, 4), 0.0);
		bombItem.AddComponent<PauseComponent>();
		bombItem.Tag("bombItem");
		bombItem.Group("pause");

		gameData.AddItem(GameData::GameItems::BOMB);
	}

	if (game.GetCamera().GetFadeAlpha() == 0 && !game.GetCamera().FadeFinished())
	{
		m_InputManager.SetPaused(false);
		game.GetCamera().SetFadeFinished(true);
		game.GetCamera().StartFadeOut(false);
		game.GetStateMachine()->PopState();
	}

	if (gameData.GetTotalBombs() == 0 && gameData.HasItem(GameData::GameItems::BOMB))
		gameData.RemoveItem(GameData::GameItems::BOMB);

	UpdateSelectedItemSprite();
}

void PauseState::Render()
{
	Registry::Instance().GetSystem<RenderPauseSystem>().Update(game.GetRenderer(), game.GetAssetManager());
	Registry::Instance().GetSystem<RenderHUDSystem>().Update(game.GetRenderer(), game.GetAssetManager());
	Registry::Instance().GetSystem<RenderTextSystem>().Update();
}

bool PauseState::OnEnter()
{
	// Turn music volume down while paused
	Mix_VolumeMusic(3);

	game.GetCamera().StartFadeIn(true);
	// =============================================================================================================================
	// Add all necessary systems to the registry if they are not yet registered
	// =============================================================================================================================
	if (!reg.HasSystem<RenderPauseSystem>()) 
		reg.AddSystem<RenderPauseSystem>();
	// =============================================================================================================================


	game.GetAssetManager()->AddTextures(game.GetRenderer(), "pause_hud", "./Assets/HUDSprites/pauseHud.png");
	firstEnter = true;

	Entity pauseSelector = reg.CreateEntity();
	pauseSelector.AddComponent<SpriteComponent>("box", 16, 16, 0, false, 16, 0);
	pauseSelector.AddComponent<TransformComponent>(glm::vec2(386, 190), glm::vec2(4, 4), 0.0);
	pauseSelector.AddComponent<PauseComponent>();
	pauseSelector.AddComponent<KeyboardControlComponent>();
	pauseSelector.AddComponent<GamePadComponent>();
	pauseSelector.Tag("pauseSelector");
	pauseSelector.Group("pause");

	Entity selectedItem = reg.CreateEntity();
	selectedItem.AddComponent<SpriteComponent>("items", 16, 16, 0, false, 48, 16);
	selectedItem.AddComponent<TransformComponent>(glm::vec2(200, 185), glm::vec2(6, 6), 0.0);
	selectedItem.AddComponent<PauseComponent>();
	selectedItem.AddComponent<KeyboardControlComponent>();
	selectedItem.Tag("selectedItem");
	selectedItem.Group("pause");

	Entity hudHolder = reg.CreateEntity();
	hudHolder.AddComponent<TransformComponent>(glm::vec2(325, 100), glm::vec2(5, 5), 0.0);
	hudHolder.AddComponent<SpriteComponent>("pause_hud", 96, 64, 5, true, 0, 0);
	hudHolder.AddComponent<PauseComponent>();
	hudHolder.Group("pause");

	Entity hudHolder2 = reg.CreateEntity();
	hudHolder2.AddComponent<TransformComponent>(glm::vec2(144, 135), glm::vec2(6, 6), 0.0);
	hudHolder2.AddComponent<SpriteComponent>("hud_holder", 32, 32, 5, true, 0, 0);
	hudHolder2.AddComponent<PauseComponent>();
	hudHolder2.Group("pause");

	Entity triforce = reg.CreateEntity();

	triforce.AddComponent<SpriteComponent>("triforce", 96, 64, 0, false, gameData.GetTotalTriforce() * 96, 0);
	triforce.AddComponent<TransformComponent>(glm::vec2(325, 400), glm::vec2(4, 4), 0.0);
	triforce.AddComponent<PauseComponent>();

	// Top Row Items
	if (gameData.HasItem(GameData::GameItems::BOOMERANG))
	{
		Entity boomerang = reg.CreateEntity();
		boomerang.AddComponent<SpriteComponent>("items", 16, 16, 0, false, 0, 112);
		boomerang.AddComponent<TransformComponent>(glm::vec2(386, 190), glm::vec2(4, 4), 0.0);
		boomerang.AddComponent<PauseComponent>();
		boomerang.Group("pause");
	}

	if (gameData.GetTotalBombs() > 0 && gameData.HasItem(GameData::GameItems::BOMB))
	{
		Entity bombs = reg.CreateEntity();
		bombs.AddComponent<SpriteComponent>("items", 16, 16, 0, false, 64, 112);
		bombs.AddComponent<TransformComponent>(glm::vec2(486, 190), glm::vec2(4, 4), 0.0);
		bombs.AddComponent<PauseComponent>();
		bombs.Tag("bombItem");
		bombs.Group("pause");
	}

	if (gameData.HasItem(GameData::GameItems::BOW))
	{
		Entity bow = reg.CreateEntity();
		bow.AddComponent<SpriteComponent>("items", 16, 16, 0, false, 64, 0);
		bow.AddComponent<TransformComponent>(glm::vec2(586, 190), glm::vec2(4, 4), 0.0);
		bow.AddComponent<PauseComponent>();
		bow.Group("pause");
	}

	if (gameData.HasItem(GameData::GameItems::CANDLE))
	{
		Entity candle = reg.CreateEntity();
		candle.AddComponent<SpriteComponent>("items", 16, 16, 0, false, 0, 48);
		candle.AddComponent<TransformComponent>(glm::vec2(686, 190), glm::vec2(4, 4), 0.0);
		candle.AddComponent<PauseComponent>();
		candle.Group("pause");
	}

	// Bottom Row Items
	if (gameData.HasItem(GameData::GameItems::FLUTE))
	{
		Entity flute = reg.CreateEntity();
		flute.AddComponent<SpriteComponent>("items", 16, 16, 0, false, 80, 16);
		flute.AddComponent<TransformComponent>(glm::vec2(386, 260), glm::vec2(4, 4), 0.0);
		flute.AddComponent<PauseComponent>();
		flute.Group("pause");
	}

	if (gameData.HasItem(GameData::GameItems::FOOD))
	{
		Entity meat = reg.CreateEntity();
		meat.AddComponent<SpriteComponent>("items", 16, 16, 0, false, 112, 16);
		meat.AddComponent<TransformComponent>(glm::vec2(486, 260), glm::vec2(4, 4), 0.0);
		meat.AddComponent<PauseComponent>();
		meat.Group("pause");
	}

	if (gameData.HasItem(GameData::GameItems::RED_POTION))
	{
		Entity potion = reg.CreateEntity();
		potion.AddComponent<SpriteComponent>("items", 16, 16, 0, false, 32, 32);
		potion.AddComponent<TransformComponent>(glm::vec2(586, 260), glm::vec2(4, 4), 0.0);
		potion.AddComponent<PauseComponent>();
		potion.Group("pause");
	}

	if (gameData.HasItem(GameData::GameItems::MAGIC_ROD))
	{
		Entity magicalRod = reg.CreateEntity();
		magicalRod.AddComponent<SpriteComponent>("items", 16, 16, 0, false, 64, 80);
		magicalRod.AddComponent<TransformComponent>(glm::vec2(686, 260), glm::vec2(4, 4), 0.0);
		magicalRod.AddComponent<PauseComponent>();
		magicalRod.Group("pause");
	}
	
	return false;
}

bool PauseState::OnExit()
{
	Registry::Instance().GetSystem<RenderPauseSystem>().OnExit();
	return true;
}

void PauseState::ProcessEvents(SDL_Event& event)
{
	const auto& selector = reg.GetEntityByTag("pauseSelector");
	auto& sprite = selector.GetComponent<SpriteComponent>();
	auto& transform = selector.GetComponent<TransformComponent>();
	auto& keyboard = m_InputManager.GetKeyboard();
	auto& gamepad = m_InputManager.GetGamepad();

	if (keyboard.IsKeyJustPressed(KEY_W) || gamepad.IsButtonJustPressed(GP_BTN_DPAD_UP))
	{
		transform.position.y -= ((sprite.height * transform.scale.y) + 6);
		game.GetSoundPlayer().PlaySoundFX("text_slow", 0, SoundChannel::TEXT);
		if (transform.position.y < 190) 
			transform.position.y = 260;
	}
	else if (keyboard.IsKeyJustPressed(KEY_S) || gamepad.IsButtonJustPressed(GP_BTN_DPAD_DOWN))
	{
		transform.position.y += ((sprite.height * transform.scale.y) + 6);
		game.GetSoundPlayer().PlaySoundFX("text_slow", 0, SoundChannel::TEXT);
		if (transform.position.y > 260) 
			transform.position.y = 190;
	}
	else if (keyboard.IsKeyJustPressed(KEY_A) || gamepad.IsButtonJustPressed(GP_BTN_DPAD_LEFT))
	{
		transform.position.x -= 100;
		game.GetSoundPlayer().PlaySoundFX("text_slow", 0, SoundChannel::TEXT);
		if (transform.position.x < 386)
			transform.position.x = 686;
	}
	else if (keyboard.IsKeyJustPressed(KEY_D) || gamepad.IsButtonJustPressed(GP_BTN_DPAD_RIGHT))
	{
		transform.position.x += 100;
		game.GetSoundPlayer().PlaySoundFX("text_slow", 0, SoundChannel::TEXT);
		if (transform.position.x > 686) 
			transform.position.x = 386;
	}
	else if (keyboard.IsKeyJustPressed(KEY_SPACE) || gamepad.IsButtonJustPressed(GP_BTN_A))
	{
		if (transform.position.x == 386 && transform.position.y == 190 && gameData.HasItem(GameData::GameItems::BOOMERANG))
		{
			gameData.SetSelectedItem(GameData::ItemType::BOOMERANG);
		}
		else if ((transform.position.x == 486 && transform.position.y == 190) && gameData.GetTotalBombs() > 0 && gameData.HasItem(GameData::GameItems::BOMB))
		{
			gameData.SetSelectedItem(GameData::ItemType::BOMB);
		}
		else if (transform.position.x == 586 && transform.position.y == 190 && gameData.HasItem(GameData::GameItems::BOW))
		{
			gameData.SetSelectedItem(GameData::ItemType::BOW);
		}
		else if (transform.position.x == 686 && transform.position.y == 190 && gameData.HasItem(GameData::GameItems::CANDLE))
		{
			gameData.SetSelectedItem(GameData::ItemType::CANDLE);
		}
		else if (transform.position.x == 386 && transform.position.y == 260)
		{

		}
		else if (transform.position.x == 486 && transform.position.y == 260 && gameData.HasItem(GameData::GameItems::FOOD))
		{
			gameData.SetSelectedItem(GameData::ItemType::FOOD);
		}
		else if (transform.position.x == 586 && transform.position.y == 260 && gameData.HasItem(GameData::GameItems::RED_POTION))
		{
			gameData.SetSelectedItem(GameData::ItemType::POTION_RED);
		}
		else if (transform.position.x == 686 && transform.position.y == 260 && gameData.HasItem(GameData::GameItems::MAGIC_ROD))
		{
			gameData.SetSelectedItem(GameData::ItemType::MAGIC_ROD);
		}
	}
	else if (keyboard.IsKeyJustReleased(KEY_P) || gamepad.IsButtonJustReleased(GP_BTN_Y))
	{
		game.GetCamera().SetFadeFinished(false);
		game.GetCamera().StartFadeOut(true);
	}
	else if (keyboard.IsKeyJustReleased(KEY_M) || gamepad.IsButtonJustReleased(GP_BTN_START))
	{
		// Open the save Screen!
		game.GetStateMachine()->PushState(new SaveGameState());
	}
}
