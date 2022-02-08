#include "EditorFileLoader.h"
#include "../Components/TransformComponent.h"
#include "../Components/SpriteComponent.h"
#include "../Components/BoxColliderComponent.h"
#include "../Components/KeyboardControlComponent.h"
#include "../Components/RigidBodyComponent.h"
#include "../Components/AnimationComponent.h"
#include "../Components/ScriptComponent.h"
#include "../Components/ProjectileEmitterComponent.h"
#include "../Components/HealthComponent.h"
#include "../Components/EditorComponent.h"
#include "../Components/AIComponent.h"
#include "../Components/SecretComponent.h"

#include "../Game/LevelLoader.h"

#include <string>
#include <iostream>
#include <fstream>
#include "../Systems/EditorSystems/MouseControlSystem.h"
#include <filesystem>
EditorFileLoader::EditorFileLoader()
	: reg(Registry::Instance())
{
	RequiredComponent<TransformComponent>();
	RequiredComponent<SpriteComponent>();
	Logger::Log("Level Loader Constructor Run");
}

EditorFileLoader::~EditorFileLoader()
{

}

void EditorFileLoader::LoadTilemap(const std::unique_ptr<AssetManager>& assetManager, SDL_Renderer* renderer)
{
	std::string filename = std::filesystem::path(imageName).stem().string();
	// Adding Textures to the asset Manager
	assetManager->AddTextures(renderer, "image-Name", imageName);
	Logger::Err(filename);
	
	// Load the tilemap
	int tileSize = 16;
	
	// Open and read the text file
	std::fstream mapFile;
	mapFile.open(fileName);
	
	// Check to see if the file opened correctly
	if (!mapFile.is_open())
	{
		Logger::Err("Unable to open [" + fileName + "]");
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
		std::string assetID = "";
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
		tile.Group(group);
		tile.AddComponent<SpriteComponent>("image-Name", tileWidth, tileHeight, zIndex, false, srcRectX, srcRectY);
		tile.AddComponent<TransformComponent>(glm::vec2(tranX, tranY), glm::vec2(tileScaleX, tileScaleX), 0.0);
		tile.AddComponent<EditorComponent>();

		// If the tile is a collider, add a boxColliderComponent
		if (collider)
		{
			tile.AddComponent<BoxColliderComponent>(colWidth, colHeight, glm::vec2(offset.x, offset.y));
		}
	}
	// Close the file
	mapFile.close();
}

void EditorFileLoader::LoadObjectMap(const std::unique_ptr<AssetManager>& assetManager, SDL_Renderer* renderer)
{
	// Load the tilemap
	int tileSize = 32; // This is considered a magic number!!
	
	// Open and read the text file
	std::fstream mapFile;
	mapFile.open(fileName);
	
	// Check to see if the file opened correctly
	if (!mapFile.is_open())
	{
		Logger::Err("Unable to open [" + fileName + "]");
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
		std::string group = "";
		std::string assetID = "";
		glm::vec2 offset = glm::vec2(0, 0);
		bool collider = false;
		
		// Load the file into each variable
		mapFile >> group >> assetID >> srcRectY >> srcRectX >> zIndex >> tranX >> tranY >> tileScaleX >> tileScaleY >> collider;

		if (collider) mapFile >> colWidth >> colHeight >> offset.x >> offset.y;

		// If we are at the end of the File --> Leave the File!!
		if (mapFile.eof()) break;

		// Create a new entity for each tile
		Entity object = reg.CreateEntity();
		object.Group(group);
		object.AddComponent<SpriteComponent>(assetID, tileSize, tileSize, zIndex, false, srcRectX, srcRectY);
		object.AddComponent<TransformComponent>(glm::vec2(tranX, tranY), glm::vec2(tileScaleX, tileScaleX), 0.0);
		object.AddComponent<EditorComponent>();
		// If the obstacle has a collider, add a BoxColliderComponent
		if (collider)
		{
			object.AddComponent<BoxColliderComponent>(colWidth, colHeight, glm::vec2(offset.x, offset.y));
		}
	}
	
	// Close the file
	mapFile.close();
}

