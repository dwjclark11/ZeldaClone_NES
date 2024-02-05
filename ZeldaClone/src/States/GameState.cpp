#include "GameState.h"
#include "../Game/Game.h"
#include "MenuState.h"
#include "GameOverState.h"
#include "../Systems/GameSystems/CollisionSystem.h"
#include "../AssetManager/AssetManager.h"
#include "../Systems/GameSystems/RenderSystem.h"
#include "../Systems/GameSystems/DamageSystem.h"
#include "../Systems/GameSystems/RenderHUDSystem.h"
#include "../Systems/GameSystems/MovementSystem.h"
#include "../Systems/RenderTextSystem.h"
#include "../Systems/GameSystems/CaptionSystem.h"
#include "../Systems/GameSystems/RenderHealthSystem.h"
#include "../Systems/GameSystems/RenderTileSystem.h"
#include "../Systems/GameSystems/RenderGameOverTextSystem.h"
#include "../Systems/GameSystems/RenderGameOverSystem.h"
#include "../Systems/GameSystems/HealthSystem.h"
#include "../Systems/GameSystems/TriggerSystem.h"
#include "../Systems/CameraMovementSystem.h"
#include "../Systems/GameSystems/CollectItemSystem.h"
#include "../Systems/GameSystems/ProjectileEmitterSystem.h"
#include "../Systems/GameSystems/ProjectileLifeCycleSystem.h"
#include "../Systems/GameSystems/AISystem.h"
#include "../Systems/MenuSystems/RenderMainMenuSystem.h"
#include "../Components/SpriteComponent.h"
#include "../Components/TransformComponent.h"
#include "../Components/RigidBodyComponent.h"
#include "../Components/AnimationComponent.h"
#include "../Components/BoxColliderComponent.h"
#include "../Components/KeyboardControlComponent.h"
#include "../Components/TriggerBoxComponent.h"
#include "../Components/ProjectileComponent.h"
#include "../Components/HealthComponent.h"
#include "../Components/RupeeGameComponent.h"
#include "../Components/EnemyComponent.h"
#include "../Components/SecretComponent.h"
#include "../Components/GameComponent.h"
#include "../Components/ItemComponent.h"
#include "../Components/ProjectileEmitterComponent.h"
#include "../Components/CaptionComponent.h"
#include "../Events/KeyPressedEvent.h"
#include "../Events/EventManager.h"
#include "../Events/TriggerEvent.h"
#include "../Game/LevelLoader.h"
#include "../Utilities/Utility.h"
#include "../StateMachines/NewPlayerStates.h"
#include "../Systems/SoundFXSystem.h"

#include "../Utilities/GameData.h"
#include "../Utilities/Camera.h"
#include "../Inputs/InputManager.h"
#include "../Inputs/Gamepad.h"
#include "../Game/Player.h"

// Set the values of the statics
const std::string GameState::gameID = "GAMESTATE";

bool GameState::firstEntered = false;

void GameState::UpdatePlayerKeys()
{
	const auto& player = game.GetPlayer();
	player->UpdatePlayer(game, inputManager);
}

void GameState::UpdatePauseContol()
{
	const auto& keyboard = inputManager.GetKeyboard();
	const auto& gamepad = inputManager.GetGamepad();
	auto& camera = game.GetCamera();
	auto& player = game.GetPlayer();

	// Set to paused
	if (keyboard.IsKeyJustReleased(KEY_P) || gamepad.IsButtonJustReleased(GP_BTN_Y))
	{
		camera.SetFadeFinished(false);
		camera.StartFadeOut(true);
		camera.StartFadeIn(false);
		inputManager.SetPaused(true);
		m_bPaused = true;
	}
	else if (keyboard.IsKeyJustPressed(KEY_C) || gamepad.IsButtonHeld(GP_BTN_BACK))
	{
		if (!game.IsDebugging())
			game.SetDebug(true);
		else
			game.SetDebug(false);
	}

	if (keyboard.IsKeyJustReleased(KEY_SPACE) || keyboard.IsKeyJustReleased(KEY_RSHIFT) || 
		gamepad.IsButtonJustReleased(GP_BTN_A) || gamepad.IsButtonJustReleased(GP_BTN_X))
	{
		player->SetAttacking(false);
		inputManager.SetAttack(false);
	}
}

