#include "GameState.h"
#include "../Game/Game.h"
//#include "PauseState.h"
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
#include "../Systems/GameSystems/AISystem.h"
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
bool GameState::firstEntered = false;
bool GameState::unpause = false;
int GameState::totalRupees = 0;
int GameState::totalBombs = 0;
int GameState::totalKeys = 0;


GameState::GameState()
	: game(*Game::Instance()), reg(*Registry::Instance()), cameraOffset(glm::vec2(0))
{
}

GameState::~GameState()
{
	
}

GameState::GameState(glm::vec2 cameraOffset)
	: GameState()
{
	this->cameraOffset = cameraOffset; 
	game.GetCamera().x = this->cameraOffset.x;
	game.GetCamera().y = this->cameraOffset.y;
}

void GameState::Update(const double& deltaTime)
{
	if (GamePadSystem::paused && game.GetFadeAlpha() == 0)
	{
		game.FadeFinished() = true;
		game.StartFadeOut() = false;

		game.GetStateMachine()->PushState(new PauseState());
	}

	// Check to see if level music has been paused
	if (!GamePadSystem::paused && unpause)
	{
		// Turn music volume up
		Mix_VolumeMusic(10);
		
		game.StartFadeIn() = true;
		unpause = false;
	}

	if (State::exitToMain) 
	{
		game.GetStateMachine()->PopState();
		game.GetStateMachine()->PushState(new MenuState());
		State::exitToMain = false;
	}
	// Reset the event manager queue
	game.GetEventManager()->Reset();

	reg.GetSystem<CollectItemSystem>().SubscribeToEvents(game.GetEventManager());
	reg.GetSystem<DamageSystem>().SubscribeToEvents(game.GetEventManager());
	reg.GetSystem<ProjectileEmitterSystem>().SubscribeToEvents(game.GetEventManager());
	reg.GetSystem<MovementSystem>().SubscribeToEvents(game.GetEventManager());
	reg.GetSystem<KeyboardControlSystem>().SubscribeToEvents(game.GetEventManager());

	reg.GetSystem<TriggerSystem>().SubscribeToEvents(game.GetEventManager());
	
	// Update the registry values
	reg.Update();
	
	// Update all Game systems
	auto player = reg.GetEntityByTag("player");
	game.GetPlayerStateMachine().Update(player);

	reg.GetSystem<CollectItemSystem>().Update();
	reg.GetSystem<AnimationSystem>().Update();
	reg.GetSystem<ProjectileEmitterSystem>().Update(Registry::Instance());

	reg.GetSystem<MovementSystem>().Update(deltaTime);
	reg.GetSystem<CameraMovementSystem>().Update(game.GetCamera());
	reg.GetSystem<ProjectileLifeCycleSystem>().Update();
	reg.GetSystem<CollisionSystem>().Update(game.GetEventManager());

	//reg.GetSystem<ScriptSystem>().Update(deltaTime, SDL_GetTicks());
	reg.GetSystem<AISystem>().Update();

	ConvertHUDNumbers();

}

void GameState::Render()
{
	// Create the HUD rect --> black rectangle that all the HUD items are on
	SDL_Rect hudRect = { 0, 0, game.windowWidth, game.windowHeight / 6 + (game.tilePixels * game.gameScale) + 64};
	
	game.GetSystem<RenderTileSystem>().Update(game.GetRenderer(), game.GetAssetManager(), game.GetCamera());
	
	// Update all other render systems
	game.GetSystem<RenderSystem>().Update(game.GetRenderer(), game.GetAssetManager(), game.GetCamera());
	
	// Render all HUD objects
	SDL_SetRenderDrawColor(game.GetRenderer(), 0, 0, 0, 255);
	SDL_RenderFillRect(game.GetRenderer(), &hudRect);
	SDL_RenderDrawRect(game.GetRenderer(), &hudRect);
	SDL_SetRenderDrawColor(game.GetRenderer(), 0, 0, 0, 255);
	
	game.GetSystem<RenderHUDSystem>().Update(game.GetRenderer(), game.GetAssetManager());
/*	game.GetSystem<RenderTextSystem>().Update(game.GetRenderer(), game.GetAssetManager(), game.GetCamera())*/;

	reg.GetSystem<HealthSystem>().Update();
	game.GetSystem<RenderHealthSystem>().Update(game.GetRenderer(), game.GetCamera());
	
	// If the game is in debug mode, render the collision system
	if (Game::isDebug)
	{
		game.GetSystem<RenderCollisionSystem>().Update(game.GetRenderer(), game.GetCamera());
	}
}

