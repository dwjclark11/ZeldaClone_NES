#include "SoundFXSystem.h"

SoundFXSystem::SoundFXSystem()
{

}

SoundFXSystem::~SoundFXSystem()
{

}

void SoundFXSystem::PlaySoundFX(std::unique_ptr<AssetManager>& assetManager, const std::string& assetID, int loops, int channel)
{
	Mix_PlayChannel(channel, assetManager->GetSoundFX(assetID), loops);
}
