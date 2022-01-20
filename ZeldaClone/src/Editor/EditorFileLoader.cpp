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
#include <string>
#include <iostream>
#include <fstream>

EditorFileLoader::EditorFileLoader()
	: reg(*Registry::Instance())
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
	// Adding Textures to the asset Manager
	assetManager->AddTextures(renderer, "image-Name", imageName);
	Logger::Err(imageName);
	
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

void EditorFileLoader::LoadBoxColliderMap(const std::unique_ptr<AssetManager>& assetManager, SDL_Renderer* renderer)
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
		bool collider = false;
		bool trigger = false;
		int type = 0;
		std::string triggerLevel = "";
		TriggerType triggerType = NO_TRIGGER;
		
		mapFile >> group >> tranX >> tranY >> colliderScaleX >> colliderScaleY >> collider >> trigger;

		if (collider && !trigger) mapFile >> colWidth >> colHeight >> offset.x >> offset.y;
		else if (collider && trigger) mapFile >> colWidth >> colHeight >> offset.x >> offset.y >> type >> triggerOffset.x >> triggerOffset.y >> triggerCamOffset.x >> triggerCamOffset.y >> triggerLevel;
		
		// Convert the trigger type to TriggerType enum
		triggerType = ConvertToTriggerType(type);

		// If we are at the end of the File --> Leave the File!!
		if (mapFile.eof()) break;

		// Create a new entity for each tile
		Entity boxCollider = reg.CreateEntity();
		boxCollider.Group(group);
		boxCollider.AddComponent<TransformComponent>(glm::vec2(tranX, tranY), glm::vec2(colliderScaleX, colliderScaleY), 0.0);
		
		// Check to see if it has only a box collider
		if (collider && !trigger)
		{
			boxCollider.AddComponent<BoxColliderComponent>(colWidth, colHeight, glm::vec2(offset.x, offset.y));
		}
		// Check to see if there is both a box collider and a trigger 
		if (collider && trigger)
		{
			boxCollider.AddComponent<BoxColliderComponent>(colWidth, colHeight, glm::vec2(offset.x, offset.y));
			boxCollider.AddComponent<TriggerBoxComponent>(triggerType);
		}
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
				auto& obstacle = entity.GetComponent<SpriteComponent>();
				auto& transform = entity.GetComponent<TransformComponent>();
				auto collision = entity.GetComponent<BoxColliderComponent>();

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
			
			mapFile << group << " " << transform.position.x << " " << transform.position.y << " " << transform.scale.x << " " << transform.scale.y << " ";

			if (entity.HasComponent<BoxColliderComponent>() && !entity.HasComponent<TriggerBoxComponent>())
			{
				//auto& trig = entity.GetComponent<TriggerBoxComponent>();
				collider = true;
				trigger = false;
				mapFile << collider << " " << trigger << " " << collision.width << " " << collision.height << " " << collision.offset.x << " " << collision.offset.y << " " << std::endl;
			}
		}
	}

	if (!reg.GetEntitiesByGroup("trigger").empty())
	{
		for (auto entity : reg.GetEntitiesByGroup("trigger"))
		{
			bool collider = false;
			bool trigger = false;
			std::string group = "trigger";
			auto& transform = entity.GetComponent<TransformComponent>();
			auto collision = entity.GetComponent<BoxColliderComponent>();
			auto trig = entity.GetComponent<TriggerBoxComponent>();

			mapFile << group << " " << transform.position.x << " " << transform.position.y << " " << transform.scale.x << " " << transform.scale.y << " ";

			if (entity.HasComponent<BoxColliderComponent>() && entity.HasComponent<TriggerBoxComponent>())
			{
				collider = true;
				trigger = true;
				mapFile << collider << " " << trigger << " " << collision.width << " " << collision.height << " " << collision.offset.x << " " << collision.offset.y << " " <<
					trig.triggerType << " " << trig.transportOffset.x << " " << trig.transportOffset.y << " " << trig.levelMusic << " " << std::endl;
			}
		}
	}
	// Close the mapfile
	mapFile.close(); 
}

