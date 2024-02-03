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
#include "../Components/SecretComponent.h"
#include "../Components/GameComponent.h"
#include "../Components/ScriptComponent.h"
#include "../Components/ProjectileEmitterComponent.h"
#include "../Components/CameraFollowComponent.h"
#include "../Components/SceneChangeComponent.h"
#include "../Components/EditorComponent.h"
#include "../Components/ItemComponent.h"
#include "../States/NameState.h"
#include "../States/MenuState.h"
#include "../States/GameState.h"
#include "../Game/Game.h"
#include "../Utilities/LuaTableWriter.h"
#include "../Systems/GameSystems/HealthSystem.h"
#include "../Systems/SoundFXSystem.h"
#include "../Systems/GameSystems/TriggerSystem.h"
#include <string>
#include <fstream>
#include <filesystem>
#include <SDL.h>
#include "../Utilities/GameData.h"

LevelLoader::LevelLoader()
	: game(Game::Instance()), gameData(GameData::GetInstance()), reg(Registry::Instance())
{
}

LevelLoader::~LevelLoader()
{
}

void LevelLoader::LoadMap(const std::string& mapName, int image_width, int image_height, bool offset)
{
	int x = 0;
	int y = 0;
	if (offset)
		y = -192;

	Entity secret = reg.CreateEntity();
	secret.AddComponent<TransformComponent>(glm::vec2(x, y), glm::vec2(4, 4), 0.0);
	secret.AddComponent<SpriteComponent>(mapName, image_width, image_height, 1, false, 0, 0);
	secret.AddComponent<TileComponent>();
	secret.Group("map");
}

void LevelLoader::LoadTilemap(const std::string& fileName, const std::string& imageName)
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
		Logger::Err("ERROR: LEVEL_LOADER: __LINE__ 73: Unable to open [" + fileName + "]");
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
			tile.AddComponent<GameComponent>(); // RenderTileSystem needs to be changed to to layers
			tile.Group("layered");
		}

		// If the tile is a collider, add a boxColliderComponent
		if (collider)
		{
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
		AssetType assetType = LevelLoader::AssetType::TEXTURE;
		lvlFile >> type >> assetID >> assetPath;

		assetType = ConvertToAssetType(type);

		switch (assetType)
		{
		case LevelLoader::AssetType::TEXTURE:
			assetManager->AddTextures(game.GetRenderer(), assetID, (path + assetPath));
			break;
		case LevelLoader::AssetType::MUSIC:
			assetManager->AddMusic(assetID, path + assetPath);
			break;
		case LevelLoader::AssetType::SOUNDFX:
			assetManager->AddSoundFX(assetID, path + assetPath);
			break;
		case LevelLoader::AssetType::FONT:
			assetManager->AddFonts(assetID, path + assetPath, 14);
			break;
		default:
			break;
		}
	}
}

LevelLoader::AssetType LevelLoader::ConvertToAssetType(std::string& type)
{
	if (type == "TEXTURE")
		return LevelLoader::AssetType::TEXTURE;
	if (type == "SOUNDFX")
		return LevelLoader::AssetType::SOUNDFX;
	if (type == "FONT")
		return LevelLoader::AssetType::FONT;
	if (type == "MUSIC")
		return LevelLoader::AssetType::MUSIC;
}

