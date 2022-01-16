#include "LevelLoader.h"
#include "../Components/AnimationComponent.h"
#include "../Components/RigidBodyComponent.h"
#include "../Components/SpriteComponent.h"
#include "../Components/TransformComponent.h"
#include "../Components/AIComponent.h"
#include "../Components/HealthComponent.h"
#include "../Components/BoxColliderComponent.h"
#include "../Components/TriggerBoxComponent.h"
#include "../Components/PlayerComponent.h"
#include "../Components/EnemyComponent.h"
#include "../Components/ColliderComponent.h"
#include "../Components/KeyboardControlComponent.h"
#include "../Components/HUDComponent.h"
#include "../Components/MenuComponent.h"
#include "../Components/TileComponent.h"
#include "../Components/CaptionComponent.h"
#include "../Components/GameComponent.h"
#include "../Components/ScriptComponent.h"
#include "../Components/ProjectileEmitterComponent.h"
#include "../Components/CameraFollowComponent.h"
#include "../Components/ItemComponent.h"
#include "../States/NameState.h"
#include "../States/MenuState.h"
#include "../States/GameState.h"
#include "../Game/Game.h"
#include "../Utilities/LuaTableWriter.h"
#include "../Systems/GameSystems/RenderHealthSystem.h"
#include "../Systems/SoundFXSystem.h"
#include <string>
#include <fstream>
#include <SDL.h>
LevelLoader::LevelLoader()
	: game(*Game::Instance()), reg(*Registry::Instance())
{
}

LevelLoader::~LevelLoader()
{
}

void LevelLoader::LoadMap(std::string mapName, int image_width, int image_height)
{
	Entity secret = reg.CreateEntity();
	secret.AddComponent<TransformComponent>(glm::vec2(0, 0), glm::vec2(4, 4), 0.0);
	secret.AddComponent<SpriteComponent>(mapName, image_width, image_height, 1, false, 0, 0);
	secret.AddComponent<TileComponent>();
	secret.Group("map");
}

void LevelLoader::LoadTilemap(std::string fileName, std::string imageName)
{
	// Adding Textures to the asset Manager
	std::string assetID = imageName;

	int tileSize = 16; // Should not be hard coded

	// Open and read the text file
	std::fstream mapFile;
	mapFile.open(fileName);

	// Check to see if the file opened correctly
	if (!mapFile.is_open())
	{
		Logger::Err("Unable to open [" + fileName + "] This");
	}

	// Load the tilemap file 
	while (true)
	{
		int srcRectX = 0;
		int srcRectY = 0;
		int tranX = 0;
		int tranY = 0;
		int colWidth = 0;
		int colHeight = 0;
		int tileScaleX = 0;
		int tileScaleY = 0;
		int zIndex = 0;
		int tileWidth = 0;
		int tileHeight = 0;
		std::string group = "";
		//std::string assetID = "";
		glm::vec2 offset = glm::vec2(0, 0);
		bool collider = false;

		// Read the contents of the file into the declared variables
		mapFile >> group >> tileWidth >> tileHeight >> srcRectY >> srcRectX >> zIndex >> tranX >> tranY >> tileScaleX >> tileScaleY >> collider;

		// Check to see if there is a collider and load data accordingly
		if (collider) mapFile >> colWidth >> colHeight >> offset.x >> offset.y;

		// If we are at the end of the File --> Leave the File!!
		if (mapFile.eof()) break;

		// Create a new entity for each tile
		Entity tile = reg.CreateEntity();
		tile.AddComponent<SpriteComponent>(assetID, tileWidth, tileHeight, zIndex, false, srcRectX, srcRectY);
		tile.AddComponent<TransformComponent>(glm::vec2(tranX, tranY), glm::vec2(tileScaleX, tileScaleX), 0.0);
		tile.AddComponent<ColliderComponent>();
		if (zIndex < 2)
		{
			tile.AddComponent<TileComponent>();
			tile.Group(group);
		}

		else
		{
			//Logger::Log("Game Tile");
			tile.AddComponent<GameComponent>(); // RenderTileSystem needs to be changed to to layers
			tile.Group("layered");
		}

		// If the tile is a collider, add a boxColliderComponent
		if (collider)
		{
			//Logger::Log("Collider");
			tile.AddComponent<BoxColliderComponent>(colWidth, colHeight, glm::vec2(offset.x, offset.y));
		}
	}
	// Close the file
	mapFile.close();
}

void LevelLoader::LoadLevelAssets(std::unique_ptr<AssetManager>& assetManager, const std::string& fileName)
{
	std::fstream lvlFile;
	lvlFile.open("Assets/LevelAssets/" + fileName);

	if (!lvlFile.is_open())
	{
		Logger::Err("Unable to open [" + fileName + "]");
	}

	while (!lvlFile.eof())
	{
		// Define Variables
		std::string type = "";
		std::string assetID = "";
		std::string path = "./Assets/";
		std::string assetPath = "";
		AssetType assetType = TEXTURE;
		lvlFile >> type >> assetID >> assetPath;

		assetType = ConvertToAssetType(type);

		switch (assetType)
		{
		case TEXTURE:
			assetManager->AddTextures(game.GetRenderer(), assetID, (path + assetPath));
			break;
		case MUSIC:
			assetManager->AddMusic(assetID, path + assetPath);
			break;
		case SOUNDFX:
			assetManager->AddSoundFX(assetID, path + assetPath);
			break;
		case FONT:
			assetManager->AddFonts(assetID, path + assetPath, 14);
			break;
		default:
			break;
		}
	}
}

AssetType LevelLoader::ConvertToAssetType(std::string& type)
{
	if (type == "TEXTURE")
		return TEXTURE;
	if (type == "SOUNDFX")
		return SOUNDFX;
	if (type == "FONT")
		return FONT;
	if (type == "MUSIC")
		return MUSIC;
}

void LevelLoader::LoadMenuScreenFromLuaTable(sol::state& lua, std::string fileName)
{
	bool valid = false;
	sol::load_result script = lua.load_file("./Assets/SavedFiles/" + fileName + ".lua");
	// Check the syntax of the Lua script 
	if (!script.valid())
	{
		sol::error err = script;
		std::string errorMessage = err.what(); // What is the error
		Logger::Err("Error loading the Lua Script: " + errorMessage);
		return;
	}


	// Execute the script using the sol state 
	lua.script_file("./Assets/SavedFiles/" + fileName + ".lua");

	/* Read the player data and check for:
					- Number of hearts
					- Name of the Saved Player for that slot
					- What tunic the Player has to set the sprite srcRect positions
	*/
	// Read the main table first
	sol::table data = lua["player_data"];

	int i = 1;
	int numHearts = 0;
	int transformX = 0;
	int xTransform = 0;
	int transformY = 0;
	int yTransform = 0;
	int srcRectX = 0;
	bool blueRing = false;
	bool redRing = false;

	std::string entityTag = "";
	while (true)
	{
		sol::optional<sol::table> hasData = data[i];

		if (hasData == sol::nullopt)
		{
			Logger::Err("There is no Player Data!");
			break;
		}
		sol::table player = data[i];
		// Name 
		sol::optional<sol::table> menuSharedValues = player["menu_shared_values"];
		if (menuSharedValues != sol::nullopt)
		{
			if (fileName == "save1")
			{
				MenuState::player1Name = player["menu_shared_values"]["name"];
				ConvertName(MenuState::player1Name, 400, 220);
				transformX = 550;
				transformY = 200;
				entityTag = "first_menu_slot";

			}
			if (fileName == "save2")
			{
				MenuState::player2Name = player["menu_shared_values"]["name"];
				ConvertName(MenuState::player2Name, 400, 320);
				transformX = 550;
				transformY = 300;
				entityTag = "second_menu_slot";
			}
			if (fileName == "save3")
			{
				MenuState::player3Name = player["menu_shared_values"]["name"];
				ConvertName(MenuState::player3Name, 400, 420);
				transformX = 550;
				transformY = 400;
				entityTag = "third_menu_slot";
			}

			// Read the number of Hearts from the file and display them accordingly on the screen
			numHearts = player["menu_shared_values"]["num_hearts"];

			for (int j = 0; j < numHearts; j++)
			{
				if (j > 0) xTransform += 32;
				if (j == 8)
				{
					yTransform += 32;
					xTransform = 0;
				}

				// Create new heart Entity for each parsed tile
				Entity heart = reg.CreateEntity();
				heart.Group("hearts_" + fileName);
				heart.AddComponent<SpriteComponent>("hearts", 16, 16, 0, false, 0, 0);
				heart.AddComponent<TransformComponent>(glm::vec2(transformX + xTransform, transformY + yTransform), glm::vec2(4, 4), 0.0);
				heart.AddComponent<MenuComponent>();
			}

			auto entity = reg.GetEntityByTag(entityTag);
			auto& sprite = entity.GetComponent<SpriteComponent>();

			blueRing = player["menu_shared_values"]["blue_ring"];
			redRing = player["menu_shared_values"]["red_ring"];

			if (blueRing && !redRing) sprite.srcRect.x = 64;
			if (redRing) sprite.srcRect.x = 128;
		}
		i++;
	}
	i = 1;
}

