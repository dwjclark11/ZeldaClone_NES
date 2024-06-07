#include "AssetManager/AssetManager.h"
#include "Logger/Logger.h"

// Functions
void AssetManager::AddTextures( SDL_Renderer* renderer, const std::string& assetID, const std::string& filePath )
{
	if ( HasTexture( assetID ) )
	{
		Logger::Err( "Unable to create texture [" + assetID + "] - Already Exists" );
		return;
	}

	SDL_Surface* surface = IMG_Load( filePath.c_str() );
	SDL_Texture* texture = SDL_CreateTextureFromSurface( std::move( renderer ), std::move( surface ) );

	// Free the surface once the texture is created
	SDL_FreeSurface( surface );

	if ( !texture )
	{
		Logger::Err( "Unable to create texture [" + assetID + "] " + " at filepath: " + filePath );
		return;
	}

	// Add the Textures to the map
	m_mapTextures.emplace( assetID, std::move( texture ) );
}

SDL_Texture* AssetManager::GetTexture( const std::string& assetID )
{
	auto textItr = m_mapTextures.find( assetID );
	if ( textItr == m_mapTextures.end() )
	{
		Logger::Err( "Asset Manager - Failed to get texture: " + assetID + "- Does not exist" );
		return nullptr;
	}

	return textItr->second;
}

bool AssetManager::HasTexture( const std::string& assetID )
{
	return m_mapTextures.contains( assetID );
}

void AssetManager::RemoveTexture( const std::string& assetID )
{
	auto it = m_mapTextures.find( assetID );
	if ( it == m_mapTextures.end() )
	{
		Logger::Log( "ASSET_MANAGER: TEXTURE: __LINE__66: [" + assetID + "] DOES NOT EXIST SO CANNOT BE REMOVED!" );
		return;
	}

	SDL_DestroyTexture( it->second );
	m_mapTextures.erase( it );
}

void AssetManager::AddFonts( const std::string& assetID, const std::string filePath, int fontSize )
{
	if ( m_mapFonts.contains( assetID ) )
	{
		Logger::Err( "ASSET_MANAGER: FONTS: __LINE__64: " + assetID + " ALREADY EXISTS" );
		return;
	}

	m_mapFonts.emplace( assetID, TTF_OpenFont( filePath.c_str(), fontSize ) );
}

TTF_Font* AssetManager::GetFont( const std::string& assetID )
{
	auto fontItr = m_mapFonts.find( assetID );
	if ( fontItr == m_mapFonts.end() )
	{
		Logger::Err( "Asset Manager - Failed to get font: " + assetID + "- Does not exist" );
		return nullptr;
	}

	return fontItr->second;
}

bool AssetManager::HasFont( const std::string& assetID )
{
	return m_mapFonts.contains( assetID );
}

void AssetManager::RemoveFont( const std::string& assetID )
{
}

void AssetManager::AddMusic( const std::string& assetID, const std::string& filePath )
{
	if ( m_mapMusic.contains( assetID ) )
	{
		Logger::Err( "ASSET_MANAGER: MUSIC: __LINE__86: [" + assetID + "] ALREADY EXISTS" );
		return;
	}

	auto pMusic = Mix_LoadMUS( filePath.c_str() );
	if ( !pMusic )
	{
		std::string error{ SDL_GetError() };
		Logger::Err( "Failed to load music [" + assetID + "] Error: " + error );
		return;
	}

	m_mapMusic.emplace( assetID, pMusic );
}

Mix_Music* AssetManager::GetMusic( const std::string& assetID )
{
	auto musicItr = m_mapMusic.find( assetID );
	if ( musicItr == m_mapMusic.end() )
	{
		Logger::Err( "ASSET_MANAGER: Failed to get music: [" + assetID + "] Does not exist!" );
		return nullptr;
	}

	return musicItr->second;
}

bool AssetManager::HasMusic( const std::string& assetID )
{
	return m_mapMusic.contains( assetID );
}

void AssetManager::RemoveMusic( const std::string& assetID )
{
	auto it = m_mapMusic.find( assetID );
	if ( it == m_mapMusic.end() )
	{
		Logger::Log( "ASSET_MANAGER: MUSIC: __LINE__123: [" + assetID + "] DOES NOT EXIST SO CANNOT BE REMOVED!" );
		return;
	}

	Mix_FreeMusic( it->second );
	m_mapMusic.erase( it );
}

void AssetManager::AddSoundFX( const std::string& assetID, const std::string& filePath )
{
	if ( m_mapSoundFx.contains( assetID ) )
	{
		Logger::Err( "ASSET_MANAGER: SOUNDFX: __LINE__106: [" + assetID + "] ALREADY EXISTS" );
		return;
	}

	m_mapSoundFx.emplace( assetID, Mix_LoadWAV( filePath.c_str() ) );
}

Mix_Chunk* AssetManager::GetSoundFX( const std::string& assetID )
{
	auto soundItr = m_mapSoundFx.find( assetID );
	if ( soundItr == m_mapSoundFx.end() )
	{
		Logger::Err( "ASSET_MANAGER: Failed to get soundFX: [" + assetID + "] Does not exist!" );
		return nullptr;
	}

	return soundItr->second;
}

bool AssetManager::HasSoundFX( const std::string& assetID )
{
	return m_mapSoundFx.contains( assetID );
}

void AssetManager::RemoveSoundFX( const std::string& assetID )
{
	auto it = m_mapSoundFx.find( assetID );
	if ( it == m_mapSoundFx.end() )
	{
		Logger::Log( "ASSET_MANAGER: SOUNDFX: __LINE__159: [" + assetID + "] DOES NOT EXIST SO CANNOT BE REMOVED!" );
		return;
	}

	Mix_FreeChunk( it->second );
	m_mapSoundFx.erase( it );
}

void AssetManager::ClearAssets()
{
	for ( auto& texture : m_mapTextures )
	{
		// Destroy all the m_mapTextures
		SDL_DestroyTexture( texture.second ); // Clear the SDL_Textures
	}
	m_mapTextures.clear();

	for ( auto& font : m_mapFonts )
	{
		TTF_CloseFont( font.second );
	}
	m_mapFonts.clear();

	// Destroy/ Remove all Music
	for ( auto& fx : m_mapSoundFx )
	{
		Mix_FreeChunk( fx.second );
	}
	m_mapSoundFx.clear();

	// Destroy / Remove all SoundFX
	for ( auto& m_mapMusic : m_mapMusic )
	{
		Mix_FreeMusic( m_mapMusic.second );
	}

	m_mapMusic.clear();
}
