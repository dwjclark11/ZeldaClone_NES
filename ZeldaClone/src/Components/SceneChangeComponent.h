#pragma once
#include <string>

struct SceneChangeComponent
{
	std::string levelMusic, assetFile, enemyFile, colliderFile, tileMapName;
	std::string tileImageName, mapImageName, entityFileName, triggerFile;

	int imageHeight, imageWidth;

	SceneChangeComponent(const std::string& levelMusic = "no_file", const std::string& assetFile = "no_file", 
		const std::string& enemyFile = "no_file", const std::string& colliderFile = "no_file", 
		const std::string& tileMapName = "no_file", const std::string& tileImageName = "no_file", 
		const std::string& mapImageName = "no_file", const std::string& entityFileName = "no_file", 
		const std::string& triggerFile = "no_file", int imageWidth = 0, int imageHeight = 0)
		: levelMusic{ levelMusic }, assetFile{assetFile}, enemyFile{enemyFile}, colliderFile{colliderFile}
		, tileMapName{ tileMapName }, tileImageName{ tileImageName}, mapImageName{mapImageName}
		, entityFileName{entityFileName}, triggerFile{triggerFile}, imageHeight{imageHeight}, imageWidth{imageWidth}
	{
		
	}
};