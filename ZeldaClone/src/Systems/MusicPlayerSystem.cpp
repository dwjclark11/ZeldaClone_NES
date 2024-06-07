#include "Systems/MusicPlayerSystem.h"
#include "AssetManager/AssetManager.h"
#include "Logger/Logger.h"

MusicPlayer::MusicPlayer( AssetManager& assetManager )
	: m_AssetManager( assetManager )
{
	if ( Mix_OpenAudio( 44100, MIX_DEFAULT_FORMAT, 2, 4096 ) == -1 )
	{
		Logger::Err( "Unable to open the Music Mixer!" );
	}
}

MusicPlayer::~MusicPlayer()
{
	Mix_Quit();
}

void MusicPlayer::PlayMusic( const std::string& assetID, int loops )
{
	if ( Mix_PlayMusic( m_AssetManager.GetMusic( assetID ), loops ) == -1 )
	{
		std::string error{ SDL_GetError() };
		Logger::Err( "Failed to play: " + assetID + ", Error: " + error );
	}
}

void MusicPlayer::PauseMusic()
{
	Mix_PauseMusic();
}

void MusicPlayer::ResumeMusic()
{
	Mix_ResumeMusic();
}

void MusicPlayer::StopMusic()
{
	Mix_HaltMusic();
}