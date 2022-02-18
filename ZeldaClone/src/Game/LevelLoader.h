#pragma once
#include "../ECS/ECS.h"
#include "../AssetManager/AssetManager.h"
#include "../Components/TriggerBoxComponent.h"
#include "../Components/ItemComponent.h"
#include "../Components/AIComponent.h"
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
		FONT
	};

	class Game& game;
	class Registry& reg;

	// Helper Functions
	AssetType ConvertToAssetType(std::string& type);
	ItemComponent::SpecialItemType ConvertLuaStringToSpecial(std::string& special);
	
	ItemComponent::ItemCollectType ConvertLuaStringToItem(std::string& type);
	bool CheckForItemInInventory(ItemComponent::SpecialItemType& type);
	void ConvertName(std::string name, int x, int y);
	bool ReadSpriteComponent(sol::table& table, Entity& entity);
	void ReadBoxColliderComponent(sol::table& table, Entity& entity);
	void ReadAnimationComponent(sol::table& table, Entity& entity);
	void ReadHealthComponent(sol::table& table, Entity& entity);
	void ReadKeyboardControlComponent(sol::table& table, Entity& entity);
	void ReadProjectileEmitterComponent(sol::table& table, Entity& entity);
	void ReadTransformComponent(sol::table& table, Entity& entity);
	void ReadRigidBodyComponent(sol::table& table, Entity& entity);

	void CreateNewGameSecretsFile(int slotNum);

public:
	LevelLoader();
	~LevelLoader();

	void LoadMap(std::string mapName, int image_width, int image_height, bool offset = false);
	void LoadTilemap(std::string fileName, std::string imageName);
	void LoadLevelAssets(std::unique_ptr<AssetManager>& assetManager, const std::string& fileName);

	void LoadLevel(const std::string& level);
	void LoadColliders(const std::string& filename);
	void LoadTriggers(sol::state& lua, const std::string& fileName);

	void SavePlayerDataToLuaTable(std::string saveNum);
	void SavePlayerNameToLuaTable(std::string saveNum, std::string& name);

	void CreatePlayerEntityFromLuaTable(sol::state& lua, std::string fileName);
	void LoadPlayerDataFromLuaTable(sol::state& lua, std::string fileName, int slotNum);
	void LoadMenuScreenFromLuaTable(sol::state& lua, std::string fileName, int slotNum);
	void LoadMenuUIFromLuaTable(sol::state& lua, std::string fileName);
	void LoadEnemiesFromLuaTable(sol::state& lua, std::string fileName);
	void LoadHUDFromLuaTable(sol::state& lua, std::string fileName);
	void LoadAssetsFromLuaTable(sol::state& lua, std::string fileName);
	void LoadEntitiesFromLuaTable(sol::state& lua, std::string filename);

	void EliminatePlayerToDefault(int slotNum, std::string& name);

	void SaveSecrets();
	void ReadInSecrets(sol::state& lua);

	TriggerBoxComponent::TriggerType ConvertStringToTriggerType(std::string type);
	AIComponent::EnemyType ConvertStringToEnemyType(std::string enemyType);

};