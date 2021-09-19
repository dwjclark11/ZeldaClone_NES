#include "GameState.h"
#include "../Game/Game.h"
#include "PauseState.h"
#include "GameOverState.h"
#include "../Systems/GameSystems/CollisionSystem.h"
#include "../AssetManager/AssetManager.h"
#include "../Systems/GameSystems/RenderSystem.h"
#include "../Systems/GameSystems/DamageSystem.h"
#include "../Systems/GameSystems/RenderHUDSystem.h"
#include "../Systems/GameSystems/MovementSystem.h"
#include "../Systems/RenderTextSystem.h"
#include "../Systems/GameSystems/RenderHealthSystem.h"
#include "../Systems/GameSystems/KeyboardControlSystem.h"
#include "../Systems/GameSystems/RenderTileSystem.h"
#include "../Systems/GameSystems/HealthSystem.h"
#include "../Systems/GameSystems/TriggerSystem.h"
#include "../Systems/CameraMovementSystem.h"
#include "../Systems/GameSystems/CollectItemSystem.h"
#include "../Systems/GameSystems/ProjectileEmitterSystem.h"
#include "../Systems/GameSystems/ProjectileLifeCycleSystem.h"
#include "../Systems/GameSystems/ScriptSystem.h"
#include "../Components/SpriteComponent.h"
#include "../Components/TransformComponent.h"
#include "../Components/RigidBodyComponent.h"
#include "../Components/AnimationComponent.h"
#include "../Components/TriggerBoxComponent.h"
#include "../Components/ProjectileComponent.h"
#include "../Components/RupeeTypeComponent.h"
#include "../Components/ItemComponent.h"
#include "../Components/ProjectileEmitterComponent.h"
#include "../Events/KeyPressedEvent.h"
#include "../Events/EventManager.h"
#include "../Events/TriggerEvent.h"
#include "../Game/LevelLoader.h"
#include "../Systems/GameSystems/GamePadSystem.h"
#include "../Utilities/Utility.h"


// Set the values of the statics
const std::string GameState::gameID = "GAMESTATE";
bool GameState::playerDead = false;
bool GameState::firstEntered = false;
int GameState::totalRupees = 0;
int GameState::totalBombs = 0;
int GameState::totalKeys = 0;


GameState::GameState(glm::vec2 cameraOffset)
{
	this->cameraOffset = cameraOffset; 
	Game::Instance()->GetCamera().x = this->cameraOffset.x;
	Game::Instance()->GetCamera().y = this->cameraOffset.y;
}

void GameState::Update(const double& deltaTime)
{
	// Check to see if level music has been paused
	if (GamePadSystem::paused)
	{
		Game::Instance()->GetSystem<MusicPlayerSystem>().PlayMusic(Game::Instance()->GetAssetManager(), "Overworld", -1);
		GamePadSystem::paused = false;
	}
	if (State::exitToMain) 
	{
		Game::Instance()->GetStateMachine()->PopState();
		Game::Instance()->GetStateMachine()->PushState(new MenuState());
		State::exitToMain = false;
	}
	// Reset the event manager queue
	Game::Instance()->GetEventManager()->Reset();
	

	Registry::Instance()->GetSystem<DamageSystem>().SubscribeToEvents(Game::Instance()->GetEventManager());
	
	if (DamageSystem::hitEnemyTimerStarted && !enemyTimer.isStarted()) enemyTimer.Start();
	if (DamageSystem::hitPlayerTimerStarted && !playerTimer.isStarted()) playerTimer.Start();

	// If the enemy has been hit, wait
	if (enemyTimer.GetTicks() > 1000)
	{
		DamageSystem::hitEnemyTimerStarted = false;
		enemyTimer.Stop();
	}
	
	// If the player has been hit, wait
	if (playerTimer.GetTicks() > 1000)
	{
		DamageSystem::hitPlayerTimerStarted = false;
		playerTimer.Stop();
	}
	
	Registry::Instance()->GetSystem<ProjectileEmitterSystem>().SubscribeToEvents(Game::Instance()->GetEventManager());
	Registry::Instance()->GetSystem<MovementSystem>().SubscribeToEvents(Game::Instance()->GetEventManager());
	Registry::Instance()->GetSystem<TriggerSystem>().SubscribeToEvents(Game::Instance()->GetEventManager());
	Registry::Instance()->GetSystem<CollectItemSystem>().SubscribeToEvents(Game::Instance()->GetEventManager());
	Registry::Instance()->GetSystem<KeyboardControlSystem>().SubscribeToEvents(Game::Instance()->GetEventManager());
	
	// Update the registry values
	Registry::Instance()->Update();
	
	// Update all Game systems
	
	Registry::Instance()->GetSystem<AnimationSystem>().Update();
	Registry::Instance()->GetSystem<ProjectileEmitterSystem>().Update(Registry::Instance());
	Registry::Instance()->GetSystem<ProjectileEmitterSystem>().UpdateGamePad();
	// Check to see if there are any events in the queue and execute them
	//Registry::Instance()->GetSystem<KeyboardControlSystem>().SubscribeToEvents(Game::Instance()->GetEventManager());

	Registry::Instance()->GetSystem<CollisionSystem>().Update(Game::Instance()->GetEventManager(), Game::Instance()->GetAssetManager(), Game::Instance()->GetCamera());
	Registry::Instance()->GetSystem<MovementSystem>().Update(deltaTime);
	Registry::Instance()->GetSystem<CameraMovementSystem>().Update(Game::Instance()->GetCamera());
	Registry::Instance()->GetSystem<ProjectileLifeCycleSystem>().Update();
	
	// If the player is dead, remove all entities and go to the game over state
	if (playerDead)
	{
		Registry::Instance()->GetSystem<RenderHUDSystem>().OnExit();
		Registry::Instance()->GetSystem<RenderHealthSystem>().OnExit();
		//firstEntered = false;
		Game::Instance()->GetStateMachine()->PopState();
		Game::Instance()->GetStateMachine()->PushState(new GameOverState());
	}

	Registry::Instance()->GetSystem<ScriptSystem>().Update(deltaTime, SDL_GetTicks());

	// Convert the total Rupees
	ConvertNumberParser("rupee_hundreds", totalRupees, 2);
	ConvertNumberParser("rupee_tens", totalRupees, 1);
	ConvertNumberParser("rupee_ones", totalRupees, 0);
	ConvertNumberParser("keys_tens", totalKeys, 1);
	ConvertNumberParser("key_ones", totalKeys, 0);
	ConvertNumberParser("bombs_tens", totalBombs, 1);
	ConvertNumberParser("bombs_ones", totalBombs, 0);
}

