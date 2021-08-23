#include "TitleState.h"
#include "../Game/Game.h"
#include "MenuState.h"
#include "../ECS/ECS.h"
#include "../Components/AnimationComponent.h"
#include "../Components/RigidBodyComponent.h"
#include "../Systems/CameraMovementSystem.h"
#include "../Systems/GameSystems/GamePadSystem.h"

const std::string TitleState::titleID = "TITLESTATE";

void TitleState::Update(const double& deltaTime)
{
	Registry::Instance()->Update();
	
	if (timer < 150)
	{
		
		Sleep(100);
		timer++;
		RenderTitleSystem::titleTimer++;
	}

	Registry::Instance()->GetSystem<AnimationSystem>().Update();
	
	if (timer >= 150)
	{
		if (!scroll)
		{
			Entity scrollSheet = Registry::Instance()->CreateEntity();
			scrollSheet.AddComponent<SpriteComponent>("title_scroll", 256, 1920, 2, false);
			scrollSheet.AddComponent<BackgroundImageComponent>(glm::vec2(0,0), glm::vec2(4,4));
			scrollSheet.AddComponent<RigidBodyComponent>(glm::vec2(0));
			scrollSheet.AddComponent<AnimationComponent>(2, 6, false);
			scrollSheet.Tag("title_scroll");
			
			scroll = true;
		}
		// Start the camera Movement System to Scroll the screen down
		Registry::Instance()->GetSystem<CameraMovementSystem>().Update(Game::Instance()->GetCamera());
		
		// Reset the TitleScreen --> Pop and Push the TitleState after a certain amount of time!
		if (Registry::Instance()->GetSystem<CameraMovementSystem>().GetScrollFinished())
		{
			Registry::Instance()->GetSystem<CameraMovementSystem>().GetScrollFinished() = false;
			Game::Instance()->GetStateMachine()->PopState();
			Game::Instance()->GetStateMachine()->PushState(new TitleState());
		}
	}
}

void TitleState::Render()
{
	Game::Instance()->GetSystem<RenderTitleSystem>().Update(Game::Instance()->GetRenderer(), Game::Instance()->GetAssetManager(), Game::Instance()->GetCamera());
}

bool TitleState::OnEnter()
{
	// Initialize Variables
	scroll = false;
	timer = 0;
	
	if (!Registry::Instance()->HasSystem<GamePadSystem>()) 	Registry::Instance()->AddSystem<GamePadSystem>();
	Registry::Instance()->GetSystem<GamePadSystem>().Init();

	// Add Assets to the Asset Manager
	Game::Instance()->GetAssetManager()->AddMusic("Title", "Assets/Music/Title_Theme.mp3");
	if (!Game::Instance()->GetAssetManager()->HasTexture("TitleScreen")) 
		Game::Instance()->GetAssetManager()->AddTextures(Game::Instance()->GetRenderer(), "TitleScreen", "./Assets/Backgrounds/DemoZeldaTitleScreen.png");
	if (!Game::Instance()->GetAssetManager()->HasTexture("title_scroll")) 
		Game::Instance()->GetAssetManager()->AddTextures(Game::Instance()->GetRenderer(), "title_scroll", "./Assets/Backgrounds/titleScroll.png");
	if (!Game::Instance()->GetAssetManager()->HasTexture("waterfall")) 
		Game::Instance()->GetAssetManager()->AddTextures(Game::Instance()->GetRenderer(), "waterfall", "./Assets/Backgrounds/waterfall.png");
	
	// Start the Title Screen Music
	Game::Instance()->GetSystem<MusicPlayerSystem>().PlayMusic(Game::Instance()->GetAssetManager(), "Title", -1);
	
	// Create the Title Screen Entity
	Entity titleScreen = Registry::Instance()->CreateEntity();
	titleScreen.AddComponent<SpriteComponent>("TitleScreen", 256, 240, 0, true);
	titleScreen.AddComponent<BackgroundImageComponent>(glm::vec2(0,0), glm::vec2(4,4));
	titleScreen.AddComponent<AnimationComponent>(3, 4, false);
	titleScreen.AddComponent<RigidBodyComponent>(glm::vec2(0));
	titleScreen.Group("title");

	//Entity waterfall = Registry::Instance()->CreateEntity();
	//waterfall.AddComponent<SpriteComponent>("waterfall", 32, 64, 1, true);
	//waterfall.AddComponent<BackgroundImageComponent>(glm::vec2(0, 0), glm::vec2(4, 4));
	//waterfall.AddComponent<AnimationComponent>(6, 20, false);
	//waterfall.AddComponent<RigidBodyComponent>(glm::vec2(0));
	return true;
}

bool TitleState::OnExit()
{
	// Remove Assets/Entities from the game that are not needed anymore
	Game::Instance()->GetSystem<RenderTitleSystem>().OnExit();
	Game::Instance()->GetSystem<CameraMovementSystem>().OnExit();
	
	// Remove the Render Title System If we are not using it!
	Registry::Instance()->RemoveSystem<RenderTitleSystem>();
	return true;
}

void TitleState::ProcessEvents(SDL_Event& event)
{
	Game::Instance()->GetSystem<GamePadSystem>().UpdateOtherStates(event);
}

void TitleState::OnKeyDown(SDL_Event* event)
{
	if (event->key.keysym.sym == SDLK_RETURN)
	{
		Game::Instance()->GetStateMachine()->PopState();
		Game::Instance()->GetStateMachine()->PushState(new MenuState());
	}
}

void TitleState::OnKeyUp(SDL_Event* event)
{

}

