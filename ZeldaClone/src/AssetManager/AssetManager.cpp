#include "AssetManager.h"
#include "../Logger/Logger.h"

// Constructor  
AssetManager::AssetManager()
{
	//Logger::Log("AssetManager constructor called");
}

// Destructor
AssetManager::~AssetManager()
{
	//Logger::Log("AssetManager destructor called");
}

// Functions
void AssetManager::AddTextures(SDL_Renderer* renderer, const std::string& assetID, const std::string& filePath)
{
	if (!HasTexture(assetID))
	{
		SDL_Surface* surface = IMG_Load(filePath.c_str());
		SDL_Texture* texture = SDL_CreateTextureFromSurface(std::move(renderer), std::move(surface));
		
		
		if (!texture)
		{
			Logger::Err("Unable to create texture [" + assetID + "] " + " at filepath: " + filePath);
		}

		// Free the surface once the texture is created
		SDL_FreeSurface(surface);

		// Add the Textures to the map
		textures.emplace(assetID, std::move(texture));
	}
}

SDL_Texture* AssetManager::GetTexture(const std::string& assetID)
{
	return textures[assetID];
}

bool AssetManager::HasTexture(const std::string& assetID)
{
	// search the map of textures and return true if the key has been found
	if (textures.find(assetID) != textures.end()) return true;
	
	// Unable to find the key, return false
	return false;
}

void AssetManager::RemoveTexture(const std::string& assetID)
{
	auto it = textures.find(assetID);
	if (it != textures.end())
	{
		SDL_DestroyTexture(it->second);
		textures.erase(it);
	}
	else
		Logger::Log("ASSET_MANAGER: TEXTURE: __LINE__66: [" + assetID + "] DOES NOT EXIST SO CANNOT BE REMOVED!");
}

void AssetManager::AddFonts(const std::string& assetID, const std::string filePath, int fontSize)
{
	if (fonts.find(assetID) == fonts.end())
		fonts.emplace(assetID, TTF_OpenFont(filePath.c_str(), fontSize));
	else
		Logger::Err("ASSET_MANAGER: FONTS: __LINE__64: " + assetID + " ALREADY EXISTS");
}

TTF_Font* AssetManager::GetFont(const std::string& assetID)
{
	return fonts[assetID];
}

bool AssetManager::HasFont(const std::string& assetID)
{
	// search the map of font and return true if the key has been found
	if (fonts.find(assetID) != fonts.end()) return true;

	// Unable to find the key, return false
	return false;
}

void AssetManager::RemoveFont(const std::string& assetID)
{
}

void AssetManager::AddMusic(const std::string& assetID, const std::string& filePath)
{
	if (music.find(assetID) == music.end())
		music.emplace(assetID, Mix_LoadMUS(filePath.c_str()));
	else
		Logger::Err("ASSET_MANAGER: MUSIC: __LINE__86: [" + assetID + "] ALREADY EXISTS");
}

Mix_Music* AssetManager::GetMusic(const std::string& assetID)
{
	return music[assetID];
}

bool AssetManager::HasMusic(const std::string& assetID)
{
	if (music.find(assetID) != music.end()) return true;

	return false;
}

void AssetManager::RemoveMusic(const std::string& assetID)
{
	auto it = music.find(assetID);
	if (it != music.end())
	{
		
		Mix_FreeMusic(it->second);
		music.erase(it);
	}
	else
		Logger::Log("ASSET_MANAGER: MUSIC: __LINE__123: [" + assetID + "] DOES NOT EXIST SO CANNOT BE REMOVED!");
}

void AssetManager::AddSoundFX(const std::string& assetID, const std::string& filePath)
{
	if (soundFX.find(assetID) == soundFX.end())
		soundFX.emplace(assetID, Mix_LoadWAV(filePath.c_str()));
	else
		Logger::Err("ASSET_MANAGER: SOUNDFX: __LINE__106: [" + assetID + "] ALREADY EXISTS");
}

Mix_Chunk* AssetManager::GetSoundFX(const std::string& assetID)
{
	return soundFX[assetID];
}

bool AssetManager::HasSoundFX(const std::string& assetID)
{
	if (soundFX.find(assetID) != soundFX.end()) return true;
	
	return false;
}

void AssetManager::RemoveSoundFX(const std::string& assetID)
{
	auto it = soundFX.find(assetID);
	if (it != soundFX.end())
	{
		Mix_FreeChunk(it->second);
		soundFX.erase(it);
	}
	else
		Logger::Log("ASSET_MANAGER: SOUNDFX: __LINE__159: [" + assetID + "] DOES NOT EXIST SO CANNOT BE REMOVED!");
}

void AssetManager::ClearAssets()
{
	for (auto& texture : textures)
	{
		// Destroy all the textures
		SDL_DestroyTexture(texture.second); // Clear the SDL_Textures
	}
	textures.clear();

	for (auto& font : fonts)
	{
		TTF_CloseFont(font.second);
	}
	fonts.clear();
	
	// Destroy/ Remove all Music
	for (auto& fx : soundFX)
	{
		Mix_FreeChunk(fx.second);
	}
	soundFX.clear();

	// Destroy / Remove all SoundFX
	for (auto& music : music)
	{
		Mix_FreeMusic(music.second);
	}

	music.clear();
}
