#pragma once

#include "../AssetManager/AssetManager.h"
#include "../ECS/ECS.h"
#include <memory>

class MusicPlayerSystem : public System
{
public:

	MusicPlayerSystem()
	{
		if (Mix_OpenAudio(44100, MIX_DEFAULT_FORMAT, 2, 4096) == -1)
		{
			Logger::Err("Unable to open the Music Mixer!");
		}
		//Logger::Log("Music Player Constructor Called");
	}
	
	~MusicPlayerSystem()
	{
		Mix_Quit();
		//Logger::Err("Music Destructor called");
	}
	
	void PlayMusic(std::unique_ptr<AssetManager>& assetManager, const std::string& assetID, int loops)
	{
		Mix_PlayMusic(assetManager->GetMusic(assetID), loops);
	}

	void PauseMusic()
	{
		Mix_PauseMusic();
	}
	
	void ResumeMusic()
	{
		Mix_ResumeMusic();
	}

	void StopMusic()
	{
		Mix_HaltMusic();
	}
};