void EditorFileLoader::SaveBoxColliderMapToLuaFile(std::string filepath, const std::unique_ptr<AssetManager>& assetManager, SDL_Renderer* renderer)
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
		for (auto entity : reg.GetEntitiesByGroup("colliders"))
		{
			Logger::Log("In the collider");
			bool collider = false;
			bool trigger = false;
			std::string group = "colliders";
			
			
			const auto& transform = entity.GetComponent<TransformComponent>();
			const auto& collision = entity.GetComponent<BoxColliderComponent>();

			m_writer.WriteStartTable(i, false, file);
			m_writer.WriteKeyAndQuotedValue("group", "colliders", file);
			m_writer.WriteDeclareTable("components", file);
			m_writer.WriteDeclareTable("transform", file);
			m_writer.WriteDeclareTable("position", file);
			m_writer.WriteKeyAndValue('x', transform.position.x, false, file);
			m_writer.WriteKeyAndValue('y',transform.position.y, true, file);
			m_writer.WriteEndTable(true, file);
			m_writer.WriteDeclareTable("scale", file);
			m_writer.WriteKeyAndValue('x', transform.scale.x, false, file);
			m_writer.WriteKeyAndValue('y', transform.scale.y, true, file);
			m_writer.WriteEndTable(true, file);
			m_writer.WriteDeclareTable("rotation", file);
			m_writer.WriteUnquotedValue(transform.rotation, false, file);
			m_writer.WriteEndTable(true, file);
			m_writer.WriteEndTable(false, file);
			m_writer.WriteDeclareTable("boxCollider", file);
			m_writer.WriteKeyAndUnquotedValue("width", collision.width, file);
			m_writer.WriteKeyAndUnquotedValue("height", collision.height, file);
			m_writer.WriteKeyAndUnquotedValue("offset_x", collision.offset.x, file);
			m_writer.WriteKeyAndUnquotedValue("offset_y", collision.offset.y, file);
			m_writer.WriteEndTable(false, file);
			m_writer.WriteEndTable(false, file);

			m_writer.WriteEndTableWithSeparator(false, file);
			
			i++;
		}
	}
	m_writer.WriteEndTable(false, file);
	// Currently there is a glitch with the save --> There must be at least one trigger 
	
	// If there are triggers in the registry, save them to a lua file, using the LuaTableWriter
	if (!reg.GetEntitiesByGroup("trigger").empty())
	{
		// reset i to 1
		i = 1;
		m_writer.WriteDeclareTable("triggers", file);
		// Loop through all Triggers
		for (auto entity : reg.GetEntitiesByGroup("trigger"))
		{
			Logger::Log("In the trigger");
			bool collider = false;
			bool trigger = true;
			
			std::string group = "trigger";
			const auto& transform = entity.GetComponent<TransformComponent>();
			const auto& trig = entity.GetComponent<TriggerBoxComponent>();
			const auto& collision = entity.GetComponent<BoxColliderComponent>();
			
			m_writer.WriteStartTable(i, false, file);
			m_writer.WriteKeyAndQuotedValue("group", "trigger", file);
			m_writer.WriteDeclareTable("components", file);
			m_writer.WriteDeclareTable("transform", file);
			m_writer.WriteDeclareTable("position", file);
			m_writer.WriteKeyAndValue('x', transform.position.x, false, file);
			m_writer.WriteKeyAndValue('y',transform.position.y, true, file);
			m_writer.WriteEndTable(true, file);
			m_writer.WriteDeclareTable("scale", file);
			m_writer.WriteKeyAndValue('x', transform.scale.x, false, file);
			m_writer.WriteKeyAndValue('y', transform.scale.y, true, file);
			m_writer.WriteEndTable(true, file);
			m_writer.WriteDeclareTable("rotation", file);
			m_writer.WriteUnquotedValue(transform.rotation, false, file);
			m_writer.WriteEndTable(true, file);
			m_writer.WriteEndTable(false, file);
			
			// Write box Collider --> All triggers are box Colliders
			m_writer.WriteDeclareTable("boxCollider", file);
			m_writer.WriteKeyAndUnquotedValue("width", collision.width, file);
			m_writer.WriteKeyAndUnquotedValue("height", collision.height, file);
			m_writer.WriteKeyAndUnquotedValue("offset_x", collision.offset.x, file);
			m_writer.WriteKeyAndUnquotedValue("offset_y", collision.offset.y, file);
			m_writer.WriteEndTable(true, file);
			m_writer.WriteEndTable(false, file);
			
			// Add the type of trigger
			m_writer.WriteDeclareTable("triggerComponent", file);
			m_writer.WriteKeyAndUnquotedValue("type", trig.triggerType, file);
			m_writer.WriteKeyAndUnquotedValue("transport_offset_x", trig.transportOffset.x, file);
			m_writer.WriteKeyAndUnquotedValue("transport_offset_y", trig.transportOffset.y, file);
			m_writer.WriteKeyAndUnquotedValue("level", trig.levelMusic, file);
			//m_writer.WriteEndTable(false, file);
			m_writer.WriteEndTable(false, file);
		
			m_writer.WriteEndTableWithSeparator(false, file);
			
			// This is to number the triggers
			i++;
		}	
	}

	Logger::Log("Out the trigger written");
	// Write the end of the tables and close the file
	//m_writer.WriteEndTable(false, file);
	m_writer.WriteEndTable(false, file);
	Logger::Log("Out the table written");
	file.close();
	
}