void LevelLoader::EliminatePlayerToDefault(int slotNum, std::string& name)
{
	// Completly Delete the player
	for (auto& nameImage : reg.GetEntitiesByGroup(name))
		nameImage.Kill();

	for (auto& playerHearts : reg.GetEntitiesByGroup("hearts_save" + std::to_string(slotNum)))
		playerHearts.Kill();

	// Play Eliminate Sound
	game.GetSystem<SoundFXSystem>().PlaySoundFX(game.GetAssetManager(), "Eliminate", 0, 1);
}

void LevelLoader::LoadMenuUIFromLuaTable(sol::state& lua, std::string fileName)
{
	sol::load_result script = lua.load_file("./Assets/LuaFiles/" + fileName + ".lua");
	// Check the syntax of the Lua script 
	if (!script.valid())
	{
		sol::error err = script;
		std::string errorMessage = err.what(); // What is the error
		Logger::Err("Error loading the Lua Script: " + errorMessage);
		return;
	}

	// Execute the script using the sol state 
	lua.script_file("./Assets/LuaFiles/" + fileName + ".lua");

	/* Read the player data and check for:
					- Number of hearts
					- Name of the Saved Player for that slot
					- What tunic the Player has to set the sprite srcRect positions
	*/
	// Read the main table first
	sol::table data = lua["menu"];

	int i = 1;

	while (true)
	{
		sol::optional<sol::table> hasData = data[i];

		if (hasData == sol::nullopt)
		{
			//Logger::Err("Menu File Read!");
			break;
		}
		sol::table ui = data[i];
		auto entity = reg.CreateEntity();


		sol::optional tag = ui["tag"];
		if (tag != sol::nullopt)
		{
			std::string tempTag = ui["tag"];
			entity.Tag(tempTag);
		}

		// Components
		sol::optional<sol::table> hasComponents = ui["components"];
		if (hasComponents != sol::nullopt)
		{
			// Transform Component
			sol::optional<sol::table> transform = ui["components"]["transform"];
			if (transform != sol::nullopt)
			{
				entity.AddComponent<TransformComponent>(
					glm::vec2(
						ui["components"]["transform"]["position"]["x"],
						ui["components"]["transform"]["position"]["y"]
					),
					glm::vec2(
						ui["components"]["transform"]["scale"]["x"].get_or(1),
						ui["components"]["transform"]["scale"]["y"].get_or(1)
					),
					ui["components"]["rotation"].get_or(0)
					);
			}

			// Sprite Component
			sol::optional<sol::table> sprite = ui["components"]["sprite"];
			if (sprite != sol::nullopt)
			{
				entity.AddComponent<SpriteComponent>(
					ui["components"]["sprite"]["asset_id"],
					ui["components"]["sprite"]["width"],
					ui["components"]["sprite"]["height"],
					ui["components"]["sprite"]["z_index"].get_or(0),
					ui["components"]["sprite"]["fixed"].get_or(false),
					ui["components"]["sprite"]["src_rect_x"].get_or(0),
					ui["components"]["sprite"]["src_rect_y"].get_or(0)
					);
			}
		}
		entity.AddComponent<MenuComponent>();
		i++;
	}
}


std::string LevelLoader::LoadSlotName(unsigned int& slotNum)
{

	// open slot file
	std::fstream slotFile;
	slotFile.open("./Assets/SavedFiles/slot" + std::to_string(slotNum) + ".txt");
	std::string name = "";
	if (!slotFile.is_open())
	{
		Logger::Err("Unable to open slot " + std::to_string(slotNum) + " File");
	}

	while (true)
	{
		if (slotFile.eof()) break;

		slotFile >> name;

	}slotFile.close();

	return name;
}

void LevelLoader::SaveSlotData(unsigned int& slotNum)
{
	std::fstream slotFile;
	slotFile.open("./Assets/SavedFiles/slot" + std::to_string(slotNum) + ".txt");

	if (!slotFile.is_open())
	{
		Logger::Err("Unable to open slot " + std::to_string(slotNum) + " File");
	}

	std::string name = NameState::name;
	slotFile << name;

	//Logger::Log("Slot number " + std::to_string(slotNum) + " is open");
	slotFile.close();
}

void LevelLoader::LoadLevel(const std::string& level)
{
	// Open and read the thext file
	std::fstream mapFile;
	mapFile.open("./Assets/Levels/overworld.map");

	if (!mapFile.is_open())
	{
		Logger::Err("Unable to open file");
	}

	while (true)
	{
		int srcRectX = 0;
		int srcRectY = 0;
		int tranX = 0;
		int tranY = 0;
		int colWidth = 0;
		int colHeight = 0;
		int tileScaleX = 0;
		int tileScaleY = 0;
		int zIndex = 0;
		int tileWidth = 0;
		int tileHeight = 0;
		std::string group = "";
		std::string assetID = "";
		glm::vec2 offset = glm::vec2(0, 0);
		bool collider = false;
		mapFile >> group >> tileWidth >> tileHeight >> srcRectY >> srcRectX >> zIndex >> tranX >> tranY >> tileScaleX >> tileScaleY >> collider;

		if (collider) mapFile >> colWidth >> colHeight >> offset.x >> offset.y;

		// If we are at the end of the File --> Leave the File!!
		if (mapFile.eof()) break;

		// Create a new entity for each tile
		Entity tile = reg.CreateEntity();
		tile.Group(group);
		tile.AddComponent<SpriteComponent>("tiles", tileWidth, tileHeight, 0, false, srcRectX, srcRectY);
		tile.AddComponent<TransformComponent>(glm::vec2(tranX, tranY), glm::vec2(tileScaleX, tileScaleX), 0.0);
		tile.AddComponent<TileComponent>();
		if (collider)
		{
			tile.AddComponent<BoxColliderComponent>(colWidth, colHeight, glm::vec2(offset.x, offset.y));
		}
	}
	mapFile.close();
}

void LevelLoader::LoadColliders(const std::string& filename)
{

	// Open and read the text file
	std::fstream mapFile;
	mapFile.open("./Assets/Levels/" + filename + ".map");

	if (!mapFile.is_open())
	{
		Logger::Err("Unable to open file");
	}

	while (true)
	{
		int tranX = 0;
		int tranY = 0;
		int colWidth = 0;
		int colHeight = 0;
		int colliderScaleX = 0;
		int colliderScaleY = 0;
		std::string group = "";
		glm::vec2 offset = glm::vec2(0, 0);
		glm::vec2 triggerOffset = glm::vec2(0, 0);
		glm::vec2 triggerCamOffset = glm::vec2(0, 0);
		bool collider = false;
		bool trigger = false;
		int type = 0;
		std::string triggerLevel = "";
		std::string triggerAssetFile = "";
		std::string triggerEnemyFile = "";
		std::string triggerColliderFile = "";
		std::string triggerTilemapFile = "";
		std::string triggerTilemapImage = "";
		std::string triggerEntityFile = "";
		int triggerImageHeight = 0;
		int triggerImageWidth = 0;
		TriggerType triggerType = NO_TRIGGER;

		mapFile >> group >> tranX >> tranY >> colliderScaleX >> colliderScaleY >> collider >> trigger;

		if (collider && !trigger) mapFile >> colWidth >> colHeight >> offset.x >> offset.y;
		else if (collider && trigger) mapFile >> colWidth >> colHeight >> offset.x >> offset.y >> type >> triggerOffset.x >>
			triggerOffset.y >> triggerCamOffset.x >> triggerCamOffset.y >> triggerLevel >> triggerAssetFile >> triggerEnemyFile >>
			triggerColliderFile >> triggerTilemapFile >> triggerTilemapImage >> triggerEntityFile >> triggerImageHeight >> triggerImageWidth;


		triggerType = ConvertToTriggerType(type);

		// If we are at the end of the File --> Leave the File!!
		if (mapFile.eof()) break;

		// Create a new entity for each tile
		Entity boxCollider = reg.CreateEntity();
		boxCollider.Group(group);
		boxCollider.AddComponent<TransformComponent>(glm::vec2(tranX, tranY), glm::vec2(colliderScaleX, colliderScaleY), 0.0);
		boxCollider.AddComponent<GameComponent>();
		boxCollider.AddComponent<ColliderComponent>();
		if (collider && !trigger)
		{
			boxCollider.AddComponent<BoxColliderComponent>(colWidth, colHeight, glm::vec2(offset.x, offset.y));
		}
		if (collider && trigger)
		{
			Logger::Log("Trigger Created");
			boxCollider.AddComponent<BoxColliderComponent>(colWidth, colHeight, glm::vec2(offset.x, offset.y));
			boxCollider.AddComponent<TriggerBoxComponent>(triggerType, glm::vec2(triggerOffset.x, triggerOffset.y), glm::vec2(triggerCamOffset.x, triggerCamOffset.y),
				triggerLevel, triggerAssetFile, triggerEnemyFile, triggerColliderFile, triggerTilemapFile, triggerTilemapImage, triggerEntityFile, triggerImageHeight, triggerImageWidth);
			boxCollider.AddComponent<GameComponent>();
		}
	}
	mapFile.close();
}

