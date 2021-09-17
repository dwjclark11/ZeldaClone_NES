#include "DungeonState.h"
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
#include "../Systems/GameSystems/RenderTileSystem.h"
#include "../Systems/GameSystems/HealthSystem.h"
#include "../Systems/GameSystems/TriggerSystem.h"
#include "../Systems/CameraMovementSystem.h"
#include "../Systems/GameSystems/CollectItemSystem.h"
#include "../Systems/GameSystems/ProjectileEmitterSystem.h"
#include "../Systems/GameSystems/ProjectileLifeCycleSystem.h"
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

const std::string DungeonState::dungeonID = "DUNGEON";

DungeonState::DungeonState(std::string level)
	: paused(false)
	, level(level)
	
{

}

void DungeonState::Update(const double& deltaTime)
{
	Game::Instance()->GetEventManager()->Reset();
	
	Registry::Instance()->GetSystem<KeyboardControlSystem>().SubscribeToEvents(Game::Instance()->GetEventManager());
	Registry::Instance()->GetSystem<DamageSystem>().SubscribeToEvents(Game::Instance()->GetEventManager());
	Registry::Instance()->GetSystem<ProjectileEmitterSystem>().SubscribeToEvents(Game::Instance()->GetEventManager());
	Registry::Instance()->GetSystem<MovementSystem>().SubscribeToEvents(Game::Instance()->GetEventManager());
	Registry::Instance()->GetSystem<TriggerSystem>().SubscribeToEvents(Game::Instance()->GetEventManager());
	Registry::Instance()->GetSystem<CollectItemSystem>().SubscribeToEvents(Game::Instance()->GetEventManager());
	Registry::Instance()->Update();
	Registry::Instance()->GetSystem<MovementSystem>().Update(deltaTime);
	Registry::Instance()->GetSystem<AnimationSystem>().Update();
	Registry::Instance()->GetSystem<ProjectileEmitterSystem>().Update(Registry::Instance());
	Registry::Instance()->GetSystem<CollisionSystem>().Update(Game::Instance()->GetEventManager(), Game::Instance()->GetAssetManager(), Game::Instance()->GetCamera());
	Registry::Instance()->GetSystem<CameraMovementSystem>().Update(Game::Instance()->GetCamera());
	Registry::Instance()->GetSystem<ProjectileLifeCycleSystem>().Update();

	if (GameState::playerDead)
	{
		Game::Instance()->GetStateMachine()->PopState();
		Game::Instance()->GetStateMachine()->PushState(new GameOverState());
	}
	if (State::exitToMain) Game::Instance()->GetStateMachine()->PopState();
}

void DungeonState::Render()
{
	SDL_Rect hudRect = { 0, 0, 1024, 256 };
	Game::Instance()->GetSystem<RenderTileSystem>().Update(Game::Instance()->GetRenderer(), Game::Instance()->GetAssetManager(), Game::Instance()->GetCamera());
	SDL_SetRenderDrawColor(Game::Instance()->GetRenderer(), 0, 0, 0, 255);
	SDL_RenderFillRect(Game::Instance()->GetRenderer(), &hudRect);
	SDL_RenderDrawRect(Game::Instance()->GetRenderer(), &hudRect);
	SDL_SetRenderDrawColor(Game::Instance()->GetRenderer(), 0, 0, 0, 255);

	Game::Instance()->GetSystem<RenderSystem>().Update(Game::Instance()->GetRenderer(), Game::Instance()->GetAssetManager(), Game::Instance()->GetCamera());
	Game::Instance()->GetSystem<RenderHUDSystem>().Update(Game::Instance()->GetRenderer(), Game::Instance()->GetAssetManager());
	Game::Instance()->GetSystem<RenderTextSystem>().Update(Game::Instance()->GetRenderer(), Game::Instance()->GetAssetManager(), Game::Instance()->GetCamera());

	Registry::Instance()->GetSystem<HealthSystem>().Update();
	Game::Instance()->GetSystem<RenderHealthSystem>().Update(Game::Instance()->GetRenderer(), Game::Instance()->GetCamera());
	
	if (Game::isDebug)
	{
		Game::Instance()->GetSystem<RenderCollisionSystem>().Update(Game::Instance()->GetRenderer(), Game::Instance()->GetCamera());
	}
}

bool DungeonState::OnEnter()
{
	Game::Instance()->GetSystem<MusicPlayerSystem>().PlayMusic(Game::Instance()->GetAssetManager(), "Main_Menu", -1);

	Logger::Log("Level:" + level);
	LevelLoader loader;
	if (level == "level1")
	{
		Game::Instance()->GetLevelWidth() = 6;
		Game::Instance()->GetLevelHeight() = 7;
		Logger::Log("In Eagle Level");
		Entity secret = Registry::Instance()->CreateEntity();
		secret.AddComponent<TransformComponent>(glm::vec2(0, 0), glm::vec2(4, 4), 0.0);
		secret.AddComponent<SpriteComponent>("level_1", 1543, 1063, 1, false, 0, 0);
		secret.AddComponent<TileComponent>();
		secret.Group("map");
	}
	if (level == "secret")
	{
		loader.LoadColliders(Game::Instance()->GetAssetManager(), Game::Instance()->GetRenderer(), "secret_dungeon_colliders.map");
		Entity secret = Registry::Instance()->CreateEntity();
		secret.AddComponent<TransformComponent>(glm::vec2(0, 256), glm::vec2(4, 4), 0.0);
		secret.AddComponent<SpriteComponent>("secret_dungeon", 256, 176, 1, false, 0, 0);
		secret.AddComponent<TileComponent>();
		secret.Group("map");
	}
	
	//loader.CreatePlayerEntityFromLuaTable(lua, "new_player_create");
	Game::Instance()->GetCamera().x = 0;
	Game::Instance()->GetCamera().y = 0;
	

	

	/*Entity bombItem = Registry::Instance()->CreateEntity();
	bombItem.AddComponent<TransformComponent>(glm::vec2(150, 400), glm::vec2(4, 4), 0.0);
	bombItem.AddComponent<SpriteComponent>("items", 16, 16, 5, false, 64, 112);
	bombItem.AddComponent<RigidBodyComponent>(glm::vec2(0));
	bombItem.AddComponent<ItemComponent>(BOMBS);
	bombItem.AddComponent<BoxColliderComponent>(8, 16, glm::vec2(15, 0));
	bombItem.AddComponent<GameComponent>();
	bombItem.Group("items");*/

	Logger::Log("Entering Dungeon State");
	return false;
}

bool DungeonState::OnExit()
{
	Logger::Log("Exiting Dungeon State");
	return false;
}

void DungeonState::ProcessEvents(SDL_Event& event)
{

}

void DungeonState::OnKeyDown(SDL_Event* event)
{
	// Toggle Collisions to show for debugging
	if (event->key.keysym.sym == SDLK_c)
	{
		Game::isDebug = !Game::isDebug;
	}

	if (event->key.keysym.sym == SDLK_q)
	{
		paused = true;
		Game::Instance()->GetStateMachine()->PushState(new PauseState());
	}
}

void DungeonState::OnKeyUp(SDL_Event* event)
{
	Registry::Instance()->GetSystem<KeyboardControlSystem>().UpdatePlayer();
	Game::Instance()->GetEventManager()->Reset();
}
