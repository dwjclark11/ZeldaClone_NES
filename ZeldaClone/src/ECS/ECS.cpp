#include "ECS.h"
#include <algorithm>

// Initialize Statics
int BaseComponent::nextID = 0;

std::unique_ptr<Registry> Registry::instance = nullptr;

int Entity::GetID() const
{
	return ID;
}

void Entity::Kill()
{
	registry->KillEntity(*this);
}

void Entity::Tag(const std::string& tag)
{
	registry->TagEntity(*this, tag);
}

bool Entity::HasTag(const std::string& tag) const
{
	return registry->EntityHasTag(*this, tag);
}

const std::vector<std::string>& Entity::GetTags() const
{
	return registry->GetEntityTags(*this);
}

void Entity::Group(const std::string& group)
{
	registry->GroupEntity(*this, group);
}

bool Entity::BelongsToGroup(const std::string& group) const
{
	return registry->EntityBelongsToGroup(*this, group);
}

void System::AddEntityToSystem(Entity entity)
{
	entities.push_back(entity);
}

void System::RemoveEntityFromSystem(Entity entity)
{
	// This uses the functions of the vector class. It contains a Lambda function that looks for a 
	// match amd then removes it
	entities.erase(std::remove_if(entities.begin(), entities.end(), [&entity](Entity otherEntity)
		{
			return entity == otherEntity;
		}),
		entities.end());
}

std::vector<Entity> System::GetSystemEntities()
{
	return entities;
}

const Signature& System::GetComponentSignature() const
{
	return componentSignature;
}

Entity Registry::CreateEntity()
{
	int entityID;

	if (freeIDs.empty())
	{
		// If there are no freeIDs to be used
		entityID = numEntities++;

		if (entityID >= entityComponentSignatures.size())
		{
			entityComponentSignatures.resize(entityID + 1);
		}
	}
	else
	{
		//Reuse an ID from the list of previously removed entities
		entityID = freeIDs.front();
		freeIDs.pop_front();
	}

	Entity entity(entityID);
	entity.registry = this; // The object pointer of the registry class
	entitiesToBeAdded.insert(entity); // New entity that needs to be added 

	return entity;
}

void Registry::KillEntity(Entity entity)
{
	entitiesToBeKilled.insert(entity);
}

void Registry::AddEntityToSystem(Entity entity)
{
	const auto& entityComponentSignature = entityComponentSignatures[entity.GetID()];

	for (auto& system : systems)
	{
		const auto& systemComponentSignature = system.second->GetComponentSignature();

		/*	Perform a Bitwise Comparison of the component signatures --> the and operation will only work
			if the bits it is comparing are equal to 1. signatures are equal than we are interested!!
		*/
		bool isInterested = (entityComponentSignature & systemComponentSignature) == systemComponentSignature;

		if (isInterested)
		{
			system.second->AddEntityToSystem(entity);
		}
	}
}

void Registry::RemoveEntityFromSystems(Entity entity)
{
	for (const auto& system : systems)
	{
		system.second->RemoveEntityFromSystem(entity);
	}
}


void Registry::AddEntities()
{
	// Add the entities that are waiting to be created to the active systems
	for (const auto& entity : entitiesToBeAdded)
	{
		AddEntityToSystem(entity);
	}

	// once the entities are added we must clear the entities that were added
	entitiesToBeAdded.clear();
}

void Registry::RemoveEntities()
{
	// Process the entities that are waiting to be klled from the Active Systems
	for (const auto& entity : entitiesToBeKilled)
	{
		// Get the entity's ID 
		const auto& entID = entity.GetID();

		RemoveEntityFromSystems(entity);

		entityComponentSignatures[entID].reset(); // Reset the component signatures from the removed entity

		// Remove the entity from the component pools
		for (const auto& pool : componentPools)
		{
			if (pool) // If the pool is not null
			{
				pool->RemoveEntityFromPool(entID);
			}
		}

		// Make the Entity ID available to be used
		freeIDs.push_back(entID); // Push back the ID that was just removed

		// Remove any traces of that entity from the  tag/groups maps
		RemoveEntityTag(entity);
		RemoveEntityGroup(entity);
	}

	entitiesToBeKilled.clear();
}