void EditorFileLoader::LoadBoxColliderMap(const std::unique_ptr<AssetManager>& assetManager, SDL_Renderer* renderer, std::string& fileName)
{
	// Open and read the text file
	std::fstream mapFile;
	mapFile.open(fileName);

	if (!mapFile.is_open())
	{
		Logger::Err("Unable to open [" + fileName + "]");
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

		mapFile >> group >> tranX >> tranY >> colliderScaleX >> colliderScaleY  >> colWidth >> colHeight >> offset.x >> offset.y;


		// If we are at the end of the File --> Leave the File!!
		if (mapFile.eof()) break;

		// Create a new entity for each tile
		Entity boxCollider = reg.CreateEntity();
		boxCollider.Group(group);
		boxCollider.AddComponent<TransformComponent>(glm::vec2(tranX, tranY), glm::vec2(colliderScaleX, colliderScaleY), 0.0);
		boxCollider.AddComponent<BoxColliderComponent>(colWidth, colHeight, glm::vec2(offset.x, offset.y));

	}
	// Close the file
	mapFile.close();
}

void EditorFileLoader::SaveTilemap(std::string filepath, const std::unique_ptr<AssetManager>& assetManager, SDL_Renderer* renderer)
{
	std::ofstream mapFile;
	
	mapFile.open(filepath);
	// Log the path where we saved the file!!
	Logger::Err(filepath);

	if (!reg.GetEntitiesByGroup("tiles").empty())
	{
		for (auto entity : reg.GetEntitiesByGroup("tiles"))
		{
			if (entity.BelongsToGroup("tiles"))
			{
				bool collider = false;
				std::string group = "tiles";
				auto& tile = entity.GetComponent<SpriteComponent>();
				auto& transform = entity.GetComponent<TransformComponent>();
				auto collision = BoxColliderComponent(); 

				if (entity.HasComponent<BoxColliderComponent>())
					collision = entity.GetComponent<BoxColliderComponent>();
				
				if (entity.HasComponent<EditorComponent>())
				{
					Logger::Err("Removed Editor Component");
					entity.RemoveComponent<EditorComponent>();
				}

				// Save to the mapfile
				mapFile << group << " " << tile.width << " "<< tile.height << " " << tile.srcRect.y << " " << tile.srcRect.x << " " << tile.layer << " " <<
					transform.position.x << " " << transform.position.y << " " << transform.scale.x << " " << transform.scale.y << " ";

				if (entity.HasComponent<BoxColliderComponent>())
				{
					collider = true;
					mapFile << collider << " " << collision.width << " " << collision.height << " " << collision.offset.x << " " << collision.offset.y << " " << std::endl;
				}
				else
				{
					collider = false;
					mapFile << collider << " " << std::endl; 
				}
			}
		}
	}
	// Close the file
	mapFile.close(); 
	Logger::Log("Level Saved!");
}

void EditorFileLoader::SaveObjectMap(std::string filepath, const std::unique_ptr<AssetManager>& assetManager, SDL_Renderer* renderer)
{
	std::ofstream mapFile;

	mapFile.open(filepath);
	// Log the path where we saved the file!!
	Logger::Err(filepath);

	if (!reg.GetEntitiesByGroup("obstacles").empty())
	{
		for (auto entity : reg.GetEntitiesByGroup("obstacles"))
		{
			if (entity.BelongsToGroup("obstacles"))
			{
				bool collider = false;
				std::string group = "obstacles";
				const auto& obstacle = entity.GetComponent<SpriteComponent>();
				const auto& transform = entity.GetComponent<TransformComponent>();
				const auto& collision = entity.GetComponent<BoxColliderComponent>();

				mapFile << group << " " << obstacle.assetID << " " << obstacle.srcRect.y << " " << obstacle.srcRect.x << " " << obstacle.layer << " " <<
					transform.position.x << " " << transform.position.y << " " << transform.scale.x << " " << transform.scale.y << " ";

				if (entity.HasComponent<BoxColliderComponent>())
				{
					collider = true;
					mapFile << collider << " " << collision.width << " " << collision.height << " " << collision.offset.x << " " << collision.offset.y << " " << std::endl;
				}
				else
				{
					collider = false;
					mapFile << collider << " " << std::endl;
				}
			}
		}
	}

	mapFile.close(); // We must close the fstream after reading  
}

void EditorFileLoader::SaveBoxColliderMap(std::string filepath, const std::unique_ptr<AssetManager>& assetManager, SDL_Renderer* renderer)
{
	std::ofstream mapFile;

	mapFile.open(filepath);
	// Log the path where we saved the file!!
	Logger::Err(filepath);

	if (!reg.GetEntitiesByGroup("colliders").empty())
	{
		for (auto entity : reg.GetEntitiesByGroup("colliders"))
		{
			bool collider = false;
			bool trigger = false;
			std::string group = "colliders";
			auto& transform = entity.GetComponent<TransformComponent>();
			auto& collision = entity.GetComponent<BoxColliderComponent>();

			mapFile << group << " " << transform.position.x << " " << transform.position.y << " " << transform.scale.x << " "
				<< transform.scale.y << " " << collision.width << " " << collision.height << " " << collision.offset.x << " "
				<< collision.offset.y << " " << std::endl;;
		}
	}

	// Close the mapfile
	mapFile.close(); 
}

