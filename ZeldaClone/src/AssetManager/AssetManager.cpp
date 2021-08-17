#include "AssetManager.h"
#include "../Logger/Logger.h"

// Constructor  
AssetManager::AssetManager()
{
	Logger::Log("AssetManager constructor called");
}

// Destructor
AssetManager::~AssetManager()
{
	Logger::Log("AssetManager destructor called");
}

// Functions
void AssetManager::AddTextures(SDL_Renderer* renderer, const std::string& assetID, const std::string& filePath)
{
	SDL_Surface* surface = IMG_Load(filePath.c_str());
	SDL_Texture* texture = SDL_CreateTextureFromSurface(renderer, surface);

	if (!texture)
	{
		Logger::Err("Unable to create texture [" + assetID + "] " + " at filepath: " + filePath);
	}

	// Free the surface once the texture is created
	SDL_FreeSurface(surface);

	// Add the Textures to the map
	textures.emplace(assetID, texture);

	//Logger::Log("New texture add to the asset manager with ID " + assetID);
}

SDL_Texture* AssetManager::GetTexture(const std::string& assetID)
{
	return textures[assetID];
}

bool AssetManager::HasTexture(const std::string& assetID)
{
	// search the map of textures and return true if the key has been found
	for (size_t i = 0; i < textures.size(); i++)
	{
		if (textures.find(assetID) != textures.end()) return true;
	}
	
	// Unable to find the key, return false
	return false;
}

void AssetManager::AddFonts(const std::string& assetID, const std::string filePath, int fontSize)
{
	fonts.emplace(assetID, TTF_OpenFont(filePath.c_str(), fontSize));
}

TTF_Font* AssetManager::GetFont(const std::string& assetID)
{
	return fonts[assetID];
}

bool AssetManager::HasFont(const std::string& assetID)
{
	// search the map of font and return true if the key has been found
	for (size_t i = 0; i < fonts.size(); i++)
	{
		if (fonts.find(assetID) != fonts.end()) return true;
	}
	
	// Unable to find the key, return false
	return false;
}

void AssetManager::AddMusic(const std::string& assetID, const std::string& filePath)
{
	music.emplace(assetID, Mix_LoadMUS(filePath.c_str()));
}

Mix_Music* AssetManager::GetMusic(const std::string& assetID)
{
	return music[assetID];
}

bool AssetManager::HasMusic(const std::string& assetID)
{
	for (size_t i = 0; i < music.size(); i++)
	{
		if (music.find(assetID) != music.end()) return true;
	}

	return false;
}

void AssetManager::AddSoundFX(const std::string& assetID, const std::string& filePath)
{
	soundFX.emplace(assetID, Mix_LoadWAV(filePath.c_str()));
}

Mix_Chunk* AssetManager::GetSoundFX(const std::string& assetID)
{
	return soundFX[assetID];
}

bool AssetManager::HasSoundFX(const std::string& assetID)
{
	for (size_t i = 0; i < soundFX.size(); i++)
	{
		if (soundFX.find(assetID) != soundFX.end()) return true;
	}

	return false;
}

void AssetManager::ClearAssets()
{
	for (auto texture : textures)
	{
		// Destroy all the textures
		SDL_DestroyTexture(texture.second); // Clear the SDL_Textures
	}
	textures.clear();

	for (auto font : fonts)
	{
		TTF_CloseFont(font.second);
	}
	fonts.clear();
	
	// TODO: 
	// Destroy/ Remove all Music
	
	
	
	// TODO: 
	// Destroy / Remove all SoundFX
	
}
