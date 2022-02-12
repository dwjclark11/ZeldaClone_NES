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
#include <SDL.h>

LevelLoader::LevelLoader()
	: game(Game::Instance()), reg(Registry::Instance())
{
}

LevelLoader::~LevelLoader()
{
}

void LevelLoader::LoadMap(std::string mapName, int image_width, int image_height, bool offset)
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
	Registry::Instance().GetSystem<SoundFXSystem>().PlaySoundFX(game.GetAssetManager(), "Eliminate", 0, 1);
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
		//Logger::Err("Has Box Collider Component");
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
			table["components"]["health"]["health_percentage"].get_or(9),
			table["components"]["health"]["max_hearts"].get_or(3));
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
			std::string tempTag = ui["tag"];
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
	Logger::Log("Loading Menu assets!!");
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

		mapFile >> group >> tranX >> tranY >> colliderScaleX >> colliderScaleY >> colWidth >> colHeight >> offset.x >> offset.y;

		// If we are at the end of the File --> Leave the File!!
		if (mapFile.eof()) break;

		// Create a new entity for each tile
		Entity boxCollider = reg.CreateEntity();
		boxCollider.Group(group);
		boxCollider.AddComponent<TransformComponent>(glm::vec2(tranX, tranY), glm::vec2(colliderScaleX, colliderScaleY), 0.0);
		boxCollider.AddComponent<GameComponent>();
		boxCollider.AddComponent<ColliderComponent>();
		boxCollider.AddComponent<BoxColliderComponent>(colWidth, colHeight, glm::vec2(offset.x, offset.y));
	}
	mapFile.close();
}

TriggerBoxComponent::TriggerType LevelLoader::ConvertStringToTriggerType(std::string type)
{
	if (type == "no_trigger")
		return TriggerBoxComponent::TriggerType::NO_TRIGGER;
	else if (type == "secret_area")
		return TriggerBoxComponent::TriggerType::SECRET_AREA;
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
}

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

//std::string LevelLoader::SetName(std::string filePath, bool wExtension, char separator)
//{
//	// Get the last '.' position
//	std::size_t sepPos = filePath.rfind(separator);
//	std::size_t found = filePath.find_last_of('.');
//	if (sepPos != std::string::npos)
//	{
//		std::string name = filePath = filePath.substr(0, found);
//		return name.substr(sepPos + 1);
//	}
//	return "";
//}

