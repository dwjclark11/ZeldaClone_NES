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
	const auto& playerEnt = player->GetPlayer();
	auto& playerRigidbody = playerEnt.GetComponent<RigidBodyComponent>();

	auto shield = player->GetShield();
	auto& shieldRigidbody = shield.GetComponent<RigidBodyComponent>();

	auto sword = player->GetSword();
	auto& swordRigidbody = sword.GetComponent<RigidBodyComponent>();

	if (game.GetCamera().GetFadeAlpha() != 255 || game.PlayerHold())
	{
		playerRigidbody.velocity = glm::vec2(0);
		shieldRigidbody.velocity = glm::vec2(0);
		swordRigidbody.velocity = glm::vec2(0);
		return;
	}

	auto& playerTransform = playerEnt.GetComponent<TransformComponent>();
	auto& playerCollider = playerEnt.GetComponent<BoxColliderComponent>();
	auto& playerSprite = playerEnt.GetComponent<SpriteComponent>();
	auto& playerControl = playerEnt.GetComponent<KeyboardControlComponent>();
	auto& shieldTransform = shield.GetComponent<TransformComponent>();
	auto& shieldCollider = shield.GetComponent<BoxColliderComponent>();
	auto& swordTransform = sword.GetComponent<TransformComponent>();
	auto& swordCollider = sword.GetComponent<BoxColliderComponent>();

	const auto& keyboard = inputManager.GetKeyboard();
	const auto& gamepad = inputManager.GetGamepad();

	if (keyboard.IsKeyHeld(KEY_W) || gamepad.IsButtonHeld(GP_BTN_DPAD_UP))
	{
		playerRigidbody.velocity = playerControl.upVelocity;
		playerSprite.srcRect.x = playerSprite.width * 2;

		shieldTransform.position = playerTransform.position;
		shieldCollider.height = 2;
		shieldCollider.width = 24;
		shieldCollider.offset = glm::vec2(48, 32);
		shieldRigidbody = playerRigidbody;

		swordTransform.position = playerTransform.position;
		swordCollider.height = 2;
		swordCollider.width = 2;
		swordCollider.offset = glm::vec2(64, 60);
		swordRigidbody = playerRigidbody;

		playerRigidbody.dir = RigidBodyComponent::Dir::UP;
	}

	if (keyboard.IsKeyHeld(KEY_D) || gamepad.IsButtonHeld(GP_BTN_DPAD_RIGHT))
	{
		playerSprite.srcRect.x = playerSprite.width * 3;
		playerRigidbody.velocity = playerControl.rightVelocity;

		shieldTransform.position = playerTransform.position;
		shieldCollider.height = 30;
		shieldCollider.width = 2;
		shieldCollider.offset = glm::vec2(90, 56);
		shieldRigidbody = playerRigidbody;

		swordTransform.position = playerTransform.position;
		swordCollider.height = 2;
		swordCollider.width = 2;
		swordCollider.offset = glm::vec2(64, 60);
		swordRigidbody = playerRigidbody;

		playerRigidbody.dir = RigidBodyComponent::Dir::RIGHT;
	}

	if (keyboard.IsKeyHeld(KEY_S) || gamepad.IsButtonHeld(GP_BTN_DPAD_DOWN))
	{
		playerRigidbody.velocity = playerControl.downVelocity;
		playerSprite.srcRect.x = playerSprite.width * 0;

		shieldTransform.position = playerTransform.position;
		shieldCollider.height = 2;
		shieldCollider.width = 24;
		shieldCollider.offset = glm::vec2(40, 84);
		shieldRigidbody = playerRigidbody;

		swordTransform.position = playerTransform.position;
		swordCollider.height = 2;
		swordCollider.width = 2;
		swordCollider.offset = glm::vec2(64, 60);
		swordRigidbody = playerRigidbody;

		playerRigidbody.dir = RigidBodyComponent::Dir::DOWN;
	}

	if (keyboard.IsKeyHeld(KEY_A) || gamepad.IsButtonHeld(GP_BTN_DPAD_LEFT))
	{
		playerRigidbody.velocity = playerControl.leftVelocity;
		playerSprite.srcRect.x = playerSprite.width * 1;

		shieldTransform.position = playerTransform.position;
		shieldCollider.height = 30;
		shieldCollider.width = 2;
		shieldCollider.offset = glm::vec2(30, 50);
		shieldRigidbody = playerRigidbody;

		swordTransform.position = playerTransform.position;
		swordCollider.height = 2;
		swordCollider.width = 2;
		swordCollider.offset = glm::vec2(64, 60);
		swordRigidbody = playerRigidbody;

		playerRigidbody.dir = RigidBodyComponent::Dir::LEFT;
	}
}
void GameState::UpdatePauseContol()
{
	const auto& keyboard = inputManager.GetKeyboard();
	const auto& gamepad = inputManager.GetGamepad();

	// Set to paused
	if (keyboard.IsKeyJustReleased(KEY_P) || gamepad.IsButtonJustReleased(GP_BTN_Y))
	{
		game.GetCamera().SetFadeFinished(false);
		game.GetCamera().StartFadeOut(true);
		game.GetCamera().StartFadeIn(false);
		inputManager.SetPaused(true);
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
		inputManager.SetAttack(false);
	}
}

