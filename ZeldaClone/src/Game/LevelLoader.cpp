#include "LevelLoader.h"
#include "../Components/AnimationComponent.h"
#include "../Components/RigidBodyComponent.h"
#include "../Components/SpriteComponent.h"
#include "../Components/TransformComponent.h"
#include "../Components/HealthComponent.h"
#include "../Components/BoxColliderComponent.h"
#include "../Components/TriggerBoxComponent.h"
#include "../Components/HUDComponent.h"
#include "../Components/MenuComponent.h"
#include "../Components/TileComponent.h"
#include "../Components/GameComponent.h"
#include "../Components/ScriptComponent.h"
#include "../Components/ProjectileEmitterComponent.h"
#include "../Components/CameraFollowComponent.h"
#include "../States/NameState.h"
#include "../States/MenuState.h"
#include "../States/GameState.h"
#include "../Game/Game.h"
#include "../Utilities/LuaTableWriter.h"
#include "../Systems/GameSystems/RenderHealthSystem.h"
#include <string>
#include <fstream>

LevelLoader::LevelLoader()
{
	Logger::Log("Level Loader Constructor run");
}

LevelLoader::~LevelLoader()
{
	Logger::Log("Level Loader Constructor run");
}

void LevelLoader::LoadLevelAssets(SDL_Renderer* renderer, std::unique_ptr<AssetManager>& assetManager, const std::string& fileName)
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

		//Entity newEntity = Registry::instance()->CreateEntity();
		Logger::Log(assetPath);
		switch (assetType)
		{
		case TEXTURE:
			assetManager->AddTextures(renderer, assetID, (path + assetPath));
			Logger::Log("Texture Added with ID " + assetID);
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

void LevelLoader::LoadHud(std::unique_ptr<AssetManager>& assetManager, SDL_Renderer* renderer)
{
	// Open and read the text file
	std::fstream hudBox;
	if (Game::Instance()->GetStateMachine()->GetCurrentState() == "GAMESTATE")
	{
		hudBox.open("./Assets/LevelAssets/hudBox.txt");
	}
	else if (Game::Instance()->GetStateMachine()->GetCurrentState() == "PAUSE")
	{
		hudBox.open("./Assets/LevelAssets/hudBoxPause.txt");
	}
	
	if (!hudBox.is_open())
	{
		Logger::Err("Unable to open file");
	}

	while (!hudBox.eof())
	{
		// Mapfile variables
		int srcRectX = 0;
		int srcRectY = 0;
		int transformX = 0;
		int transformY = 0;
		int tileWidth = 0;
		int tileHeight = 0;
		int tileScaleX = 0;
		int tileScaleY = 0;
		int zIndex = 0;
		size_t numLines = 0;
		bool vertical = false;
		bool horizontal = false;
		int xTransform = 0;
		int yTransform = 0;
		std::string group = "";
		glm::vec2 colliderOffset = glm::vec2(0);
		bool collider = false;
		std::string assetID = "";

		// If we are at the end of the File --> Leave the file!
		if (hudBox.eof()) break;
		// Start parsing the text file
		hudBox >> group >> assetID >> tileWidth >> tileHeight >> srcRectY >> srcRectX >> zIndex >> transformX >> transformY >> tileScaleX >> tileScaleY >> vertical >> horizontal >> numLines;

		for (size_t i = 0; i < numLines; i++)
		{
			if (i > 0 && horizontal) xTransform += 32;
			if (i > 0 && vertical) yTransform += 32;

			// Create new tile Entity for each parsed tile
			Entity tile = Registry::Instance()->CreateEntity();
			tile.Group(group);
			tile.AddComponent<SpriteComponent>(assetID, tileWidth, tileHeight, zIndex, true, srcRectX, srcRectY);
			tile.AddComponent<TransformComponent>(glm::vec2(transformX + xTransform, transformY + yTransform), glm::vec2(tileScaleX, tileScaleY), 0.0);

			if (Game::Instance()->GetStateMachine()->GetCurrentState() == "GAMESTATE")
			{
				tile.Group("game");
			}

			else if (Game::Instance()->GetStateMachine()->GetCurrentState() == "PAUSE")
			{
				tile.Group("pause");
			}
		}
	} hudBox.close();

	std::fstream hudHearts;
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

void LevelLoader::LoadMenuScreen(std::unique_ptr<AssetManager>& assetManager, SDL_Renderer* renderer, unsigned int& slotNum)
{
	// Load Assets the the assetManager

	// Load the Tilemap
	int tileSize = 16;
	int tileScale = 4;

	// Open and read the thext file
	std::fstream menufile;
	menufile.open("./Assets/LevelAssets/menuFiles" + std::to_string(slotNum) + ".txt");

	if (!menufile.is_open())
	{
		Logger::Err("Unable to open file");
	}

	while (!menufile.eof())
	{
		// Mapfile variables
		int srcRectX = 0;
		int srcRectY = 0;
		int transformX = 0;
		int transformY = 0;
		int tileWidth = 0;
		int tileHeight = 0;
		int tileScaleX = 0;
		int tileScaleY = 0;
		int zIndex = 0;
		size_t numHearts = 0;
		int xTransform = 0;
		int yTransform = 0;
		std::string group = "";
		glm::vec2 colliderOffset = glm::vec2(0);
		bool collider = false;
		std::string assetID = "";

		// If we are at the end of the File --> Leave the file!
		if (menufile.eof()) break;
		// Start parsing the text file
		menufile >> group >> assetID >> tileWidth >> tileHeight >> srcRectY >> srcRectX >> zIndex >> transformX >> transformY >> tileScaleX >> tileScaleY >> numHearts;

		for (size_t i = 0; i < numHearts; i++)
		{
			if (i > 0) xTransform += 32;
			if (i == 8)
			{
				yTransform += 32;
				xTransform = 0;
			}

			// Create new tile Entity for each parsed tile
			Entity tile = Registry::Instance()->CreateEntity();
			tile.Group(group);
			tile.AddComponent<SpriteComponent>(assetID, tileWidth, tileHeight, zIndex, false, srcRectX, srcRectY);
			tile.AddComponent<TransformComponent>(glm::vec2(transformX + xTransform, transformY + yTransform), glm::vec2(tileScaleX, tileScaleY), 0.0);
			tile.AddComponent<MenuComponent>();
		}

	
	} menufile.close();

	// Open and read the thext file
	std::fstream menuBox;
	menuBox.open("./Assets/LevelAssets/menuBox.txt");

	if (!menuBox.is_open())
	{
		Logger::Err("Unable to open file");
	}

	while (!menuBox.eof())
	{
		// Mapfile variables
		int srcRectX = 0;
		int srcRectY = 0;
		int transformX = 0;
		int transformY = 0;
		int tileWidth = 0;
		int tileHeight = 0;
		int tileScaleX = 0;
		int tileScaleY = 0;
		int zIndex = 0;
		size_t numLines = 0;
		bool vertical = false;
		bool horizontal = false;
		int xTransform = 0;
		int yTransform = 0;
		std::string group = "";
		glm::vec2 colliderOffset = glm::vec2(0);
		bool collider = false;
		std::string assetID = "";

		// If we are at the end of the File --> Leave the file!
		if (menuBox.eof()) break;
		// Start parsing the text file
		menuBox >> group >> assetID >> tileWidth >> tileHeight >> srcRectY >> srcRectX >> zIndex >> transformX >> transformY >> tileScaleX >> tileScaleY >> vertical >> horizontal >>numLines;

		for (size_t i = 0; i < numLines; i++)
		{
			if (i > 0 && horizontal) xTransform += 32;
			if (i > 0 && vertical) yTransform += 32;
	
			// Create new tile Entity for each parsed tile
			Entity tile = Registry::Instance()->CreateEntity();
			tile.Group(group);
			tile.AddComponent<SpriteComponent>(assetID, tileWidth, tileHeight, zIndex, false, srcRectX, srcRectY);
			tile.AddComponent<TransformComponent>(glm::vec2(transformX + xTransform, transformY + yTransform), glm::vec2(tileScaleX, tileScaleY), 0.0);
		}


	} menuBox.close();
}
void LevelLoader::LoadMenuScreenFromLuaTable(sol::state& lua, std::string fileName)
{
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
				Entity heart = Registry::Instance()->CreateEntity();
				heart.Group("hearts");
				heart.AddComponent<SpriteComponent>("hearts", 16, 16, 0, false, 0, 0);
				heart.AddComponent<TransformComponent>(glm::vec2(transformX + xTransform, transformY + yTransform), glm::vec2(4, 4), 0.0);
				heart.AddComponent<MenuComponent>();
			}
			
			auto entity = Registry::Instance()->GetEntityByTag(entityTag);
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
void LevelLoader::LoadPauseScreen( std::unique_ptr<AssetManager>& assetManager,  SDL_Renderer* renderer)
{

	// Open and read the thext file
	std::fstream menuBox;
	menuBox.open("./Assets/LevelAssets/pauseMenu.txt");

	if (!menuBox.is_open())
	{
		Logger::Err("Unable to open file");
	}
}

std::string LevelLoader::LoadSlotName(unsigned int& slotNum)
{
	// What to load?
	/*
		name           --> std::string 
		candle	       --> bool
		sword type     --> enum SwordType{NO_SWORD, IRON_SWORD, SILVER_SWORD, ULTRA_SWORD};
		bow		       --> bool
		arrows	       --> unsigned int --> variable container
		boomerang	   --> bool
		boomerang type --> enum BoomerangType{CLOSE_RANGE = 0, LONG_RANGE};
		tunic type     --> enum TunicType{GREEN, BLUE, RED};
			Tunic Damage will be based on the type!
	*/

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

	Logger::Log("Slot number " + std::to_string(slotNum) + " is open");
	slotFile.close();
}

void LevelLoader::LoadLevel(std::unique_ptr<AssetManager>& assetManager, SDL_Renderer* renderer, const std::string& level)
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
		Entity tile = Registry::Instance()->CreateEntity();
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

void LevelLoader::LoadColliders(std::unique_ptr<AssetManager>& assetManager, SDL_Renderer* renderer, const std::string& filename)
{

	// Open and read the text file
	std::fstream mapFile;
	mapFile.open("./Assets/Levels/" + filename);

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
		bool collider = false;
		bool trigger = false;
		int type = 0;
		int triggerLevel = 0;
		TriggerType triggerType = NO_TRIGGER;

		mapFile >> group >> tranX >> tranY >> colliderScaleX >> colliderScaleY >> collider >> trigger;

		if (collider && !trigger) mapFile >> colWidth >> colHeight >> offset.x >> offset.y;
		else if (collider && trigger) mapFile >> colWidth >> colHeight >> offset.x >> offset.y >> type >> triggerOffset.x >> triggerOffset.y >> triggerLevel;


		triggerType = ConvertToTriggerType(type);

		// If we are at the end of the File --> Leave the File!!
		if (mapFile.eof()) break;

		// Create a new entity for each tile
		Entity boxCollider = Registry::Instance()->CreateEntity();
		boxCollider.Group(group);
		boxCollider.AddComponent<TransformComponent>(glm::vec2(tranX, tranY), glm::vec2(colliderScaleX, colliderScaleY), 0.0);
		boxCollider.AddComponent<GameComponent>();

		if (collider && !trigger)
		{
			boxCollider.AddComponent<BoxColliderComponent>(colWidth, colHeight, glm::vec2(offset.x, offset.y));
		}
		if (collider && trigger)
		{
			boxCollider.AddComponent<BoxColliderComponent>(colWidth, colHeight, glm::vec2(offset.x, offset.y));
			boxCollider.AddComponent<TriggerBoxComponent>(triggerType, glm::vec2(triggerOffset.x, triggerOffset.y), triggerLevel);
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
	case 5: return CAMERA_RIGHT; break;
	case 6: return TRAP; break;
	case 7: return HIDDEN_SWITCH; break;
	case 8: return HIDDEN_OBJECT; break;
	}
}

TriggerType LevelLoader::ConvertToTriggerType(std::string triggerType)
{
	if (triggerType == "NO_TRIGGER") return NO_TRIGGER;
	else if (triggerType == "SECRET_AREA") return SECRET_AREA;
}

void LevelLoader::LoadCollidersFromLuaTable(sol::state& lua, std::unique_ptr<AssetManager>& assetManager, SDL_Renderer* renderer, const std::string& fileName)
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
			//Logger::Err("No Colliders");
			break;
		}
		//Logger::Err("Has Colliders");
		sol::table collider = colliders[i];

		Entity newCollider = Registry::Instance()->CreateEntity();

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
			//Logger::Err("Has Components!!");
			// Transform Component
			sol::optional<sol::table> transform = collider["components"]["transform"];
			if (transform != sol::nullopt)
			{
				//Logger::Err("Has Transform Component!");
				newCollider.AddComponent<TransformComponent>(
					glm::vec2(
						collider["components"]["transform"]["position"]["x"],
						collider["components"]["transform"]["position"]["y"]
					),
					glm::vec2(
						collider["components"]["transform"]["scale"]["x"].get_or(1.0), // The Get_or is a Lua/Sol Convention that gets a value or uses the provided
						collider["components"]["transform"]["scale"]["y"].get_or(1.0)
					),
					collider["components"]["transform"]["rotation"].get_or(0.0)
				);
			}
			
			// Box Collider Component
			sol::optional<sol::table> boxCollider = collider["components"]["boxCollider"];
			if (boxCollider != sol::nullopt)
			{
				//Logger::Err("Has Box Collider Component");
				newCollider.AddComponent<BoxColliderComponent>(
					collider["components"]["boxCollider"]["width"],
					collider["components"]["boxCollider"]["height"],
					glm::vec2(
						collider["components"]["boxCollider"]["offset_x"].get_or(0.0),
						collider["components"]["boxCollider"]["offset_y"].get_or(0.0)
					)
				);
			}
			
			// Trigger Component -->Still Needs to be Tested!!
			sol::optional<sol::table> trigger = collider["triggerComponent"];
			if (trigger != sol::nullopt)
			{
				Logger::Err("Has Trigger Component");
				newCollider.AddComponent<TriggerBoxComponent>(
					collider["triggerComponent"]["type"],
					glm::vec2(
						collider["triggerComponent"]["transport_offset_x"].get_or(0.0),
						collider["triggerComponent"]["transport_offset_y"].get_or(0.0)
					),
					collider["triggerComponent"]["level"].get_or(0.0)
				);
				//Logger::Err(std::to_string(newCollider.GetComponent<TriggerBoxComponent>().triggerType));
			}
			else
			{
				Logger::Err("No Trigger");
			}
		}
		i++;
	}
}