void LevelLoader::LoadTriggers(sol::state& lua, const std::string& fileName)
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

		sol::table trigger = triggers[i];

		Entity newTrigger = reg.CreateEntity();

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
				//std::string type = trigger["components"]["trigger_box"]["trigger_type"];
				Logger::Log(trigger["components"]["trigger_box"]["trigger_file"]);
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
					trigger["components"]["trigger_box"]["level_music"],
					trigger["components"]["trigger_box"]["asset_file"],
					trigger["components"]["trigger_box"]["enemy_file"],
					trigger["components"]["trigger_box"]["collider_file"],
					trigger["components"]["trigger_box"]["tilemap_name"],
					trigger["components"]["trigger_box"]["tilemap_image"],
					trigger["components"]["trigger_box"]["entity_file"],
					trigger["components"]["trigger_box"]["image_width"].get_or(0),
					trigger["components"]["trigger_box"]["image_height"].get_or(0),
					trigger["components"]["trigger_box"]["trigger_file"],
					trigger["components"]["trigger_box"]["collider"].get_or(false)
					);
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
					trigger["components"]["secret"]["sprite_src_y"].get_or(0)
					);


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
	int numHearts = entity.GetComponent<HealthComponent>().maxHearts;//RenderHealthSystem::numHearts;
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

	if (game.GetGameItems().blueRing) m_writer.WriteKeyAndUnquotedValue("blue_ring", "true", file);
	else m_writer.WriteKeyAndUnquotedValue("blue_ring", "false", file);
	if (game.GetGameItems().redRing) m_writer.WriteKeyAndUnquotedValue("red_ring", "true", file);
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
	if (game.GetGameItems().woodBoomerang)
		m_writer.WriteKeyAndUnquotedValue("boomerang", "true", file);
	else
		m_writer.WriteKeyAndUnquotedValue("boomerang", "false", file);
	// Magic Boomerang
	if (game.GetGameItems().magicBoomerang)
		m_writer.WriteKeyAndUnquotedValue("magic_boomerang", "true", file);
	else
		m_writer.WriteKeyAndUnquotedValue("magic_boomerang", "false", file);
	// Wood Sword
	if (game.GetGameItems().woodSword)
		m_writer.WriteKeyAndUnquotedValue("wood_sword", "true", file);
	else
		m_writer.WriteKeyAndUnquotedValue("wood_sword", "false", file);
	// Steel Sword
	if (game.GetGameItems().steelSword)
		m_writer.WriteKeyAndUnquotedValue("steel_sword", "true", file);
	else
		m_writer.WriteKeyAndUnquotedValue("steel_sword", "false", file);
	// Magic Sword
	if (game.GetGameItems().magicSword)
		m_writer.WriteKeyAndUnquotedValue("magic_sword", "true", file);
	else
		m_writer.WriteKeyAndUnquotedValue("magic_sword", "false", file);
	// Magic Rod
	if (game.GetGameItems().magicRod)
		m_writer.WriteKeyAndUnquotedValue("magic_rod", "true", file);
	else
		m_writer.WriteKeyAndUnquotedValue("magic_rod", "false", file);
	// Bombs
	if (game.GetGameItems().bombs)
		m_writer.WriteKeyAndUnquotedValue("bombs", "true", file);
	else
		m_writer.WriteKeyAndUnquotedValue("bombs", "false", file);
	// Food
	if (game.GetGameItems().food)
		m_writer.WriteKeyAndUnquotedValue("food", "true", file);
	else
		m_writer.WriteKeyAndUnquotedValue("food", "false", file);
	// Flute
	if (game.GetGameItems().flute)
		m_writer.WriteKeyAndUnquotedValue("flute", "true", file);
	else
		m_writer.WriteKeyAndUnquotedValue("flute", "false", file);
	// Raft
	if (game.GetGameItems().raft)
		m_writer.WriteKeyAndUnquotedValue("raft", "true", file);
	else
		m_writer.WriteKeyAndUnquotedValue("raft", "false", file);
	// Ladder
	if (game.GetGameItems().ladder)
		m_writer.WriteKeyAndUnquotedValue("ladder", "true", file);
	else
		m_writer.WriteKeyAndUnquotedValue("ladder", "false", file);
	// Bow
	if (game.GetGameItems().bow)
		m_writer.WriteKeyAndUnquotedValue("bow_wood", "true", file);
	else
		m_writer.WriteKeyAndUnquotedValue("bow_wood", "false", file);
	// Magic Shield
	if (game.GetGameItems().magicShield)
		m_writer.WriteKeyAndUnquotedValue("magic_shield", "true", file);
	else
		m_writer.WriteKeyAndUnquotedValue("magic_shield", "false", file);
	// Power Braclet
	if (game.GetGameItems().powerBraclet)
		m_writer.WriteKeyAndUnquotedValue("power_braclet", "true", file);
	else
		m_writer.WriteKeyAndUnquotedValue("power_braclet", "false", file);
	// Map
	if (game.GetGameItems().map)
		m_writer.WriteKeyAndUnquotedValue("map", "true", file);
	else
		m_writer.WriteKeyAndUnquotedValue("map", "false", file);
	// Blue Potion
	if (game.GetGameItems().bluePotion)
		m_writer.WriteKeyAndUnquotedValue("blue_potion", "true", file);
	else
		m_writer.WriteKeyAndUnquotedValue("blue_potion", "false", file);
	// Red Potion
	if (game.GetGameItems().redPotion)
		m_writer.WriteKeyAndUnquotedValue("red_potion", "true", file);
	else
		m_writer.WriteKeyAndUnquotedValue("red_potion", "false", file);
	// Master Key
	if (game.GetGameItems().masterKey)
		m_writer.WriteKeyAndUnquotedValue("master_key", "true", file);
	else
		m_writer.WriteKeyAndUnquotedValue("master_key", "false", file);

	// Close the items table
	m_writer.WriteEndTable(true, file);

	// Write the numbered values of current inventory and life hearts
	m_writer.WriteDeclareTable("inventory", file);
	m_writer.WriteKeyAndUnquotedValue("num_rupees", GameState::totalRupees, file);
	m_writer.WriteKeyAndUnquotedValue("num_bombs", GameState::totalBombs, file);
	m_writer.WriteKeyAndUnquotedValue("num_keys", GameState::totalKeys, file);
	m_writer.WriteEndTable(false, file);
	m_writer.WriteEndTable(false, file);
	m_writer.WriteEndTable(false, file);

	file.close();
}