void EditorFileLoader::SaveBoxColliderMapToLuaFile(std::string filepath)
{
	// Create variables
	LuaTableWriter m_writer;
	std::fstream file;
	int i = 1;
	bool trigger = false;
	Logger::Err("FilePath: " + filepath);
	// open the file to save to
	file.open(filepath, std::ios::out);

	// Start the Lua Table
	m_writer.WriteStartDocument();

	// Write a comment in Lua format
	m_writer.WriteCommentSeparation(file);
	m_writer.WriteCommentLine("Game box Colliders", file);
	m_writer.WriteCommentSeparation(file);
	m_writer.WriteDeclareTable("colliders", file);
	
	// If there are colliders in the registry, save them to a lua file, using the LuaTableWriter
	if (!reg.GetEntitiesByGroup("colliders").empty())
	{
		for (const auto& entity : reg.GetEntitiesByGroup("colliders"))
		{
			const auto& transform = entity.GetComponent<TransformComponent>();
			const auto& collision = entity.GetComponent<BoxColliderComponent>();

			m_writer.WriteStartTable(i, false, file);
			m_writer.WriteKeyAndQuotedValue("group", "colliders", file);
			m_writer.WriteDeclareTable("components", file);

			WriteTransformComponent(m_writer, transform, file, false);
			Logger::Log("Middle");
			WriteBoxColliderComponent(m_writer, collision, file, false);

			//m_writer.WriteEndTable(false, file);

			m_writer.WriteEndTableWithSeparator(false, file);
			
			i++;
			
		}
	}

	m_writer.WriteEndTable(false, file);
	Logger::Log("Out the table written");
	m_writer.WriteEndDocument(file);
	file.close();
	
}

