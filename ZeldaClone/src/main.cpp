
#include "Game/Game.h"
#include "Systems/CameraMovementSystem.h"
#include "Systems/NameSystems/NameSelectKeyboardControlSystem.h"

int main()
{
	if (!Registry::Instance()->HasSystem<SoundFXSystem>()) Registry::Instance()->AddSystem<SoundFXSystem>();
	if (!Registry::Instance()->HasSystem<MusicPlayerSystem>()) Registry::Instance()->AddSystem<MusicPlayerSystem>();
	if (!Registry::Instance()->HasSystem<CameraMovementSystem>()) Registry::Instance()->AddSystem<CameraMovementSystem>();
	if (!Registry::Instance()->HasSystem<NameSelectKeyboardControlSystem>())Registry::Instance()->AddSystem<NameSelectKeyboardControlSystem>();
	
	
	Mix_VolumeMusic(10);


	Game::Instance()->Run();
	Game::Instance()->Shutdown();
}