void GameState::Render()
{
	// Create the HUD rect --> black rectangle that all the HUD items are on
	SDL_Rect hudRect = { 0, 0, Game::Instance()->windowWidth, Game::Instance()->windowHeight / 6 + (Game::Instance()->tilePixels * Game::Instance()->gameScale) + 64};
	Game::Instance()->GetSystem<RenderTileSystem>().Update(Game::Instance()->GetRenderer(), Game::Instance()->GetAssetManager(), Game::Instance()->GetCamera());

	
	// Update all other render systems
	Game::Instance()->GetSystem<RenderSystem>().Update(Game::Instance()->GetRenderer(), Game::Instance()->GetAssetManager(), Game::Instance()->GetCamera());
	
	// Render all HUD objects
	SDL_SetRenderDrawColor(Game::Instance()->GetRenderer(), 0, 0, 0, 255);
	SDL_RenderFillRect(Game::Instance()->GetRenderer(), &hudRect);
	SDL_RenderDrawRect(Game::Instance()->GetRenderer(), &hudRect);
	SDL_SetRenderDrawColor(Game::Instance()->GetRenderer(), 0, 0, 0, 255);
	
	Game::Instance()->GetSystem<RenderHUDSystem>().Update(Game::Instance()->GetRenderer(), Game::Instance()->GetAssetManager());
	Game::Instance()->GetSystem<RenderTextSystem>().Update(Game::Instance()->GetRenderer(), Game::Instance()->GetAssetManager(), Game::Instance()->GetCamera());

	Registry::Instance()->GetSystem<HealthSystem>().Update();
	Game::Instance()->GetSystem<RenderHealthSystem>().Update(Game::Instance()->GetRenderer(), Game::Instance()->GetCamera());
	
	// If the game is in debug mode, render the collision system
	if (Game::isDebug)
	{
		Game::Instance()->GetSystem<RenderCollisionSystem>().Update(Game::Instance()->GetRenderer(), Game::Instance()->GetCamera());
	}
}

