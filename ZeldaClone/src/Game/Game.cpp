#include "Game.h"
#include "../States/TitleState.h"
#include "../Logger/Logger.h"
#include <SDL_ttf.h>
#include "../Utilities/Timer.h"
#include "../States/GameState.h"
#include "../States/EditorState.h"
#include "../Events/KeyPressedEvent.h"

// Initialize all static variables
std::unique_ptr<Game> Game::mInstance = nullptr;

Game::ItemType Game::mSelectedItem = Game::ItemType::EMPTY;

int Game::gameScale = 4;
int Game::tilePixels = 16;
int Game::windowWidth = 256 * Game::gameScale;
int Game::windowHeight = 240 * Game::gameScale;
bool Game::isDebug = true;

// Constructor 
Game::Game()
	: mIsRunning(true)
	, mWindow(nullptr)
	, mRenderer(nullptr)
	, gameStateMachine(nullptr)
	, milliSecondsPerFrame(0)
	, attack(false)
	, keydown(false)
	, deltaTime(0.0f)
	, gamePlayerNum(0)
	, playerCreated(false)
	, cameraMoving(false)
	, mLevelWidth(16)
	, mLevelHeight(8)
{
	assetManager = std::make_unique<AssetManager>();
	eventManager = std::make_unique<EventManager>();
}
// Destructor
Game::~Game()
{

}

Game& Game::Instance()
{
	if (mInstance == nullptr)
	{
		Logger::Log("Creating new Game instance!\n");
		mInstance.reset(new Game());
	}
	return *mInstance;
}

// Initialize SDL/IMGUI and variables
void Game::Initialize()
{
	SDL_SetMainReady();

	if (SDL_Init(SDL_INIT_EVERYTHING) != 0)
	{
		Logger::Err("Failed to initialize: " + *SDL_GetError());
	}

	// In order to use the SDL extensions, we must initialize them
	if (TTF_Init() != 0)
	{
		Logger::Err("Failed to initialize TTF");
		return;
	}
	
	// Allocate channels and control the volume of the music
	Mix_AllocateChannels(16);
	Mix_Volume(1, MIX_MAX_VOLUME / 4);

	/* Use SDL to grab the displays Resolution */
	SDL_DisplayMode displayMode;
	SDL_GetCurrentDisplayMode(0, &displayMode);
	
	// Create the window
	mWindow = SDL_CreateWindow(
		"Zelda Clone",
		SDL_WINDOWPOS_CENTERED,
		SDL_WINDOWPOS_CENTERED,
		windowWidth,
		windowHeight,
		SDL_WINDOW_OPENGL // | SDL_WINDOW_RESIZABLE // We don't want resizable in game
	);
	
	// Check to see if the window creation worked correctly
	if (!mWindow)
	{
		Logger::Err("Failed to Create Window: " + *SDL_GetError());
	}
	
	// Create the Renderer
	mRenderer = SDL_CreateRenderer(
		mWindow,
		-1,
		SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC // Added!!
	);
	// Check if the renderer was created correctly
	if (!mRenderer)
	{
		Logger::Err("Failed to create renderer: " + *SDL_GetError());
		return;
	}
	
	// Enable Alpha Blending
	SDL_SetRenderDrawBlendMode(GetRenderer(), SDL_BLENDMODE_BLEND);
	
	// Initialize IMGUI context
	ImGui::CreateContext();
	ImGuiSDL::Initialize(mRenderer, windowWidth, windowHeight);


	// Add the required systems that are needed at the beginning
	Registry::Instance().AddSystem<RenderSystem>();
	Registry::Instance().AddSystem<RenderTitleSystem>();
	Registry::Instance().AddSystem<RenderCollisionSystem>();
	Registry::Instance().AddSystem<AnimationSystem>();
	
	// Initialize Camera
	camera.x = 7168;
	camera.y = 4704;
	camera.w = windowWidth;
	camera.h = windowHeight;

	// Initialize Mouse Box for Editor State
	mouseBox.w = 1;
	mouseBox.h = 1;
	
	// Add the needed sounds, textures, and fonts to the asset manager 
	assetManager->AddSoundFX("text_slow", "./Assets/sounds/Text_Slow.wav");
	assetManager->AddTextures(mRenderer, "map", "./Assets/Backgrounds/entire_worldmap_single_image.png");

	// Set Initial Fade Alpha
	fadeAlpha = 255;
	fadeFinished = true;
	startFadeIn = false;
	startFadeOut = false;

	CreateDefaultKeyBindings();
	CreateDefaultBtnBindings();

	// Create the finite Game State Machine
	gameStateMachine = std::make_unique<GameStateMachine>();
	
	// Push the title screen
	gameStateMachine->PushState(new TitleState());

	// Change to the state you want to work on
	//gameStateMachine->PushState(new EditorState());
}

