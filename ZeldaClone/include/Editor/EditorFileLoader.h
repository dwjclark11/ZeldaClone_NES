#pragma once
#include "ECS/ECS.h"
#include "AssetManager/AssetManager.h"
#include "Components/TriggerBoxComponent.h"
#include "Components/AIComponent.h"
#include "Utilities/LuaTableWriter.h"

#include <SDL.h>
#include <memory>
#include <sol/sol.hpp>

class EditorFileLoader : public System
{
  public:
	// Constructor/Destructor
	EditorFileLoader( class MouseControlSystem& mouseControl );
	~EditorFileLoader();

	// Load Functions
	void LoadTilemap( const std::unique_ptr<AssetManager>& assetManager, SDL_Renderer* renderer );
	void LoadObjectMap( const std::unique_ptr<AssetManager>& assetManager, SDL_Renderer* renderer );
	void LoadBoxColliderMap( const std::unique_ptr<AssetManager>& assetManager, SDL_Renderer* renderer,
							 std::string& fileName );

	// Save functions
	void SaveTilemap( std::string filepath, const std::unique_ptr<AssetManager>& assetManager, SDL_Renderer* renderer );
	void SaveObjectMap( std::string filepath, const std::unique_ptr<AssetManager>& assetManager,
						SDL_Renderer* renderer );
	void SaveBoxColliderMap( std::string filepath, const std::unique_ptr<AssetManager>& assetManager,
							 SDL_Renderer* renderer );
	void SaveBoxColliderMapToLuaFile( std::string filepath );
	void SaveEnemiesToLuaFile( std::string filepath );
	void SaveTriggersToLuaFile( std::string filepath );

	// Setters
	void SetFileName( std::string filename );
	void SetImageName( std::string imageName );
	std::string SetName( std::string filePath, bool wExtension = true, char separator = '\\' );

	void LoadEnemiesAttributes( sol::state& lua, std::string& fileName, std::string& enemy_name );
	void CreateNewEnemy( sol::state& lua, std::string& fileName, std::string& enemy_name, Entity& newEnemy );

	// Converters
	std::string ConvertToString( TriggerType triggerType );
	std::string ConvertAIEnemyToString( EnemyType type );

  private:
	// Declaration of variables
	std::string m_sFileName;
	std::string m_sImageName;
	class Registry& m_Registry;
	class MouseControlSystem& m_MouseControl;

	void WriteTransformComponent( LuaTableWriter& writer, const class TransformComponent& transform, std::fstream& file,
								  bool last );
	void WriteBoxColliderComponent( LuaTableWriter& writer, const class BoxColliderComponent& collision,
									std::fstream& file, bool last );
	void WriteTriggerBoxComponent( LuaTableWriter& writer, const class TriggerBoxComponent& trigger, std::fstream& file,
								   std::string& trigger_type, bool last );
	void WriteSecretComponent( LuaTableWriter& writer, const class SecretComponent& secret, std::fstream& file,
							   bool last );
	void WriteSpriteComponent( LuaTableWriter& writer, const class SpriteComponent& sprite, std::fstream& file,
							   bool last );
};