TriggerType LevelLoader::ConvertToTriggerType(int triggerType)
{
	// This function takes the int value from the text file and converts it to the 
	// proper trigger type enum value.
	switch (triggerType)
	{
	case 0: return NO_TRIGGER; break;
	case 1: return SECRET_AREA; break;
	case 2: return ENTER_DUNGEON; break;
	case 3: return BURN_BUSHES; break;
	case 4: return PUSH_ROCKS; break;
	case 5: return COLLECT_ITEM; break;
	case 6: return TRAP; break;
	case 7: return HIDDEN_SWITCH; break;
	case 8: return HIDDEN_OBJECT; break;
	case 9: return SHOP_ITEM; break;
	default: break;
	}
}

//TriggerType LevelLoader::ConvertToTriggerType(std::string triggerType)
//{
//	if (triggerType == "NO_TRIGGER") return NO_TRIGGER;
//	else if (triggerType == "SECRET_AREA") return SECRET_AREA;
//}

AIComponent::EnemyType LevelLoader::ConvertStringToEnemyType(std::string enemyType)
{
	if (enemyType == "octorok")
		return AIComponent::EnemyType::OCTOROK;
	else if (enemyType == "moblin")
		return AIComponent::EnemyType::MOBLIN;
	else if (enemyType == "darknut")
		return AIComponent::EnemyType::DARKNUT;
	else if (enemyType == "leever")
		return AIComponent::EnemyType::LEEVER;
	else if (enemyType == "tektite")
		return AIComponent::EnemyType::TEKTITE;
	else if (enemyType == "peahat")
		return AIComponent::EnemyType::PEAHAT;
	else if (enemyType == "armos")
		return AIComponent::EnemyType::ARMOS;
	else if (enemyType == "ghini")
		return AIComponent::EnemyType::GHINI;
	else if (enemyType == "lynel")
		return AIComponent::EnemyType::LYNEL;
	else if (enemyType == "zora")
		return AIComponent::EnemyType::ZORA;
	else
		return AIComponent::EnemyType::OCTOROK;
}

std::string LevelLoader::SetName(std::string filePath, bool wExtension, char separator)
{
	// Get the last '.' position
	std::size_t sepPos = filePath.rfind(separator);
	std::size_t found = filePath.find_last_of('.');
	if (sepPos != std::string::npos)
	{
		std::string name = filePath = filePath.substr(0, found);
		return name.substr(sepPos + 1);
	}
	return "";
}

void LevelLoader::LoadCollidersFromLuaTable(sol::state& lua, const std::string& fileName)
{
	sol::load_result script = lua.load_file("./Assets/Levels/" + fileName + ".lua");

	// This checks the syntax of our script, but it does not execute the script
	if (!script.valid())
	{
		sol::error err = script;
		std::string errorMessage = err.what();
		Logger::Err("Error loading the Lua Script: " + errorMessage);
		return;
	}

	// Executes the script using the sol State
	lua.script_file("./Assets/Levels/" + fileName + ".lua");

	// =========================================================================================================
	// Read the colliders
	// =========================================================================================================

	// Read the big table for the colliders
	sol::table colliders = lua["colliders"];

	int i = 1;

	while (true)
	{
		sol::optional<sol::table> hasCollider = colliders[i];
		if (hasCollider == sol::nullopt)
		{
			break;
		}

		sol::table collider = colliders[i];

		Entity newCollider = reg.CreateEntity();

		// Group
		sol::optional<std::string> group = collider["group"];
		if (group != sol::nullopt)
		{
			newCollider.Group(collider["group"]);
		}
		// Components
		sol::optional<sol::table> hasComponents = collider["components"];
		if (hasComponents != sol::nullopt)
		{
			// Transform Component
			sol::optional<sol::table> transform = collider["components"]["transform"];
			if (transform != sol::nullopt)
			{
				newCollider.AddComponent<TransformComponent>(
					glm::vec2(
						collider["components"]["transform"]["position"]["x"],
						collider["components"]["transform"]["position"]["y"]),
					glm::vec2(
						collider["components"]["transform"]["scale"]["x"].get_or(1.0),
						collider["components"]["transform"]["scale"]["y"].get_or(1.0)),
					collider["components"]["transform"]["rotation"].get_or(0.0));
			}
			// Box Collider Component
			sol::optional<sol::table> boxCollider = collider["components"]["boxCollider"];
			if (boxCollider != sol::nullopt)
			{
				newCollider.AddComponent<BoxColliderComponent>(
					collider["components"]["boxCollider"]["width"],
					collider["components"]["boxCollider"]["height"],
					glm::vec2(
						collider["components"]["boxCollider"]["offset_x"].get_or(0.0),
						collider["components"]["boxCollider"]["offset_y"].get_or(0.0)));
			}
			// Trigger Component
			sol::optional<sol::table> trigger = collider["triggerComponent"];
			if (trigger != sol::nullopt)
			{
				newCollider.AddComponent<TriggerBoxComponent>(
					collider["triggerComponent"]["type"],
					glm::vec2(
						collider["triggerComponent"]["transport_offset_x"].get_or(0.0),
						collider["triggerComponent"]["transport_offset_y"].get_or(0.0)),
					collider["triggerComponent"]["level"]);
			}
		}
		i++;
	}
}