void Game::Update()
{

	// If we are too fast, waste some time until we reach the desired time per frame.
	int timeToWait = MILLISECONDS_PER_FRAME - (SDL_GetTicks() - milliSecondsPerFrame);

	if (timeToWait > 0 && timeToWait <= milliSecondsPerFrame)
	{
		SDL_Delay(timeToWait);
	}

	// The difference in ticks since the last frame, converted to seconds
	deltaTime = (SDL_GetTicks() - milliSecondsPreviousFrame) / 1000.0f;

	// Store the current time frame
	milliSecondsPreviousFrame = SDL_GetTicks();

	gameStateMachine->Update(deltaTime);
	//Logger::Err(std::to_string(milliSecondsPreviousFrame));
}

void Game::ProcessEvents()
{
	// Update the Game Controller 
	SDL_GameControllerUpdate();

	while (SDL_PollEvent(&sdlEvent))
	{
		// ImGui SDL input
		ImGui_ImplSDL2_ProcessEvent(&sdlEvent);
		ImGuiIO& io = ImGui::GetIO();

		int mouseX, mouseY;
		const int buttons = SDL_GetMouseState(&mouseX, &mouseY);

		// Set up mouse inpots for ImGui
		io.MousePos = ImVec2(static_cast<float>(mouseX), static_cast<float>(mouseY));
		io.MouseDown[0] = buttons & SDL_BUTTON(SDL_BUTTON_LEFT);
		io.MouseDown[1] = buttons & SDL_BUTTON(SDL_BUTTON_RIGHT);
		
		gameStateMachine->ProcessEvents(sdlEvent);

		// Handle Core SDL Events
		switch (sdlEvent.type)
		{
			case SDL_QUIT:
			mIsRunning = false;
			break;

			case SDL_KEYDOWN:
				
				eventManager->EmitEvent<KeyPressedEvent>(sdlEvent.key.keysym.sym);

				if (sdlEvent.key.keysym.sym == SDLK_ESCAPE) mIsRunning = false;

				if(sdlEvent.key.keysym.sym)
					gameStateMachine->OnKeyDown(&sdlEvent);
				break;
				
			case SDL_KEYUP:

				if (sdlEvent.key.keysym.sym)
					gameStateMachine->OnKeyUp(&sdlEvent);
				break;

			case SDL_MOUSEMOTION:
			{
				/*int x, y;
				SDL_GetMouseState(&x, &y);*/
				break;
			}
			case SDL_CONTROLLERBUTTONDOWN:
			{
				eventManager->EmitEvent<GamePadButtonPressedEvent>(sdlEvent.cbutton.button);
				
				gameStateMachine->OnBtnDown(&sdlEvent);
				
				break;
			}
			
			case SDL_CONTROLLERBUTTONUP:
			{
				
				gameStateMachine->OnBtnUp(&sdlEvent);
				
				break;
			}
		default:
			break;
		}
	}
}

void Game::Draw()
{
	SDL_SetRenderDrawColor(mRenderer, 0, 0, 0, 255);
	SDL_RenderClear(mRenderer);

	gameStateMachine->Render();

	SDL_RenderPresent(mRenderer);
}

void Game::CreateDefaultBtnBindings()
{
	AddBtnToMap(Action::MOVE_UP, SDL_CONTROLLER_BUTTON_DPAD_UP);
	AddBtnToMap(Action::MOVE_RIGHT, SDL_CONTROLLER_BUTTON_DPAD_RIGHT);
	AddBtnToMap(Action::MOVE_DOWN, SDL_CONTROLLER_BUTTON_DPAD_DOWN);
	AddBtnToMap(Action::MOVE_LEFT, SDL_CONTROLLER_BUTTON_DPAD_LEFT);
	AddBtnToMap(Action::ATTACK, SDL_CONTROLLER_BUTTON_A);
	AddBtnToMap(Action::USE_ITEM, SDL_CONTROLLER_BUTTON_X);
	AddBtnToMap(Action::PAUSE, SDL_CONTROLLER_BUTTON_START);
	AddBtnToMap(Action::SELECT, SDL_CONTROLLER_BUTTON_A);
	AddBtnToMap(Action::CANCEL, SDL_CONTROLLER_BUTTON_B);
}

