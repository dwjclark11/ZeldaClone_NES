#pragma once
#include "../ECS/ECS.h"
#include <memory>

class AssetManager;

class MusicPlayerSystem : public System
{
public:
	MusicPlayerSystem();	
	~MusicPlayerSystem();

	void PlayMusic(std::unique_ptr<AssetManager>& assetManager, const std::string& assetID, int loops);
	void PauseMusic();
	void ResumeMusic();
	void StopMusic();
	
};