GameState::GameState()
	: GameState(glm::vec2{0.f})
{
}

GameState::~GameState()
{

}

GameState::GameState(glm::vec2 cameraOffset)
	: game(Game::Instance()), reg(Registry::Instance())
	, inputManager(InputManager::GetInstance()), cameraOffset(cameraOffset)
	, gameData(GameData::GetInstance())
	, hudRect{ 0, 0, game.GetWindowWidth(), 256 }
	, m_bPaused{false}
	, m_TriggerSystem{ reg.GetSystem<TriggerSystem>() }
	, m_CollectItemSystem{ reg.GetSystem<CollectItemSystem>() }
	, m_AnimationSystem{ reg.GetSystem<AnimationSystem>() }
	, m_MovementSystem{ reg.GetSystem<MovementSystem>() }
	, m_ProjectileEmitterSystem{ reg.GetSystem<ProjectileEmitterSystem>() }
	, m_CameraMovementSystem{ reg.GetSystem<CameraMovementSystem>() }
	, m_ProjectileLifeCycleSystem{ reg.GetSystem<ProjectileLifeCycleSystem>() }
	, m_AISystem{ reg.GetSystem<AISystem>() }
	, m_CaptionSystem{ reg.GetSystem<CaptionSystem>() }
	, m_CollisionSystem{ reg.GetSystem<CollisionSystem>() }
	, m_DamageSystem{ reg.GetSystem<DamageSystem>() }
	, m_HealthSystem { reg.GetSystem<HealthSystem>() }
	, m_RenderSystem{ reg.GetSystem<RenderSystem>() }
	, m_RenderTileSystem{ reg.GetSystem<RenderTileSystem>() }
	, m_RenderHUDSystem{ reg.GetSystem<RenderHUDSystem>() }
	, m_RenderCollisionSystem{ reg.GetSystem<RenderCollisionSystem>() }
{
	game.GetCamera().SetCameraPosition(cameraOffset.x, cameraOffset.y);
}

void GameState::Update(const float& deltaTime)
{
	auto& camera = game.GetCamera();
	if (inputManager.IsPaused() && camera.GetFadeAlpha() == 0)
	{
		camera.SetFadeFinished(true);
		camera.StartFadeOut(false);
		game.GetStateMachine()->PushState(std::make_unique<PauseState>(), false);
	}

	// Check to see if level music has been paused
	if (!inputManager.IsPaused() && !camera.FadeOutStarted() && m_bPaused)
	{
		// Turn music volume up
		Mix_VolumeMusic(10);
		camera.StartFadeIn(true);
		m_bPaused = false;
	}

	m_TriggerSystem.Update(deltaTime);

	// Update the registry values
	reg.Update();

	m_CollectItemSystem.Update();
	m_AnimationSystem.Update();
	m_MovementSystem.Update(deltaTime);
	m_ProjectileEmitterSystem.Update();
	m_CameraMovementSystem.UpdatePlayerCam(game.GetCamera(), deltaTime);

	m_ProjectileLifeCycleSystem.Update();

	m_AISystem.Update();
	m_CaptionSystem.Update(deltaTime);

	m_CollisionSystem.Update();

	// Update the registry values
	reg.Update();

	// Update all Game systems
	game.GetPlayer()->UpdateStateMachine();
}

void GameState::Render()
{
	auto& camera = game.GetCamera();
	m_RenderTileSystem.Update();
	camera.UpdateScreenFlash();
	m_RenderSystem.Update();
	
	// Render all HUD objects
	SDL_SetRenderDrawColor(game.GetRenderer(), 0, 0, 0, 255);
	SDL_RenderFillRect(game.GetRenderer(), &hudRect);
	SDL_RenderDrawRect(game.GetRenderer(), &hudRect);
	SDL_SetRenderDrawColor(game.GetRenderer(), 0, 0, 0, 255);
	
	camera.UpdateCurtain();

	m_RenderHUDSystem.Update(game.GetRenderer(), game.GetAssetManager());
	m_HealthSystem.Update();

	// If the game is in debug mode, render the collision system
	if (game.IsDebugging())
	{
		m_RenderCollisionSystem.Update(game.GetRenderer(), camera.GetCameraRect());
	}
}

