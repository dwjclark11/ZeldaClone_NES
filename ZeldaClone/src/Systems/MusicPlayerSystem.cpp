#include "MusicPlayerSystem.h"
#include "../AssetManager/AssetManager.h"

MusicPlayerSystem::MusicPlayerSystem()
{
	if (Mix_OpenAudio(44100, MIX_DEFAULT_FORMAT, 2, 4096) == -1)
	{
		Logger::Err("Unable to open the Music Mixer!");
	}
}

MusicPlayerSystem::~MusicPlayerSystem()
{
	Mix_Quit();
}

void MusicPlayerSystem::PlayMusic(std::unique_ptr<AssetManager>& assetManager, const std::string& assetID, int loops)
{
	Mix_PlayMusic(assetManager->GetMusic(assetID), loops);
}

void MusicPlayerSystem::PauseMusic()
{
	Mix_PauseMusic();
}

void MusicPlayerSystem::ResumeMusic()
{
	Mix_ResumeMusic();
}

void MusicPlayerSystem::StopMusic()
{
	Mix_HaltMusic();
}