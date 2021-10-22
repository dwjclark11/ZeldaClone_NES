#pragma once
#include "../ECS/ECS.h"
#include "../AssetManager/AssetManager.h"
#include "../Components/TriggerBoxComponent.h"
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
public:
	LevelLoader();
	~LevelLoader();

	void LoadMap(const std::unique_ptr<AssetManager>& assetManager, std::string mapName, int image_width, int image_height);
	void LoadTilemap(const std::unique_ptr<AssetManager>& assetManager, SDL_Renderer* renderer, std::string fileName, std::string imageName);
	void LoadLevelAssets(SDL_Renderer* renderer, std::unique_ptr<AssetManager>& assetManager,  const std::string& fileName);
	void LoadHud(std::unique_ptr<AssetManager>& assetManager, SDL_Renderer* renderer);
	AssetType ConvertToAssetType(std::string& type);

	void LoadMenuScreen(std::unique_ptr<AssetManager>& assetManager, SDL_Renderer* renderer, unsigned int& slotNum);

	void LoadPauseScreen(std::unique_ptr<AssetManager>& assetManager, SDL_Renderer* renderer);

	std::string LoadSlotName(unsigned int& slotNum);
	void SaveSlotData(unsigned int& slotNum);

	void LoadLevel(std::unique_ptr<AssetManager>& assetManager, SDL_Renderer* renderer, const std::string& level);
	void LoadColliders(std::unique_ptr<AssetManager>& assetManager, SDL_Renderer* renderer, const std::string& filename);
	
	void LoadCollidersFromLuaTable(sol::state& lua, std::unique_ptr<AssetManager>& assetManager, SDL_Renderer* renderer, const std::string& fileName);
	
	void SavePlayerDataToLuaTable(std::string saveNum, const std::unique_ptr<AssetManager>& assetManager, SDL_Renderer* renderer);
	void SavePlayerNameToLuaTable(std::string saveNum, std::string& name);
	

	void CreatePlayerEntityFromLuaTable(sol::state& lua, std::string fileName);
	void LoadPlayerDataFromLuaTable(sol::state& lua, std::string fileName);
	void LoadMenuScreenFromLuaTable(sol::state& lua, std::string fileName);
	void LoadMenuUIFromLuaTable(sol::state& lua, std::string fileName);
	void LoadEnemiesFromLuaTable(sol::state& lua, std::string fileName, const std::unique_ptr<AssetManager>& assetManager);
	void LoadHUDFromLuaTable(sol::state& lua, std::string fileName);
	void LoadAssetsFromLuaTable(sol::state& lua, std::string fileName);
	void LoadEntitiesFromLuaTable(sol::state& lua, std::string filename);

	// Helper Functions
	void ConvertName(std::string name, int x, int y);
	TriggerType ConvertToTriggerType(int triggerType);
	TriggerType ConvertToTriggerType(std::string triggerType);
	std::string SetName(std::string filePath, bool wExtension = true, char separator = '/');
};