// This is for a new player -->Default values for items
void LevelLoader::SavePlayerNameToLuaTable(std::string saveNum, std::string& newName)
{
	LuaTableWriter m_writer;

	std::ofstream newFile("./Assets/SavedFiles/save" + saveNum + ".lua");
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
			game.GetGameItems().woodBoomerang = player["items"]["boomerang"].get_or(false);
			game.GetGameItems().magicBoomerang = player["items"]["magic_boomerang"].get_or(false);
			game.GetGameItems().woodSword = player["items"]["wood_sword"].get_or(false);
			game.GetGameItems().steelSword = player["items"]["steel_sword"].get_or(false);
			game.GetGameItems().magicSword = player["items"]["magic_sword"].get_or(false);
			game.GetGameItems().magicRod = player["items"]["magic_rod"].get_or(false);
			game.GetGameItems().bombs = player["items"]["bombs"].get_or(false);
			game.GetGameItems().food = player["items"]["food"].get_or(false);
			game.GetGameItems().flute = player["items"]["flute"].get_or(false);
			game.GetGameItems().raft = player["items"]["raft"].get_or(false);
			game.GetGameItems().ladder = player["items"]["ladder"].get_or(false);
			game.GetGameItems().bow = player["items"]["bow_wood"].get_or(false);
			game.GetGameItems().magicBow = player["items"]["bow_magic"].get_or(false);
			game.GetGameItems().shield = player["items"]["shield"].get_or(false);
			game.GetGameItems().magicShield = player["items"]["magic_shield"].get_or(false);
			game.GetGameItems().powerBraclet = player["items"]["power_braclet"].get_or(false);
			game.GetGameItems().map = player["items"]["map"].get_or(false);
			game.GetGameItems().bluePotion = player["items"]["blue_potion"].get_or(false);
			game.GetGameItems().redPotion = player["items"]["red_potion"].get_or(false);
			game.GetGameItems().masterKey = player["items"]["master_key"].get_or(false);
			game.GetGameItems().candle = player["items"]["candle"].get_or(false);
		}
		// Inventory
		sol::optional<sol::table> inventory = player["inventory"];
		if (inventory != sol::nullopt)
		{
			GameState::totalRupees = player["inventory"]["num_rupees"].get_or(0);
			GameState::totalBombs = player["inventory"]["num_bombs"].get_or(0);
			GameState::totalKeys = player["inventory"]["num_keys"].get_or(0);
		}
		i++;
		Logger::Log("Player Loaded Today");
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