GameState::GameState()
	: GameState(glm::vec2{0.f})
{
}

GameState::GameState(glm::vec2 cameraOffset)
	: game(Game::Instance()), reg(Registry::Instance())
	, inputManager(InputManager::GetInstance()), cameraOffset(cameraOffset)
	, gameData(GameData::GetInstance())
	, hudRect{ 0, 0, game.GetWindowWidth(), 256 }

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
	if (!inputManager.IsPaused() && !camera.FadeOutStarted())
	{
		// Turn music volume up
		Mix_VolumeMusic(10);
		camera.StartFadeIn(true);
	}
	Registry::Instance().GetSystem<TriggerSystem>().Update(deltaTime);

	// Update the registry values
	reg.Update();

	// Update all Game systems
	game.GetPlayer()->UpdateStateMachine();

	Registry::Instance().GetSystem<CollectItemSystem>().Update();
	Registry::Instance().GetSystem<AnimationSystem>().Update();
	Registry::Instance().GetSystem<MovementSystem>().Update(deltaTime);
	Registry::Instance().GetSystem<ProjectileEmitterSystem>().Update();
	Registry::Instance().GetSystem<CameraMovementSystem>().UpdatePlayerCam(game.GetCamera(), deltaTime);

	Registry::Instance().GetSystem<ProjectileLifeCycleSystem>().Update();

	Registry::Instance().GetSystem<AISystem>().Update();
	Registry::Instance().GetSystem<CaptionSystem>().Update(deltaTime);
	
	Registry::Instance().GetSystem<CollisionSystem>().Update();

	// Update the registry values
	reg.Update();
}

void GameState::Render()
{
	auto& camera = game.GetCamera();
	Registry::Instance().GetSystem<RenderTileSystem>().Update();
	camera.UpdateScreenFlash();
	Registry::Instance().GetSystem<RenderSystem>().Update();
	
	// Render all HUD objects
	SDL_SetRenderDrawColor(game.GetRenderer(), 0, 0, 0, 255);
	SDL_RenderFillRect(game.GetRenderer(), &hudRect);
	SDL_RenderDrawRect(game.GetRenderer(), &hudRect);
	SDL_SetRenderDrawColor(game.GetRenderer(), 0, 0, 0, 255);
	
	camera.UpdateCurtain();

	Registry::Instance().GetSystem<RenderHUDSystem>().Update(game.GetRenderer(), game.GetAssetManager());
	Registry::Instance().GetSystem<HealthSystem>().Update();

	// If the game is in debug mode, render the collision system
	if (game.IsDebugging())
	{
		Registry::Instance().GetSystem<RenderCollisionSystem>().Update(game.GetRenderer(), camera.GetCameraRect());
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

		// ===============================================================================================
		// Add all necessary systems to the registry if they are not yet registered
		// ===============================================================================================
		reg.AddSystem<RenderHUDSystem>();
		reg.AddSystem<ProjectileEmitterSystem>();
		reg.AddSystem<ProjectileLifeCycleSystem>();
		reg.AddSystem<DamageSystem>();
		reg.AddSystem<RenderHealthSystem>();
		reg.AddSystem<RenderTileSystem>();
		reg.AddSystem<HealthSystem>();
		reg.AddSystem<CollisionSystem>(game.GetEventManager());
		reg.AddSystem<MovementSystem>();
		reg.AddSystem<TriggerSystem>();
		reg.AddSystem<CollectItemSystem>();
	
		reg.AddSystem<RenderTextSystem>();
		reg.AddSystem<AISystem>();
		reg.AddSystem<CaptionSystem>();
		// =================================================================================================

		game.GetMusicPlayer().PlayMusic("Overworld", -1);
		// Load the overworld map
		loader.LoadMap("map", 4096, 1344);

		Registry::Instance().GetSystem<CollectItemSystem>().SubscribeToEvents(game.GetEventManager());
		Registry::Instance().GetSystem<TriggerSystem>().SubscribeToEvents(game.GetEventManager());
		Registry::Instance().GetSystem<MovementSystem>().SubscribeToEvents(game.GetEventManager());
		Registry::Instance().GetSystem<ProjectileEmitterSystem>().SubscribeKeyToEvents(game.GetEventManager());
		Registry::Instance().GetSystem<ProjectileEmitterSystem>().SubscribeBtnToEvents(game.GetEventManager());
		Registry::Instance().GetSystem<DamageSystem>().SubscribeToEvents(game.GetEventManager());

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
	reg.GetSystem<RenderCollisionSystem>().OnExit();
	reg.GetSystem<RenderSystem>().OnExit();
	reg.GetSystem<RenderTileSystem>().OnExit();
	firstEntered = false;
	return true;
}

void GameState::ProcessEvents(SDL_Event& event)
{
	UpdatePlayerKeys();
	UpdatePauseContol();
}
