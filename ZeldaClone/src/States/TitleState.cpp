#include "TitleState.h"
#include "../Game/Game.h"
#include "MenuState.h"
#include "../ECS/ECS.h"
#include "../Components/AnimationComponent.h"
#include "../Components/RigidBodyComponent.h"
#include "../Components/SpriteComponent.h"
#include "../Components/BackgroundImageComponent.h"
#include "../Systems/CameraMovementSystem.h"
#include "../Systems/GameSystems/GamePadSystem.h"
#include "../Systems/GameSystems/AnimationSystem.h"
#include "../Systems/GameSystems/KeyboardControlSystem.h"
#include "../Systems/SoundFXSystem.h"

const std::string TitleState::titleID = "TITLESTATE";

TitleState::TitleState()
	: game(Game::Instance()), reg(Registry::Instance())
{
}

void TitleState::Update(const double& deltaTime)
{
	reg.Update();
	
	if (timer < 150)
	{
		
		Sleep(100);
		timer++;
		RenderTitleSystem::titleTimer++;
	}

	reg.GetSystem<AnimationSystem>().Update();
	
	if (timer >= 150)
	{
		if (!scroll)
		{
			Entity scrollSheet = reg.CreateEntity();
			scrollSheet.AddComponent<SpriteComponent>("title_scroll", 256, 1920, 2, false);
			scrollSheet.AddComponent<BackgroundImageComponent>(glm::vec2(0,0), glm::vec2(4,4));
			scrollSheet.AddComponent<RigidBodyComponent>(glm::vec2(0));
			scrollSheet.AddComponent<AnimationComponent>(2, 6, false);
			scrollSheet.Tag("title_scroll");
			
			scroll = true;
		}
		// Start the camera Movement System to Scroll the screen down
		reg.GetSystem<CameraMovementSystem>().Update(game.GetCamera());
		
		// Reset the TitleScreen --> Pop and Push the TitleState after a certain amount of time!
		if (reg.GetSystem<CameraMovementSystem>().GetScrollFinished())
		{
			reg.GetSystem<CameraMovementSystem>().GetScrollFinished() = false;
			game.GetStateMachine()->PopState();
			game.GetStateMachine()->PushState(new TitleState());
		}
	}
}

void TitleState::Render()
{
	game.GetSystem<RenderTitleSystem>().Update(game.GetRenderer(), game.GetAssetManager(), game.GetCamera());
}

bool TitleState::OnEnter()
{
	// Initialize Variables
	scroll = false;
	timer = 0;
	
	if (!reg.HasSystem<GamePadSystem>()) 	reg.AddSystem<GamePadSystem>();
	if (!reg.HasSystem<KeyboardControlSystem>()) 	reg.AddSystem<KeyboardControlSystem>();


	// Turn music volume down
	Mix_VolumeMusic(10);


	reg.GetSystem<GamePadSystem>().Init();

	// Add Assets to the Asset Manager
	game.GetAssetManager()->AddMusic("Title", "Assets/Music/Title_Theme.mp3");
	if (!game.GetAssetManager()->HasTexture("TitleScreen")) 
		game.GetAssetManager()->AddTextures(game.GetRenderer(), "TitleScreen", "./Assets/Backgrounds/DemoZeldaTitleScreen.png");
	if (!game.GetAssetManager()->HasTexture("title_scroll")) 
		game.GetAssetManager()->AddTextures(game.GetRenderer(), "title_scroll", "./Assets/Backgrounds/titleScroll.png");
	if (!game.GetAssetManager()->HasTexture("waterfall")) 
		game.GetAssetManager()->AddTextures(game.GetRenderer(), "waterfall", "./Assets/Backgrounds/waterfall.png");
	
	// Start the Title Screen Music
	game.GetSystem<MusicPlayerSystem>().PlayMusic(game.GetAssetManager(), "Title", -1);
	
	// Create the Title Screen Entity
	Entity titleScreen = reg.CreateEntity();
	titleScreen.AddComponent<SpriteComponent>("TitleScreen", 256, 240, 0, true);
	titleScreen.AddComponent<BackgroundImageComponent>(glm::vec2(0,0), glm::vec2(4,4));
	titleScreen.AddComponent<AnimationComponent>(3, 4, false);
	titleScreen.AddComponent<RigidBodyComponent>(glm::vec2(0));
	titleScreen.Group("title");

	//Entity waterfall = reg.CreateEntity();
	//waterfall.AddComponent<SpriteComponent>("waterfall", 32, 64, 1, true);
	//waterfall.AddComponent<BackgroundImageComponent>(glm::vec2(0, 0), glm::vec2(4, 4));
	//waterfall.AddComponent<AnimationComponent>(6, 20, false);
	//waterfall.AddComponent<RigidBodyComponent>(glm::vec2(0));
	return true;
}

bool TitleState::OnExit()
{
	// Remove Assets/Entities from the game that are not needed anymore
	game.GetSystem<RenderTitleSystem>().OnExit();
	game.GetSystem<CameraMovementSystem>().OnExit();
	
	// Remove the Render Title System If we are not using it!
	reg.RemoveSystem<RenderTitleSystem>();
	return true;
}

void TitleState::ProcessEvents(SDL_Event& event)
{
	
}

void TitleState::OnKeyDown(SDL_Event* event)
{
	if (event->key.keysym.sym == SDLK_RETURN)
	{
		game.GetStateMachine()->PopState();
		game.GetStateMachine()->PushState(new MenuState());
	}
}

void TitleState::OnKeyUp(SDL_Event* event)
{

}

void TitleState::OnBtnDown(SDL_Event* event)
{
	if (event->cbutton.button == SDL_CONTROLLER_BUTTON_START)
	{
		game.GetStateMachine()->PopState();
		game.GetStateMachine()->PushState(new MenuState());
	}
}

void TitleState::OnBtnUp(SDL_Event* event)
{

}