void LevelLoader::LoadMenuScreenFromLuaTable(sol::state& lua, const std::string& fileName, int slotNum)
{
	bool valid = false;
	sol::load_result script = lua.load_file("./Assets/SavedFiles/slot_" + std::to_string(slotNum) + "/" + fileName + ".lua");
	// Check the syntax of the Lua script 
	if (!script.valid())
	{
		sol::error err = script;
		std::string errorMessage = err.what(); // What is the error
		Logger::Err("Error loading the Lua Script: " + errorMessage);
		return;
	}


	// Execute the script using the sol state 
	lua.script_file("./Assets/SavedFiles/slot_" + std::to_string(slotNum) + "/" + fileName + ".lua");

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
			//Logger::Err("There is no Player Data!");
			break;
		}
		sol::table player = data[i];
		// Name 
		sol::optional<sol::table> menuSharedValues = player["menu_shared_values"];
		if (menuSharedValues != sol::nullopt)
		{
			if (fileName == "save1")
			{
				gameData.SetPlayer1Name(player["menu_shared_values"]["name"] );
				ConvertName(gameData.GetPlayer1Name(), 400, 220);
				transformX = 550;
				transformY = 200;
				entityTag = "first_menu_slot";

			}
			if (fileName == "save2")
			{
				gameData.SetPlayer2Name(player["menu_shared_values"]["name"] );
				ConvertName(gameData.GetPlayer2Name(), 400, 320);
				transformX = 550;
				transformY = 300;
				entityTag = "second_menu_slot";
			}
			if (fileName == "save3")
			{
				gameData.SetPlayer3Name(player["menu_shared_values"]["name"] );
				ConvertName(gameData.GetPlayer3Name(), 400, 420);
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

void LevelLoader::EliminatePlayerToDefault(int slotNum, const std::string& name)
{
	// Completly Delete the player
	for (auto& nameImage : reg.GetEntitiesByGroup(name))
		nameImage.Kill();

	for (auto& playerHearts : reg.GetEntitiesByGroup("hearts_save" + std::to_string(slotNum)))
		playerHearts.Kill();

	// Play Eliminate Sound
	game.GetSoundPlayer().PlaySoundFX("Eliminate", 0, SoundChannel::ANY);
}

bool LevelLoader::ReadSpriteComponent(sol::table& table, Entity& entity)
{
	sol::optional<sol::table> sprite = table["components"]["sprite"];
	if (sprite != sol::nullopt)
	{
		entity.AddComponent<SpriteComponent>(
			table["components"]["sprite"]["asset_id"],
			table["components"]["sprite"]["width"],
			table["components"]["sprite"]["height"],
			table["components"]["sprite"]["z_index"].get_or(1), // Layer
			table["components"]["sprite"]["is_fixed"].get_or(false),
			table["components"]["sprite"]["src_rect_x"].get_or(0),
			table["components"]["sprite"]["src_rect_y"].get_or(0),
			glm::vec2(
				table["components"]["sprite"]["offset"]["x"].get_or(0),
				table["components"]["sprite"]["offset"]["y"].get_or(0)));
		
		return true;
	}
	return false;
}
void LevelLoader::ReadBoxColliderComponent(sol::table& table, Entity& entity)
{
	sol::optional<sol::table> boxCollider = table["components"]["box_collider"];
	if (boxCollider != sol::nullopt)
	{
		entity.AddComponent<BoxColliderComponent>(
			table["components"]["box_collider"]["width"],
			table["components"]["box_collider"]["height"],
			glm::vec2(
				table["components"]["box_collider"]["offset_x"].get_or(0.0),
				table["components"]["box_collider"]["offset_y"].get_or(0.0)));
	}
}

void LevelLoader::ReadAnimationComponent(sol::table& table, Entity& entity)
{
	sol::optional<sol::table> animation = table["components"]["animation"];
	if (animation != sol::nullopt)
	{
		entity.AddComponent<AnimationComponent>(
			table["components"]["animation"]["num_frames"].get_or(1),
			table["components"]["animation"]["frame_rate"].get_or(1),
			table["components"]["animation"]["vertical"].get_or(false),
			table["components"]["animation"]["looped"].get_or(false),
			table["components"]["animation"]["frame_offset"].get_or(0));
	}
}

void LevelLoader::ReadHealthComponent(sol::table& table, Entity& entity)
{
	sol::optional<sol::table> health = table["components"]["health"];
	if (health != sol::nullopt)
	{
		entity.AddComponent<HealthComponent>(
			HealthComponent{
				.healthPercentage = table["components"]["health"]["health_percentage"].get_or(9),
				.maxHearts = table["components"]["health"]["max_hearts"].get_or(3)
		});
	}
}

void LevelLoader::ReadKeyboardControlComponent(sol::table& table, Entity& entity)
{
	sol::optional<sol::table> keyboard = table["components"]["keyboard_control"];
	if (keyboard != sol::nullopt)
	{
		entity.AddComponent<KeyboardControlComponent>(
			glm::vec2(
				table["components"]["keyboard_control"]["up_velocity"]["x"].get_or(0),
				table["components"]["keyboard_control"]["up_velocity"]["y"].get_or(0)
			),
			glm::vec2(
				table["components"]["keyboard_control"]["right_velocity"]["x"].get_or(0),
				table["components"]["keyboard_control"]["right_velocity"]["y"].get_or(0)
			),
			glm::vec2(
				table["components"]["keyboard_control"]["down_velocity"]["x"].get_or(0),
				table["components"]["keyboard_control"]["down_velocity"]["y"].get_or(0)
			),
			glm::vec2(
				table["components"]["keyboard_control"]["left_velocity"]["x"].get_or(0),
				table["components"]["keyboard_control"]["left_velocity"]["y"].get_or(0)
			));
	}
}

void LevelLoader::ReadProjectileEmitterComponent(sol::table& table, Entity& entity)
{
	sol::optional<sol::table> projectile = table["components"]["projectile_emitter"];
	if (projectile != sol::nullopt)
	{
		entity.AddComponent<ProjectileEmitterComponent>(
			glm::vec2(
				table["components"]["projectile_emitter"]["velocity"]["x"].get_or(0),
				table["components"]["projectile_emitter"]["velocity"]["y"].get_or(0)
			),
			table["components"]["projectile_emitter"]["repeat_frequency"].get_or(0),
			table["components"]["projectile_emitter"]["projectile_duration"].get_or(10000),
			table["components"]["projectile_emitter"]["hit_percent_damage"].get_or(2),
			table["components"]["projectile_emitter"]["is_friendly"].get_or(false)
			);
	}
}

void LevelLoader::ReadTransformComponent(sol::table& table, Entity& entity)
{
	sol::optional<sol::table> transformComp = table["components"]["transform"];
	if (transformComp != sol::nullopt)
	{
		entity.AddComponent<TransformComponent>(
			glm::vec2(
				table["components"]["transform"]["position"]["x"].get_or(0),
				table["components"]["transform"]["position"]["y"].get_or(0)
			),
			glm::vec2(
				table["components"]["transform"]["scale"]["x"].get_or(1.0),
				table["components"]["transform"]["scale"]["y"].get_or(1.0)),
			table["components"]["transform"]["rotation"].get_or(0.0));
	}
}
void LevelLoader::ReadRigidBodyComponent(sol::table& table, Entity& entity)
{
	sol::optional<sol::table> rigidBody = table["components"]["rigidbody"];
	if (rigidBody != sol::nullopt)
	{
		entity.AddComponent<RigidBodyComponent>(
			glm::vec2(
				table["components"]["rigidbody"]["velocity"]["x"].get_or(0),
				table["components"]["rigidbody"]["velocity"]["y"].get_or(0)));
	}
}

void LevelLoader::CreateNewGameSecretsFile(int slotNum)
{
	LuaTableWriter m_writer;

	std::ofstream newFile("./Assets/SavedFiles/slot_" + std::to_string(slotNum) + "/GameSecrets_" + std::to_string(slotNum) + ".lua");
	newFile.close();

	std::fstream file;

	file.open("./Assets/SavedFiles/slot_" + std::to_string(slotNum) + "/GameSecrets_" + std::to_string(slotNum) + ".lua");

	// Start the Lua Table
	m_writer.WriteStartDocument();

	// Write a comment in Lua format
	m_writer.WriteCommentSeparation(file);
	m_writer.WriteCommentLine("Game Secrets file#: " + std::to_string(slotNum), file);
	m_writer.WriteCommentSeparation(file);
	m_writer.WriteDeclareTable("secrets", file);

	m_writer.WriteStartTable(1, false, file);
	m_writer.WriteKeyAndQuotedValue("location", "Sword_cave_Area", file);
	m_writer.WriteKeyAndUnquotedValue("found", "false", file);
	m_writer.WriteEndTable(false, file);

	m_writer.WriteStartTable(2, false, file);
	m_writer.WriteKeyAndQuotedValue("location", "bomb_secret_hearts_1", file);
	m_writer.WriteKeyAndUnquotedValue("found", "false", file);
	m_writer.WriteEndTable(false, file);

	m_writer.WriteStartTable(3, false, file);
	m_writer.WriteKeyAndQuotedValue("location", "bomb_secret_money_1", file);
	m_writer.WriteKeyAndUnquotedValue("found", "false", file);
	m_writer.WriteEndTable(false, file);

	m_writer.WriteStartTable(4, false, file);
	m_writer.WriteKeyAndQuotedValue("location", "bomb_secret_rupees_1", file);
	m_writer.WriteKeyAndUnquotedValue("found", "false", file);
	m_writer.WriteEndTable(false, file);

	m_writer.WriteStartTable(5, false, file);
	m_writer.WriteKeyAndQuotedValue("location", "bomb_secret_rupees_2", file);
	m_writer.WriteKeyAndUnquotedValue("found", "false", file);
	m_writer.WriteEndTable(false, file);

	m_writer.WriteStartTable(6, false, file);
	m_writer.WriteKeyAndQuotedValue("location", "burn_bush_potion_shop_1", file);
	m_writer.WriteKeyAndUnquotedValue("found", "false", file);
	m_writer.WriteEndTable(false, file);

	m_writer.WriteStartTable(7, false, file);
	m_writer.WriteKeyAndQuotedValue("location", "push_rock_to_highway_1", file);
	m_writer.WriteKeyAndUnquotedValue("found", "false", file);
	m_writer.WriteEndTable(false, file);

	// Close the above tables
	m_writer.WriteEndTable(false, file);
	// Close the new lua file
	file.close();
}

void LevelLoader::LoadMenuUIFromLuaTable(sol::state& lua, const std::string& fileName)
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

	// Read the main table first
	sol::table data = lua["menu"];

	int i = 1;

	while (true)
	{
		sol::optional<sol::table> hasData = data[i];
		if (hasData == sol::nullopt)
		{
			break;
		}
		sol::table ui = data[i];
		auto entity = reg.CreateEntity();
		sol::optional tag = ui["tag"];
		if (tag != sol::nullopt)
		{
			std::string tempTag = ui["tag"].get_or(std::string{""});
			entity.Tag(tempTag);
		}

		// Components
		sol::optional<sol::table> hasComponents = ui["components"];
		if (hasComponents != sol::nullopt)
		{
			// Transform Component
			ReadTransformComponent(ui, entity);
			// Sprite Component
			ReadSpriteComponent(ui, entity);
		}
		entity.AddComponent<MenuComponent>();
		i++;
	}
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
	mapFile.open("./Assets/Levels/Colliders/" + filename + ".map");

	if (!mapFile.is_open())
	{
		Logger::Err("Unable to open file");
		return;
	}

	while (!mapFile.eof())
	{
		int tranX = 0;
		int tranY = 0;
		int colWidth = 0;
		int colHeight = 0;
		int colliderScaleX = 0;
		int colliderScaleY = 0;
		std::string group = "";
		glm::vec2 offset = glm::vec2(0, 0);

		mapFile >> group >> tranX >> tranY >> colliderScaleX >> colliderScaleY >> colWidth >> colHeight >> offset.x >> offset.y;

		// If we are at the end of the File --> Leave the File!!
		if (mapFile.eof()) break;

		// Create a new entity for each tile
		Entity boxCollider = reg.CreateEntity();
		boxCollider.Group(group);
		boxCollider.AddComponent<ColliderComponent>();
		boxCollider.AddComponent<BoxColliderComponent>(colWidth, colHeight, glm::vec2(offset.x, offset.y));
		boxCollider.AddComponent<TransformComponent>(glm::vec2{ tranX, tranY }, glm::vec2{ colliderScaleX, colliderScaleY }, 0.0);
		boxCollider.AddComponent<GameComponent>();
	}
	mapFile.close();
}

TriggerBoxComponent::TriggerType LevelLoader::ConvertStringToTriggerType(const std::string& type)
{
	if (type == "no_trigger")
		return TriggerBoxComponent::TriggerType::NO_TRIGGER;
	else if (type == "scene_change")
		return TriggerBoxComponent::TriggerType::SCENE_CHANGE;
	else if (type == "burn_bush")
		return TriggerBoxComponent::TriggerType::BURN_BUSHES;
	else if (type == "transport")
		return TriggerBoxComponent::TriggerType::TRANSPORT;
	else if (type == "push_rocks")
		return TriggerBoxComponent::TriggerType::PUSH_ROCKS;
	else if (type == "collect_item")
		return TriggerBoxComponent::TriggerType::COLLECT_ITEM;
	else if (type == "bomb_secret")
		return TriggerBoxComponent::TriggerType::BOMB_SECRET;
	else if (type == "locked_door")
		return TriggerBoxComponent::TriggerType::LOCKED_DOOR;
	else if (type == "hidden_object")
		return TriggerBoxComponent::TriggerType::HIDDEN_OBJECT;
	else if (type == "shop_item")
		return TriggerBoxComponent::TriggerType::SHOP_ITEM;
	else if (type == "raft")
		return TriggerBoxComponent::TriggerType::RAFT;
	else if (type == "ladder")
		return TriggerBoxComponent::TriggerType::LADDER;
	else if (type == "money_game")
		return TriggerBoxComponent::TriggerType::MONEY_GAME;
	else if (type == "trap_door")
		return TriggerBoxComponent::TriggerType::TRAP_DOOR;
	else if (type == "fairy_circle")
		return TriggerBoxComponent::TriggerType::FAIRY_CIRCLE;
	else 
		return TriggerBoxComponent::TriggerType::NO_TRIGGER;
}

AIComponent::EnemyType LevelLoader::ConvertStringToEnemyType(const std::string& enemyType)
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
	else if (enemyType == "keese")
		return AIComponent::EnemyType::KEESE;
	else if (enemyType == "blade_trap")
		return AIComponent::EnemyType::BLADE_TRAP;
	else if (enemyType == "stalfos")
		return AIComponent::EnemyType::STALFOS;
	else if (enemyType == "goriya")
		return AIComponent::EnemyType::GORIYA;
	else
		return AIComponent::EnemyType::NO_TYPE;
}