void LevelLoader::SavePlayerDataToLuaTable(std::string saveNum)
{
	LuaTableWriter m_writer;
	std::fstream file;
	
	file.open("./Assets/SavedFiles/save" + saveNum + ".lua");

	// Start the Lua Table
	m_writer.WriteStartDocument();

	// Write a comment in Lua format
	m_writer.WriteCommentSeparation(file);
	m_writer.WriteCommentLine("Save file#: " + saveNum, file);
	m_writer.WriteCommentSeparation(file);
	m_writer.WriteDeclareTable("player_data", file);

	// Get the player so we can get their stats
	auto entity = reg.GetEntityByTag("player");
	// Player Transform
	const auto& transform = entity.GetComponent<TransformComponent>();

	// Local variables that are based on the game Items
	bool hasBoomerang 			= game.GetGameItems().woodBoomerang;
	bool hasMagicBoomerang 		= game.GetGameItems().magicBoomerang;
	bool hasSword 				= game.GetGameItems().woodSword;
	bool hasSteelSword 			= game.GetGameItems().steelSword;
	bool hasMagicSword 			= game.GetGameItems().magicSword;
	bool hasMagicRod 			= game.GetGameItems().magicRod;
	bool hasBombs 				= game.GetGameItems().bombs;
	bool hasFood 				= game.GetGameItems().food;
	bool hasFlute 				= game.GetGameItems().flute;
	bool hasRaft 				= game.GetGameItems().raft;
	bool hasLadder 				= game.GetGameItems().ladder;
	bool hasBlueRing 			= game.GetGameItems().blueRing;
	bool hasRedRing 			= game.GetGameItems().redRing;
	bool hasBow 				= game.GetGameItems().bow;
	bool hasMagicBow 			= game.GetGameItems().magicBow;
	bool hasShield 				= game.GetGameItems().shield;
	bool hasMagicShield 		= game.GetGameItems().magicShield;
	bool hasPowerBraclet 		= game.GetGameItems().powerBraclet;
	bool hasMap 				= game.GetGameItems().map;
	bool hasBluePotion 			= game.GetGameItems().bluePotion;
	bool hasRedPotion 			= game.GetGameItems().redPotion;
	bool hasMasterKey 			= game.GetGameItems().masterKey;
	
	int numRupees 				= GameState::totalRupees;
	int numBombs 				= GameState::totalBombs;
	int numArrows 				= 10; // This needs to be changed 
	int numHearts 				= RenderHealthSystem::numHearts;
	int numKeys 				= GameState::totalKeys;
	int triforcePieces 			= 0;
	
	std::string name = "";
	
	// Start a new lua table
	m_writer.WriteStartTable(1, false, file);

	// Get the name of the player to save to the File --> Add this to the game class
	if (saveNum == "1") name = MenuState::player1Name;
	if (saveNum == "2") name = MenuState::player2Name;
	if (saveNum == "3") name = MenuState::player3Name;
	
	
	m_writer.WriteDeclareTable("menu_shared_values", file);
	m_writer.WriteKeyAndQuotedValue("name", name, file);
	m_writer.WriteKeyAndUnquotedValue("num_hearts", numHearts, file);
	
	if (hasBlueRing) m_writer.WriteKeyAndUnquotedValue("blue_ring", "true", file);
	else m_writer.WriteKeyAndUnquotedValue("blue_ring", "false", file);
	if (hasRedRing) m_writer.WriteKeyAndUnquotedValue("red_ring", "true", file);
	else m_writer.WriteKeyAndUnquotedValue("red_ring", "false", file);
	m_writer.WriteEndTable(false, file);

	// Write the players components --> Transform is all that really matters
	m_writer.WriteDeclareTable("components", file);
	m_writer.WriteDeclareTable("transform", file);
	m_writer.WriteDeclareTable("position", file);
	m_writer.WriteKeyAndValue('x', transform.position.x, false, file);
	m_writer.WriteKeyAndValue('y', transform.position.y, true, file);
	m_writer.WriteEndTable(true, file);
	m_writer.WriteEndTable(true, file);
	m_writer.WriteEndTable(false, file);
	
	// Declare a new table for the player's items
	m_writer.WriteDeclareTable("items", file);

	// Write if we currently have these items in our inventory
	// Boomerang
	if (hasBoomerang) 
		m_writer.WriteKeyAndUnquotedValue("boomerang", "true", file);
	else 
		m_writer.WriteKeyAndUnquotedValue("boomerang", "false", file);
	// Magic Boomerang
	if (hasMagicBoomerang) 
		m_writer.WriteKeyAndUnquotedValue("magic_boomerang", "true", file);
	else 
		m_writer.WriteKeyAndUnquotedValue("magic_boomerang", "false", file);
	// Wood Sword
	if (hasSword) 
		m_writer.WriteKeyAndUnquotedValue("wood_sword", "true", file);
	else 
		m_writer.WriteKeyAndUnquotedValue("wood_sword", "false", file);
	// Steel Sword
	if (hasSteelSword) 
		m_writer.WriteKeyAndUnquotedValue("steel_sword", "true", file);
	else 
		m_writer.WriteKeyAndUnquotedValue("steel_sword", "false", file);
	// Magic Sword
	if (hasMagicSword) 
		m_writer.WriteKeyAndUnquotedValue("magic_sword", "true", file);
	else 
		m_writer.WriteKeyAndUnquotedValue("magic_sword", "false", file);
	// Magic Rod
	if (hasMagicRod) 
		m_writer.WriteKeyAndUnquotedValue("magic_rod", "true", file);
	else 
		m_writer.WriteKeyAndUnquotedValue("magic_rod", "false", file);
	// Bombs
	if (hasBombs)
		m_writer.WriteKeyAndUnquotedValue("bombs", "true", file);
	else 
		m_writer.WriteKeyAndUnquotedValue("bombs", "false", file);
	// Food
	if (hasFood) 
		m_writer.WriteKeyAndUnquotedValue("food", "true", file);
	else 
		m_writer.WriteKeyAndUnquotedValue("food", "false", file);
	// Flute
	if (hasFlute)
		m_writer.WriteKeyAndUnquotedValue("flute", "true", file);
	else 
		m_writer.WriteKeyAndUnquotedValue("flute", "false", file);
	// Raft
	if (hasRaft)
		m_writer.WriteKeyAndUnquotedValue("raft", "true", file);
	else 
		m_writer.WriteKeyAndUnquotedValue("raft", "false", file);
	// Ladder
	if (hasLadder) 
		m_writer.WriteKeyAndUnquotedValue("ladder", "true", file);
	else 
		m_writer.WriteKeyAndUnquotedValue("ladder", "false", file);
	// Bow
	if (hasBow) 
		m_writer.WriteKeyAndUnquotedValue("bow_wood", "true", file);
	else 
		m_writer.WriteKeyAndUnquotedValue("bow_wood", "false", file);
	// Magic Shield
	if (hasShield)
		m_writer.WriteKeyAndUnquotedValue("magic_shield", "true", file);
	else
		m_writer.WriteKeyAndUnquotedValue("magic_shield", "false", file);
	// Power Braclet
	if (hasPowerBraclet)
		m_writer.WriteKeyAndUnquotedValue("power_braclet", "true", file);
	else 
		m_writer.WriteKeyAndUnquotedValue("power_braclet", "false", file);
	// Map
	if (hasMap)
		m_writer.WriteKeyAndUnquotedValue("map", "true", file);
	else
		m_writer.WriteKeyAndUnquotedValue("map", "false", file);
	
	// Blue Potion
	if (hasBluePotion) 
		m_writer.WriteKeyAndUnquotedValue("blue_potion", "true", file);
	else 
		m_writer.WriteKeyAndUnquotedValue("blue_potion", "false", file);
	// Red Potion
	if (hasRedPotion)
		m_writer.WriteKeyAndUnquotedValue("red_potion", "true", file);
	else 
		m_writer.WriteKeyAndUnquotedValue("red_potion", "false", file);
	// Master Key
	if (hasMasterKey)
		m_writer.WriteKeyAndUnquotedValue("master_key", "true", file);
	else
		m_writer.WriteKeyAndUnquotedValue("master_key", "false", file);
	
	// Close the items table
	m_writer.WriteEndTable(true, file);

	// Write the numbered values of current inventory and life hearts
	m_writer.WriteDeclareTable("inventory", file);
	m_writer.WriteKeyAndUnquotedValue("num_rupees", numRupees, file);
	m_writer.WriteKeyAndUnquotedValue("num_bombs", numBombs, file);
	m_writer.WriteKeyAndUnquotedValue("num_arrows", numArrows, file);
	m_writer.WriteKeyAndUnquotedValue("num_keys", numKeys, file);
	//m_writer.WriteKeyAndUnquotedValue("num_hearts", numHearts, file);
	m_writer.WriteEndTable(false, file);
	m_writer.WriteEndTable(false, file);
	m_writer.WriteEndTable(false, file);

	file.close();
}