bool GameState::OnEnter()
{
	LevelLoader loader;
	auto& camera = game.GetCamera();
	if (!firstEntered)
	{
		// Always start the player and the camera from the beginning Location for now --> Create Constants for Special CAM Locations
		if (camera.GetCameraPos().x != 7168 && camera.GetCameraPos().x != 6720)
		{
			camera.SetCameraPosition(7168, 6720);
		}
		camera.StartCurtainOpen();

		// Open the lua libraries into the game
		game.GetLuaState().open_libraries(sol::lib::base, sol::lib::math, sol::lib::os);
		
		// Make sure the game is not in debug mode!!
		game.SetDebug(false);
		
		loader.LoadAssetsFromLuaTable(game.GetLuaState(), "game_state_assets");
		loader.LoadHUDFromLuaTable(game.GetLuaState(), "hud");
		loader.LoadEnemiesFromLuaTable(game.GetLuaState(), "overworld_enemies_2");
		loader.LoadEntitiesFromLuaTable(game.GetLuaState(), "over_world_entities");
		loader.LoadColliders("overworld_colliders_1");
		loader.LoadTriggers(game.GetLuaState(), "overworld_triggers");

		game.GetMusicPlayer().PlayMusic("Overworld", -1);
		// Load the overworld map
		loader.LoadMap("map", 4096, 1344);

		m_TriggerSystem.SubscribeToEvents(game.GetEventManager());
		m_CollectItemSystem.SubscribeToEvents(game.GetEventManager());
		m_MovementSystem.SubscribeToEvents(game.GetEventManager());
		m_ProjectileEmitterSystem.SubscribeKeyToEvents(game.GetEventManager());
		m_ProjectileEmitterSystem.SubscribeBtnToEvents(game.GetEventManager());
		m_DamageSystem.SubscribeToEvents(game.GetEventManager());

		firstEntered = true;
	}

	if (!game.GetPlayer())
	{
		loader.CreatePlayerEntityFromLuaTable(game.GetLuaState(), "new_player_create");
		// Load the player file based on the selected slot
		auto playerNum = GameData::GetInstance().PlayerNum();
		loader.LoadPlayerDataFromLuaTable(game.GetLuaState(), "save" + std::to_string(playerNum), playerNum);

		auto player = Registry::Instance().GetEntityByTag("player");
		// Reset the player health after pressing continue [Game Over]
		auto& playerHealth = player.GetComponent<HealthComponent>();

		if (playerHealth.healthPercentage <= 0)
		{
			playerHealth.healthPercentage = 2 * playerHealth.maxHearts;
		}

		game.GetPlayer()->SetPlayerCreated(true);
		{
			game.GetPlayer()->GetPlayerStateMachine().AddState(std::make_unique<IdleState>());
			game.GetPlayer()->GetPlayerStateMachine().ChangeState(player);
		}
	}

	// Read in all secrets!!
	loader.ReadInSecrets(game.GetLuaState());

	// Remove the menu/Game over system for it is not needed in the game state
	reg.RemoveSystem<RenderMainMenuSystem>();	
	game.GetAssetManager()->AddFonts("game_font", "./Assets/Fonts/prstart.ttf", 30);
	game.GetAssetManager()->AddTextures(game.GetRenderer(), "simple_enemies", "./Assets/EnemySprites/simple_enemies.png");

	return true;
}

bool GameState::OnExit()
{
	game.GetEventManager().Reset();
	m_RenderCollisionSystem.OnExit();
	m_RenderSystem.OnExit();
	m_RenderTileSystem.OnExit();
	firstEntered = false;
	return true;
}

void GameState::ProcessEvents(SDL_Event& event)
{
	UpdatePlayerKeys();
	UpdatePauseContol();
}