void LevelLoader::SavePlayerDataToLuaTable(std::string saveNum, const std::unique_ptr<AssetManager>& assetManager, SDL_Renderer* renderer)
{
	Logger::Err("In Save Player to Lua Table Function");
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
	
		
	auto entity = Registry::Instance()->GetEntityByTag("player");
		
		
	const auto& transform = entity.GetComponent<TransformComponent>();
	
	// Local variables that are based on the game Items
	bool hasBoomerang = Game::Instance()->GetGameItems().woodBoomerang;
	bool hasMagicBoomerang = Game::Instance()->GetGameItems().magicBoomerang;
	bool hasSword = Game::Instance()->GetGameItems().woodSword;
	bool hasMagicSword = Game::Instance()->GetGameItems().magicSword;
	bool hasMagicRod = Game::Instance()->GetGameItems().magicRod;
	bool hasBombs = Game::Instance()->GetGameItems().bombs;
	bool hasFood = Game::Instance()->GetGameItems().food;
	bool hasFlute = Game::Instance()->GetGameItems().flute;
	bool hasRaft = Game::Instance()->GetGameItems().raft;
	bool hasLadder = Game::Instance()->GetGameItems().ladder;
	bool hasBlueRing = Game::Instance()->GetGameItems().blueRing;
	bool hasRedRing = Game::Instance()->GetGameItems().redRing;
	bool hasBow = Game::Instance()->GetGameItems().bow;
	bool hasMagicBow = Game::Instance()->GetGameItems().magicBow;
	bool hasShield = Game::Instance()->GetGameItems().shield;
	bool hasMagicShield = Game::Instance()->GetGameItems().magicShield;
	bool hasPowerBraclet = Game::Instance()->GetGameItems().powerBraclet;
	bool hasMap = Game::Instance()->GetGameItems().map;
	bool hasBluePotion = Game::Instance()->GetGameItems().bluePotion;
	bool hasRedPotion = Game::Instance()->GetGameItems().redPotion;
	bool hasMasterKey = Game::Instance()->GetGameItems().masterKey;
	int numRupees = GameState::totalRupees;
	int numBombs = GameState::totalBombs;
	int numArrows = 10;
	int numHearts = RenderHealthSystem::numHearts;
	int numKeys = GameState::totalKeys;
	int triforcePieces = 0;
	std::string name = "";
	m_writer.WriteStartTable(1, false, file);
	//m_writer.WriteKeyAndQuotedValue("group", "colliders", file);
	
	if (saveNum == "1") name = MenuState::player1Name;
	if (saveNum == "2") name = MenuState::player2Name;
	if (saveNum == "3") name = MenuState::player3Name;
	
	m_writer.WriteDeclareTable("menu_shared_values", file);
	m_writer.WriteKeyAndQuotedValue("name", name, file);
	m_writer.WriteKeyAndUnquotedValue("num_hearts", numHearts, file);
	if(hasBlueRing) m_writer.WriteKeyAndUnquotedValue("blue_ring", "true", file);
	else m_writer.WriteKeyAndUnquotedValue("blue_ring", "false", file);
	if(hasRedRing) m_writer.WriteKeyAndUnquotedValue("red_ring", "true", file);
	else m_writer.WriteKeyAndUnquotedValue("red_ring", "false", file);
	m_writer.WriteEndTable(false, file);
	
	m_writer.WriteDeclareTable("components", file);
	m_writer.WriteDeclareTable("transform", file);
	m_writer.WriteDeclareTable("position", file);
	m_writer.WriteKeyAndValue('x', transform.position.x, false, file);
	m_writer.WriteKeyAndValue('y',transform.position.y, true, file);
	m_writer.WriteEndTable(true, file);
	m_writer.WriteEndTable(true, file);
	m_writer.WriteEndTable(false, file);
	m_writer.WriteDeclareTable("items", file);
		
	// Write if we currently have these items in our inventory
	if(hasBoomerang) m_writer.WriteKeyAndUnquotedValue("boomerang", "true", file);
	else m_writer.WriteKeyAndUnquotedValue("boomerang", "false", file);
	if(hasSword) m_writer.WriteKeyAndUnquotedValue("sword", "true", file);
	else m_writer.WriteKeyAndUnquotedValue("sword", "false", file);
	if(hasMagicRod) m_writer.WriteKeyAndUnquotedValue("magic_rod", "true", file);
	else m_writer.WriteKeyAndUnquotedValue("magic_rod", "false", file);
	if(hasBombs) m_writer.WriteKeyAndUnquotedValue("bombs", "true", file);
	else m_writer.WriteKeyAndUnquotedValue("bombs", "false", file);
	if(hasFood) m_writer.WriteKeyAndUnquotedValue("food", "true", file);
	else m_writer.WriteKeyAndUnquotedValue("food", "false", file);
	if(hasFlute) m_writer.WriteKeyAndUnquotedValue("flute", "true", file);
	else m_writer.WriteKeyAndUnquotedValue("flute", "false", file);
	if(hasRaft) m_writer.WriteKeyAndUnquotedValue("raft", "true", file);
	else m_writer.WriteKeyAndUnquotedValue("raft", "false", file);
	if(hasLadder) m_writer.WriteKeyAndUnquotedValue("ladder", "true", file);
	else m_writer.WriteKeyAndUnquotedValue("ladder", "false", file);
	if(hasBow) m_writer.WriteKeyAndUnquotedValue("bow_wood", "true", file);
	else m_writer.WriteKeyAndUnquotedValue("bow_wood", "false", file);
	if(hasShield) m_writer.WriteKeyAndUnquotedValue("magic_shield", "true", file);
	else m_writer.WriteKeyAndUnquotedValue("magic_shield", "false", file);
	if(hasPowerBraclet) m_writer.WriteKeyAndUnquotedValue("power_braclet", "true", file);
	else m_writer.WriteKeyAndUnquotedValue("power_braclet", "false", file);
	if(hasMap) m_writer.WriteKeyAndUnquotedValue("map", "true", file);
	else m_writer.WriteKeyAndUnquotedValue("map", "false", file);
	if(hasBluePotion) m_writer.WriteKeyAndUnquotedValue("blue_potion", "true", file);
	else m_writer.WriteKeyAndUnquotedValue("blue_potion", "false", file);
	if(hasRedPotion) m_writer.WriteKeyAndUnquotedValue("red_potion", "true", file);
	else m_writer.WriteKeyAndUnquotedValue("red_potion", "false", file);
	if(hasMasterKey) m_writer.WriteKeyAndUnquotedValue("master_key", "true", file);
	else m_writer.WriteKeyAndUnquotedValue("master_key", "false", file);
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
	Logger::Err("In Save Player to Lua Table Function");
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
	
	// Create variables for default new player
	bool hasBoomerang = false;
	bool hasSword = true;
	bool hasMagicRod = false;
	bool hasBombs = false;
	bool hasFood = false;
	bool hasFlute = false;
	bool hasRaft = false;
	bool hasLadder = false;
	bool hasBlueRing = false;
	bool hasRedRing = false;
	bool hasBow = false;
	bool hasShield = false;
	bool hasPowerBraclet = false;
	bool hasMap = false;
	bool hasBluePotion = false;
	bool hasRedPotion = false;
	bool hasMasterKey = false;
	int numRupees = 0;
	int numBombs = 0;
	int numArrows = 0;
	int numHearts = 3;
	int numKeys = 0;
	int triforcePieces = 0;
	std::string name = newName;
	m_writer.WriteStartTable(1, false, file);

	m_writer.WriteDeclareTable("menu_shared_values", file);
	m_writer.WriteKeyAndQuotedValue("name", name, file);
	m_writer.WriteKeyAndUnquotedValue("num_hearts", numHearts, file);
	if (hasBlueRing) m_writer.WriteKeyAndUnquotedValue("blue_ring", "true", file);
	else m_writer.WriteKeyAndUnquotedValue("blue_ring", "false", file);
	if (hasRedRing) m_writer.WriteKeyAndUnquotedValue("red_ring", "true", file);
	else m_writer.WriteKeyAndUnquotedValue("red_ring", "false", file);
	m_writer.WriteEndTable(false, file);

	m_writer.WriteDeclareTable("components", file);
	m_writer.WriteDeclareTable("transform", file);
	m_writer.WriteDeclareTable("position", file);
	
	// Write the default player start position
	m_writer.WriteKeyAndValue('x', 7615, false, file);
	m_writer.WriteKeyAndValue('y', 5060, true, file);
	m_writer.WriteEndTable(true, file);
	m_writer.WriteEndTable(true, file);
	m_writer.WriteEndTable(false, file);
	
	// Declare a new Table "items"
	m_writer.WriteDeclareTable("items", file);
	
	if (hasBoomerang) m_writer.WriteKeyAndUnquotedValue("boomerang", "true", file);
	else m_writer.WriteKeyAndUnquotedValue("boomerang", "false", file);
	if (hasSword) m_writer.WriteKeyAndUnquotedValue("sword", "true", file);
	else m_writer.WriteKeyAndUnquotedValue("sword", "false", file);
	if (hasMagicRod) m_writer.WriteKeyAndUnquotedValue("magic_rod", "true", file);
	else m_writer.WriteKeyAndUnquotedValue("magic_rod", "false", file);
	if (hasBombs) m_writer.WriteKeyAndUnquotedValue("bombs", "true", file);
	else m_writer.WriteKeyAndUnquotedValue("bombs", "false", file);
	if (hasFood) m_writer.WriteKeyAndUnquotedValue("food", "true", file);
	else m_writer.WriteKeyAndUnquotedValue("food", "false", file);
	if (hasFlute) m_writer.WriteKeyAndUnquotedValue("flute", "true", file);
	else m_writer.WriteKeyAndUnquotedValue("flute", "false", file);
	if (hasRaft) m_writer.WriteKeyAndUnquotedValue("raft", "true", file);
	else m_writer.WriteKeyAndUnquotedValue("raft", "false", file);
	if (hasLadder) m_writer.WriteKeyAndUnquotedValue("ladder", "true", file);
	else m_writer.WriteKeyAndUnquotedValue("ladder", "false", file);
	if (hasBow) m_writer.WriteKeyAndUnquotedValue("bow_wood", "true", file);
	else m_writer.WriteKeyAndUnquotedValue("bow_wood", "false", file);
	if (hasShield) m_writer.WriteKeyAndUnquotedValue("magic_shield", "true", file);
	else m_writer.WriteKeyAndUnquotedValue("magic_shield", "false", file);
	if (hasPowerBraclet) m_writer.WriteKeyAndUnquotedValue("power_braclet", "true", file);
	else m_writer.WriteKeyAndUnquotedValue("power_braclet", "false", file);
	if (hasMap) m_writer.WriteKeyAndUnquotedValue("map", "true", file);
	else m_writer.WriteKeyAndUnquotedValue("map", "false", file);
	if (hasBluePotion) m_writer.WriteKeyAndUnquotedValue("blue_potion", "true", file);
	else m_writer.WriteKeyAndUnquotedValue("blue_potion", "false", file);
	if (hasRedPotion) m_writer.WriteKeyAndUnquotedValue("red_potion", "true", file);
	else m_writer.WriteKeyAndUnquotedValue("red_potion", "false", file);
	if (hasMasterKey) m_writer.WriteKeyAndUnquotedValue("master_key", "true", file);
	else m_writer.WriteKeyAndUnquotedValue("master_key", "false", file);
	m_writer.WriteEndTable(true, file);

	// Write the numbered values of current inventory and life hearts
	m_writer.WriteDeclareTable("inventory", file);
	m_writer.WriteKeyAndUnquotedValue("num_rupees", numRupees, file);
	m_writer.WriteKeyAndUnquotedValue("num_bombs", numBombs, file);
	m_writer.WriteKeyAndUnquotedValue("num_arrows", numArrows, file);
	m_writer.WriteKeyAndUnquotedValue("num_keys", numKeys, file);

	m_writer.WriteEndTable(false, file);
	m_writer.WriteEndTable(false, file);
	m_writer.WriteEndTable(false, file);

	file.close();
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
		auto entity = Registry::Instance()->GetEntityByTag("player");

		auto& transform = entity.GetComponent<TransformComponent>();

		sol::optional<sol::table> hasData = data[i];
		if (hasData == sol::nullopt)
		{
			//Logger::Err("No Colliders");
			break;
		}
		//Logger::Err("Has Colliders");
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
			//Logger::Err("Has Components!!");
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
			Game::Instance()->GetGameItems().woodBoomerang = player["items"]["boomerang"].get_or(false);
			Game::Instance()->GetGameItems().magicBoomerang = player["items"]["magic_boomerang"].get_or(false);
			Game::Instance()->GetGameItems().woodSword = player["items"]["wood_sword"].get_or(false);
			Game::Instance()->GetGameItems().steelSword = player["items"]["steel_sword"].get_or(false);
			Game::Instance()->GetGameItems().magicSword = player["items"]["magic_sword"].get_or(false);
			Game::Instance()->GetGameItems().bombs = player["items"]["bombs"].get_or(false);
			Game::Instance()->GetGameItems().food = player["items"]["food"].get_or(false);
			Game::Instance()->GetGameItems().flute = player["items"]["flute"].get_or(false);
			Game::Instance()->GetGameItems().raft = player["items"]["raft"].get_or(false);
			Game::Instance()->GetGameItems().ladder = player["items"]["ladder"].get_or(false);
			Game::Instance()->GetGameItems().bow = player["items"]["bow_wood"].get_or(false);
			Game::Instance()->GetGameItems().magicBow = player["items"]["bow_magic"].get_or(false);
			Game::Instance()->GetGameItems().shield = player["items"]["shield"].get_or(false);
			Game::Instance()->GetGameItems().magicShield = player["items"]["magic_shield"].get_or(false);
			Game::Instance()->GetGameItems().powerBraclet = player["items"]["power_braclet"].get_or(false);
			Game::Instance()->GetGameItems().map = player["items"]["map"].get_or(false);
			Game::Instance()->GetGameItems().bluePotion = player["items"]["blue_potion"].get_or(false);
			Game::Instance()->GetGameItems().redPotion = player["items"]["red_potion"].get_or(false);
			Game::Instance()->GetGameItems().masterKey = player["items"]["master_key"].get_or(false);
		}

		// Inventory
		sol::optional<sol::table> inventory = player["inventory"];
		if (inventory != sol::nullopt)
		{
			Logger::Err("Needs to be set up");
			GameState::totalRupees = player["inventory"]["num_rupees"].get_or(0);
			GameState::totalBombs = player["inventory"]["num_bombs"].get_or(0);
			GameState::totalKeys = player["inventory"]["num_keys"].get_or(0);
			//RenderHealthSystem::loadedHearts = player["inventory"]["num_hearts"].get_or(3);
		}
		i++;
	}
}