AIComponent::EnemyBossType LevelLoader::ConvertStringToEnemyBossType(const std::string& bossType)
{
	if (bossType == "aquamentus")
		return AIComponent::EnemyBossType::AQUAMENTUS;
	else if (bossType == "not_a_boss")
		return AIComponent::EnemyBossType::NOT_A_BOSS;
	else
		return AIComponent::EnemyBossType::NOT_A_BOSS;
}

void LevelLoader::LoadTriggers(sol::state& lua, const std::string& fileName)
{
	sol::load_result script = lua.load_file("./Assets/Levels/Triggers/" + fileName + ".lua");

	// This checks the syntax of our script, but it does not execute the script
	if (!script.valid())
	{
		sol::error err = script;
		std::string errorMessage = err.what();
		Logger::Err("Error loading the Lua Script: " + errorMessage);
		return;
	}

	// Executes the script using the sol State
	lua.script_file("./Assets/Levels/Triggers/" + fileName + ".lua");

	// =========================================================================================================
	// Read the Triggers
	// =========================================================================================================
	sol::table triggers = lua["triggers"];

	int i = 1;
	while (true)
	{
		sol::optional<sol::table> hasTrigger = triggers[i];
		if (hasTrigger == sol::nullopt)
		{
			break;
		}
		if (i == 17)
		{
			int here = 0;
		}
		sol::table trigger = triggers[i];

		Entity newTrigger = reg.CreateEntity();

		// Tag
		sol::optional<std::string> tag = trigger["tag"];
		

		if (tag != sol::nullopt)
		{

			std::string tagName = trigger["tag"];

			//if (reg.DoesTagExist(tagName))
			//	__debugbreak();
			//else
			//	Logger::Log("NOPE");

			newTrigger.Tag(tagName);
		}
		
		// Group
		sol::optional<std::string> group = trigger["group"];
		if (group != sol::nullopt)
		{
			newTrigger.Group(trigger["group"]);
		}

		// Components
		sol::optional<sol::table> hasComponents = trigger["components"];
		if (hasComponents != sol::nullopt)
		{
			// Transform Component
			ReadTransformComponent(trigger, newTrigger);
			// Box Collider Component
			ReadBoxColliderComponent(trigger, newTrigger);
			// Trigger Component
			sol::optional<sol::table> triggerBox = trigger["components"]["trigger_box"];
			if (triggerBox != sol::nullopt)
			{
				//Logger::Log(trigger["components"]["trigger_box"]["trigger_file"]);
				TriggerBoxComponent::TriggerType triggerType = ConvertStringToTriggerType(trigger["components"]["trigger_box"]["trigger_type"]);
			
				newTrigger.AddComponent<TriggerBoxComponent>(
					triggerType,
					glm::vec2(
						trigger["components"]["trigger_box"]["transport_offset"]["x"].get_or(0.0),
						trigger["components"]["trigger_box"]["transport_offset"]["y"].get_or(0.0)
					),
					glm::vec2(
						trigger["components"]["trigger_box"]["camera_offset"]["x"].get_or(0.0),
						trigger["components"]["trigger_box"]["camera_offset"]["y"].get_or(0.0)
					),

					trigger["components"]["trigger_box"]["collider"].get_or(false),
					trigger["components"]["trigger_box"]["entity_removed_tag"].get_or(std::string(""))
					);
			}

			sol::optional<sol::table> scene = trigger["components"]["scene_change"];
			
			// Check if the trigger has a scene component
			if (scene != sol::nullopt)
			{
				newTrigger.AddComponent<SceneChangeComponent>(
					trigger["components"]["scene_change"]["level_music"].get_or(std::string("stop")),
					trigger["components"]["scene_change"]["asset_file"].get_or(std::string("no_file")),
					trigger["components"]["scene_change"]["enemy_file"].get_or(std::string("no_file")),
					trigger["components"]["scene_change"]["collider_file"].get_or(std::string("no_file")),
					trigger["components"]["scene_change"]["tilemap_name"].get_or(std::string("no_file")),
					trigger["components"]["scene_change"]["tilemap_image"].get_or(std::string("no_file")),
					trigger["components"]["scene_change"]["map_image"].get_or(std::string("no_file")),
					trigger["components"]["scene_change"]["entity_file"].get_or(std::string("no_file")),
					trigger["components"]["scene_change"]["trigger_file"].get_or(std::string("no_file")),
					trigger["components"]["scene_change"]["image_width"].get_or(0),
					trigger["components"]["scene_change"]["image_height"].get_or(0));
			}

			// Check if the trigger is a secret/Hidden Area
			sol::optional<sol::table> secret = trigger["components"]["secret"];
			if (secret != sol::nullopt)
			{
				newTrigger.AddComponent<SecretComponent>(
					trigger["components"]["secret"]["location_id"],
					trigger["components"]["secret"]["new_trigger"],
					trigger["components"]["secret"]["new_sprite_id"],
					trigger["components"]["secret"]["sprite_width"].get_or(0),
					trigger["components"]["secret"]["sprite_height"].get_or(0),
					trigger["components"]["secret"]["sprite_src_x"].get_or(0),
					trigger["components"]["secret"]["sprite_src_y"].get_or(0),
					glm::vec2(
						trigger["components"]["secret"]["start_pos"]["x"].get_or(0),
						trigger["components"]["secret"]["start_pos"]["y"].get_or(0)),
					trigger["components"]["secret"]["move_up"].get_or(false),
					trigger["components"]["secret"]["move_down"].get_or(false)
					);

				// Set the start postition to the transform position
				newTrigger.GetComponent<SecretComponent>().startPos = newTrigger.GetComponent<TransformComponent>().position;
			}

			// Is there a sprite Component
			if (ReadSpriteComponent(trigger, newTrigger))
			{
				newTrigger.AddComponent<GameComponent>();
			}
		}
		i++;
	}
}