void EditorFileLoader::SaveEnemiesToLuaFile(std::string filepath)
{
	// Create variables
	LuaTableWriter m_writer;
	std::fstream file;
	int i = 1;
	//bool trigger = false;
	Logger::Err("FilePath: " + filepath);
	// open the file to save to
	file.open(filepath, std::ios::out);

	// Start the Lua Table
	m_writer.WriteStartDocument();

	// Write a comment in Lua format
	m_writer.WriteCommentSeparation(file);
	m_writer.WriteCommentLine(filepath, file);
	m_writer.WriteCommentSeparation(file);
	m_writer.WriteDeclareTable("enemies", file);

	// If there are Enemies in the registry, save them to a lua file, using the LuaTableWriter
	if (!reg.GetEntitiesByGroup("enemies").empty())
	{
		for (const auto& entity : reg.GetEntitiesByGroup("enemies"))
		{
			bool animation = false;
			bool projectile = false;
			bool script = false;
			bool rigid = false;
			bool ai = false;

			auto animationComponent = AnimationComponent();
			if (entity.HasComponent<AnimationComponent>())
			{
				animationComponent = entity.GetComponent<AnimationComponent>();
				animation = true;
			}

			auto projectileEmitter = ProjectileEmitterComponent();
			if (entity.HasComponent<ProjectileEmitterComponent>())
			{
				projectileEmitter = entity.GetComponent<ProjectileEmitterComponent>();
				projectile = true;
			}
			
			auto scriptComponent = ScriptComponent();
			if (entity.HasComponent<ScriptComponent>())
			{
				scriptComponent = entity.GetComponent<ScriptComponent>();
				script = true;
			}

			auto rigidbody = RigidBodyComponent();
			if (entity.HasComponent<RigidBodyComponent>())
			{
				rigidbody = entity.GetComponent<RigidBodyComponent>();
				rigid = true;
			}

			auto ai_comp = AIComponent();
			if (entity.HasComponent<AIComponent>())
			{
				ai_comp = entity.GetComponent<AIComponent>();
				ai = true;
			}


			// All Enemies will have these Components
			const auto& transform = entity.GetComponent<TransformComponent>();
			const auto& collider = entity.GetComponent<BoxColliderComponent>();
			const auto& health = entity.GetComponent<HealthComponent>();
			const auto& sprite = entity.GetComponent<SpriteComponent>();

			// Start the Components/Group Tables
			m_writer.WriteStartTable(i, false, file);
			m_writer.WriteKeyAndQuotedValue("group", "enemies", file);
			m_writer.WriteDeclareTable("components", file);

			WriteTransformComponent(m_writer, transform, file, false);
			//m_writer.WriteDeclareTable("transform", file);
			//m_writer.WriteDeclareTable("position", file);
			//m_writer.WriteKeyAndValue('x', transform.position.x, false, file);
			//m_writer.WriteKeyAndValue('y', transform.position.y, true, file);
			//m_writer.WriteEndTable(true, file);
			//m_writer.WriteDeclareTable("scale", file);
			//m_writer.WriteKeyAndValue('x', transform.scale.x, false, file);
			//m_writer.WriteKeyAndValue('y', transform.scale.y, true, file);
			//m_writer.WriteEndTable(true, file);
			//m_writer.WriteDeclareTable("rotation", file);
			//m_writer.WriteUnquotedValue(transform.rotation, false, file);
			//m_writer.WriteEndTable(true, file);

			//m_writer.WriteEndTable(false, file);

			// Box Collider Component
			m_writer.WriteDeclareTable("box_collider", file);
			m_writer.WriteKeyAndUnquotedValue("width", collider.width, file);
			m_writer.WriteKeyAndUnquotedValue("height", collider.height, file);
			m_writer.WriteKeyAndUnquotedValue("offset_x", collider.offset.x, file);
			m_writer.WriteKeyAndUnquotedValue("offset_y", collider.offset.y, file);
			//m_writer.WriteEndTable(true, file);
			m_writer.WriteEndTable(false, file);

			// Sprite Component 
			m_writer.WriteDeclareTable("sprite", file);
			m_writer.WriteKeyAndQuotedValue("asset_id", sprite.assetID, file);
			m_writer.WriteKeyAndUnquotedValue("width", sprite.width, file);
			m_writer.WriteKeyAndUnquotedValue("height", sprite.height, file);
			m_writer.WriteKeyAndUnquotedValue("z_index", sprite.layer, file);
			m_writer.WriteKeyAndUnquotedValue("is_fixed", (sprite.isFixed ? "true" : "false"), file);
			m_writer.WriteKeyAndUnquotedValue("src_rect_x", sprite.srcRect.x, file);
			m_writer.WriteKeyAndUnquotedValue("src_rect_y", sprite.srcRect.y, file);
			m_writer.WriteDeclareTable("offset", file);
			m_writer.WriteKeyAndValue("x", sprite.offset.x, false, file);
			m_writer.WriteKeyAndValue("y", sprite.offset.y, true, file);
			m_writer.WriteEndTable(true, file);
			m_writer.WriteEndTable(false, file);

			// Health Component
			m_writer.WriteDeclareTable("health", file);
			m_writer.WriteKeyAndUnquotedValue("health_percentage", health.healthPercentage, file);
			m_writer.WriteKeyAndUnquotedValue("max_hearts", health.maxHearts, file);
			//m_writer.WriteEndTable(true, file);
			m_writer.WriteEndTable(false, file);

			// Rigidbody Component
			if (rigid)
			{
				m_writer.WriteDeclareTable("rigidbody", file);
				m_writer.WriteDeclareTable("velocity", file);
				m_writer.WriteKeyAndValue("x", rigidbody.velocity.x, false, file);
				m_writer.WriteKeyAndValue("y", rigidbody.velocity.y, true, file);
				//m_writer.WriteEndTable(true, file);
				m_writer.WriteEndTable(true, file);
				m_writer.WriteEndTable(false, file);
			}

			// Projectile Emitter Component 
			if (projectile)
			{
				m_writer.WriteDeclareTable("projectile_emitter", file);
				m_writer.WriteDeclareTable("velocity", file);
				m_writer.WriteKeyAndValue("x", projectileEmitter.projectileVelocity.x, false, file);
				m_writer.WriteKeyAndValue("y", projectileEmitter.projectileVelocity.y, true, file);
				m_writer.WriteEndTable(true, file);
				m_writer.WriteKeyAndUnquotedValue("repeat_frequency", projectileEmitter.repeatFrequency, file);
				m_writer.WriteKeyAndUnquotedValue("projectile_duration", projectileEmitter.projectileDuration, file);
				m_writer.WriteKeyAndUnquotedValue("hit_percent_damage", projectileEmitter.hitPercentDamage, file);
				m_writer.WriteKeyAndUnquotedValue("is_friendly", (projectileEmitter.isFriendly ? "true" : "false"), file);
				//m_writer.WriteEndTable(true, file);
				m_writer.WriteEndTable(false, file);
			}

			// Animation Component int numFrames = 1, int frameSpeedRate = 1, bool vertical = true, bool isLooped = true, int frameOffset = 0
			if (animation)
			{
				m_writer.WriteDeclareTable("animation", file);
				m_writer.WriteKeyAndUnquotedValue("num_frames", animationComponent.numFrames, file);
				m_writer.WriteKeyAndUnquotedValue("frame_rate", animationComponent.frameSpeedRate, file);
				m_writer.WriteKeyAndUnquotedValue("vertical", (animationComponent.vertical ? "true" : "false"), file);
				m_writer.WriteKeyAndUnquotedValue("looped", (animationComponent.isLooped ? "true" : "false"), file);
				m_writer.WriteKeyAndUnquotedValue("frame_offset", animationComponent.frameOffset, file);
				//m_writer.WriteEndTable(false, file);
				m_writer.WriteEndTable(false, file);
			}

			if (ai)
			{
				std::string enemy_type_str = ConvertAIEnemyToString(ai_comp.GetEnemyType());
				m_writer.WriteDeclareTable("ai_component", file);
				m_writer.WriteDeclareTable("enemy_pos", file);
				m_writer.WriteKeyAndValue("x", 0, false, file);
				m_writer.WriteKeyAndValue("y", 0, true, file);
				m_writer.WriteEndTable(true, file);
				m_writer.WriteKeyAndQuotedValue("enemy_type", enemy_type_str, file);
				m_writer.WriteEndTable(false, file);

			}
			//// Script Component
			//if (script)
			//{
			//	// TODO: Add a proper way to save the scripts to a lua table
			//	// ImGui::EnterText? MultiText? 
			//}
			//m_writer.WriteEndTable(false, file);
			m_writer.WriteEndTableWithSeparator(false, file);
			m_writer.WriteEndTableWithSeparator(false, file);

			i++;
		}
	}

	
	// Write the end of the tables and close the file
	m_writer.WriteEndTable(false, file);
	Logger::Log("Out the table written");
	file.close();
}

