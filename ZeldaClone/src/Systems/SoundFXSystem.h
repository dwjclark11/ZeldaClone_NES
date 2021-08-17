#pragma once
#include "../ECS/ECS.h"
#include "../AssetManager/AssetManager.h"
#include "../Logger/Logger.h"
#include <memory>

class SoundFXSystem : public System
{
public:
	// Constructor / Destructor
	SoundFXSystem() {}
	~SoundFXSystem() {}
	
	// Functions --> Play sound effects
	void PlaySoundFX(std::unique_ptr<AssetManager>& assetManager, const std::string& assetID, int loops = 1, int channel = 1)
	{
		Mix_PlayChannel(channel, assetManager->GetSoundFX(assetID), loops);
	}
};