void LevelLoader::SavePlayerDataToLuaTable(const std::string& saveNum)
{
	LuaTableWriter m_writer;
	std::fstream file;

	file.open("./Assets/SavedFiles/slot_" + saveNum + "/save" + saveNum + ".lua", std::ios::out | std::ios::trunc);

	// Start the Lua Table
	m_writer.WriteStartDocument();

	// Write a comment in Lua format
	m_writer.WriteCommentSeparation(file);
	m_writer.WriteCommentLine("Save file#: " + saveNum, file);
	m_writer.WriteCommentSeparation(file);
	m_writer.WriteDeclareTable("player_data", file);

	// Get the player so we can get their stats
	const auto& entity = reg.GetEntityByTag("player");
	// Player Transform
	const auto& transform = entity.GetComponent<TransformComponent>();
	int numHearts = entity.GetComponent<HealthComponent>().maxHearts;//RenderHealthSystem::numHearts;
	std::string name = "";

	// Start a new lua table
	m_writer.WriteStartTable(1, false, file);

	// Get the name of the player to save to the File --> Add this to the game class
	if (saveNum == "1") name = gameData.GetPlayer1Name();
	if (saveNum == "2") name = gameData.GetPlayer2Name();
	if (saveNum == "3") name = gameData.GetPlayer3Name();


	m_writer.WriteDeclareTable("menu_shared_values", file);
	m_writer.WriteKeyAndQuotedValue("name", name, file);
	m_writer.WriteKeyAndUnquotedValue("num_hearts", numHearts, file);

	// Get the game data
	auto& gameData = GameData::GetInstance();

	if (gameData.HasItem(GameData::GameItems::BLUE_RING)) m_writer.WriteKeyAndUnquotedValue("blue_ring", "true", file);
	else m_writer.WriteKeyAndUnquotedValue("blue_ring", "false", file);
	if (gameData.HasItem(GameData::GameItems::RED_RING)) m_writer.WriteKeyAndUnquotedValue("red_ring", "true", file);
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
	if (gameData.HasItem(GameData::GameItems::BOOMERANG))
		m_writer.WriteKeyAndUnquotedValue("boomerang", "true", file);
	else
		m_writer.WriteKeyAndUnquotedValue("boomerang", "false", file);
	// Magic Boomerang
	if (gameData.HasItem(GameData::GameItems::MAGIC_BOOMERANG))
		m_writer.WriteKeyAndUnquotedValue("magic_boomerang", "true", file);
	else
		m_writer.WriteKeyAndUnquotedValue("magic_boomerang", "false", file);
	// Wood Sword
	if (gameData.HasItem(GameData::GameItems::WOOD_SWORD))
		m_writer.WriteKeyAndUnquotedValue("wood_sword", "true", file);
	else
		m_writer.WriteKeyAndUnquotedValue("wood_sword", "false", file);
	// Steel Sword
	if (gameData.HasItem(GameData::GameItems::SILVER_SWORD))
		m_writer.WriteKeyAndUnquotedValue("steel_sword", "true", file);
	else
		m_writer.WriteKeyAndUnquotedValue("steel_sword", "false", file);
	// Magic Sword
	if (gameData.HasItem(GameData::GameItems::MAGIC_SWORD))
		m_writer.WriteKeyAndUnquotedValue("magic_sword", "true", file);
	else
		m_writer.WriteKeyAndUnquotedValue("magic_sword", "false", file);
	// Magic Rod
	if (gameData.HasItem(GameData::GameItems::MAGIC_ROD))
		m_writer.WriteKeyAndUnquotedValue("magic_rod", "true", file);
	else
		m_writer.WriteKeyAndUnquotedValue("magic_rod", "false", file);
	// Bombs
	if (gameData.HasItem(GameData::GameItems::BOMB))
		m_writer.WriteKeyAndUnquotedValue("bombs", "true", file);
	else
		m_writer.WriteKeyAndUnquotedValue("bombs", "false", file);
	// Food
	if (gameData.HasItem(GameData::GameItems::FOOD))
		m_writer.WriteKeyAndUnquotedValue("food", "true", file);
	else
		m_writer.WriteKeyAndUnquotedValue("food", "false", file);
	// Flute
	if (gameData.HasItem(GameData::GameItems::FLUTE))
		m_writer.WriteKeyAndUnquotedValue("flute", "true", file);
	else
		m_writer.WriteKeyAndUnquotedValue("flute", "false", file);
	// Raft
	if (gameData.HasItem(GameData::GameItems::RAFT))
		m_writer.WriteKeyAndUnquotedValue("raft", "true", file);
	else
		m_writer.WriteKeyAndUnquotedValue("raft", "false", file);
	// Ladder
	if (gameData.HasItem(GameData::GameItems::LADDER))
		m_writer.WriteKeyAndUnquotedValue("ladder", "true", file);
	else
		m_writer.WriteKeyAndUnquotedValue("ladder", "false", file);
	// Bow
	if (gameData.HasItem(GameData::GameItems::BOW))
		m_writer.WriteKeyAndUnquotedValue("bow_wood", "true", file);
	else
		m_writer.WriteKeyAndUnquotedValue("bow_wood", "false", file);
	// Magic Shield
	if (gameData.HasItem(GameData::GameItems::MAGIC_SHIELD))
		m_writer.WriteKeyAndUnquotedValue("magic_shield", "true", file);
	else
		m_writer.WriteKeyAndUnquotedValue("magic_shield", "false", file);
	// Power Braclet
	if (gameData.HasItem(GameData::GameItems::POWER_BRACLET))
		m_writer.WriteKeyAndUnquotedValue("power_braclet", "true", file);
	else
		m_writer.WriteKeyAndUnquotedValue("power_braclet", "false", file);
	// Map
	if (gameData.HasItem(GameData::GameItems::MAP))
		m_writer.WriteKeyAndUnquotedValue("map", "true", file);
	else
		m_writer.WriteKeyAndUnquotedValue("map", "false", file);
	// Blue Potion
	if (gameData.HasItem(GameData::GameItems::BLUE_POTION))
		m_writer.WriteKeyAndUnquotedValue("blue_potion", "true", file);
	else
		m_writer.WriteKeyAndUnquotedValue("blue_potion", "false", file);
	// Red Potion
	if (gameData.HasItem(GameData::GameItems::RED_POTION))
		m_writer.WriteKeyAndUnquotedValue("red_potion", "true", file);
	else
		m_writer.WriteKeyAndUnquotedValue("red_potion", "false", file);
	// Master Key
	if (gameData.HasItem(GameData::GameItems::MASTER_KEY))
		m_writer.WriteKeyAndUnquotedValue("master_key", "true", file);
	else
		m_writer.WriteKeyAndUnquotedValue("master_key", "false", file);

	// Close the items table
	m_writer.WriteEndTable(true, file);

	// Write the numbered values of current inventory and life hearts
	m_writer.WriteDeclareTable("inventory", file);
	m_writer.WriteKeyAndUnquotedValue("num_rupees", gameData.GetTotalRupees(), file);
	m_writer.WriteKeyAndUnquotedValue("num_bombs", gameData.GetTotalBombs(), file);
	m_writer.WriteKeyAndUnquotedValue("num_keys", gameData.GetTotalKeys(), file);
	// Close the remaining tables
	m_writer.WriteEndTable(false, file);
	m_writer.WriteEndTable(false, file);
	m_writer.WriteEndTable(false, file);

	file.close();
}