// This is for a new player -->Default values for items
void LevelLoader::SavePlayerNameToLuaTable(std::string saveNum, std::string& newName)
{	
	LuaTableWriter m_writer;
	
	std::ofstream newFile ("./Assets/SavedFiles/save" + saveNum + ".lua");
	newFile.close();

	std::fstream file;

	file.open("./Assets/SavedFiles/save" + saveNum + ".lua");

	// Start the Lua Table
	m_writer.WriteStartDocument();

	// Write a comment in Lua format
	m_writer.WriteCommentSeparation(file);
	m_writer.WriteCommentLine("Save file#: " + saveNum, file);
	m_writer.WriteCommentSeparation(file);
	m_writer.WriteDeclareTable("player_data", file);

	int numRupees 			= 0;
	int numBombs 			= 0;
	int numArrows 			= 0;
	int numHearts 			= 3;
	int numKeys 			= 0;
	int triforcePieces 		= 0;
	
	std::string name 		= newName;
	m_writer.WriteStartTable(1, false, file);

	m_writer.WriteDeclareTable("menu_shared_values", file);
	m_writer.WriteKeyAndQuotedValue("name", name, file);
	m_writer.WriteKeyAndUnquotedValue("num_hearts", numHearts, file);
	
	// Write Blue Ring --> Set to false
	m_writer.WriteKeyAndUnquotedValue("blue_ring", "false", file);
	// Write Red Ring  --> Set to false
	m_writer.WriteKeyAndUnquotedValue("red_ring", "false", file);
	m_writer.WriteEndTable(false, file);
	
	// Start new table for components --> Only care about the transform here
	m_writer.WriteDeclareTable("components", file);
	m_writer.WriteDeclareTable("transform", file);
	m_writer.WriteDeclareTable("position", file);

	// Write the default player start position
	m_writer.WriteKeyAndValue('x', 7615, false, file);
	m_writer.WriteKeyAndValue('y', 5060, true, file);
	
	// Close the above tables
	m_writer.WriteEndTable(true, file);
	m_writer.WriteEndTable(true, file);
	m_writer.WriteEndTable(false, file);

	// Declare a new Table "items"
	m_writer.WriteDeclareTable("items", file);

	// boomerang
	m_writer.WriteKeyAndUnquotedValue("boomerang", "false", file);
	// magic boomerang
	m_writer.WriteKeyAndUnquotedValue("magic_boomerang", "false", file);
	// Wood Sword
	m_writer.WriteKeyAndUnquotedValue("wood_sword", "false", file);
	// Steel Sword
	m_writer.WriteKeyAndUnquotedValue("steel_sword", "false", file);
	// Magic Sword
	m_writer.WriteKeyAndUnquotedValue("magic_sword", "false", file);
	// Magic Rod
	m_writer.WriteKeyAndUnquotedValue("magic_rod", "false", file);
	// Bomgs
	m_writer.WriteKeyAndUnquotedValue("bombs", "false", file);
	// Food
	m_writer.WriteKeyAndUnquotedValue("food", "false", file);
	// Magic Flute
	m_writer.WriteKeyAndUnquotedValue("flute", "false", file);
	// Raft
	m_writer.WriteKeyAndUnquotedValue("raft", "false", file);
	// Ladder
	m_writer.WriteKeyAndUnquotedValue("ladder", "false", file);
	// Bow
	m_writer.WriteKeyAndUnquotedValue("bow_wood", "false", file);
	// Magic Shield 
	m_writer.WriteKeyAndUnquotedValue("magic_shield", "false", file);
	// Power Braclet
	m_writer.WriteKeyAndUnquotedValue("power_braclet", "false", file);
	// Map
	m_writer.WriteKeyAndUnquotedValue("map", "false", file);
	// Blue Potion
	m_writer.WriteKeyAndUnquotedValue("blue_potion", "false", file);
	// Red Potion
	m_writer.WriteKeyAndUnquotedValue("red_potion", "false", file);
	// Master Key
	m_writer.WriteKeyAndUnquotedValue("master_key", "false", file);
	// Write the end of the items table
	m_writer.WriteEndTable(true, file);

	// Write the numbered values of current inventory and life hearts
	m_writer.WriteDeclareTable("inventory", file);
	m_writer.WriteKeyAndUnquotedValue("num_rupees", numRupees, file);
	m_writer.WriteKeyAndUnquotedValue("num_bombs", numBombs, file);
	m_writer.WriteKeyAndUnquotedValue("num_arrows", numArrows, file);
	m_writer.WriteKeyAndUnquotedValue("num_keys", numKeys, file);
	// Close the remaining tables
	m_writer.WriteEndTable(false, file);
	m_writer.WriteEndTable(false, file);
	m_writer.WriteEndTable(false, file);
	
	// Close the new lua file
	file.close();
}


void LevelLoader::CreatePlayerEntityFromLuaTable(sol::state& lua, std::string fileName)
{
	sol::load_result script = lua.load_file("./Assets/LuaFiles/" + fileName + ".lua");

	// This checks the syntax of our script, but it does not execute the script
	if (!script.valid())
	{
		sol::error err = script;
		std::string errorMessage = err.what();
		Logger::Err("Error loading the Lua Script: " + errorMessage);
		return;
	}

	// Executes the script using the sol State
	lua.script_file("./Assets/LuaFiles/" + fileName + ".lua");

	sol::table data = lua["player"];

	int i = 1;

	while (true)
	{
		sol::optional<sol::table> hasData = data[i];
		if (hasData == sol::nullopt)
		{
			break;
		}
		sol::table player = data[i];

		auto entity = reg.CreateEntity();

		entity.Tag(player["tag"]);

		// Components
		sol::optional<sol::table> hasComponents = player["components"];
		if (hasComponents != sol::nullopt)
		{
			// Transform Component
			sol::optional<sol::table> transformComp = player["components"]["transform"];
			if (transformComp != sol::nullopt)
			{
				entity.AddComponent<TransformComponent>(
					glm::vec2(
						player["components"]["transform"]["position"]["x"],
						player["components"]["transform"]["position"]["y"]
					),
					glm::vec2(
						player["components"]["transform"]["scale"]["x"].get_or(1.0),
						player["components"]["transform"]["scale"]["y"].get_or(1.0)),
						player["components"]["transform"]["rotation"].get_or(0.0));
			}

			// Box Collider Component
			sol::optional<sol::table> boxCollider = player["components"]["box_collider"];
			if (boxCollider != sol::nullopt)
			{
				//Logger::Err("Has Box Collider Component");
				entity.AddComponent<BoxColliderComponent>(
					player["components"]["box_collider"]["width"],
					player["components"]["box_collider"]["height"],
					glm::vec2(
						player["components"]["box_collider"]["offset_x"].get_or(0.0),
						player["components"]["box_collider"]["offset_y"].get_or(0.0)));
			}

			// Rigid Body Component
			sol::optional<sol::table> rigidBody = player["components"]["rigidbody"];
			if (rigidBody != sol::nullopt)
			{
				entity.AddComponent<RigidBodyComponent>(
					glm::vec2(
						player["components"]["rigidbody"]["velocity"]["x"].get_or(0),
						player["components"]["rigidbody"]["velocity"]["y"].get_or(0)));
			}
			// Health Component
			sol::optional<sol::table> health = player["components"]["health"];
			if (health != sol::nullopt)
			{
				entity.AddComponent<HealthComponent>(
					player["components"]["health"]["health_percentage"].get_or(9),
					player["components"]["health"]["max_hearts"].get_or(3));
			}
			// Sprite
			sol::optional<sol::table> sprite = player["components"]["sprite"];
			if (sprite != sol::nullopt)
			{
				entity.AddComponent<SpriteComponent>(
					player["components"]["sprite"]["asset_id"],
					player["components"]["sprite"]["width"],
					player["components"]["sprite"]["height"],
					player["components"]["sprite"]["z_index"].get_or(1), // Layer
					player["components"]["sprite"]["is_fixed"].get_or(false),
					player["components"]["sprite"]["src_rect_x"].get_or(0),
					player["components"]["sprite"]["src_rect_y"].get_or(0),
					glm::vec2(
						player["components"]["sprite"]["offset"]["x"].get_or(0),
						player["components"]["sprite"]["offset"]["y"].get_or(0)));
			}
			// Animation  
			sol::optional<sol::table> animation = player["components"]["animation"];
			if (animation != sol::nullopt)
			{
				entity.AddComponent<AnimationComponent>(
					player["components"]["animation"]["num_frames"].get_or(1),
					player["components"]["animation"]["frame_rate"].get_or(1),
					player["components"]["animation"]["vertical"].get_or(false),
					player["components"]["animation"]["looped"].get_or(false),
					player["components"]["animation"]["frame_offset"].get_or(0));
			}
			// Projectile Emitter  
			sol::optional<sol::table> projectile = player["components"]["projectile_emitter"];
			if (projectile != sol::nullopt)
			{
				entity.AddComponent<ProjectileEmitterComponent>(
					glm::vec2(
						player["components"]["projectile_emitter"]["velocity"]["x"].get_or(0),
						player["components"]["projectile_emitter"]["velocity"]["y"].get_or(0)
					),
					player["components"]["projectile_emitter"]["repeat_frequency"].get_or(0),
					player["components"]["projectile_emitter"]["projectile_duration"].get_or(10000),
					player["components"]["projectile_emitter"]["hit_percent_damage"].get_or(10),
					player["components"]["projectile_emitter"]["is_friendly"].get_or(false)
					);
			}
			// Keyboard Control Component 
			sol::optional<sol::table> keyboard = player["components"]["keyboard_control"];
			if (keyboard != sol::nullopt)
			{
				entity.AddComponent<KeyboardControlComponent>(
					glm::vec2(
						player["components"]["keyboard_control"]["up_velocity"]["x"].get_or(0),
						player["components"]["keyboard_control"]["up_velocity"]["y"].get_or(0)
					),
					glm::vec2(
						player["components"]["keyboard_control"]["right_velocity"]["x"].get_or(0),
						player["components"]["keyboard_control"]["right_velocity"]["y"].get_or(0)
					),
					glm::vec2(
						player["components"]["keyboard_control"]["down_velocity"]["x"].get_or(0),
						player["components"]["keyboard_control"]["down_velocity"]["y"].get_or(0)
					),
					glm::vec2(
						player["components"]["keyboard_control"]["left_velocity"]["x"].get_or(0),
						player["components"]["keyboard_control"]["left_velocity"]["y"].get_or(0)
					));
			}

			if (entity.HasTag("player"))
			{
				entity.AddComponent<CameraFollowComponent>();
				entity.AddComponent<PlayerComponent>();
			}
			entity.AddComponent<GameComponent>();
		}
		i++;
	}
}

