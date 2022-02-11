#define SDL_MAIN_HANDLED
#define main SDL_main

#include "Game/Game.h"
#include "Systems/CameraMovementSystem.h"
#include "Systems/SoundFXSystem.h"

int main(int argc, char* argv[])
{
	if (!Registry::Instance().HasSystem<SoundFXSystem>()) 
		Registry::Instance().AddSystem<SoundFXSystem>();
	if (!Registry::Instance().HasSystem<MusicPlayerSystem>()) 
		Registry::Instance().AddSystem<MusicPlayerSystem>();
	if (!Registry::Instance().HasSystem<CameraMovementSystem>()) 
		Registry::Instance().AddSystem<CameraMovementSystem>();

	// Turn music volume down
	Mix_VolumeMusic(10);

	// Run the game Instance--> There is a loop inside this
	Game::Instance().Run();
	Game::Instance().Shutdown();
	return 0;
}	