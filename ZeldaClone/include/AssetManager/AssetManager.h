#pragma once
#include <map>
#include <string>
#include <SDL.h>
#include <SDL_image.h>
#include <SDL_ttf.h>
#include <SDL_mixer.h>

class AssetManager
{
  private:
	std::map<std::string, SDL_Texture*> m_mapTextures;
	std::map<std::string, TTF_Font*> m_mapFonts;
	std::map<std::string, Mix_Music*> m_mapMusic;
	std::map<std::string, Mix_Chunk*> m_mapSoundFx;

  public:
	AssetManager() = default;
	~AssetManager() = default;

	// Add Textures
	void AddTextures( SDL_Renderer* renderer, const std::string& assetID, const std::string& filePath );
	SDL_Texture* GetTexture( const std::string& assetID );
	bool HasTexture( const std::string& assetID );
	std::map<std::string, SDL_Texture*> GetTextureMap() { return m_mapTextures; }
	void RemoveTexture( const std::string& assetID );

	// Fonts
	void AddFonts( const std::string& assetID, const std::string filePath, int fontSize );
	TTF_Font* GetFont( const std::string& assetID );
	bool HasFont( const std::string& assetID );
	void RemoveFont( const std::string& assetID );

	// Music
	void AddMusic( const std::string& assetID, const std::string& filePath );
	Mix_Music* GetMusic( const std::string& assetID );
	bool HasMusic( const std::string& assetID );
	void RemoveMusic( const std::string& assetID );

	// SoundFX
	void AddSoundFX( const std::string& assetID, const std::string& filePath );
	Mix_Chunk* GetSoundFX( const std::string& assetID );
	bool HasSoundFX( const std::string& assetID );
	void RemoveSoundFX( const std::string& assetID );
	void ClearAssets();
};