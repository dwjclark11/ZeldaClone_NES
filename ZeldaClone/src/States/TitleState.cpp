#include "TitleState.h"
#include "../Game/Game.h"
#include "MenuState.h"
#include "../ECS/ECS.h"
#include "../Components/AnimationComponent.h"
#include "../Components/RigidBodyComponent.h"
#include "../Components/TransformComponent.h"
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
	, timer(0)
	, scroll(false)
{
}

void TitleState::Update(const float& deltaTime)
{
	reg.Update();
	
	if (timer < 150)
	{
		std::this_thread::sleep_for(std::chrono::milliseconds(100));
		timer++;
		RenderTitleSystem::titleTimer++;
	}

	Registry::Instance().GetSystem<AnimationSystem>().Update();
	
	if (timer >= 150)
	{
		if (!scroll)
		{
			Entity scrollSheet = reg.CreateEntity();
			scrollSheet.AddComponent<SpriteComponent>("title_scroll", 256, 1920, 2, false);
			scrollSheet.AddComponent<TransformComponent>(glm::vec2(0,0), glm::vec2(4,4));
			scrollSheet.AddComponent<BackgroundImageComponent>(glm::vec2(0,0), glm::vec2(4,4));
			scrollSheet.AddComponent<RigidBodyComponent>(glm::vec2(0));
			scrollSheet.AddComponent<AnimationComponent>(2, 6, false);
			scrollSheet.Tag("title_scroll");
			
			scroll = true;
		}
		// Start the camera Movement System to Scroll the screen down
		Registry::Instance().GetSystem<CameraMovementSystem>().Update(game.GetCamera(), deltaTime);
		
		// Reset the TitleScreen --> Pop and Push the TitleState after a certain amount of time!
		if (Registry::Instance().GetSystem<CameraMovementSystem>().GetScrollFinished())
		{
			Registry::Instance().GetSystem<CameraMovementSystem>().SetScrollFinished(false);
			game.GetStateMachine()->PopState();
			game.GetStateMachine()->PushState(new TitleState());
		}
	}
}

void TitleState::Render()
{
	Registry::Instance().GetSystem<RenderTitleSystem>().Update(game.GetRenderer(), game.GetAssetManager(), game.GetCamera());
}

bool TitleState::OnEnter()
{
	// Initialize Variables
	scroll = false;
	timer = 0;
	
	if (!reg.HasSystem<GamePadSystem>()) 	
		reg.AddSystem<GamePadSystem>();
	if (!reg.HasSystem<KeyboardControlSystem>()) 	
		reg.AddSystem<KeyboardControlSystem>();

	// Turn music volume down
	Mix_VolumeMusic(10);

	// Initialize the Game Pad
	Registry::Instance().GetSystem<GamePadSystem>().Init();

	// Add Assets to the Asset Manager
	game.GetAssetManager()->AddMusic("Title", "Assets/Music/Title_Theme.mp3");
	if (!game.GetAssetManager()->HasTexture("TitleScreen")) 
		game.GetAssetManager()->AddTextures(game.GetRenderer(), "TitleScreen", "./Assets/Backgrounds/DemoZeldaTitleScreen.png");
	if (!game.GetAssetManager()->HasTexture("title_scroll")) 
		game.GetAssetManager()->AddTextures(game.GetRenderer(), "title_scroll", "./Assets/Backgrounds/titleScroll.png");
	if (!game.GetAssetManager()->HasTexture("waterfall")) 
		game.GetAssetManager()->AddTextures(game.GetRenderer(), "waterfall", "./Assets/Backgrounds/waterfall.png");
	
	// Start the Title Screen Music
	Registry::Instance().GetSystem<MusicPlayerSystem>().PlayMusic(game.GetAssetManager(), "Title", -1);
	
	// Create the Title Screen Entity
	Entity titleScreen = reg.CreateEntity();
	titleScreen.AddComponent<SpriteComponent>("TitleScreen", 256, 240, 0, true);
	titleScreen.AddComponent<BackgroundImageComponent>(glm::vec2(0,0), glm::vec2(4,4));
	titleScreen.AddComponent<TransformComponent>(glm::vec2(0,0), glm::vec2(4,4));
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
	Registry::Instance().GetSystem<RenderTitleSystem>().OnExit();
	Registry::Instance().GetSystem<CameraMovementSystem>().OnExit();
	
	// Remove Unused Textures
	game.GetAssetManager()->RemoveTexture("TitleScreen");
	game.GetAssetManager()->RemoveTexture("title_scroll");
	game.GetAssetManager()->RemoveTexture("waterfall");
	// Remove Unused Music
	game.GetAssetManager()->RemoveMusic("Title");


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