void LevelLoader::LoadLevelFromLuaTable(sol::state& lua, std::string fileName, const std::unique_ptr<AssetManager>& assetManager)
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
	Logger::Err("In Level File");
	/*sol::table data = lua["level_data"];
	sol::table entities = data["entities"];*/

	sol::table entities = lua["enemies"];

	int i = 1;

	while (true)
	{
		sol::optional<sol::table> hasEntity = entities[i];
		
		if (hasEntity == sol::nullopt)
		{
			Logger::Err("There is no more Level Data");
			break;
		}
		
		sol::table entity = entities[i];
		
		Entity newEntity = Registry::Instance()->CreateEntity();

		//// Tag 
		//
		//if (entity["tag"] != sol::nullopt)
		//{
		//	newEntity.Tag(entity["tag"]);
		//}
		//else
		//	Logger::Err("NO TAG");

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
				Logger::Err("Has Box Collider Component");
				newEntity.AddComponent<BoxColliderComponent>(
					entity["components"]["box_collider"]["width"].get_or(16),
					entity["components"]["box_collider"]["height"].get_or(16),
					glm::vec2(
						entity["componets"]["box_collider"]["offset_x"].get_or(0),
						entity["componets"]["box_collider"]["offset_y"].get_or(0)
					));
			}
		//	// Projectile Emitter  
		//	sol::optional<sol::table> projectile = entity["components"]["projectile_emitter"];
		//	if (projectile != sol::nullopt)
		//	{
		//		newEntity.AddComponent<ProjectileEmitterComponent>(
		//			glm::vec2(
		//				entity["components"]["projectile_emitter"]["velocity"]["x"].get_or(0),
		//				entity["components"]["projectile_emitter"]["velocity"]["y"].get_or(0)
		//			),
		//			entity["components"]["projectile_emitter"]["repeat_frequency"].get_or(0),
		//			entity["components"]["projectile_emitter"]["projectile_duration"].get_or(10000),
		//			entity["components"]["projectile_emitter"]["hit_percent_damage"].get_or(10),
		//			entity["components"]["projectile_emitter"]["is_friendly"].get_or(false)
		//			);
		//	}

			// Script Component
		/*	sol::optional<sol::table> script = entity["components"]["on_update_script"];
			if (script != sol::nullopt)
			{
				sol::function func = entity["components"]["on_update_script"][0];
				newEntity.AddComponent<ScriptComponent>(func);
			}*/
			newEntity.AddComponent<GameComponent>();
			Logger::Err("Created Enemy");
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
			Logger::Err("No HUD Data");
			break;
		}

		sol::table hudData = data[i];

		Entity newHUDObject = Registry::Instance()->CreateEntity();
		
		// Add tag if there is one
		sol::optional<std::string> tag = hudData["tag"];
		if (tag != sol::nullopt)
		{
			newHUDObject.Tag(hudData["tag"]);
			if (newHUDObject.HasTag("heart1"))
			{
				newHUDObject.AddComponent<HealthComponent>(0);
			}
			if (newHUDObject.HasTag("heart2"))
			{
				newHUDObject.AddComponent<HealthComponent>(0);
			}
			if (newHUDObject.HasTag("heart3"))
			{
				newHUDObject.AddComponent<HealthComponent>(0);
			}
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
			Logger::Err("No Assets Data");
			break;
		}

		sol::table asset = assets[i];
		std::string assetType = asset["type"];
		std::string assetID = asset["id"];

		if (assetType == "texture")
		{
			if (!Game::Instance()->GetAssetManager()->HasTexture(assetID))
			{
				Game::Instance()->GetAssetManager()->AddTextures(Game::Instance()->GetRenderer(), assetID, asset["file"]);
				Logger::Log("New Texture asset loaded to the asset store, id: " + assetID);
			}

		}

		if (assetType == "font")
		{
			if (!Game::Instance()->GetAssetManager()->HasFont(assetID))
			{
				Game::Instance()->GetAssetManager()->AddFonts(assetID, asset["file"], asset["font_size"]);
				Logger::Log("New Font asset loaded to the asset store, id: " + assetID);
			}
		}

		if (assetType == "soundFX")
		{
			if (!Game::Instance()->GetAssetManager()->HasSoundFX(assetID))
			{
				Game::Instance()->GetAssetManager()->AddSoundFX(assetID, asset["file"]);
				Logger::Log("New SoundFX asset loaded to the asset store, id: " + assetID);
			}
		}

		if (assetType == "music")
		{
			if (!Game::Instance()->GetAssetManager()->HasMusic(assetID))
			{
				Game::Instance()->GetAssetManager()->AddMusic(assetID, asset["file"]);
				Logger::Log("New Music asset loaded to the asset store, id: " + assetID);
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

		Entity name = Registry::Instance()->CreateEntity();

		name.AddComponent<SpriteComponent>("name-letters", 16, 16, 0, true, 0, 0);
		name.AddComponent<TransformComponent>(glm::vec2(x, y), glm::vec2(1.5, 1.5), 0);
		name.AddComponent<MenuComponent>();

		switch (letter)
		{
		case 'A':
			name.GetComponent<SpriteComponent>().srcRect.x = 0;
			name.GetComponent<SpriteComponent>().srcRect.y = 0;
			name.GetComponent<TransformComponent>().position.x += space;
			break;
		case 'B':
			name.GetComponent<SpriteComponent>().srcRect.x = 16;
			name.GetComponent<SpriteComponent>().srcRect.y = 0;
			name.GetComponent<TransformComponent>().position.x += space;
			break;
		case 'C':
			name.GetComponent<SpriteComponent>().srcRect.x = 32;
			name.GetComponent<SpriteComponent>().srcRect.y = 0;
			name.GetComponent<TransformComponent>().position.x += space;
			break;
		case 'D':
			name.GetComponent<SpriteComponent>().srcRect.x = 48;
			name.GetComponent<SpriteComponent>().srcRect.y = 0;
			name.GetComponent<TransformComponent>().position.x += space;
			break;
		case 'E':
			name.GetComponent<SpriteComponent>().srcRect.x = 64;
			name.GetComponent<SpriteComponent>().srcRect.y = 0;
			name.GetComponent<TransformComponent>().position.x += space;
			break;
		case 'F':
			name.GetComponent<SpriteComponent>().srcRect.x = 80;
			name.GetComponent<SpriteComponent>().srcRect.y = 0;
			name.GetComponent<TransformComponent>().position.x += space;
			break;
		case 'G':
			name.GetComponent<SpriteComponent>().srcRect.x = 96;
			name.GetComponent<SpriteComponent>().srcRect.y = 0;
			name.GetComponent<TransformComponent>().position.x += space;
			break;
		case 'H':
			name.GetComponent<SpriteComponent>().srcRect.x = 112;
			name.GetComponent<SpriteComponent>().srcRect.y = 0;
			name.GetComponent<TransformComponent>().position.x += space;
			break;
		case 'I':
			name.GetComponent<SpriteComponent>().srcRect.x = 0;
			name.GetComponent<SpriteComponent>().srcRect.y = 32;
			name.GetComponent<TransformComponent>().position.x += space;
			break;
		case 'J':
			name.GetComponent<SpriteComponent>().srcRect.x = 16;
			name.GetComponent<SpriteComponent>().srcRect.y = 32;
			name.GetComponent<TransformComponent>().position.x += space;
			break;
		case 'K':
			name.GetComponent<SpriteComponent>().srcRect.x = 32;
			name.GetComponent<SpriteComponent>().srcRect.y = 32;
			name.GetComponent<TransformComponent>().position.x += space;
			break;
		case 'L':
			name.GetComponent<SpriteComponent>().srcRect.x = 48;
			name.GetComponent<SpriteComponent>().srcRect.y = 32;
			name.GetComponent<TransformComponent>().position.x += space;
			break;
		case 'M':
			name.GetComponent<SpriteComponent>().srcRect.x = 64;
			name.GetComponent<SpriteComponent>().srcRect.y = 32;
			name.GetComponent<TransformComponent>().position.x += space;
			break;
		case 'N':
			name.GetComponent<SpriteComponent>().srcRect.x = 80;
			name.GetComponent<SpriteComponent>().srcRect.y = 32;
			name.GetComponent<TransformComponent>().position.x += space;
			break;
		case 'O':
			name.GetComponent<SpriteComponent>().srcRect.x = 96;
			name.GetComponent<SpriteComponent>().srcRect.y = 32;
			name.GetComponent<TransformComponent>().position.x += space;
			break;
		case 'P':
			name.GetComponent<SpriteComponent>().srcRect.x = 112;
			name.GetComponent<SpriteComponent>().srcRect.y = 32;
			name.GetComponent<TransformComponent>().position.x += space;
			break;
		case 'Q':
			name.GetComponent<SpriteComponent>().srcRect.x = 0;
			name.GetComponent<SpriteComponent>().srcRect.y = 64;
			name.GetComponent<TransformComponent>().position.x += space;
			break;
		case 'R':
			name.GetComponent<SpriteComponent>().srcRect.x = 16;
			name.GetComponent<SpriteComponent>().srcRect.y = 64;
			name.GetComponent<TransformComponent>().position.x += space;
			break;
		case 'S':
			name.GetComponent<SpriteComponent>().srcRect.x = 32;
			name.GetComponent<SpriteComponent>().srcRect.y = 64;
			name.GetComponent<TransformComponent>().position.x += space;
			break;
		case 'T':
			name.GetComponent<SpriteComponent>().srcRect.x = 48;
			name.GetComponent<SpriteComponent>().srcRect.y = 64;
			name.GetComponent<TransformComponent>().position.x += space;
			break;
		case 'U':
			name.GetComponent<SpriteComponent>().srcRect.x = 64;
			name.GetComponent<SpriteComponent>().srcRect.y = 64;
			name.GetComponent<TransformComponent>().position.x += space;
			break;
		case 'V':
			name.GetComponent<SpriteComponent>().srcRect.x = 80;
			name.GetComponent<SpriteComponent>().srcRect.y = 64;
			name.GetComponent<TransformComponent>().position.x += space;
			break;
		case 'W':
			name.GetComponent<SpriteComponent>().srcRect.x = 96;
			name.GetComponent<SpriteComponent>().srcRect.y = 64;
			name.GetComponent<TransformComponent>().position.x += space;
			break;
		case 'X':
			name.GetComponent<SpriteComponent>().srcRect.x = 112;
			name.GetComponent<SpriteComponent>().srcRect.y = 64;
			name.GetComponent<TransformComponent>().position.x += space;
			break;
		case 'Y':
			name.GetComponent<SpriteComponent>().srcRect.x = 0;
			name.GetComponent<SpriteComponent>().srcRect.y = 96;
			name.GetComponent<TransformComponent>().position.x += space;
			break;
		case 'Z':
			name.GetComponent<SpriteComponent>().srcRect.x = 16;
			name.GetComponent<SpriteComponent>().srcRect.y = 96;
			name.GetComponent<TransformComponent>().position.x += space;
			break;
		}

		space += 24;
	}
}


