#pragma once
#include <memory>
#include <string>

class AssetManager;

class MusicPlayer
{
  private:
	AssetManager& m_AssetManager;

  public:
	MusicPlayer( AssetManager& assetManager );
	~MusicPlayer();

	void PlayMusic( const std::string& assetID, int loops );
	void PauseMusic();
	void ResumeMusic();
	void StopMusic();
};