// This is for a new player -->Default values for items
void LevelLoader::SavePlayerNameToLuaTable(const std::string& saveNum, std::string& newName)
{
	LuaTableWriter m_writer;
	
	// Check to see if the directory exists and if not, create it
	if (!std::filesystem::is_directory("./Assets/SavedFiles/slot_" + saveNum) || !std::filesystem::exists("./Assets/SavedFiles/slot_" + saveNum))
		std::filesystem::create_directory("./Assets/SavedFiles/slot_" + saveNum);

	std::ofstream newFile("./Assets/SavedFiles/slot_" + saveNum + "/save" + saveNum + ".lua");
	newFile.close();

	std::fstream file;

	file.open("./Assets/SavedFiles/slot_" + saveNum + "/save" + saveNum + ".lua");

	// Start the Lua Table
	m_writer.WriteStartDocument();

	// Write a comment in Lua format
	m_writer.WriteCommentSeparation(file);
	m_writer.WriteCommentLine("Save file#: " + saveNum, file);
	m_writer.WriteCommentSeparation(file);
	m_writer.WriteDeclareTable("player_data", file);

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
	// Candle
	m_writer.WriteKeyAndUnquotedValue("candle", "false", file);
	// Bombs
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
	//m_writer.WriteKeyAndUnquotedValue("num_arrows", numArrows, file);
	m_writer.WriteKeyAndUnquotedValue("num_keys", numKeys, file);
	// Close the remaining tables
	m_writer.WriteEndTable(false, file);
	m_writer.WriteEndTable(false, file);
	m_writer.WriteEndTable(false, file);

	// Close the new lua file
	file.close();

	CreateNewGameSecretsFile(atoi(saveNum.c_str()));
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
			ReadTransformComponent(player, entity);
			// Box Collider Component
			ReadBoxColliderComponent(player, entity);
			// Rigid Body Component
			ReadRigidBodyComponent(player, entity);
			// Health Component
			ReadHealthComponent(player, entity);
			// Sprite
			ReadSpriteComponent(player, entity);
			// Animation  
			ReadAnimationComponent(player, entity);
			// Projectile Emitter  
			ReadProjectileEmitterComponent(player, entity);
			// Keyboard Control Component 
			ReadKeyboardControlComponent(player, entity);
			if (entity.HasTag("player") || entity.HasTag("the_shield") || entity.HasTag("the_sword"))
			{
				entity.AddComponent<CameraFollowComponent>();
				entity.AddComponent<PlayerComponent>();
			}
			entity.AddComponent<GameComponent>();
		}
		i++;
	}

	auto player = reg.GetEntityByTag("player");
	auto shield = reg.GetEntityByTag("the_shield");
	auto sword = reg.GetEntityByTag("the_sword");
	if (!game.CreateNewPlayer(player, sword, shield))
	{
		Logger::Err("LevelLoader -- Unable to create player!");
	}

}