void EditorFileLoader::SaveTriggersToLuaFile(std::string filepath)
{
	// Grab the file stem for the comment Line
	std::string commentLine = std::filesystem::path(filepath).stem().string();

	// Create variables
	LuaTableWriter m_writer;
	std::fstream file;
	int i = 1;
	bool trigger = false;
	Logger::Err("FilePath: " + filepath);
	// open the file to save to
	file.open(filepath, std::ios::out);

	// Start the Lua Table
	m_writer.WriteStartDocument();
	// Write a comment in Lua format
	m_writer.WriteCommentSeparation(file);
	m_writer.WriteCommentLine(commentLine, file);
	m_writer.WriteCommentSeparation(file);
	m_writer.WriteDeclareTable("triggers", file);

	// If there are Enemies in the registry, save them to a lua file, using the LuaTableWriter
	if (reg.DoesGroupExist("triggers"))
	{
		// Write All of the triggers
		for (const auto& entity : reg.GetEntitiesByGroup("triggers"))
		{

			const auto&	transform = entity.GetComponent<TransformComponent>();
			const auto&	boxCollider = entity.GetComponent<BoxColliderComponent>();
			const auto& triggerBox = entity.GetComponent<TriggerBoxComponent>();
			auto spriteComp = SpriteComponent();


			// Convert trigger Type to string
			std::string trigger_type = ConvertToString(triggerBox.triggerType);
			Logger::Log("trig_type: " + trigger_type);
			// Start the Components/Group Tables
			m_writer.WriteStartTable(i, false, file);
			m_writer.WriteKeyAndQuotedValue("group", "triggers", file);
			m_writer.WriteDeclareTable("components", file);
			WriteTransformComponent(m_writer, transform, file, false);
			WriteBoxColliderComponent(m_writer, boxCollider, file, false);

			// Write Triggers
			WriteTriggerBoxComponent(m_writer, triggerBox, file, trigger_type, false);

			if (entity.HasComponent<SpriteComponent>())
			{
				spriteComp = entity.GetComponent<SpriteComponent>();
				WriteSpriteComponent(m_writer, spriteComp, file, false);
			}

			m_writer.WriteEndTable(false, file);
			m_writer.WriteEndTableWithSeparator(false, file);

			i++;
		}
	}
	//
	if (reg.DoesGroupExist("secret"))
	{
		// Write All of the triggers
		for (const auto& entity : reg.GetEntitiesByGroup("secret"))
		{
			const auto& transform = entity.GetComponent<TransformComponent>();
			const auto& boxCollider = entity.GetComponent<BoxColliderComponent>();
			const auto& triggerBox = entity.GetComponent<TriggerBoxComponent>();
			const auto& secret = entity.GetComponent<SecretComponent>();
			auto spriteComp = SpriteComponent();

			// Convert trigger Type to string
			std::string trigger_type = ConvertToString(triggerBox.triggerType);

			// Start the Components/Group Tables
			m_writer.WriteStartTable(i, false, file);
			m_writer.WriteKeyAndQuotedValue("group", "secret", file);
			m_writer.WriteDeclareTable("components", file);
			WriteTransformComponent(m_writer, transform, file, false);
			WriteBoxColliderComponent(m_writer, boxCollider, file, false);
			// Write Triggers
			WriteTriggerBoxComponent(m_writer, triggerBox, file, trigger_type, false);
			WriteSecretComponent(m_writer, secret, file, false);


			if (entity.HasComponent<SpriteComponent>())
			{
				spriteComp = entity.GetComponent<SpriteComponent>();
				WriteSpriteComponent(m_writer, spriteComp, file, false);
			}
			m_writer.WriteEndTable(false, file);
			m_writer.WriteEndTableWithSeparator(false, file);

			i++;
		}
	}
	else
	{
		Logger::Log("Secret Does Not exist!!");
	}

	m_writer.WriteEndTable(false, file);
	m_writer.WriteEndDocument(file);
	file.close();
}

