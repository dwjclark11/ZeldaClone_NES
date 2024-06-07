#pragma once
#include "ECS/ECS.h"
#include "AssetManager/AssetManager.h"
#include "Components/TriggerBoxComponent.h"
#include "Components/ItemComponent.h"
#include "Components/AIComponent.h"
#include <SDL.h>
#include <memory>
#include <sol/sol.hpp>

class LevelLoader
{
  private:
	enum class AssetType
	{
		TEXTURE = 0,
		SOUNDFX,
		MUSIC,
		FONT,
		NO_TYPE
	};

	class Game& m_Game;
	class GameData& m_GameData;
	class Registry& m_Registry;

  private:
	// Helper Functions
	AssetType ConvertToAssetType( std::string& type );
	SpecialItemType ConvertLuaStringToSpecial( std::string& special );

	ItemCollectType ConvertLuaStringToItem( std::string& type );
	bool CheckForItemInInventory( SpecialItemType& type );
	void ConvertName( const std::string& name, int x, int y );
	bool ReadSpriteComponent( sol::table& table, Entity& entity );
	void ReadBoxColliderComponent( sol::table& table, Entity& entity );
	void ReadAnimationComponent( sol::table& table, Entity& entity );
	void ReadHealthComponent( sol::table& table, Entity& entity );
	void ReadKeyboardControlComponent( sol::table& table, Entity& entity );
	void ReadProjectileEmitterComponent( sol::table& table, Entity& entity );
	void ReadTransformComponent( sol::table& table, Entity& entity );
	void ReadRigidBodyComponent( sol::table& table, Entity& entity );

	void CreateNewGameSecretsFile( int slotNum );

  public:
	LevelLoader();
	~LevelLoader() = default;

	void LoadMap( const std::string& mapName, int image_width, int image_height, bool offset = false );
	void LoadTilemap( const std::string& fileName, const std::string& imageName );
	void LoadLevelAssets( std::unique_ptr<AssetManager>& assetManager, const std::string& fileName );

	void LoadLevel( const std::string& level );
	void LoadColliders( const std::string& filename );
	void LoadTriggers( sol::state& lua, const std::string& fileName );

	void SavePlayerDataToLuaTable( const std::string& saveNum );
	void SavePlayerNameToLuaTable( const std::string& saveNum, std::string& name );

	void CreatePlayerEntityFromLuaTable( sol::state& lua, std::string fileName );
	void LoadPlayerDataFromLuaTable( sol::state& lua, const std::string& fileName, int slotNum );
	void LoadMenuScreenFromLuaTable( sol::state& lua, const std::string& fileName, int slotNum );
	void LoadMenuUIFromLuaTable( sol::state& lua, const std::string& fileName );
	void LoadEnemiesFromLuaTable( sol::state& lua, const std::string& fileName );
	void LoadHUDFromLuaTable( sol::state& lua, const std::string& fileName );
	void LoadAssetsFromLuaTable( sol::state& lua, const std::string& fileName );
	void LoadEntitiesFromLuaTable( sol::state& lua, const std::string& filename );

	void EliminatePlayerToDefault( int slotNum, const std::string& name );

	void SaveSecrets();
	void ReadInSecrets( sol::state& lua );

	TriggerType ConvertStringToTriggerType( const std::string& type );

	EnemyType ConvertStringToEnemyType( const std::string& enemyType );
	EnemyBossType ConvertStringToEnemyBossType( const std::string& bossType );
};