void LevelLoader::LoadPlayerDataFromLuaTable(sol::state& lua, const std::string& fileName, int slotNum)
{
	sol::load_result script = lua.load_file("./Assets/SavedFiles/slot_" + std::to_string(slotNum) + "/" + fileName + ".lua");

	// This checks the syntax of our script, but it does not execute the script
	if (!script.valid())
	{
		sol::error err = script;
		std::string errorMessage = err.what();
		Logger::Err("Error loading the Lua Script: " + errorMessage);
		return;
	}

	// Executes the script using the sol State
	lua.script_file("./Assets/SavedFiles/slot_" + std::to_string(slotNum) + "/" + fileName + ".lua");

	sol::table data = lua["player_data"];

	int i = 1;
	// Get the Game data
	auto& gameData = GameData::GetInstance();

	while (true)
	{
		const auto& entity = reg.GetEntityByTag("player");

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
			HealthSystem::loadedHearts = player["menu_shared_values"]["num_hearts"].get_or(3);
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
			auto wood_boomerang = player["items"]["boomerang"].get_or(false);
			if (wood_boomerang)
				gameData.AddItem(GameData::GameItems::BOOMERANG);

			auto magic_boomerang = player["items"]["magic_boomerang"].get_or(false);
			if (magic_boomerang)
				gameData.AddItem(GameData::GameItems::BOOMERANG);

			auto wood_sword = player["items"]["wood_sword"].get_or(false);
			if (wood_sword)
				gameData.AddItem(GameData::GameItems::WOOD_SWORD);

			auto steel_sword = player["items"]["steel_sword"].get_or(false);
			if (steel_sword)
				gameData.AddItem(GameData::GameItems::SILVER_SWORD);

			auto magic_sword = player["items"]["magic_sword"].get_or(false);
			if (magic_sword)
				gameData.AddItem(GameData::GameItems::MAGIC_SWORD);

			auto magic_rod = player["items"]["magic_rod"].get_or(false);
			if (magic_rod)
				gameData.AddItem(GameData::GameItems::MAGIC_ROD);

			auto bombs = player["items"]["bombs"].get_or(false);
			if (bombs)
				gameData.AddItem(GameData::GameItems::BOMB);

			auto food = player["items"]["food"].get_or(false);
			if (food)
				gameData.AddItem(GameData::GameItems::FOOD);

			auto flute = player["items"]["flute"].get_or(false);
			if (flute)
				gameData.AddItem(GameData::GameItems::FLUTE);

			auto raft = player["items"]["raft"].get_or(false);
			if (raft)
				gameData.AddItem(GameData::GameItems::RAFT);

			auto ladder = player["items"]["ladder"].get_or(false);
			if (ladder)
				gameData.AddItem(GameData::GameItems::LADDER);

			auto bow = player["items"]["bow_wood"].get_or(false);
			if (bow)
				gameData.AddItem(GameData::GameItems::BOW);

			auto magic_bow = player["items"]["bow_magic"].get_or(false);
			if (magic_bow)
				gameData.AddItem(GameData::GameItems::MAGIC_ARROWS);

			auto shield = player["items"]["shield"].get_or(false);
			if (shield)
				gameData.AddItem(GameData::GameItems::SHIELD);

			auto magic_shield = player["items"]["magic_shield"].get_or(false);
			if (magic_bow)
				gameData.AddItem(GameData::GameItems::MAGIC_SHIELD);

			auto power_braclet = player["items"]["power_braclet"].get_or(false);
			if (power_braclet)
				gameData.AddItem(GameData::GameItems::POWER_BRACLET);

			auto map = player["items"]["map"].get_or(false);
			if (map)
				gameData.AddItem(GameData::GameItems::MAP);

			auto blue_potion = player["items"]["blue_potion"].get_or(false);
			if (blue_potion)
				gameData.AddItem(GameData::GameItems::BLUE_POTION);

			auto red_potion = player["items"]["red_potion"].get_or(false);
			if (red_potion)
				gameData.AddItem(GameData::GameItems::RED_POTION);

			auto master_key = player["items"]["master_key"].get_or(false);
			if (master_key)
				gameData.AddItem(GameData::GameItems::MASTER_KEY);

			auto candle = player["items"]["candle"].get_or(false);
			
			if (candle)
				gameData.AddItem(GameData::GameItems::CANDLE);

		}
		// Inventory
		sol::optional<sol::table> inventory = player["inventory"];
		if (inventory != sol::nullopt)
		{
			gameData.AddRupees(player["inventory"]["num_rupees"].get_or(0));
			gameData.AddBombs(player["inventory"]["num_bombs"].get_or(0));
			gameData.AddKeys(player["inventory"]["num_keys"].get_or(0));
		}
		i++;
	}
}