void LevelLoader::SaveSecrets()
{
	LuaTableWriter writer;
	std::fstream file;
	file.open("./Assets/SavedFiles/GameSecrets_" + std::to_string(game.GetPlayerNum()) + ".lua", std::ios::out);
	writer.WriteStartDocument();
	writer.WriteCommentSeparation(file);
	writer.WriteCommentLine("Game Secrets", file);
	writer.WriteCommentSeparation(file);

	writer.WriteDeclareTable("secrets", file);
	int i = 1;
	for (auto& secret : game.GetGameSecrets())
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
	sol::load_result script = lua.load_file("./Assets/SavedFiles/GameSecrets_" + std::to_string(game.GetPlayerNum()) + ".lua");

	if (!script.valid())
	{
		sol::error err = script;
		std::string errorMessage = err.what();
		Logger::Err("Error Reading in Secrets from Lua Table: " + errorMessage);
		return;
	}

	// Execute the script
	lua.script_file("./Assets/SavedFiles/GameSecrets_" + std::to_string(game.GetPlayerNum()) + ".lua");
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
		game.AddGameSecrets(secrets["location"], secrets["found"].get_or(false));

		i++;
	}

	for (auto& entity : Registry::Instance().GetEntitiesByGroup("secret"))
	{
		auto& secret = entity.GetComponent<SecretComponent>();
		if (game.IsSecretFound(secret.locationID))
		{
			entity.GetComponent<SecretComponent>().found = true;
			// Call the Trigger Function
			if (entity.GetComponent<SecretComponent>().found)
			{
				Registry::Instance().GetSystem<TriggerSystem>().SecretTrigger(entity, true);
				Logger::Log("Loading Secrets");
			}
				
			
		}
		else
			continue;
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
			ReadTransformComponent(hudData, newHUDObject);


			// Add Sprite Component
			ReadSpriteComponent(hudData, newHUDObject);
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
ItemComponent::ItemCollectType LevelLoader::ConvertLuaStringToItem(std::string& type)
{
	if (type == "default")
		return ItemComponent::ItemCollectType::DEFAULT;
	else if (type == "bombs")
		return ItemComponent::ItemCollectType::BOMBS;
	else if (type == "heart")
		return ItemComponent::ItemCollectType::HEARTS;
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
	else
		return ItemComponent::SpecialItemType::NOT_SPECIAL;
}

bool LevelLoader::CheckForItemInInventory(ItemComponent::SpecialItemType& type)
{
	switch (type)
	{
	case ItemComponent::SpecialItemType::WOOD_SWORD:
		if (game.GetGameItems().woodSword)
			return true;
		else
			return false;
		break;
	case ItemComponent::SpecialItemType::LADDER:
		if (game.GetGameItems().ladder)
			return true;
		else
			return false;
		break;
	case ItemComponent::SpecialItemType::WOOD_BOOMERANG:
		if (game.GetGameItems().woodBoomerang)
			return true;
		else
			return false;
		break;
	case ItemComponent::SpecialItemType::MAGIC_BOOMERANG:
		if (game.GetGameItems().magicBoomerang)
			return true;
		else
			return false;
		break;
	case ItemComponent::SpecialItemType::STEEL_SWORD:
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
			ItemComponent::SpecialItemType spec = ConvertLuaStringToSpecial(item);

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
				Logger::Log(lvlData["components"]["trigger_box"]["trigger_file"]);
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
					),
					lvlData["components"]["trigger_box"]["level_music"],
					lvlData["components"]["trigger_box"]["asset_file"],
					lvlData["components"]["trigger_box"]["enemy_file"],
					lvlData["components"]["trigger_box"]["collider_file"],
					lvlData["components"]["trigger_box"]["tilemap_name"],
					lvlData["components"]["trigger_box"]["tilemap_image"],
					lvlData["components"]["trigger_box"]["entity_file"],
					lvlData["components"]["trigger_box"]["image_width"].get_or(0),
					lvlData["components"]["trigger_box"]["image_height"].get_or(0),
					lvlData["components"]["trigger_box"]["trigger_file"]
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
		num = 7;
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
