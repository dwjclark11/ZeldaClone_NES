#include "SoundFXSystem.h"
#include "../AssetManager/AssetManager.h"
#include "../Logger/Logger.h"

SoundFX::SoundFX(AssetManager& assetManager)
	: m_AssetManager(assetManager)
{

}

SoundFX::~SoundFX()
{

}

void SoundFX::PlaySoundFX(const std::string& assetID, int loops, int channel)
{
	if (Mix_PlayChannel(channel, m_AssetManager.GetSoundFX(assetID), loops) == -1)
	{
		Logger::Err("PROBLEMS!");
	}
}