void Registry::Update()
{
	AddEntities();
	RemoveEntities();
}

void Registry::TagEntity(Entity entity, const std::string& tag)
{
	entityPerTag.emplace(tag, entity);
	tagPerEntity.emplace(std::pair<int, std::vector<std::string>>(entity.GetID(), std::vector<std::string>()));
	tagPerEntity[entity.GetID()].emplace_back(tag);
}

bool Registry::EntityHasTag(Entity entity, const std::string& tag) const
{
	if (DoesTagExist(tag))
		return entityPerTag.find(tag)->second == entity;
	else
		return false;
}

const std::vector<std::string>& Registry::GetEntityTags(const Entity& entity) const
{
	const auto& entID = entity.GetID();
	if (tagPerEntity.find(entID) == tagPerEntity.end())
	{
		return std::vector<std::string>();
	}

	return tagPerEntity.find(entID)->second;
}

std::vector<std::string> Registry::GetEntityGroups(const Entity& entity)
{
	std::vector<std::string> groups;
	for (const auto& group : groupPerEntity[entity.GetID()])
		groups.push_back(group);

	return groups;
}

bool Registry::DoesTagExist(const std::string& tag) const
{
	// If the group exists
	if (entityPerTag.find(tag) != entityPerTag.end())
		return true;

	return false;
}

Entity Registry::GetEntityByTag(const std::string& tag) const
{
	return entityPerTag.at(tag);
}

void Registry::RemoveEntityTag(Entity entity)
{

	const auto& entID = entity.GetID();
	if (tagPerEntity.find(entID) != tagPerEntity.end())
	{
		for (const auto& tagName : tagPerEntity[entID])
		{
			auto taggedEntity = entityPerTag.find(tagName);
			if (taggedEntity != entityPerTag.end())
			{
				entityPerTag.erase(taggedEntity);
			}
		}
		tagPerEntity.erase(entID);
	}
}

void Registry::GroupEntity(Entity entity, const std::string& group)
{
	entitiesPerGroup.emplace(group, std::set<Entity>());
	entitiesPerGroup[group].emplace(entity);
	groupPerEntity.emplace(std::pair<int, std::vector<std::string>>(entity.GetID(), std::vector<std::string>()));
	groupPerEntity[entity.GetID()].emplace_back(group);
}

bool Registry::EntityBelongsToGroup(Entity entity, const std::string& group) const
{
	// Error checking and Validation

	if (entitiesPerGroup.find(group) == entitiesPerGroup.end())
	{
		//Logger::Err("The Group [" + group + "]  Does Not Exist");
		return false;
	}

	const auto& groupEntities = entitiesPerGroup.at(group);

	return groupEntities.find(entity.GetID()) != groupEntities.end();
}

std::vector<Entity> Registry::GetEntitiesByGroup(const std::string& group) const
{
	const auto& setOfEntities = entitiesPerGroup.at(group);

	return std::vector<Entity>(setOfEntities.begin(), setOfEntities.end()); // This create a new vector from the set
}

bool Registry::DoesGroupExist(const std::string& group) const
{
	// If the group exists
	if (entitiesPerGroup.find(group) != entitiesPerGroup.end())
		return true;

	return false;
}

void Registry::RemoveEntityGroup(Entity entity)
{

	const auto& entID = entity.GetID();
	if (groupPerEntity.find(entID) != groupPerEntity.end())
	{
		for (const auto& groupName : groupPerEntity[entID])
		{
			auto group = entitiesPerGroup.find(groupName);
			if (group != entitiesPerGroup.end())
			{
				auto entityInGroup = group->second.find(entity);
				if (entityInGroup != group->second.end())
				{
					group->second.erase(entityInGroup);
				}
			}
		}
		groupPerEntity.erase(entID);
	}

}

Registry& Registry::Instance()
{
	if (instance == nullptr)
	{
		Logger::Log("Creating new Registry Instance!\n");
		instance.reset(new Registry());
	}
	return *instance;
}

Entity Registry::GetEntityFromID(int id)
{
	for (const auto& entity : entities)
	{
		if (entity.GetID() == id)
			return entity;
	}
	return Entity(-1);
}