bool GameState::OnEnter()
{
	
	if (!firstEntered)
	{
		// Set timers back to default
		enemyTimer.Stop();
		playerTimer.Stop();
		LevelLoader loader;
		// Open the lua libraries into the game
		lua.open_libraries(sol::lib::base, sol::lib::math, sol::lib::os);
		Game::isDebug = false;
		loader.LoadAssetsFromLuaTable(lua, "game_state_assets");
		loader.LoadHUDFromLuaTable(lua, "hud");
		loader.LoadEnemiesFromLuaTable(lua, "enemies", Game::Instance()->GetAssetManager());
		loader.LoadColliders(Game::Instance()->GetAssetManager(), Game::Instance()->GetRenderer(), "colliders");

		// Player is now created from a Lua script instead of Hard Coded
		if (!Game::Instance()->GetplayerCreated())
		{
			loader.CreatePlayerEntityFromLuaTable(lua, "new_player_create");
			Game::Instance()->GetplayerCreated() = true;
			Logger::Log("Player Creating");



			if (Game::Instance()->GetPlayerNum() == 1)
			{
				loader.LoadLevelAssets(Game::Instance()->GetRenderer(), Game::Instance()->GetAssetManager(), "Level1.txt");
				//loader.LoadCollidersFromLuaTable(lua, Game::Instance()->GetAssetManager(), Game::Instance()->GetRenderer(), "luaTrigger");
				loader.LoadPlayerDataFromLuaTable(lua, "save1");
				

			}
			else if (Game::Instance()->GetPlayerNum() == 2)
			{
				loader.LoadLevelAssets(Game::Instance()->GetRenderer(), Game::Instance()->GetAssetManager(), "Level1.txt");
				//loader.LoadCollidersFromLuaTable(lua, Game::Instance()->GetAssetManager(), Game::Instance()->GetRenderer(), "luaTrigger");
				loader.LoadPlayerDataFromLuaTable(lua, "save2");
				loader.LoadColliders(Game::Instance()->GetAssetManager(), Game::Instance()->GetRenderer(), "colliders");
				//Logger::Log("Second Player Column");
			}
			else if (Game::Instance()->GetPlayerNum() == 3)
			{
				loader.LoadLevelAssets(Game::Instance()->GetRenderer(), Game::Instance()->GetAssetManager(), "Level1.txt");
				//loader.LoadCollidersFromLuaTable(lua, Game::Instance()->GetAssetManager(), Game::Instance()->GetRenderer(), "luaTrigger");
				loader.LoadPlayerDataFromLuaTable(lua, "save3");
				loader.LoadColliders(Game::Instance()->GetAssetManager(), Game::Instance()->GetRenderer(), "colliders");
				//Logger::Log("Third Player Column");
			}

		}
		// =============================================================================================================================
		// Add all necessary systems to the registry if they are not yet registered
		// =============================================================================================================================
		if (!Registry::Instance()->HasSystem<RenderHUDSystem>()) Registry::Instance()->AddSystem<RenderHUDSystem>();
		if (!Registry::Instance()->HasSystem<ProjectileEmitterSystem>()) Registry::Instance()->AddSystem<ProjectileEmitterSystem>();
		if (!Registry::Instance()->HasSystem<ProjectileLifeCycleSystem>()) Registry::Instance()->AddSystem<ProjectileLifeCycleSystem>();
		if (!Registry::Instance()->HasSystem<DamageSystem>()) Registry::Instance()->AddSystem<DamageSystem>();
		if (!Registry::Instance()->HasSystem<RenderHealthSystem>()) Registry::Instance()->AddSystem<RenderHealthSystem>();	
		if (!Registry::Instance()->HasSystem<RenderTileSystem>()) Registry::Instance()->AddSystem<RenderTileSystem>();
		if (!Registry::Instance()->HasSystem<HealthSystem>()) Registry::Instance()->AddSystem<HealthSystem>();
		if (!Registry::Instance()->HasSystem<CollisionSystem>()) Registry::Instance()->AddSystem<CollisionSystem>();
		if (!Registry::Instance()->HasSystem<MovementSystem>()) Registry::Instance()->AddSystem<MovementSystem>();
		if (!Registry::Instance()->HasSystem<TriggerSystem>()) Registry::Instance()->AddSystem<TriggerSystem>();
		if (!Registry::Instance()->HasSystem<CollectItemSystem>()) Registry::Instance()->AddSystem<CollectItemSystem>();
		if (!Registry::Instance()->HasSystem<KeyboardControlSystem>()) 	Registry::Instance()->AddSystem<KeyboardControlSystem>();
		if (!Registry::Instance()->HasSystem<RenderTextSystem>()) Registry::Instance()->AddSystem<RenderTextSystem>();
		if (!Registry::Instance()->HasSystem<GamePadSystem>()) 	Registry::Instance()->AddSystem<GamePadSystem>();
		if (!Registry::Instance()->HasSystem<ScriptSystem>()) 	Registry::Instance()->AddSystem<ScriptSystem>();
		// =============================================================================================================================

		//Game::Instance()->GetSystem<MusicPlayerSystem>().PlayMusic(Game::Instance()->GetAssetManager(), "Overworld", -1);

		// TODO: Refactor all entities below to a lua table
		Entity map = Registry::Instance()->CreateEntity();
		map.AddComponent<TransformComponent>(glm::vec2(0, 0), glm::vec2(4, 4), 0.0);
		map.AddComponent<SpriteComponent>("map", 4096, 1344, 0, false, 0, 0);
		map.AddComponent<TileComponent>();
		map.Group("map");

		Entity yellowRupee = Registry::Instance()->CreateEntity();
		yellowRupee.AddComponent<TransformComponent>(glm::vec2(7648, 5050), glm::vec2(4, 4), 0.0);
		yellowRupee.AddComponent<SpriteComponent>("hearts", 16, 16, 5, false, 48, 0);
		yellowRupee.AddComponent<RigidBodyComponent>(glm::vec2(0));
		yellowRupee.AddComponent<RupeeTypeComponent>(YELLOW);
		yellowRupee.AddComponent<BoxColliderComponent>(8, 16, glm::vec2(15, 0));
		yellowRupee.AddComponent<GameComponent>();
		yellowRupee.Group("money");

		Entity yellowRupee1 = Registry::Instance()->CreateEntity();
		yellowRupee1.AddComponent<TransformComponent>(glm::vec2(7648, 4950), glm::vec2(4, 4), 0.0);
		yellowRupee1.AddComponent<SpriteComponent>("hearts", 16, 16, 5, false, 48, 0);
		yellowRupee1.AddComponent<RigidBodyComponent>(glm::vec2(0));
		yellowRupee1.AddComponent<RupeeTypeComponent>(YELLOW);
		yellowRupee1.AddComponent<BoxColliderComponent>(8, 16, glm::vec2(15, 0));
		yellowRupee1.AddComponent<GameComponent>();
		yellowRupee1.Group("money");

		Entity blueRupee = Registry::Instance()->CreateEntity();
		blueRupee.AddComponent<TransformComponent>(glm::vec2(7648, 4950), glm::vec2(4, 4), 0.0);
		blueRupee.AddComponent<SpriteComponent>("hearts", 16, 16, 5, false, 64, 0);
		blueRupee.AddComponent<RigidBodyComponent>(glm::vec2(0));
		blueRupee.AddComponent<RupeeTypeComponent>(BLUE);
		blueRupee.AddComponent<BoxColliderComponent>(8, 16, glm::vec2(15, 0));
		blueRupee.AddComponent<GameComponent>();
		blueRupee.Group("money");

		Entity blueRupee2 = Registry::Instance()->CreateEntity();
		blueRupee2.AddComponent<TransformComponent>(glm::vec2(7648, 4850), glm::vec2(4, 4), 0.0);
		blueRupee2.AddComponent<SpriteComponent>("hearts", 16, 16, 5, false, 64, 0);
		blueRupee2.AddComponent<RigidBodyComponent>(glm::vec2(0));
		blueRupee2.AddComponent<RupeeTypeComponent>(BLUE);
		blueRupee2.AddComponent<BoxColliderComponent>(8, 16, glm::vec2(15, 0));
		blueRupee2.AddComponent<GameComponent>();
		blueRupee2.Group("money");

		Entity bombItem = Registry::Instance()->CreateEntity();
		bombItem.AddComponent<TransformComponent>(glm::vec2(7648, 4750), glm::vec2(4, 4), 0.0);
		bombItem.AddComponent<SpriteComponent>("items", 16, 16, 5, false, 64, 112);
		bombItem.AddComponent<RigidBodyComponent>(glm::vec2(0));
		bombItem.AddComponent<ItemComponent>(BOMBS);
		bombItem.AddComponent<BoxColliderComponent>(8, 16, glm::vec2(15, 0));
		bombItem.AddComponent<GameComponent>();
		bombItem.Group("items");

		firstEntered = true;
		Registry::Instance()->GetSystem<ScriptSystem>().CreateLuaBindings(lua);
	}


	return true;
}

bool GameState::OnExit()
{
	Game::Instance()->GetSystem<RenderCollisionSystem>().OnExit();
	Game::Instance()->GetSystem<RenderSystem>().OnExit();
	Game::Instance()->GetSystem<RenderTileSystem>().OnExit();
	Game::Instance()->GetSystem<RenderTextSystem>().OnExit();
	firstEntered = false;
	return true;
}

void GameState::ProcessEvents(SDL_Event& event)
{
	Registry::Instance()->GetSystem<GamePadSystem>().Update(event);
}

void GameState::OnKeyDown(SDL_Event* event)
{
	// Toggle Collisions to show for debugging
	if (event->key.keysym.sym == SDLK_c)
	{
		Game::isDebug = !Game::isDebug;
	}
	// Set to paused
	if (event->key.keysym.sym == SDLK_q)
	{
		GamePadSystem::paused = true;
		Game::Instance()->GetStateMachine()->PushState(new PauseState());
	}
}

void GameState::OnKeyUp(SDL_Event* event)
{
	Registry::Instance()->GetSystem<KeyboardControlSystem>().UpdatePlayer();
	Game::Instance()->GetEventManager()->Reset();
	KeyboardControlSystem::keyDown = false;
}