void EditorFileLoader::SetFileName(std::string filename)
{
	this->fileName = filename;
}

void EditorFileLoader::SetImageName(std::string imageName)
{
	this->imageName = imageName;
}

std::string EditorFileLoader::SetName(std::string filePath, bool wExtension, char separator)
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

void EditorFileLoader::LoadEnemiesAttributes(sol::state& lua, std::string& fileName, std::string& enemy_name)
{
	sol::load_result script = lua.load_file("./Assets/EditorFiles/" + fileName + ".lua");
	// This checks the syntax of our script, but it does not execute the script
	if (!script.valid())
	{
		sol::error err = script;
		std::string errorMessage = err.what();
		Logger::Err("Error loading the Lua Script: " + errorMessage);
		return;
	}

	// Executes the script using the sol State
	lua.script_file("./Assets/EditorFiles/" + fileName + ".lua");

	sol::table enemy = lua[enemy_name];

	sol::optional<sol::table> hasComponents = enemy["components"];
	if (hasComponents != sol::nullopt)
	{
		// Set the imageID 
		sol::optional<sol::table> sprite = enemy["components"]["sprite"];
		if (sprite != sol::nullopt)
		{
			MouseControlSystem::spriteComponent.assetID = enemy["components"]["sprite"]["asset_id"];
			MouseControlSystem::spriteComponent.srcRect.x = enemy["components"]["sprite"]["src_rect_x"].get_or(0);
			MouseControlSystem::spriteComponent.srcRect.y = enemy["components"]["sprite"]["src_rect_y"].get_or(0);
		}
	}
}

