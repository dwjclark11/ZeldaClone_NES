#pragma once
#include "../ECS/ECS.h"
#include "../AssetManager/AssetManager.h"
#include "../Components/TriggerBoxComponent.h"
#include "../Components/ItemComponent.h"
#include "../Components/AIComponent.h"
#include <SDL.h>
#include <memory>
#include <sol/sol.hpp>



enum AssetType
{
	TEXTURE = 0,
	SOUNDFX,
	MUSIC,
	FONT
};

class LevelLoader
{
private:
	class Game& game;
	class Registry& reg;
	// Helper Functions
	void ConvertName(std::string name, int x, int y);
	TriggerType ConvertToTriggerType(int triggerType);
	TriggerType ConvertToTriggerType(std::string triggerType);
	AIComponent::EnemyType ConvertStringToEnemyType(std::string enemyType);
	std::string SetName(std::string filePath, bool wExtension = true, char separator = '/');
	AssetType ConvertToAssetType(std::string& type);
	SpecialItemType ConvertLuaStringToSpecial(std::string& special);
	bool CheckForItemInInventory(SpecialItemType& type);

public:
	LevelLoader();
	~LevelLoader();

	void LoadMap(std::string mapName, int image_width, int image_height);
	void LoadTilemap(std::string fileName, std::string imageName);
	void LoadLevelAssets(std::unique_ptr<AssetManager>& assetManager, const std::string& fileName);
	//AssetType ConvertToAssetType(std::string& type);

	//SpecialItemType ConvertLuaStringToSpecial(std::string& special);

	std::string LoadSlotName(unsigned int& slotNum);
	void SaveSlotData(unsigned int& slotNum);

	void LoadLevel(const std::string& level);
	void LoadColliders(const std::string& filename);

	void LoadCollidersFromLuaTable(sol::state& lua, const std::string& fileName);

	void SavePlayerDataToLuaTable(std::string saveNum);
	void SavePlayerNameToLuaTable(std::string saveNum, std::string& name);

	void CreatePlayerEntityFromLuaTable(sol::state& lua, std::string fileName);
	void LoadPlayerDataFromLuaTable(sol::state& lua, std::string fileName);
	void LoadMenuScreenFromLuaTable(sol::state& lua, std::string fileName);
	void LoadMenuUIFromLuaTable(sol::state& lua, std::string fileName);
	void LoadEnemiesFromLuaTable(sol::state& lua, std::string fileName);
	void LoadHUDFromLuaTable(sol::state& lua, std::string fileName);
	void LoadAssetsFromLuaTable(sol::state& lua, std::string fileName);
	void LoadEntitiesFromLuaTable(sol::state& lua, std::string filename);

	void EliminatePlayerToDefault(int slotNum, std::string& name);


	//// Helper Functions
	//void ConvertName(std::string name, int x, int y);
	//TriggerType ConvertToTriggerType(int triggerType);
	//TriggerType ConvertToTriggerType(std::string triggerType);
	//AIComponent::EnemyType ConvertStringToEnemyType(std::string enemyType);
	//std::string SetName(std::string filePath, bool wExtension = true, char separator = '/');
};