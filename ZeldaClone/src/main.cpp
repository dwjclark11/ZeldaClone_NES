#define _CRTDBG_MAP_ALLOC
#include <stdlib.h>
#include <crtdbg.h>

#include "Game/Game.h"
#include "Systems/CameraMovementSystem.h"
#include "Systems/NameSystems/NameSelectKeyboardControlSystem.h"

int main()
{
	if (!Registry::Instance()->HasSystem<SoundFXSystem>()) 
		Registry::Instance()->AddSystem<SoundFXSystem>();
	if (!Registry::Instance()->HasSystem<MusicPlayerSystem>()) 
		Registry::Instance()->AddSystem<MusicPlayerSystem>();
	if (!Registry::Instance()->HasSystem<CameraMovementSystem>()) 
		Registry::Instance()->AddSystem<CameraMovementSystem>();
	
	// Is this needed here?
	if (!Registry::Instance()->HasSystem<NameSelectKeyboardControlSystem>())
		Registry::Instance()->AddSystem<NameSelectKeyboardControlSystem>();
	
	// Turn music volume down
	Mix_VolumeMusic(10);

	// Run the game Instance--> There is a loop inside this
	Game::Instance()->Run();
	Game::Instance()->Shutdown();
	_CrtDumpMemoryLeaks();
}