void EditorFileLoader::CreateNewEnemy(sol::state& lua, std::string& fileName, std::string& enemy_name, Entity& newEnemy)
{
	sol::load_result script = lua.load_file("./Assets/EditorFiles/" + fileName + ".lua");
	// This checks the syntax of our script, but it does not execute the script
	if (!script.valid())
	{
		sol::error err = script;
		std::string errorMessage = err.what();
		Logger::Err("Error loading the Lua Script: " + errorMessage);
		return;
	}

	// Executes the script using the sol State
	lua.script_file("./Assets/EditorFiles/" + fileName + ".lua");

	sol::table enemy = lua[enemy_name];

	sol::optional<sol::table> hasComponents = enemy["components"];
	if (hasComponents != sol::nullopt)
	{
		// Set the Sprite Component 
		sol::optional<sol::table> sprite = enemy["components"]["sprite"];
		if (sprite != sol::nullopt)
		{
			newEnemy.AddComponent<SpriteComponent>(
				enemy["components"]["sprite"]["asset_id"], 
				enemy["components"]["sprite"]["width"], 
				enemy["components"]["sprite"]["height"], 
				enemy["components"]["sprite"]["layer"].get_or(1), 
				enemy["components"]["sprite"]["fixed"].get_or(false), 
				enemy["components"]["sprite"]["src_rect_x"].get_or(0), 
				enemy["components"]["sprite"]["src_rect_y"].get_or(0) 	
				);
		}

		// Set the BoxCollider Component
		sol::optional<sol::table> box_collider = enemy["components"]["box_collider"];
		if (box_collider != sol::nullopt)
		{
			newEnemy.AddComponent<BoxColliderComponent>(
				enemy["components"]["box_collider"]["width"],
				enemy["components"]["box_collider"]["height"],
				glm::vec2(
					enemy["components"]["box_collider"]["offset"]["x"],
					enemy["components"]["box_collider"]["offset"]["y"])
				);
		}

		// Set the Health Component
		sol::optional<sol::table> health = enemy["components"]["health"];
		if (health != sol::nullopt)
		{
			newEnemy.AddComponent<HealthComponent>(
				enemy["components"]["health"]["health_percentage"].get_or(100), 
				enemy["components"]["health"]["max_hearts"].get_or(3)
				);
		}

		// Set the Rigid Body Component 
		sol::optional<sol::table> rigid_body = enemy["components"]["rigid_body"];
		if (rigid_body != sol::nullopt)
		{
			newEnemy.AddComponent<RigidBodyComponent>(
				glm::vec2(
					enemy["components"]["rigid_body"]["velocity"]["x"],
					enemy["components"]["rigid_body"]["velocity"]["y"]
				));
		}

		// Set the animation Component
		sol::optional<sol::table> animation = enemy["components"]["animation"];
		if (animation != sol::nullopt)
		{
			newEnemy.AddComponent<AnimationComponent>(
				enemy["components"]["animation"]["num_frames"].get_or(1),
				enemy["components"]["animation"]["frame_rate"].get_or(1),
				enemy["components"]["animation"]["vertical"].get_or(true),
				enemy["components"]["animation"]["looped"].get_or(true),
				enemy["components"]["animation"]["frame_offset"].get_or(0)
				);
		}

		// Set the Projectile Emitter Component
		sol::optional<sol::table> projectile = enemy["components"]["projectile_emitter"];
		if (projectile != sol::nullopt)
		{
			newEnemy.AddComponent<ProjectileEmitterComponent>(

				);
		}

		// Set the AI Component
		sol::optional<sol::table> ai = enemy["components"]["ai_component"];
		if (ai != sol::nullopt)
		{
			newEnemy.AddComponent<AIComponent>(
				glm::vec2(
					enemy["components"]["ai_component"]["enemy_pos"]["x"],
					enemy["components"]["ai_component"]["enemy_pos"]["y"]
				),
				enemy["components"]["ai_component"]["enemy_type"]
				);
		}
	}
}


std::string EditorFileLoader::ConvertToString(TriggerType triggerType)
{
	switch (triggerType)
	{
		case NO_TRIGGER: return "no_trigger"; break;
		case SECRET_AREA: return "secret_area"; break;
		case TRANSPORT: return "transport"; break;
		case BURN_BUSHES: return "burn_bushes"; break;
		case PUSH_ROCKS: return "push_rocks"; break;
		case COLLECT_ITEM: return "collect_item"; break;
		case BOMB_SECRET: return "bomb_secret"; break;
		case LOCKED_DOOR: return "locked_door"; break;
		case HIDDEN_OBJECT: return "hidden_object"; break;
	}
}

std::string EditorFileLoader::ConvertAIEnemyToString(AIComponent::EnemyType type)
{
	switch (type)
	{
	default:
		break;
	}

	return std::string();
}


// Private Function
void EditorFileLoader::WriteTransformComponent(LuaTableWriter& writer, const TransformComponent& transform, std::fstream& file, bool last)
{
	writer.WriteDeclareTable("transform", file);
	writer.WriteDeclareTable("position", file);
	writer.WriteKeyAndValue('x', transform.position.x, false, file);
	writer.WriteKeyAndValue('y', transform.position.y, true, file);
	writer.WriteEndTable(true, file);
	writer.WriteDeclareTable("scale", file);
	writer.WriteKeyAndValue('x', transform.scale.x, false, file);
	writer.WriteKeyAndValue('y', transform.scale.y, true, file);
	writer.WriteEndTable(true, file);
	writer.WriteDeclareTable("rotation", file);
	writer.WriteUnquotedValue(transform.rotation, false, file);
	writer.WriteEndTable(true, file);
	writer.WriteEndTable(false, file);

	if (last)
		writer.WriteEndTable(false, file);
}

void EditorFileLoader::WriteBoxColliderComponent(LuaTableWriter& writer, const class BoxColliderComponent& collision, std::fstream& file, bool last)
{
	writer.WriteDeclareTable("box_collider", file);
	writer.WriteKeyAndUnquotedValue("width", collision.width, file);
	writer.WriteKeyAndUnquotedValue("height", collision.height, file);
	writer.WriteKeyAndUnquotedValue("offset_x", collision.offset.x, file);
	writer.WriteKeyAndUnquotedValue("offset_y", collision.offset.y, file);
	writer.WriteEndTable(false, file);

	if (last)
		writer.WriteEndTable(false, file);
}