void LevelLoader::LoadPlayerDataFromLuaTable(sol::state& lua, std::string fileName)
{
	sol::load_result script = lua.load_file("./Assets/SavedFiles/" + fileName + ".lua");

	// This checks the syntax of our script, but it does not execute the script
	if (!script.valid())
	{
		sol::error err = script;
		std::string errorMessage = err.what();
		Logger::Err("Error loading the Lua Script: " + errorMessage);
		return;
	}

	// Executes the script using the sol State
	lua.script_file("./Assets/SavedFiles/" + fileName + ".lua");

	sol::table data = lua["player_data"];

	int i = 1;

	while (true)
	{
		auto entity = reg.GetEntityByTag("player");

		auto& transform = entity.GetComponent<TransformComponent>();

		sol::optional<sol::table> hasData = data[i];
		if (hasData == sol::nullopt)
		{
			break;
		}
		sol::table player = data[i];

		// Menu Shared Values and Items
		sol::optional<sol::table> menuSharedValues = player["menu_shared_values"];
		if (menuSharedValues != sol::nullopt)
		{
			RenderHealthSystem::loadedHearts = player["menu_shared_values"]["num_hearts"].get_or(3);
		}
		// Components
		sol::optional<sol::table> hasComponents = player["components"];
		if (hasComponents != sol::nullopt)
		{
			// Transform Component
			sol::optional<sol::table> transformComp = player["components"]["transform"];
			if (transformComp != sol::nullopt)
			{
				transform.position.x = player["components"]["transform"]["position"]["x"];
				transform.position.y = player["components"]["transform"]["position"]["y"];
			}
		}
		// Items
		sol::optional<sol::table> hasItems = player["items"];
		if (hasItems != sol::nullopt)
		{
			game.GetGameItems().woodBoomerang 	= player["items"]["boomerang"].get_or(false);
			game.GetGameItems().magicBoomerang 	= player["items"]["magic_boomerang"].get_or(false);
			game.GetGameItems().woodSword 		= player["items"]["wood_sword"].get_or(false);
			game.GetGameItems().steelSword 		= player["items"]["steel_sword"].get_or(false);
			game.GetGameItems().magicSword 		= player["items"]["magic_sword"].get_or(false);
			game.GetGameItems().magicRod		= player["items"]["magic_rod"].get_or(false);
			game.GetGameItems().bombs 			= player["items"]["bombs"].get_or(false);
			game.GetGameItems().food 			= player["items"]["food"].get_or(false);
			game.GetGameItems().flute 			= player["items"]["flute"].get_or(false);
			game.GetGameItems().raft 			= player["items"]["raft"].get_or(false);
			game.GetGameItems().ladder 			= player["items"]["ladder"].get_or(false);
			game.GetGameItems().bow 			= player["items"]["bow_wood"].get_or(false);
			game.GetGameItems().magicBow 		= player["items"]["bow_magic"].get_or(false);
			game.GetGameItems().shield 			= player["items"]["shield"].get_or(false);
			game.GetGameItems().magicShield 	= player["items"]["magic_shield"].get_or(false);
			game.GetGameItems().powerBraclet 	= player["items"]["power_braclet"].get_or(false);
			game.GetGameItems().map 			= player["items"]["map"].get_or(false);
			game.GetGameItems().bluePotion 		= player["items"]["blue_potion"].get_or(false);
			game.GetGameItems().redPotion 		= player["items"]["red_potion"].get_or(false);
			game.GetGameItems().masterKey 		= player["items"]["master_key"].get_or(false);
		}
		// Inventory
		sol::optional<sol::table> inventory = player["inventory"];
		if (inventory != sol::nullopt)
		{
			GameState::totalRupees 				= player["inventory"]["num_rupees"].get_or(0);
			GameState::totalBombs 				= player["inventory"]["num_bombs"].get_or(0);
			GameState::totalKeys 				= player["inventory"]["num_keys"].get_or(0);
		}
		i++;
		Logger::Log("Player Loaded");
	}
}

void LevelLoader::LoadEnemiesFromLuaTable(sol::state& lua, std::string fileName)
{
	sol::load_result script = lua.load_file("./Assets/Levels/" + fileName + ".lua");
	// This checks the syntax of our script, but it does not execute the script
	if (!script.valid())
	{
		sol::error err = script;
		std::string errorMessage = err.what();
		Logger::Err("Error loading the Lua Script: " + errorMessage);
		return;
	}

	// Executes the script using the sol State
	lua.script_file("./Assets/Levels/" + fileName + ".lua");

	sol::table entities = lua["enemies"];

	int i = 1;

	while (true)
	{
		sol::optional<sol::table> hasEntity = entities[i];

		if (hasEntity == sol::nullopt)
		{
			break;
		}

		sol::table entity = entities[i];

		Entity newEntity = reg.CreateEntity();

		// Group 
		newEntity.Group(entity["group"]);

		// Components
		sol::optional<sol::table> hasComponents = entity["components"];
		if (hasComponents != sol::nullopt)
		{
			// Transform
			sol::optional<sol::table> transform = entity["components"]["transform"];
			if (transform != sol::nullopt)
			{
				newEntity.AddComponent<TransformComponent>(
					glm::vec2(
						entity["components"]["transform"]["position"]["x"],
						entity["components"]["transform"]["position"]["y"]
					),
					glm::vec2(
						entity["components"]["transform"]["scale"]["x"].get_or(1.0),
						entity["components"]["transform"]["scale"]["y"].get_or(1.0)
					),
					entity["components"]["transform"]["rotation"].get_or(0.0)
					);
			}
			// RigidBody
			sol::optional<sol::table> rigidBody = entity["components"]["rigidbody"];
			if (rigidBody != sol::nullopt)
			{
				newEntity.AddComponent<RigidBodyComponent>(
					glm::vec2(
						entity["components"]["rigidbody"]["velocity"]["x"].get_or(0),
						entity["components"]["rigidbody"]["velocity"]["y"].get_or(0)
					)
					);
			}
			// Sprite
			sol::optional<sol::table> sprite = entity["components"]["sprite"];
			if (sprite != sol::nullopt)
			{
				newEntity.AddComponent<SpriteComponent>(
					entity["components"]["sprite"]["asset_id"],
					entity["components"]["sprite"]["width"],
					entity["components"]["sprite"]["height"],
					entity["components"]["sprite"]["z_index"].get_or(1), // Layer
					entity["components"]["sprite"]["is_fixed"].get_or(false),
					entity["components"]["sprite"]["src_rect_x"].get_or(0),
					entity["components"]["sprite"]["src_rect_y"].get_or(0),
					glm::vec2(
						entity["components"]["sprite"]["offset"]["x"].get_or(0),
						entity["components"]["sprite"]["offset"]["y"].get_or(0)
					)
					);
			}
			// Animation  numFrames, frameSpeedRate, vertical, isLooped ,frameOffset
			sol::optional<sol::table> animation = entity["components"]["animation"];
			if (animation != sol::nullopt)
			{
				newEntity.AddComponent<AnimationComponent>(
					entity["components"]["animation"]["num_frames"].get_or(1),
					entity["components"]["animation"]["frame_rate"].get_or(1),
					entity["components"]["animation"]["vertical"].get_or(false),
					entity["components"]["animation"]["looped"].get_or(false),
					entity["components"]["animation"]["frame_offset"].get_or(0)
					);
			}
			// Health Component int healthPercentage = 9, int maxHearts = 3
			sol::optional<sol::table> health = entity["components"]["health"];
			if (health != sol::nullopt)
			{
				newEntity.AddComponent<HealthComponent>(
					entity["components"]["health"]["health_percentage"].get_or(100),
					entity["components"]["health"]["max_hearts"].get_or(3)
					);
			}
			// Box Collider width, height, glm::vec2 offset 
			sol::optional<sol::table> box_collider = entity["components"]["box_collider"];
			if (box_collider != sol::nullopt)
			{
				newEntity.AddComponent<BoxColliderComponent>(
					entity["components"]["box_collider"]["width"].get_or(16),
					entity["components"]["box_collider"]["height"].get_or(16),
					glm::vec2(
						entity["componets"]["box_collider"]["offset_x"].get_or(0),
						entity["componets"]["box_collider"]["offset_y"].get_or(0)
					));
			}
			// Projectile Emitter  
			sol::optional<sol::table> projectile = entity["components"]["projectile_emitter"];
			if (projectile != sol::nullopt)
			{
				newEntity.AddComponent<ProjectileEmitterComponent>(
					glm::vec2(
						entity["components"]["projectile_emitter"]["velocity"]["x"].get_or(0),
						entity["components"]["projectile_emitter"]["velocity"]["y"].get_or(0)
					),
					entity["components"]["projectile_emitter"]["repeat_frequency"].get_or(0),
					entity["components"]["projectile_emitter"]["projectile_duration"].get_or(10000),
					entity["components"]["projectile_emitter"]["hit_percent_damage"].get_or(1),
					entity["components"]["projectile_emitter"]["is_friendly"].get_or(false)
					);
			}

			// Script Component --> Not used at the moment --> Using c++ State machine --> will try to change to lua state machine
			sol::optional<sol::table> script = entity["components"]["on_update_script"];
			if (script != sol::nullopt)
			{
				//Logger::Log("HERE");
				sol::function func = entity["components"]["on_update_script"][0];
				newEntity.AddComponent<ScriptComponent>(func);
			}
			 
			// AI Component
			sol::optional<sol::table> ai = entity["components"]["ai_component"];
			if (ai != sol::nullopt)
			{
				AIComponent::EnemyType type = ConvertStringToEnemyType(entity["components"]["ai_component"]["enemy_type"]);

				newEntity.AddComponent<AIComponent>(
					glm::vec2(
						entity["components"]["ai_component"]["enemy_pos"]["x"].get_or(0),
						entity["components"]["ai_component"]["enemy_pos"]["y"].get_or(0)),
					type
					);
			}
			newEntity.AddComponent<GameComponent>();
			newEntity.AddComponent<EnemyComponent>();
		}
		i++;
	}
}