void LevelLoader::LoadEnemiesFromLuaTable(sol::state& lua, const std::string& fileName)
{
	sol::load_result script = lua.load_file("./Assets/Levels/Enemies/" + fileName + ".lua");
	// This checks the syntax of our script, but it does not execute the script
	if (!script.valid())
	{
		sol::error err = script;
		std::string errorMessage = err.what();
		Logger::Err("Error loading the Lua Script: " + errorMessage);
		return;
	}

	// Executes the script using the sol State
	lua.script_file("./Assets/Levels/Enemies/" + fileName + ".lua");

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
			ReadTransformComponent(entity, newEntity);
			// RigidBody
			ReadRigidBodyComponent(entity, newEntity);
			// Sprite
			ReadSpriteComponent(entity, newEntity);
			// Animation  
			ReadAnimationComponent(entity, newEntity);
			// Health Component 
			ReadHealthComponent(entity, newEntity);
			// Box Collider 
			ReadBoxColliderComponent(entity, newEntity);
			// Projectile Emitter  
			ReadProjectileEmitterComponent(entity, newEntity);
			// Script Component 
			sol::optional<sol::table> script = entity["components"]["on_update_script"];
			if (script != sol::nullopt)
			{
				sol::function func = entity["components"]["on_update_script"][0];
				newEntity.AddComponent<ScriptComponent>(func);
			}

			// AI Component
			sol::optional<sol::table> ai = entity["components"]["ai_component"];
			if (ai != sol::nullopt)
			{
				AIComponent::EnemyType type = ConvertStringToEnemyType(entity["components"]["ai_component"]["enemy_type"].get_or(std::string("no_type")));
				AIComponent::EnemyBossType boss_type = ConvertStringToEnemyBossType(entity["components"]["ai_component"]["enemy_boss_type"].get_or(std::string("not_a_boss")));

				newEntity.AddComponent<AIComponent>(
					glm::vec2(
						entity["components"]["ai_component"]["enemy_pos"]["x"].get_or(0),
						entity["components"]["ai_component"]["enemy_pos"]["y"].get_or(0)),
					type,
					boss_type,
					entity["components"]["ai_component"]["is_boss"].get_or(false)
					);
			}
			
			// Add Render Component based on the current Application State
			if (game.GetStateMachine()->GetCurrentState() == "GAMESTATE")
				newEntity.AddComponent<GameComponent>();
			else if (game.GetStateMachine()->GetCurrentState() == "EDITOR")
				newEntity.AddComponent<EditorComponent>();

			sol::optional<sol::table>enemy_component = entity["components"]["enemy_component"];
			if (enemy_component != sol::nullopt)
			{

				std::string move_dir = entity["components"]["enemy_component"]["move_dir"];
				EnemyComponent::MoveDir moveDir = EnemyComponent::MoveDir::NO_MOVE;
				if (move_dir == "up")
					moveDir = EnemyComponent::MoveDir::UP;
				else if (move_dir == "right")
					moveDir = EnemyComponent::MoveDir::RIGHT;
				else if (move_dir == "down")
					moveDir = EnemyComponent::MoveDir::DOWN;
				else if (move_dir == "left")
					moveDir = EnemyComponent::MoveDir::LEFT;
				else
					moveDir = EnemyComponent::MoveDir::NO_MOVE;

				newEntity.AddComponent<EnemyComponent>(
						entity["components"]["enemy_component"]["max_move_distance"].get_or(0),
						moveDir,
					glm::vec2(
						entity["components"]["enemy_component"]["start_pos"]["x"].get_or(0),
						entity["components"]["enemy_component"]["start_pos"]["y"].get_or(0)
						)
					);
			}
			else
			{
				newEntity.AddComponent<EnemyComponent>();
			}
		}
		i++;
	}
}

void LevelLoader::SaveSecrets()
{
	LuaTableWriter writer;
	std::fstream file;
	auto& gameData = GameData::GetInstance();
	auto playerNum = gameData.PlayerNum();
	file.open("./Assets/SavedFiles/slot_" + std::to_string(playerNum) + "/GameSecrets_" + std::to_string(playerNum) + ".lua", std::ios::out);
	writer.WriteStartDocument();
	writer.WriteCommentSeparation(file);
	writer.WriteCommentLine("Game Secrets", file);
	writer.WriteCommentSeparation(file);

	writer.WriteDeclareTable("secrets", file);
	int i = 1;
	for (auto& secret : gameData.GetGameSecrets())
	{

		std::string found = "";
		if (secret.second)
			found = "true";
		else
			found = "false";

		writer.WriteStartTable(i, false, file);
		writer.WriteKeyAndQuotedValue("location", secret.first, file);
		writer.WriteKeyAndUnquotedValue("found", found, file);
		writer.WriteEndTableWithSeparator(false, file);
		i++;
	}
	writer.WriteEndTable(false, file);
	writer.WriteEndDocument(file);
	file.close();
}

void LevelLoader::ReadInSecrets(sol::state& lua)
{
	auto& gameData = GameData::GetInstance();
	auto playerNum = gameData.PlayerNum();
	sol::load_result script = lua.load_file("./Assets/SavedFiles/slot_" + std::to_string(playerNum) + "/GameSecrets_" + std::to_string(playerNum) + ".lua");

	if (!script.valid())
	{
		sol::error err = script;
		std::string errorMessage = err.what();
		Logger::Err("Error Reading in Secrets from Lua Table: " + errorMessage);
		return;
	}

	// Execute the script
	lua.script_file("./Assets/SavedFiles/slot_"+ std::to_string(playerNum) + "/GameSecrets_" + std::to_string(playerNum) + ".lua");
	sol::table data = lua["secrets"];

	int i = 1;
	while (true)
	{
		sol::optional<sol::table> hasData = data[i];
		if (hasData == sol::nullopt)
		{
			break;
		}

		sol::table secrets = data[i];
		gameData.AddGameSecrets(secrets["location"], secrets["found"].get_or(false));

		i++;
	}

	if (!Registry::Instance().DoesGroupExist("secret"))
		return;

	for (auto& entity : Registry::Instance().GetEntitiesByGroup("secret"))
	{
		auto& secret = entity.GetComponent<SecretComponent>();
		if (gameData.IsSecretFound(secret.locationID))
		{
			entity.GetComponent<SecretComponent>().found = true;
			// Call the Trigger Function
			if (entity.GetComponent<SecretComponent>().found)
			{
				Registry::Instance().GetSystem<TriggerSystem>().SecretTrigger(entity, true);
			}
		}
		else
			continue;
	}
}

void LevelLoader::LoadHUDFromLuaTable(sol::state& lua, const std::string& fileName)
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
			ReadTransformComponent(hudData, newHUDObject);


			// Add Sprite Component
			ReadSpriteComponent(hudData, newHUDObject);
			newHUDObject.AddComponent<HUDComponenet>();
		}
		i++;
	}
}

void LevelLoader::LoadAssetsFromLuaTable(sol::state& lua, const std::string& fileName)
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
ItemComponent::ItemCollectType LevelLoader::ConvertLuaStringToItem(std::string& type)
{
	if (type == "default")
		return ItemComponent::ItemCollectType::DEFAULT;
	else if (type == "bombs")
		return ItemComponent::ItemCollectType::BOMBS;
	else if (type == "heart")
		return ItemComponent::ItemCollectType::HEARTS;
	else if (type == "keys")
		return ItemComponent::ItemCollectType::KEYS;
	else
		return ItemComponent::ItemCollectType::DEFAULT;
}
ItemComponent::SpecialItemType LevelLoader::ConvertLuaStringToSpecial(std::string& special)
{
	if (special == "none")
		return ItemComponent::SpecialItemType::NOT_SPECIAL;
	else if (special == "wood_sword")
		return ItemComponent::SpecialItemType::WOOD_SWORD;
	else if (special == "steel_sword")
		return ItemComponent::SpecialItemType::STEEL_SWORD;
	else if (special == "magic_sword")
		return ItemComponent::SpecialItemType::MAGIC_SWORD;
	else if (special == "full_heart")
		return ItemComponent::SpecialItemType::FULL_HEART;
	else if (special == "raft")
		return ItemComponent::SpecialItemType::RAFT;
	else if (special == "power_braclet")
		return ItemComponent::SpecialItemType::POWER_BRACLET;
	else if (special == "red_candle")
		return ItemComponent::SpecialItemType::RED_CANDLE;
	else if (special == "wood_boomerang")
		return ItemComponent::SpecialItemType::WOOD_BOOMERANG;
	else if (special == "magic_boomerang")
		return ItemComponent::SpecialItemType::MAGIC_BOOMERANG;
	else if (special == "ladder")
		return ItemComponent::SpecialItemType::LADDER;
	else if (special == "arrows")
		return ItemComponent::SpecialItemType::ARROWS;
	else if (special == "wood_bow")
		return ItemComponent::SpecialItemType::WOOD_BOW;
	else if (special == "triforce_piece")
		return ItemComponent::SpecialItemType::TRIFORCE_PIECE;
	else
		return ItemComponent::SpecialItemType::NOT_SPECIAL;
}