void EditorFileLoader::WriteTriggerBoxComponent(LuaTableWriter& writer, const class TriggerBoxComponent& triggerBox, std::fstream& file, std::string& trigger_type, bool last)
{
	// Write Triggers
	writer.WriteDeclareTable("trigger_box", file);
	writer.WriteKeyAndQuotedValue("trigger_type", trigger_type, file);
	writer.WriteDeclareTable("transport_offset", file);
	writer.WriteKeyAndValue("x", triggerBox.transportOffset.x, false, file);
	writer.WriteKeyAndValue("y", triggerBox.transportOffset.y, true, file);
	writer.WriteEndTable(true, file);
	writer.WriteDeclareTable("camera_offset", file);
	writer.WriteKeyAndValue("x", triggerBox.cameraOffset.x, false, file);
	writer.WriteKeyAndValue("y", triggerBox.cameraOffset.y, true, file);
	writer.WriteEndTable(true, file);
	writer.WriteKeyAndQuotedValue("level_music", triggerBox.levelMusic, file);
	writer.WriteKeyAndQuotedValue("asset_file", triggerBox.assetFile, file);
	writer.WriteKeyAndQuotedValue("enemy_file", triggerBox.enemyFile, file);
	writer.WriteKeyAndQuotedValue("collider_file", triggerBox.colliderFile, file);
	writer.WriteKeyAndQuotedValue("tilemap_name", triggerBox.tileMapName, file);
	writer.WriteKeyAndQuotedValue("tilemap_image", triggerBox.tileImageName, file);
	writer.WriteKeyAndQuotedValue("entity_file", triggerBox.entityFileName, file);
	writer.WriteKeyAndUnquotedValue("image_width", triggerBox.imageWidth, file);
	writer.WriteKeyAndUnquotedValue("image_height", triggerBox.imageHeight, file);
	writer.WriteKeyAndQuotedValue("trigger_file", triggerBox.triggerFile, file);

	std::string collider = "false";
	if (triggerBox.collider)
		collider = "true";
	writer.WriteKeyAndUnquotedValue("collider", collider, file);
	writer.WriteEndTable(false, file);

	if (last)
		writer.WriteEndTable(false, file);
}

void EditorFileLoader::WriteSecretComponent(LuaTableWriter& writer, const class SecretComponent& secretComp, std::fstream& file, bool last)
{
	writer.WriteDeclareTable("secret", file);
	writer.WriteKeyAndQuotedValue("location_id", secretComp.locationID, file);
	writer.WriteKeyAndQuotedValue("new_trigger", secretComp.newTrigger, file);
	writer.WriteKeyAndQuotedValue("new_sprite_id", secretComp.newSpriteAssetID, file);
	writer.WriteKeyAndUnquotedValue("sprite_width", secretComp.spriteWidth, file);
	writer.WriteKeyAndUnquotedValue("sprite_height", secretComp.spriteHeight, file);
	writer.WriteKeyAndUnquotedValue("sprite_src_x", secretComp.spriteSrcX, file);
	writer.WriteKeyAndUnquotedValue("sprite_src_y", secretComp.spriteSrcY, file);
	writer.WriteEndTable(false, file);

	if (last)
		writer.WriteEndTable(false, file);
}

void EditorFileLoader::WriteSpriteComponent(LuaTableWriter& writer, const SpriteComponent& sprite, std::fstream& file, bool last)
{
	std::string fixed = "false";
	if (sprite.isFixed)
		fixed = "true";

	writer.WriteDeclareTable("sprite", file);
	writer.WriteKeyAndQuotedValue("asset_id", sprite.assetID, file);
	writer.WriteKeyAndUnquotedValue("width", sprite.width, file);
	writer.WriteKeyAndUnquotedValue("height", sprite.height, file);
	writer.WriteKeyAndUnquotedValue("z_index", sprite.layer, file);
	writer.WriteKeyAndUnquotedValue("is_fixed", fixed, file);
	writer.WriteKeyAndUnquotedValue("src_rect_x", sprite.srcRect.x, file);
	writer.WriteKeyAndUnquotedValue("src_rect_y", sprite.srcRect.y, file);
	writer.WriteDeclareTable("offset", file);
	writer.WriteKeyAndUnquotedValue("x", sprite.offset.x, file, true);
	writer.WriteKeyAndUnquotedValue("y", sprite.offset.y, file, true, true);
	writer.WriteEndTable(true, file);
	writer.WriteEndTable(false, file);

	if (last)
		writer.WriteEndTable(false, file);
}
