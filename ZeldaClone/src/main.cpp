#define SDL_MAIN_HANDLED 1
#include "Game/Game.h"

bool bShowConsole{ true };
#ifdef _WIN32
#include <Windows.h>
void ShowTheConsole( bool bShowConsole )
{
	ShowWindow( ::GetConsoleWindow(), bShowConsole );
}
#else
void ShowTheConsole( bool bShowConsole )
{
	// TODO: Not windows implementation
}
#endif

int main( int argc, char* argv[] )
{
#ifdef NDEBUG
	bShowConsole = false;
#endif
	ShowTheConsole( bShowConsole );

	Game::Instance().Run();
	Game::Instance().Shutdown();
	return 0;
}