void EditorFileLoader::SaveEnemiesToLuaFile(std::string filepath)
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
	m_writer.WriteCommentLine(filepath, file);
	m_writer.WriteCommentSeparation(file);
	m_writer.WriteDeclareTable("enemies", file);

	// If there are colliders in the registry, save them to a lua file, using the LuaTableWriter
	if (!reg.GetEntitiesByGroup("enemies").empty())
	{
		for (auto entity : reg.GetEntitiesByGroup("enemies"))
		{
			bool animation = false;
			bool projectile = false;
			bool script = false;
			bool rigid = false;

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

			// All Enemies will have these Components
			const auto& transform = entity.GetComponent<TransformComponent>();
			const auto& collider = entity.GetComponent<BoxColliderComponent>();
			const auto& health = entity.GetComponent<HealthComponent>();
			const auto& sprite = entity.GetComponent<SpriteComponent>();

			// Start the Components/Group Tables
			m_writer.WriteStartTable(i, false, file);
			m_writer.WriteKeyAndQuotedValue("group", "enemies", file);
			m_writer.WriteDeclareTable("components", file);
			m_writer.WriteDeclareTable("transform", file);
			m_writer.WriteDeclareTable("position", file);
			m_writer.WriteKeyAndValue('x', transform.position.x, false, file);
			m_writer.WriteKeyAndValue('y', transform.position.y, true, file);
			m_writer.WriteEndTable(true, file);
			m_writer.WriteDeclareTable("scale", file);
			m_writer.WriteKeyAndValue('x', transform.scale.x, false, file);
			m_writer.WriteKeyAndValue('y', transform.scale.y, true, file);
			m_writer.WriteEndTable(true, file);
			m_writer.WriteDeclareTable("rotation", file);
			m_writer.WriteUnquotedValue(transform.rotation, false, file);
			m_writer.WriteEndTable(true, file);
			m_writer.WriteEndTable(false, file);

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
				m_writer.WriteEndTable(true, file);
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
			// Script Component
			if (script)
			{
				// TODO: Add a proper way to save the scripts to a lua table
				// ImGui::EnterText? MultiText? 
			}
			m_writer.WriteEndTable(false, file);
			m_writer.WriteEndTableWithSeparator(false, file);

			i++;
		}
	}

	
	// Write the end of the tables and close the file
	m_writer.WriteEndTable(false, file);
	Logger::Log("Out the table written");
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

TriggerType EditorFileLoader::ConvertToTriggerType(int triggerType)
{
	switch (triggerType)
	{
	case 0: return NO_TRIGGER; break;
	case 1: return SECRET_AREA; break;
	case 2: return ENTER_DUNGEON; break;
	case 3: return BURN_BUSHES; break;
	case 4: return PUSH_ROCKS; break;
	case 5: return COLLECT_ITEM; break;
	case 6: return BOMB_SECRET; break;
	case 7: return HIDDEN_SWITCH; break;
	case 8: return HIDDEN_OBJECT; break;
	}
}

std::string EditorFileLoader::ConvertToString(TriggerType triggerType)
{
	switch (triggerType)
	{
		case NO_TRIGGER: return "NO_TRIGGER"; break;
		case SECRET_AREA: return "SECRET_AREA"; break;
		case ENTER_DUNGEON: return "ENTER_DUNGEON"; break;
		case BURN_BUSHES: return "BURN_BUSHES"; break;
		case PUSH_ROCKS: return "PUSH_ROCKS"; break;
		case COLLECT_ITEM: return "CAMERA_RIGHT"; break;
		case BOMB_SECRET: return "TRAP"; break;
		case HIDDEN_SWITCH: return "HIDDEN_SWITCH"; break;
		case HIDDEN_OBJECT: return "HIDDEN_OBJECT"; break;
	}
}
