#pragma once
#include "../ECS/ECS.h"
#include "../AssetManager/AssetManager.h"
#include "../Components/TriggerBoxComponent.h"
#include <SDL.h>
#include <memory>
#include "../Utilities/LuaTableWriter.h"

class EditorFileLoader : public System
{
public:
	// Constructor/Destructor
	EditorFileLoader();
	~EditorFileLoader();

	// Load Functions
	void LoadTilemap(const std::unique_ptr<AssetManager>& assetManager, SDL_Renderer* renderer);
	void LoadObjectMap(const std::unique_ptr<AssetManager>& assetManager, SDL_Renderer* renderer);
	void LoadBoxColliderMap(const std::unique_ptr<AssetManager>& assetManager, SDL_Renderer* renderer);
	
	// Save functions
	void SaveTilemap(std::string filepath, const std::unique_ptr<AssetManager>& assetManager, SDL_Renderer* renderer);
	void SaveObjectMap(std::string filepath, const std::unique_ptr<AssetManager>& assetManager, SDL_Renderer* renderer);
	void SaveBoxColliderMap(std::string filepath, const std::unique_ptr<AssetManager>& assetManager, SDL_Renderer* renderer);
	void SaveBoxColliderMapToLuaFile(std::string filepath, const std::unique_ptr<AssetManager>& assetManager, SDL_Renderer* renderer);
	
	// Setters
	void SetFileName(std::string filename);
	void SetImageName(std::string imageName);
	std::string SetName(std::string filePath, bool wExtension = true, char separator = '\\');
	
	// Converters
	TriggerType ConvertToTriggerType(int triggerType);
	std::string ConvertToString(TriggerType triggerType);

private:
	// Declaration of variables
	std::string fileName;
	std::string imageName;
};