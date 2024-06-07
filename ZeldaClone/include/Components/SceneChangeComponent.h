#pragma once
#include <string>

struct SceneChangeComponent
{
	std::string levelMusic{ "no_file" }, assetFile{ "no_file" }, enemyFile{ "no_file" }, colliderFile{ "no_file" },
		tileMapName{ "no_file" };
	std::string tileImageName{ "no_file" }, mapImageName{ "no_file" }, entityFileName{ "no_file" },
		triggerFile{ "no_file" };
	int imageHeight{ 0 }, imageWidth{ 0 };
};