bool LevelLoader::CheckForItemInInventory(ItemComponent::SpecialItemType& type)
{
	auto& gameData = GameData::GetInstance();
	switch (type)
	{
	case ItemComponent::SpecialItemType::WOOD_SWORD:
		return gameData.HasItem(GameData::GameItems::WOOD_SWORD);
	case ItemComponent::SpecialItemType::LADDER:
		return gameData.HasItem(GameData::GameItems::LADDER);
	case ItemComponent::SpecialItemType::WOOD_BOOMERANG:
		return gameData.HasItem(GameData::GameItems::BOOMERANG);
	case ItemComponent::SpecialItemType::MAGIC_BOOMERANG:
		return gameData.HasItem(GameData::GameItems::MAGIC_BOOMERANG);
	case ItemComponent::SpecialItemType::STEEL_SWORD:
		return gameData.HasItem(GameData::GameItems::SILVER_SWORD);
	case ItemComponent::SpecialItemType::WOOD_BOW:
		return gameData.HasItem(GameData::GameItems::BOW);
	default:
		return false;
	}
}

void LevelLoader::LoadEntitiesFromLuaTable(sol::state& lua, const std::string& filename)
{
	sol::load_result script = lua.load_file("./Assets/Levels/Entities/" + filename + ".lua");

	// This checks the syntax of our script, but it does not execute the script
	if (!script.valid())
	{
		sol::error err = script;
		std::string errorMessage = err.what();
		Logger::Err("Error loading the Lua Script: " + errorMessage);
		return;
	}

	// Executes the script using the sol State
	lua.script_file("./Assets/Levels/Entities/" + filename + ".lua");

	sol::table data = lua["level"];

	int i = 1;

	while (true)
	{
		sol::optional<sol::table> hasData = data[i];
		if (hasData == sol::nullopt)
		{
			break;
		}

		sol::table lvlData = data[i];

		// Check to see if the special item has been loaded, if it has break out!
		sol::optional<std::string> level_item = lvlData["level_item"];

		if (level_item != sol::nullopt)
		{
			std::string item = lvlData["level_item"];
			ItemComponent::SpecialItemType spec = ConvertLuaStringToSpecial(item);

			if (CheckForItemInInventory(spec))
			{
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
			ReadTransformComponent(lvlData, newLvlObject);
			// Add Sprite Component
			ReadSpriteComponent(lvlData, newLvlObject);
			// Add Animation Component
			ReadAnimationComponent(lvlData, newLvlObject);
			// Box Collider 
			ReadBoxColliderComponent(lvlData, newLvlObject);
			// Trigger Component
			sol::optional<sol::table> triggerBox = lvlData["components"]["trigger_box"];
			if (triggerBox != sol::nullopt)
			{
				//std::string type = trigger["components"]["trigger_box"]["trigger_type"];
				//Logger::Log(lvlData["components"]["trigger_box"]["trigger_file"]);
				TriggerBoxComponent::TriggerType triggerType = ConvertStringToTriggerType(lvlData["components"]["trigger_box"]["trigger_type"]);

				newLvlObject.AddComponent<TriggerBoxComponent>(
					triggerType,
					glm::vec2(
						lvlData["components"]["trigger_box"]["transport_offset"]["x"].get_or(0.0),
						lvlData["components"]["trigger_box"]["transport_offset"]["y"].get_or(0.0)
					),
					glm::vec2(
						lvlData["components"]["trigger_box"]["camera_offset"]["x"].get_or(0.0),
						lvlData["components"]["trigger_box"]["camera_offset"]["y"].get_or(0.0)
					)
					);
			}

			// Item Type Component
			sol::optional<sol::table> item_comp = lvlData["components"]["item"];
			if (item_comp != sol::nullopt)
			{
				std::string specialType = lvlData["components"]["item"]["special"];
				std::string itemType = lvlData["components"]["item"]["item_type"];
				auto type = ConvertLuaStringToItem(itemType);
				auto special = ConvertLuaStringToSpecial(specialType);

				newLvlObject.AddComponent<ItemComponent>(
					type,
					special,
					lvlData["components"]["item"]["price"].get_or(0)
					);

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
					lvlData["components"]["caption"]["x_pos"].get_or(0),
					lvlData["components"]["caption"]["y_pos"].get_or(0),
					lvlData["components"]["caption"]["is_number"].get_or(false)
					);
			}

			// Scene Change component
			sol::optional<sol::table> scene = lvlData["components"]["scene_change"];

			// Check if the trigger has a scene component
			if (scene != sol::nullopt)
			{
				newLvlObject.AddComponent<SceneChangeComponent>(
					lvlData["components"]["scene_change"]["level_music"].get_or(std::string("stop")),
					lvlData["components"]["scene_change"]["asset_file"].get_or(std::string("no_file")),
					lvlData["components"]["scene_change"]["enemy_file"].get_or(std::string("no_file")),
					lvlData["components"]["scene_change"]["collider_file"].get_or(std::string("no_file")),
					lvlData["components"]["scene_change"]["tilemap_name"].get_or(std::string("no_file")),
					lvlData["components"]["scene_change"]["tilemap_image"].get_or(std::string("no_file")),
					lvlData["components"]["scene_change"]["map_image"].get_or(std::string("no_file")),
					lvlData["components"]["scene_change"]["entity_file"].get_or(std::string("no_file")),
					lvlData["components"]["scene_change"]["trigger_file"].get_or(std::string("no_file")),
					lvlData["components"]["scene_change"]["image_width"].get_or(0),
					lvlData["components"]["scene_change"]["image_height"].get_or(0));
			}
		}
		i++;
	}
}

// This is a letter parser to deal with the SDL_Text issues
void LevelLoader::ConvertName(const std::string& name, int x, int y)
{
	int num = name.length();
	// Clamp Num at 7
	if (num > 7)
		num = 7;
	// Variable for spaces in between letters
	int space = 0;

	// Parse the string and set the srcRect position
	for (int i = 0; i < num; i++)
	{
		char letter = std::toupper(name[i]);

		Entity nameEnt = reg.CreateEntity();

		nameEnt.Group(name);
		nameEnt.AddComponent<SpriteComponent>("caption_letters", 16, 16, 0, false, 0, 0);
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
