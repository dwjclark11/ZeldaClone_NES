#pragma once
#include <string>

struct SceneChangeComponent
{
	std::string levelMusic;
	std::string assetFile;
	std::string enemyFile;
	std::string colliderFile;
	std::string tileMapName;
	std::string tileImageName;
	std::string mapImageName;
	std::string entityFileName;
	std::string triggerFile;

	int imageHeight;
	int imageWidth;

	SceneChangeComponent(std::string levelMusic = "no_file", std::string assetFile = "no_file", std::string enemyFile = "no_file",
		std::string colliderFile = "no_file", std::string tileMapName = "no_file", std::string tileImageName = "no_file", 
		std::string mapImageName = "no_file", std::string entityFileName = "no_file", std::string triggerFile = "no_file",
		int imageWidth = 0, int imageHeight = 0)
	{
		this->levelMusic = levelMusic;
		this->assetFile = assetFile;
		this->enemyFile = enemyFile;
		this->colliderFile = colliderFile;
		this->tileMapName = tileMapName;
		this->tileImageName = tileImageName;
		this->mapImageName = mapImageName;
		this->entityFileName = entityFileName;
		this->triggerFile = triggerFile;
		this->imageHeight = imageHeight;
		this->imageWidth = imageWidth;
	}
};