void LevelLoader::LoadHUDFromLuaTable(sol::state& lua, std::string fileName)
{
	sol::load_result script = lua.load_file("./Assets/LuaFiles/" + fileName + ".lua");

	// This checks the syntax of our script, but it does not execute the script
	if (!script.valid())
	{
		sol::error err = script;
		std::string errorMessage = err.what();
		Logger::Err("Error loading the Lua Script: " + errorMessage);
		return;
	}

	// Executes the script using the sol State
	lua.script_file("./Assets/LuaFiles/" + fileName + ".lua");

	sol::table data = lua["hud"];

	int i = 1;

	while (true)
	{
		sol::optional<sol::table> hasData = data[i];
		if (hasData == sol::nullopt)
		{
			break;
		}

		sol::table hudData = data[i];

		Entity newHUDObject = reg.CreateEntity();

		// Add tag if there is one
		sol::optional<std::string> tag = hudData["tag"];
		if (tag != sol::nullopt)
		{
			newHUDObject.Tag(hudData["tag"]);
			std::string temp = hudData["tag"];

			if (temp == "heart1")
				newHUDObject.AddComponent<HealthComponent>(0);
			else if (temp == "heart2")
				newHUDObject.AddComponent<HealthComponent>(0);
			else if (temp == "heart3")
				newHUDObject.AddComponent<HealthComponent>(0);
		}

		// Add Group if there is one
		sol::optional<std::string> group = hudData["group"];
		if (group != sol::nullopt)
		{
			newHUDObject.Group(hudData["group"]);
		}

		// Add Object/Entity Components
		sol::optional<sol::table> hasComponents = hudData["components"];
		if (hasComponents != sol::nullopt)
		{
			// Transform Component
			sol::optional<sol::table> transform = hudData["components"]["transform"];
			if (transform != sol::nullopt)
			{
				newHUDObject.AddComponent<TransformComponent>(
					glm::vec2(
						hudData["components"]["transform"]["position"]["x"].get_or(0),
						hudData["components"]["transform"]["position"]["y"].get_or(0)
					),
					glm::vec2(
						hudData["components"]["transform"]["scale"]["x"].get_or(1.0),
						hudData["components"]["transform"]["scale"]["y"].get_or(1.0)
					),
					hudData["components"]["transform"]["rotation"].get_or(0.0));
			}
			// Add Sprite Component
			sol::optional<sol::table> sprite = hudData["components"]["sprite"];
			if (sprite != sol::nullopt)
			{
				newHUDObject.AddComponent<SpriteComponent>(
					hudData["components"]["sprite"]["assetID"],
					hudData["components"]["sprite"]["width"],
					hudData["components"]["sprite"]["height"],
					hudData["components"]["sprite"]["layer"],
					hudData["components"]["sprite"]["fixed"],
					hudData["components"]["sprite"]["src_rect_x"].get_or(0),
					hudData["components"]["sprite"]["src_rect_y"].get_or(0)
					);
			}
			newHUDObject.AddComponent<HUDComponenet>();
		}
		i++;
	}
}

void LevelLoader::LoadAssetsFromLuaTable(sol::state& lua, std::string fileName)
{
	sol::load_result script = lua.load_file("./Assets/LuaFiles/" + fileName + ".lua");

	// This checks the syntax of our script, but it does not execute the script
	if (!script.valid())
	{
		sol::error err = script;
		std::string errorMessage = err.what();
		Logger::Err("Error loading the Lua Script: " + errorMessage);
		return;
	}

	// Executes the script using the sol State
	lua.script_file("./Assets/LuaFiles/" + fileName + ".lua");

	sol::table assets = lua["Assets"];

	int i = 0;

	while (true)
	{
		sol::optional<sol::table> hasAssets = assets[i];
		if (hasAssets == sol::nullopt)
		{
			break;
		}

		sol::table asset = assets[i];
		std::string assetType = asset["type"];
		std::string assetID = asset["id"];

		if (assetType == "texture")
		{
			if (!game.GetAssetManager()->HasTexture(assetID))
				game.GetAssetManager()->AddTextures(game.GetRenderer(), assetID, asset["file"]);
		}

		if (assetType == "font")
		{
			if (!game.GetAssetManager()->HasFont(assetID))
				game.GetAssetManager()->AddFonts(assetID, asset["file"], asset["font_size"]);
		}

		if (assetType == "soundFX")
		{
			if (!game.GetAssetManager()->HasSoundFX(assetID))
				game.GetAssetManager()->AddSoundFX(assetID, asset["file"]);
		}

		if (assetType == "music")
		{
			if (!game.GetAssetManager()->HasMusic(assetID))
				game.GetAssetManager()->AddMusic(assetID, asset["file"]);
		}
		i++;
	}
}

SpecialItemType LevelLoader::ConvertLuaStringToSpecial(std::string& special)
{
	if (special == "none")
		return SpecialItemType::NOT_SPECIAL;
	else if (special == "wood_sword")
		return SpecialItemType::WOOD_SWORD;
	else if (special == "steel_sword")
		return SpecialItemType::STEEL_SWORD;
	else if (special == "magic_sword")
		return SpecialItemType::MAGIC_SWORD;
	else if (special == "full_heart")
		return SpecialItemType::FULL_HEART;
	else if (special == "raft")
		return SpecialItemType::RAFT;
	else if (special == "power_braclet")
		return SpecialItemType::POWER_BRACLET;
	else if (special == "red_candle")
		return SpecialItemType::RED_CANDLE;
	else if (special == "wood_boomerang")
		return SpecialItemType::WOOD_BOOMERANG;
	else if (special == "magic_boomerang")
		return SpecialItemType::MAGIC_BOOMERANG;
	else if (special == "ladder")
		return SpecialItemType::LADDER;
	else
		return SpecialItemType::NOT_SPECIAL;
}

