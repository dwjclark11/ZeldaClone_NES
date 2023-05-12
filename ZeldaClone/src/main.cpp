#define SDL_MAIN_HANDLED 1


#include "Game/Game.h"


int main(int argc, char* argv[])
{


	// Run the game Instance--> There is a loop inside this
	Game::Instance().Run();
	Game::Instance().Shutdown();
	return 0;
}	