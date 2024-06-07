#pragma once
#include <memory>
#include <string>

class SoundFX
{
  private:
	class AssetManager& m_AssetManager;

  public:
	// Constructor / Destructor
	SoundFX( class AssetManager& assetManager );
	~SoundFX();

	// Functions --> Play sound effects
	void PlaySoundFX( const std::string& assetID, int loops = 1, int channel = 1 );
};