bool LevelLoader::CheckForItemInInventory(SpecialItemType& type)
{
	switch (type)
	{
	case SpecialItemType::WOOD_SWORD:
		if (game.GetGameItems().woodSword)
			return true;
		else
			return false;
		break;
	case SpecialItemType::LADDER:
		if (game.GetGameItems().ladder)
			return true;
		else
			return false;
		break;
	case SpecialItemType::WOOD_BOOMERANG:
		if (game.GetGameItems().woodBoomerang)
			return true;
		else
			return false;
		break;
	case SpecialItemType::MAGIC_BOOMERANG:
		if (game.GetGameItems().magicBoomerang)
			return true;
		else
			return false;
		break;
	case SpecialItemType::STEEL_SWORD:
		if (game.GetGameItems().steelSword)
			return true;
		else
			return false;
		break;
	default:
		return false;
	}
}

void LevelLoader::LoadEntitiesFromLuaTable(sol::state& lua, std::string filename)
{
	sol::load_result script = lua.load_file("./Assets/Levels/" + filename + ".lua");

	// This checks the syntax of our script, but it does not execute the script
	if (!script.valid())
	{
		sol::error err = script;
		std::string errorMessage = err.what();
		Logger::Err("Error loading the Lua Script: " + errorMessage);
		return;
	}

	// Executes the script using the sol State
	lua.script_file("./Assets/Levels/" + filename + ".lua");

	sol::table data = lua["level"];

	int i = 1;

	while (true)
	{
		sol::optional<sol::table> hasData = data[i];
		if (hasData == sol::nullopt)
		{
			Logger::Err("No Loaded Entity data");
			break;
		}

		sol::table lvlData = data[i];

		// Check to see if the special item has been loaded, if it has break out!
		sol::optional<std::string> level_item = lvlData["level_item"];

		if (level_item != sol::nullopt)
		{
			std::string item = lvlData["level_item"];
			SpecialItemType spec = ConvertLuaStringToSpecial(item);

			if (CheckForItemInInventory(spec))
			{
				Logger::Err("Breaking");
				break;
			}
		}

		Entity newLvlObject = reg.CreateEntity();

		// Add tag if there is one
		sol::optional<std::string> tag = lvlData["tag"];
		if (tag != sol::nullopt)
		{
			newLvlObject.Tag(lvlData["tag"]);
		}

		// Add Group if there is one
		sol::optional<std::string> group = lvlData["group"];
		if (group != sol::nullopt)
		{
			newLvlObject.Group(lvlData["group"]);
		}

		// Add Object/Entity Components
		sol::optional<sol::table> hasComponents = lvlData["components"];
		if (hasComponents != sol::nullopt)
		{
			newLvlObject.AddComponent<GameComponent>();
			// Transform Component
			sol::optional<sol::table> transform = lvlData["components"]["transform"];
			if (transform != sol::nullopt)
			{
				newLvlObject.AddComponent<TransformComponent>(
					glm::vec2(
						lvlData["components"]["transform"]["position"]["x"].get_or(0),
						lvlData["components"]["transform"]["position"]["y"].get_or(0)
					),
					glm::vec2(
						lvlData["components"]["transform"]["scale"]["x"].get_or(1.0),
						lvlData["components"]["transform"]["scale"]["y"].get_or(1.0)
					),
					lvlData["components"]["transform"]["rotation"].get_or(0.0));
			}

			// Add Sprite Component
			sol::optional<sol::table> sprite = lvlData["components"]["sprite"];
			if (sprite != sol::nullopt)
			{
				newLvlObject.AddComponent<SpriteComponent>(
					lvlData["components"]["sprite"]["assetID"],
					lvlData["components"]["sprite"]["width"],
					lvlData["components"]["sprite"]["height"],
					lvlData["components"]["sprite"]["layer"],
					lvlData["components"]["sprite"]["fixed"],
					lvlData["components"]["sprite"]["src_rect_x"].get_or(0),
					lvlData["components"]["sprite"]["src_rect_y"].get_or(0)
					);
			}
			
			// !!! This is not working right now!! Causes issues when leaving moving to a new scene !!!
			
			// Add Animation Component
			sol::optional<sol::table> animation = lvlData["components"]["animation"];
			if (animation != sol::nullopt)
			{
				newLvlObject.AddComponent<AnimationComponent>(
					lvlData["components"]["animation"]["num_frames"].get_or(1),
					lvlData["components"]["animation"]["frame_rate"].get_or(1),
					lvlData["components"]["animation"]["vertical"].get_or(false),
					lvlData["components"]["animation"]["looped"].get_or(false),
					lvlData["components"]["animation"]["frame_offset"].get_or(0)
					);
			}

			// Box Collider width, height, glm::vec2 offset 
			sol::optional<sol::table> box_collider = lvlData["components"]["box_collider"];
			if (box_collider != sol::nullopt)
			{
				int val = lvlData["components"]["box_collider"]["offset"]["x"].get_or(0);

				newLvlObject.AddComponent<BoxColliderComponent>(
					lvlData["components"]["box_collider"]["width"].get_or(16),
					lvlData["components"]["box_collider"]["height"].get_or(16),
					glm::vec2(
						lvlData["components"]["box_collider"]["offset"]["x"].get_or(0),
						lvlData["components"]["box_collider"]["offset"]["y"].get_or(0)
					));
			}

			// Trigger Component
			sol::optional<sol::table> trigger_comp = lvlData["components"]["trigger"];
			if (trigger_comp != sol::nullopt)
			{
				auto type = ConvertToTriggerType(lvlData["components"]["trigger"]["trigger_type"].get_or(0));
				newLvlObject.AddComponent<TriggerBoxComponent>(type);
			}

			// Item Type Component
			sol::optional<sol::table> item_comp = lvlData["components"]["item"];
			if (item_comp != sol::nullopt)
			{
				std::string that = lvlData["components"]["item"]["special"];
				auto type = ItemCollectType::DEFAULT;
				auto special = ConvertLuaStringToSpecial(that);
	
				newLvlObject.AddComponent<ItemComponent>(type, special);
			
				if (CheckForItemInInventory(special))
				{
					newLvlObject.Kill();
				}
			}

			// Caption Component
			sol::optional<sol::table> caption = lvlData["components"]["caption"];
			if (caption != sol::nullopt)
			{
				newLvlObject.AddComponent<CaptionComponent>(
					lvlData["components"]["caption"]["captions"],
					lvlData["components"]["caption"]["scrollable"].get_or(true),
					lvlData["components"]["caption"]["numbers"].get_or(false),
					lvlData["components"]["caption"]["x_pos"].get_or(0),
					lvlData["components"]["caption"]["y_pos"].get_or(0),
					lvlData["components"]["caption"]["num_frames"].get_or(1),
					lvlData["components"]["caption"]["ones"].get_or(0),
					lvlData["components"]["caption"]["tens"].get_or(0),
					lvlData["components"]["caption"]["hundreds"].get_or(0)
					);
			}
		}
		i++;
	}
}

// This is a letter parser to deal with the SDL_Text issues
void LevelLoader::ConvertName(std::string name, int x, int y)
{
	int num = name.length();

	// Clamp Num at 7
	if (num > 7)
	{
		num = 7;
	}

	// Variable for spaces in between letters
	int space = 0;

	// Parse the string and set the srcRect position
	for (int i = 0; i < num; i++)
	{
		char letter = std::toupper(name[i]);

		Entity nameEnt = reg.CreateEntity();

		nameEnt.Group(name);
		nameEnt.AddComponent<SpriteComponent>("caption_letters", 16, 16, 0, true, 0, 0);
		nameEnt.AddComponent<TransformComponent>(glm::vec2(x, y), glm::vec2(1.5, 1.5), 0);
		nameEnt.AddComponent<MenuComponent>();
		
		/*
			Created a new sprite that allows me to make the code cleaner
			Code may have to be adjusted if the sprite changes
		*/

		// check to see if the char is part of the upper alphabet
		if (letter >= 65 && letter <= 97)
		{
			nameEnt.GetComponent<SpriteComponent>().srcRect.x = 16 * (letter - 65);
			nameEnt.GetComponent<SpriteComponent>().srcRect.y = 0;
			nameEnt.GetComponent<TransformComponent>().position.x += space;
		}
		// Create a space between the letters
		space += 24;
	}
}