void Game::CreateDefaultKeyBindings()
{
	AddKeyToMap(Action::MOVE_UP, SDLK_w);
	AddKeyToMap(Action::MOVE_RIGHT, SDLK_d);
	AddKeyToMap(Action::MOVE_DOWN, SDLK_s);
	AddKeyToMap(Action::MOVE_LEFT, SDLK_a);
	AddKeyToMap(Action::ATTACK, SDLK_RSHIFT);
	AddKeyToMap(Action::USE_ITEM, SDLK_SPACE);
	AddKeyToMap(Action::PAUSE, SDLK_q);
	AddKeyToMap(Action::SELECT, SDLK_SPACE);
	AddKeyToMap(Action::CANCEL, SDLK_BACKSPACE);
}
void Game::Shutdown()
{
	// Shutdown ImGui
	ImGuiSDL::Deinitialize();
	ImGui::DestroyContext();

	// Shutdown SDL
	SDL_DestroyRenderer(mRenderer);
	SDL_DestroyWindow(mWindow);
	SDL_Quit();
}

void Game::Run()
{
	// Call the initialize function
	Initialize();
	
	// while game is running
	while (mIsRunning)
	{
		ProcessEvents();
		Update();
		Draw();
	}
}

// Definitions of the Getters and Setters
SDL_Rect& Game::GetCamera()
{
	return this->camera;
}

void Game::SetCameraY(int change)
{
	this->camera.y += change;
}

void Game::SetCameraX(int change)
{
	this->camera.x += change;
}

SDL_Rect& Game::GetMouseBox()
{
	return mouseBox;
}

SDL_Event& Game::GetEvent()
{
	return sdlEvent;
}

const bool Game::HasSword() const 
{
	if (mGameItems.woodSword || mGameItems.steelSword || mGameItems.magicSword)
		return true;

	return false;
}

const bool Game::PlayerHold() const 
{
	if (playerDead || onStairs || playerItem || startFadeOut || startFadeIn || onRaft)
		return true;
	else
		return false;
}

bool& Game::GetAttack()
{
	return attack;
}

void Game::FadeScreen()
{
	if (startFadeIn)
	{
		if (fadeAlpha < 255)
		{
			fadeAlpha += 15;
		}
		else
		{
			startFadeIn = false;
		}
	}

	if (startFadeOut)
	{
		if (fadeAlpha > 0)
		{
			fadeAlpha -= 15;
		}
		else
		{
			startFadeOut = false;
		}
	}
}

const unsigned& Game::GetPlayerNum() const 
{
	return gamePlayerNum;
}

void Game::AddGameSecrets(const std::string& locationID, bool found)
{
	if (gameSecrets.find(locationID) == gameSecrets.end())
	{
		gameSecrets.emplace(locationID, found);
	}
}

bool Game::HasSecret(const std::string& locationID)
{
	if (gameSecrets.find(locationID) != gameSecrets.end())
		return true;

	// If the location is not in the map return false
	return false;
}

bool Game::IsSecretFound(const std::string& locationID)
{
	if (gameSecrets.find(locationID) != gameSecrets.end())
	{
		return gameSecrets[locationID];
	}

	return false;
}

void Game::SetSecretFound(const std::string& locationID, bool found)
{
	if (gameSecrets.find(locationID) != gameSecrets.end())
	{
		gameSecrets[locationID] = found;
	}
}

void Game::AddKeyToMap(Action action, SDL_Keycode key)
{
	if (mMappedKeys.find(action) == mMappedKeys.end())
	{
		mMappedKeys.emplace(action, key);
	}
}

void Game::ChangeKeyBinding(Action action, SDL_Keycode key)
{
	if (mMappedKeys.find(action) != mMappedKeys.end())
		mMappedKeys[action] = key;
}
void Game::AddBtnToMap(Action action, SDL_GameControllerButton button)
{
	if (mMappedButtons.find(action) == mMappedButtons.end())
	{
		mMappedButtons.emplace(action, button);
	}
}

void Game::ChangeBtnBinding(Action action, SDL_GameControllerButton button)
{
	if (mMappedButtons.find(action) != mMappedButtons.end())
		mMappedButtons[action] = button;
}