bool GameState::OnEnter()
{
	
	if (!firstEntered)
	{
		LevelLoader loader;
		
		// Always start the player and the camera from the beginning Location for now --> Create Constants for Special CAM Locations
		if (game.GetCamera().x != 7168 && game.GetCamera().x != 4416)
		{
			game.GetCamera().x = 7168;
			game.GetCamera().y = 4416;
		}
		
		
		// Open the lua libraries into the game
		game.GetLuaState().open_libraries(sol::lib::base, sol::lib::math, sol::lib::os);
		Game::isDebug = false;
		loader.LoadAssetsFromLuaTable(game.GetLuaState(), "game_state_assets");
		loader.LoadHUDFromLuaTable(game.GetLuaState(), "hud");
		loader.LoadEnemiesFromLuaTable(game.GetLuaState(), "overworld_enemies");
		loader.LoadColliders("colliders");
		
		// Player is now created from a Lua script instead of Hard Coded
		if (!game.GetplayerCreated())
		{
			loader.CreatePlayerEntityFromLuaTable(game.GetLuaState(), "new_player_create");
			game.GetplayerCreated() = true;

			if (game.GetPlayerNum() == 1)
			{
				loader.LoadPlayerDataFromLuaTable(game.GetLuaState(), "save1");
			}
			else if (game.GetPlayerNum() == 2)
			{
				loader.LoadPlayerDataFromLuaTable(game.GetLuaState(), "save2");
			}
			else if (game.GetPlayerNum() == 3)
			{
				loader.LoadLevelAssets(game.GetAssetManager(), "Level1.txt");
				loader.LoadPlayerDataFromLuaTable(game.GetLuaState(), "save3");
			}
		}
		// =============================================================================================================================
		// Add all necessary systems to the registry if they are not yet registered
		// =============================================================================================================================
		if (!reg.HasSystem<RenderHUDSystem>()) reg.AddSystem<RenderHUDSystem>();
		if (!reg.HasSystem<ProjectileEmitterSystem>()) reg.AddSystem<ProjectileEmitterSystem>();
		if (!reg.HasSystem<ProjectileLifeCycleSystem>()) reg.AddSystem<ProjectileLifeCycleSystem>();
		if (!reg.HasSystem<DamageSystem>()) reg.AddSystem<DamageSystem>();
		if (!reg.HasSystem<RenderHealthSystem>()) reg.AddSystem<RenderHealthSystem>();	
		if (!reg.HasSystem<RenderTileSystem>()) reg.AddSystem<RenderTileSystem>();
		if (!reg.HasSystem<HealthSystem>()) reg.AddSystem<HealthSystem>();
		if (!reg.HasSystem<CollisionSystem>()) reg.AddSystem<CollisionSystem>();
		if (!reg.HasSystem<MovementSystem>()) reg.AddSystem<MovementSystem>();
		if (!reg.HasSystem<TriggerSystem>()) reg.AddSystem<TriggerSystem>();
		if (!reg.HasSystem<CollectItemSystem>()) reg.AddSystem<CollectItemSystem>();
		if (!reg.HasSystem<KeyboardControlSystem>()) 	reg.AddSystem<KeyboardControlSystem>();
		if (!reg.HasSystem<RenderTextSystem>()) reg.AddSystem<RenderTextSystem>();
		if (!reg.HasSystem<GamePadSystem>()) 	reg.AddSystem<GamePadSystem>();
		if (!reg.HasSystem<ScriptSystem>()) 	reg.AddSystem<ScriptSystem>();
		if (!reg.HasSystem<AISystem>()) 	reg.AddSystem<AISystem>();
		// =============================================================================================================================

		game.GetSystem<MusicPlayerSystem>().PlayMusic(game.GetAssetManager(), "Overworld", -1);

		loader.LoadMap("map", 4096, 1344);

		firstEntered = true;
		reg.GetSystem<ScriptSystem>().CreateLuaBindings(game.GetLuaState());
	}
	return true;
}

bool GameState::OnExit()
{
	game.GetSystem<RenderCollisionSystem>().OnExit();
	game.GetSystem<RenderSystem>().OnExit();
	game.GetSystem<RenderTileSystem>().OnExit();
	//game.GetSystem<RenderTextSystem>().OnExit();
	firstEntered = false;
	return true;
}

void GameState::ProcessEvents(SDL_Event& event)
{
	reg.GetSystem<GamePadSystem>().Update(event);
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
		game.FadeFinished() = false;
		game.StartFadeOut() = true;
		game.StartFadeIn() = false;
		GamePadSystem::paused = true;
		
	}
}

void GameState::OnKeyUp(SDL_Event* event)
{
	reg.GetSystem<KeyboardControlSystem>().UpdatePlayer();
	//game.GetEventManager()->Reset();
	KeyboardControlSystem::keyDown = false;
}

void GameState::ConvertHUDNumbers()
{
	// Convert the total Rupees
	ConvertNumberParser("rupee_hundreds", totalRupees, 2);
	ConvertNumberParser("rupee_tens", totalRupees, 1);
	ConvertNumberParser("rupee_ones", totalRupees, 0);
	ConvertNumberParser("keys_tens", totalKeys, 1);
	ConvertNumberParser("key_ones", totalKeys, 0);
	ConvertNumberParser("bombs_tens", totalBombs, 1);
	ConvertNumberParser("bombs_ones", totalBombs, 0);
}


