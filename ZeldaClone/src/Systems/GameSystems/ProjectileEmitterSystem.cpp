#include "ProjectileEmitterSystem.h"
#include "../../Events/EventManager.h"

#include "../../Events/GamePadButtonPressedEvent.h"
#include "../../Components/HealthComponent.h"
#include "../../Components/TransformComponent.h"
#include "../../Components/SpriteComponent.h"
#include "../../Components/AnimationComponent.h"
#include "../../Components/RigidBodyComponent.h"
#include "../../Components/BoxColliderComponent.h"
#include "../../Components/ProjectileComponent.h"
#include "../../Components/ProjectileEmitterComponent.h"
#include "../../Components/GameComponent.h"
#include "../../Components/AIComponent.h"
#include "../GameSystems/KeyboardControlSystem.h"
#include "../GameSystems/ProjectileLifeCycleSystem.h"
#include "../../States/GameState.h"
#include <SDL.h>
#include "../../Systems/SoundFXSystem.h"
#include "GamePadSystem.h"
#include <map>

void ProjectileEmitterSystem::ItemUsed()
{
	auto player = Registry::Instance().GetEntityByTag("player");

	auto& rigid = player.GetComponent<RigidBodyComponent>();

	rigid.velocity = glm::vec2(0);

	if (Game::mSelectedItem == Game::ItemType::WOOD_BOW ||
		Game::mSelectedItem == Game::ItemType::MAGIC_BOW)
	{
		// Bow uses rupees, must be greater than zero!
		if (GameState::totalRupees > 0)
		{
			UseItem(projectileAttributeMap.at("bow"));

			// Play the arrow sound!
			Registry::Instance().GetSystem<SoundFXSystem>().PlaySoundFX(game.GetAssetManager(), "boomerang_arrow", 0, 1);
			// Subtract from the total rupees when using the bow!
			GameState::totalRupees--;
		}

		KeyboardControlSystem::keyDown = true;
	}
	else if (Game::mSelectedItem == Game::ItemType::BOMB)
	{
		if (GameState::totalBombs > 0)
		{
			UseItem(projectileAttributeMap.at("bomb"));
			GameState::totalBombs--;
			Registry::Instance().GetSystem<SoundFXSystem>().PlaySoundFX(game.GetAssetManager(), "bomb_drop", 0, 1);
		}

		KeyboardControlSystem::keyDown = true;
	}
	else if (Game::mSelectedItem == Game::ItemType::CANDLE)
	{
		UseItem(projectileAttributeMap.at("candle"));
		Registry::Instance().GetSystem<SoundFXSystem>().PlaySoundFX(game.GetAssetManager(), "candle", 0, 1);
		KeyboardControlSystem::keyDown = true;
	}
	else if (Game::mSelectedItem == Game::ItemType::BOOMERANG)
	{
		if (!boomerangReturned)
		{
			UseItem(projectileAttributeMap.at("boomerang"));
			Registry::Instance().GetSystem<SoundFXSystem>().PlaySoundFX(game.GetAssetManager(), "boomerang_arrow", 0, 1);
			KeyboardControlSystem::keyDown = true;
		}
	}
	else if (Game::mSelectedItem == Game::ItemType::MAGIC_ROD)
	{
		UseMagicWand();

		UseItem(projectileAttributeMap.at("magic_rod"));
		Registry::Instance().GetSystem<SoundFXSystem>().PlaySoundFX(game.GetAssetManager(), "magic_rod", 0, 1);
		KeyboardControlSystem::keyDown = true;
	}
}

void ProjectileEmitterSystem::SwordUsed()
{
	auto player = Registry::Instance().GetEntityByTag("player");

	auto& rigid = player.GetComponent<RigidBodyComponent>();

	rigid.velocity = glm::vec2(0);
	// Do not use the sword if we do not have a sword
	if (game.HasSword())
	{
		UseSword();
		// If the player life is full, allow sword beam projectile
		if (fullLife)
		{
			// Create Sword beam projectile
			UseItem(projectileAttributeMap.at("beam"));

			Registry::Instance().GetSystem<SoundFXSystem>().PlaySoundFX(game.GetAssetManager(), "sword_shoot", 0, 1);
		}
		else
			Registry::Instance().GetSystem<SoundFXSystem>().PlaySoundFX(game.GetAssetManager(), "sword_slash", 0, 1);

		swordTimer.Start();
		KeyboardControlSystem::keyDown = true;
	}
}

ProjectileEmitterSystem::ProjectileEmitterSystem()
	: game(Game::Instance())
{
	RequiredComponent<ProjectileEmitterComponent>();
	RequiredComponent<TransformComponent>();

	boomerangReturned = false;
	magicBeam = false;
	fullLife = false;
	swordTimer.Stop();

	LoadMapAttributes(game.GetLuaState(), "ProjectileAttributes");
}

void ProjectileEmitterSystem::UseItem(ProjectileAttrib attrib)
{
	auto player = Registry::Instance().GetEntityByTag("player");

	auto& projectileEmitter = player.GetComponent<ProjectileEmitterComponent>();
	const auto& transform = player.GetComponent<TransformComponent>();
	const auto& rigidbody = player.GetComponent<RigidBodyComponent>();

	// If the entity has a sprite --> Position the projectile in the center
	glm::vec2 projectilePosition = transform.position;

	const auto& sprite = player.GetComponent<SpriteComponent>();
	projectilePosition.x += (transform.scale.x * sprite.width / 2);
	projectilePosition.y += (transform.scale.y * sprite.height / 2);

	glm::vec2 projectileVelocity = projectileEmitter.projectileVelocity;
	glm::vec2 BoxOffset = glm::vec2(0, 0);
	glm::vec2 BoxSize = glm::vec2(0, 0);

	// If the duration is not put in, default duration 
	if (attrib.duration == 0)
	{
		//attrib.duration = projectileEmitter.projectileDuration;
	}

	// Create direction variables the change the velocity direction based on which way we are facing
	int directionX = 0;
	int directionY = 0;

	// Use the fire ahead of the player in the direction the player is facing
	if (KeyboardControlSystem::dir == UP)
	{
		if (attrib.numFrames == 1)
		{
			projectilePosition.x = (transform.position.x + attrib.transOffsetUp.x);
			projectilePosition.y = (transform.position.y + attrib.transOffsetUp.x) - sprite.height * 3;
		}
		else
		{
			projectilePosition.x = (transform.position.x + attrib.transOffsetUp.x);
			projectilePosition.y = (transform.position.y + attrib.transOffsetUp.x) - sprite.height * 3;
			// attrib.srcRectX  += attrib.width * 1;

		}
		BoxSize = attrib.boxSizeUp;
		BoxOffset = attrib.upOffset;
		directionY = -1;
	}
	if (KeyboardControlSystem::dir == RIGHT)
	{
		if (attrib.numFrames == 1)
		{
			projectilePosition.x = ((transform.position.x + attrib.transOffsetRight.x) + sprite.width * 3);
			projectilePosition.y = (transform.position.y + attrib.transOffsetRight.y);
		}
		else
		{
			projectilePosition.x = ((transform.position.x + attrib.transOffsetRight.x) + sprite.width * 3);
			projectilePosition.y = (transform.position.y + attrib.transOffsetRight.y);
			attrib.srcRectX += attrib.width * 1;
		}
		BoxSize = attrib.boxSizeRight;
		BoxOffset = attrib.rightOffset;
		directionX = 1;
	}
	if (KeyboardControlSystem::dir == DOWN)
	{
		if (attrib.numFrames == 1)
		{
			projectilePosition.x = (transform.position.x + attrib.transOffsetDown.x);
			projectilePosition.y = ((transform.position.y + attrib.transOffsetDown.y) + sprite.height * 3);
		}
		else
		{
			projectilePosition.x = (transform.position.x + attrib.transOffsetDown.x);
			projectilePosition.y = ((transform.position.y + attrib.transOffsetDown.y) + sprite.height * 3);
			attrib.srcRectX += attrib.width * 2;
		}
		BoxSize = attrib.boxSizeDown;
		BoxOffset = attrib.downOffset;
		directionY = 1;
	}
	if (KeyboardControlSystem::dir == LEFT)
	{
		if (attrib.numFrames == 1)
		{
			projectilePosition.x = ((transform.position.x + attrib.transOffsetLeft.x) - sprite.width * 3);
			projectilePosition.y = (transform.position.y + attrib.transOffsetLeft.y);
		}
		else
		{
			projectilePosition.x = ((transform.position.x + attrib.transOffsetLeft.x) - sprite.width * 3);
			projectilePosition.y = (transform.position.y + attrib.transOffsetLeft.y);
			attrib.srcRectX += attrib.width * 3;
		}
		BoxSize = attrib.boxSizeLeft;
		BoxOffset = attrib.leftOffset;
		directionX = -1;
	}

	// Create new projectile entity and add it to the world
	Entity newItem = player.registry->CreateEntity();
	newItem.Group(attrib.group);
	newItem.AddComponent<TransformComponent>(projectilePosition, glm::vec2(attrib.scale.x, attrib.scale.y), 0.0);

	if (attrib.group != "bomber")
	{
		// Set the projectile velocity based on the direction 
		projectileVelocity.x = projectileEmitter.projectileVelocity.x * directionX;
		projectileVelocity.y = projectileEmitter.projectileVelocity.y * directionY;
		newItem.AddComponent<RigidBodyComponent>(projectileVelocity);
		newItem.AddComponent<BoxColliderComponent>((int)BoxSize.x / transform.scale.x, (int)BoxSize.y / transform.scale.y, glm::vec2(BoxOffset.x, BoxOffset.y));
	}
	// Does the projectile have an animation component?
	if (attrib.animation)
	{
		// TODO: Change the frame speed from hard coded to individual --> attrib.frame_speed
		newItem.AddComponent<AnimationComponent>(attrib.numFrames, 10, attrib.vertical, true);
	}
	projectileEmitter.isFriendly = true;
	newItem.AddComponent<SpriteComponent>(attrib.sprite_name, attrib.width, attrib.height, 2, false, attrib.srcRectX, attrib.srcRectY);
	newItem.AddComponent<ProjectileEmitterComponent>(projectileEmitter.projectileVelocity, 0, attrib.duration, projectileEmitter.hitPercentDamage, projectileEmitter.isFriendly);
	newItem.AddComponent<ProjectileComponent>(true, 10, attrib.duration);
	newItem.AddComponent<GameComponent>();

	if (attrib.group == "boomerang")
	{
		auto& boomerTimer = newItem.GetComponent<ProjectileComponent>();
		boomerTimer.boomTimer.Start();
	}
}

void ProjectileEmitterSystem::UseSword()
{
	auto player = Registry::Instance().GetEntityByTag("player");
	auto& playerSprite = player.GetComponent<SpriteComponent>();
	auto& playerCollider = player.GetComponent<BoxColliderComponent>();
	const auto& playerHealth = player.GetComponent<HealthComponent>();

	auto sword = Registry::Instance().GetEntityByTag("the_sword");
	auto& swordCollider = sword.GetComponent<BoxColliderComponent>();

	auto shield = Registry::Instance().GetEntityByTag("the_shield");
	auto& shieldCollider = shield.GetComponent<BoxColliderComponent>();

	if (KeyboardControlSystem::dir == UP)
	{
		playerSprite.srcRect.y = playerSprite.height * 5;
		playerCollider.offset.y = 60;

		swordCollider.height = 40;
		swordCollider.width = 10;
		swordCollider.offset.x = 60;
		swordCollider.offset.y = 12;

		shieldCollider.offset.y = 60;
	}

	if (KeyboardControlSystem::dir == RIGHT)
	{
		playerSprite.srcRect.y = playerSprite.height * 5;
		playerCollider.offset.x = 30;

		swordCollider.height = 10;
		swordCollider.width = 40;
		swordCollider.offset.x = 78;
		swordCollider.offset.y = 58;

		shieldCollider.offset.x = 70;
	}

	if (KeyboardControlSystem::dir == DOWN)
	{
		playerSprite.srcRect.y = playerSprite.height * 5;
		playerCollider.offset.y = 30;

		swordCollider.height = 40;
		swordCollider.width = 10;
		swordCollider.offset.x = 64;
		swordCollider.offset.y = 74;

		shieldCollider.offset.y = 60;
	}

	if (KeyboardControlSystem::dir == LEFT)
	{
		playerSprite.srcRect.y = playerSprite.height * 5;
		playerCollider.offset.x = 60;

		swordCollider.height = 10;
		swordCollider.width = 40;
		swordCollider.offset.x = 12;
		swordCollider.offset.y = 56;

		shieldCollider.offset.x = 64;
	}

	// Check to see if health is full for sword beam
	if (playerHealth.healthPercentage == (playerHealth.maxHearts * 2)) fullLife = true;
	else fullLife = false;
}

void ProjectileEmitterSystem::UseMagicWand()
{
	auto player = Registry::Instance().GetEntityByTag("player");

	const auto& health = player.GetComponent<HealthComponent>();
	auto& sprite = player.GetComponent<SpriteComponent>();
	sprite.srcRect.y = sprite.height * 6;
}

void ProjectileEmitterSystem::LoadMapAttributes(sol::state& lua, const std::string& fileName)
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

	sol::table atrib = lua["items"];

	int i = 1;

	while (true)
	{
		sol::optional<sol::table> hasAttribs = atrib[i];

		if (hasAttribs == sol::nullopt)
			break;

		if (hasAttribs != sol::nullopt)
		{
			sol::table item_attrib = atrib[i];

			ProjectileAttrib newItem;
			std::string name = item_attrib["name"];

			newItem.group = item_attrib["group"];
			newItem.sprite_name = item_attrib["asset_id"];
			newItem.width = item_attrib["width"].get_or(16);
			newItem.height = item_attrib["height"].get_or(16);
			newItem.srcRectX = item_attrib["src_rect_x"].get_or(0);
			newItem.srcRectY = item_attrib["src_rect_y"].get_or(0);
			newItem.numFrames = item_attrib["num_frames"].get_or(1);

			newItem.scale = glm::vec2(
				item_attrib["scale"]["x"].get_or(4),
				item_attrib["scale"]["y"].get_or(4)
			);

			newItem.transOffsetUp = glm::vec2(
				item_attrib["trans_offset_up"]["x"].get_or(0),
				item_attrib["trans_offset_up"]["y"].get_or(0)
			);

			newItem.transOffsetDown = glm::vec2(
				item_attrib["trans_offset_down"]["x"].get_or(0),
				item_attrib["trans_offset_down"]["y"].get_or(0)
			);

			newItem.transOffsetRight = glm::vec2(
				item_attrib["trans_offset_right"]["x"].get_or(0),
				item_attrib["trans_offset_right"]["y"].get_or(0)
			);

			newItem.transOffsetLeft = glm::vec2(
				item_attrib["trans_offset_left"]["x"].get_or(0),
				item_attrib["trans_offset_left"]["y"].get_or(0)
			);

			newItem.boxSizeUp = glm::vec2(
				item_attrib["box_size_up"]["x"].get_or(0),
				item_attrib["box_size_up"]["y"].get_or(0)
			);

			newItem.boxSizeDown = glm::vec2(
				item_attrib["box_size_down"]["x"].get_or(0),
				item_attrib["box_size_down"]["y"].get_or(0)
			);

			newItem.boxSizeRight = glm::vec2(
				item_attrib["box_size_right"]["x"].get_or(0),
				item_attrib["box_size_right"]["y"].get_or(0)
			);

			newItem.boxSizeLeft = glm::vec2(
				item_attrib["box_size_left"]["x"].get_or(0),
				item_attrib["box_size_left"]["y"].get_or(0)
			);

			newItem.upOffset = glm::vec2(
				item_attrib["up_offset"]["x"].get_or(0),
				item_attrib["up_offset"]["y"].get_or(0)
			);

			newItem.downOffset = glm::vec2(
				item_attrib["down_offset"]["x"].get_or(0),
				item_attrib["down_offset"]["y"].get_or(0)
			);

			newItem.rightOffset = glm::vec2(
				item_attrib["right_offset"]["x"].get_or(0),
				item_attrib["right_offset"]["y"].get_or(0)
			);

			newItem.leftOffset = glm::vec2(
				item_attrib["left_offset"]["x"].get_or(0),
				item_attrib["left_offset"]["y"].get_or(0)
			);
			newItem.duration = item_attrib["duration"].get_or(3000);
			newItem.animation = item_attrib["animation"].get_or(false);
			newItem.vertical = item_attrib["vertical"].get_or(false);

			// Place the item in the map
			projectileAttributeMap.emplace(name, newItem);

		}
		i++;
	}
}

void ProjectileEmitterSystem::OnKeyPressed(KeyPressedEvent& event)
{
	if (!game.PlayerHold())
	{
		if (event.symbol == game.GetKeyBindings().at(Game::Action::USE_ITEM) && !KeyboardControlSystem::keyDown)
		{
			ItemUsed();
		}
		else if (event.symbol == game.GetKeyBindings().at(Game::Action::ATTACK) && !KeyboardControlSystem::keyDown && swordTimer.GetTicks() == 0)
		{
			SwordUsed();
		}
	}
}

void ProjectileEmitterSystem::OnBtnPressed(GamePadButtonPressedEvent& event)
{
	if (!game.PlayerHold())
	{
		if (event.button == game.GetBtnBindings().at(Game::Action::USE_ITEM) && !KeyboardControlSystem::keyDown)
		{
			ItemUsed();
			
		}
		else if (event.button == game.GetBtnBindings().at(Game::Action::ATTACK) && !KeyboardControlSystem::keyDown && swordTimer.GetTicks() == 0)
		{
			SwordUsed();
		}
	}
}

void ProjectileEmitterSystem::Update(Registry& registry)
{
	for (auto& entity : GetSystemEntities())
	{
		// Update the boomerang position
		if (entity.BelongsToGroup("boomerang"))
			UpdateBoomerang(entity);

		// This is the wait time for the player sword 
		if (swordTimer.GetTicks() > 250)
			swordTimer.Stop();

		// Check the enemy projectile 
		if (entity.BelongsToGroup("enemies"))
		{
			const auto& ai = entity.GetComponent<AIComponent>();
			// If the entity is just a regular enmemy, use this update
			if (ai.GetEnemyType() != AIComponent::EnemyType::NO_TYPE)
				EnemyProjectileUpdate(entity);
			// If the entity is Boss, use this update
			if (ai.GetBossType() != AIComponent::EnemyBossType::NOT_A_BOSS)
				BossProjectileUpdate(entity);
		}
	}
}

void ProjectileEmitterSystem::EnemyProjectileUpdate(Entity& entity)
{
	auto& projectileEmitter = entity.GetComponent<ProjectileEmitterComponent>();
	auto& projectileTransform = entity.GetComponent<TransformComponent>();
	auto& rigid = entity.GetComponent<RigidBodyComponent>();
	const auto& ai = entity.GetComponent<AIComponent>();

	if (projectileEmitter.shotTriggered && !projectileEmitter.shotFired)
	{
		Entity enemyProjectile = entity.registry->CreateEntity();
		enemyProjectile.Group("projectile");
		// Variables that can change based on the type of enemy
		int offsetX = 0;
		int offsetY = 0;
		int srcRectX = 0;
		int srcRectY = 0;
		int scale = 0;

		switch (ai.GetEnemyType())
		{
		case AIComponent::EnemyType::OCTOROK:
			srcRectX = 48;
			srcRectY = 0;
			scale = 2;
			break;

		case AIComponent::EnemyType::MOBLIN:
			// Set the srcRect for the projectile sprite
			srcRectY = 4 * 16;
			scale = 4;
			if (projectileEmitter.shootUp)
				srcRectX = 0 * 16;
			else if (projectileEmitter.shootRight)
				srcRectX = 1 * 16;
			else if (projectileEmitter.shootDown)
				srcRectX = 2 * 16;
			else if (projectileEmitter.shootLeft)
				srcRectX = 3 * 16;
			break;

		case AIComponent::EnemyType::ZORA:
			// TODO: Create a sprite sheet of enemy projectiles
			// enemyProjectile.AddComponent<AnimationComponent>(4, 20, false);
			break;
		default:
			break;
		}

		// Set the velocity and direction of the projectile based on when it 
		// was triggered
		if (projectileEmitter.shootUp)
		{
			projectileEmitter.projectileVelocity = glm::vec2(0, -200);
			projectileEmitter.shootUp = false;
			rigid.up = true;
			offsetX = 20;
			offsetY = 0;

		}
		else if (projectileEmitter.shootRight)
		{
			projectileEmitter.projectileVelocity = glm::vec2(200, 0);
			projectileEmitter.shootRight = false;
			rigid.right = true;
			offsetX = 0;
			offsetY = 20;
		}
		else if (projectileEmitter.shootDown)
		{
			projectileEmitter.projectileVelocity = glm::vec2(0, 200);
			projectileEmitter.shootDown = false;
			rigid.down = true;
			offsetX = 20;
			offsetY = 0;
		}
		else if (projectileEmitter.shootLeft)
		{
			projectileEmitter.projectileVelocity = glm::vec2(-200, 0);
			projectileEmitter.shootLeft = false;
			rigid.left = true;
			offsetX = 0;
			offsetY = 20;
		}

		enemyProjectile.AddComponent<TransformComponent>(glm::vec2(projectileTransform.position.x + offsetX, projectileTransform.position.y + offsetY), glm::vec2(scale, scale), 0.0);

		enemyProjectile.AddComponent<RigidBodyComponent>(projectileEmitter.projectileVelocity);
		enemyProjectile.AddComponent<BoxColliderComponent>(16, 16, glm::vec2(0, 0));

		projectileEmitter.isFriendly = false;
		enemyProjectile.AddComponent<SpriteComponent>("items", 16, 16, 2, false, srcRectX, srcRectY);
		enemyProjectile.AddComponent<ProjectileComponent>(false, projectileEmitter.hitPercentDamage, projectileEmitter.projectileDuration);
		enemyProjectile.AddComponent<GameComponent>();

		projectileEmitter.shotFired = true;
	}
}

void ProjectileEmitterSystem::BossProjectileUpdate(Entity& entity)
{
	// Player variables
	auto player = Registry::Instance().GetEntityByTag("player");
	auto& playerPos = player.GetComponent<TransformComponent>();

	// Boss Variables
	auto& projectileEmitter = entity.GetComponent<ProjectileEmitterComponent>();
	auto& projectileTransform = entity.GetComponent<TransformComponent>();
	auto& rigid = entity.GetComponent<RigidBodyComponent>();
	const auto& ai = entity.GetComponent<AIComponent>();

	if (ai.GetBossType() == AIComponent::EnemyBossType::AQUAMENTUS)
	{
		if (projectileEmitter.shotTriggered && !projectileEmitter.shotFired)
		{
			Entity enemyProjectile = entity.registry->CreateEntity();
			Entity enemyProjectile2 = entity.registry->CreateEntity();
			Entity enemyProjectile3 = entity.registry->CreateEntity();

			// Have the fireball change direction based on the player position
			glm::vec2 direction = glm::normalize(playerPos.position - projectileTransform.position);


			enemyProjectile.Group("projectile");
			enemyProjectile.AddComponent<TransformComponent>(glm::vec2(projectileTransform.position.x, projectileTransform.position.y), glm::vec2(4, 4), 0.0);

			enemyProjectile.AddComponent<RigidBodyComponent>(glm::vec2(direction.x * 300, direction.y * 300));
			enemyProjectile.AddComponent<BoxColliderComponent>(16, 16, glm::vec2(0, 0));

			projectileEmitter.isFriendly = false;
			enemyProjectile.AddComponent<SpriteComponent>("fire_ball", 16, 16, 2, false, 0, 0);
			enemyProjectile.AddComponent<AnimationComponent>(4, 20, false, true, 0);
			enemyProjectile.AddComponent<ProjectileComponent>(false, projectileEmitter.hitPercentDamage, projectileEmitter.projectileDuration);
			enemyProjectile.AddComponent<GameComponent>();

			enemyProjectile2.Group("projectile");
			enemyProjectile2.AddComponent<TransformComponent>(glm::vec2(projectileTransform.position.x, projectileTransform.position.y), glm::vec2(4, 4), 0.0);

			enemyProjectile2.AddComponent<RigidBodyComponent>(glm::vec2(direction.x * 300, direction.y * 400));
			enemyProjectile2.AddComponent<BoxColliderComponent>(16, 16, glm::vec2(0, 0));

			projectileEmitter.isFriendly = false;
			enemyProjectile2.AddComponent<SpriteComponent>("fire_ball", 16, 16, 2, false, 0, 0);
			enemyProjectile2.AddComponent<AnimationComponent>(4, 20, false, true, 0);
			enemyProjectile2.AddComponent<ProjectileComponent>(false, projectileEmitter.hitPercentDamage, projectileEmitter.projectileDuration);
			enemyProjectile2.AddComponent<GameComponent>();

			enemyProjectile3.Group("projectile");
			enemyProjectile3.AddComponent<TransformComponent>(glm::vec2(projectileTransform.position.x, projectileTransform.position.y), glm::vec2(4, 4), 0.0);

			enemyProjectile3.AddComponent<RigidBodyComponent>(glm::vec2(direction.x * 300, direction.y * -300));
			enemyProjectile3.AddComponent<BoxColliderComponent>(16, 16, glm::vec2(0, 0));

			projectileEmitter.isFriendly = false;
			enemyProjectile3.AddComponent<SpriteComponent>("fire_ball", 16, 16, 2, false, 0, 0);
			enemyProjectile3.AddComponent<AnimationComponent>(4, 20, false, true, 0);
			enemyProjectile3.AddComponent<ProjectileComponent>(false, projectileEmitter.hitPercentDamage, projectileEmitter.projectileDuration);
			enemyProjectile3.AddComponent<GameComponent>();



			projectileEmitter.shotFired = true;
		}
	}
}

void ProjectileEmitterSystem::UpdateBoomerang(Entity& entity)
{
	// Player variables
	auto player = Registry::Instance().GetEntityByTag("player");
	auto& playerPos = player.GetComponent<TransformComponent>();

	// Boomerang variables
	auto& rigidbody = entity.GetComponent<RigidBodyComponent>();
	auto& proj = entity.GetComponent<ProjectileComponent>();
	auto& transform = entity.GetComponent<TransformComponent>();

	// Have the boomerang change direction based on the player position
	glm::vec2 direction = glm::normalize(playerPos.position - transform.position);

	if (proj.boomTimer.GetTicks() > 300) // TODO: Change the time based on type of boomerang-->wood/magic
	{
		rigidbody.velocity.x = direction.x * 500; // Changed from 300
		rigidbody.velocity.y = direction.y * 500; // Changed from 300
		boomerangReturned = true;
	}
	if (boomerangReturned)
	{
		bool setKill = false;

		// If the boomerang position is within any of these given parameters --> Kill it
		if (rigidbody.velocity.x > 0 && rigidbody.velocity.y > 0 &&
			transform.position.x >= playerPos.position.x - 32 && transform.position.y >= playerPos.position.y - 32)
			setKill = true;
		else if (rigidbody.velocity.x < 0 && rigidbody.velocity.y > 0 &&
			transform.position.x <= playerPos.position.x + 32 && transform.position.y >= playerPos.position.y - 32)
			setKill = true;
		else if (rigidbody.velocity.x > 0 && rigidbody.velocity.y < 0 &&
			transform.position.x >= playerPos.position.x - 32 && transform.position.y <= playerPos.position.y + 32)
			setKill = true;
		else if (rigidbody.velocity.x < 0 && rigidbody.velocity.y < 0 &&
			transform.position.x <= playerPos.position.x + 32 && transform.position.y <= playerPos.position.y + 32)
			setKill = true;
		else if (rigidbody.velocity.x > 0 && rigidbody.velocity.y == 0 && transform.position.x >= playerPos.position.x - 32)
			setKill = true;
		else if (rigidbody.velocity.x < 0 && rigidbody.velocity.y == 0 && transform.position.x <= playerPos.position.x + 32)
			setKill = true;
		else if (rigidbody.velocity.x == 0 && rigidbody.velocity.y > 0 && transform.position.y >= playerPos.position.y - 32)
			setKill = true;
		else if (rigidbody.velocity.x == 0 && rigidbody.velocity.y < 0 && transform.position.y <= playerPos.position.y + 32)
			setKill = true;

		if (setKill)
		{
			entity.Kill();
			boomerangReturned = false;
		}
	}
}

std::string ProjectileEmitterSystem::ProjectileAttrib::ToString()
{
	std::stringstream ss;
	ss << "Group: " << group << "\n"
		<< "Sprite Name: " << sprite_name << "\n"
		<< "Sprite width: " << width << "\n"
		<< "Sprite height: " << height << "\n"
		<< "Num Frames: " << numFrames << "\n"
		<< "Src Rect: [X: " << srcRectX << ", Y: " << srcRectY << "]" << "\n"
		<< "Scale: [X: " << scale.x << ", Y: " << scale.y << "]" << "\n"
		<< "Tran Offset Up: [X: " << transOffsetUp.x << ", Y: " << transOffsetUp.y << "]" << "\n"
		<< "Tran Offset Down: [X: " << transOffsetDown.x << ", Y: " << transOffsetDown.y << "]" << "\n"
		<< "Tran Offset Left: [X: " << transOffsetLeft.x << ", Y: " << transOffsetLeft.y << "]" << "\n"
		<< "Tran Offset Right: [X: " << transOffsetRight.x << ", Y: " << transOffsetRight.y << "]" << "\n"
		<< "Box Size Up: [X: " << boxSizeUp.x << ", Y: " << boxSizeUp.y << "]" << "\n"
		<< "Box Size Down: [X: " << boxSizeDown.x << ", Y: " << boxSizeDown.y << "]" << "\n"
		<< "Box Size Right: [X: " << boxSizeRight.x << ", Y: " << boxSizeRight.y << "]" << "\n"
		<< "Box Size Left: [X: " << boxSizeLeft.x << ", Y: " << boxSizeLeft.y << "]" << "\n"
		<< "Box offset Up: [X: " << upOffset.x << ", Y: " << upOffset.y << "]" << "\n"
		<< "Box offset Down: [X: " << downOffset.x << ", Y: " << downOffset.y << "]" << "\n"
		<< "Box offset Right: [X: " << rightOffset.x << ", Y: " << rightOffset.y << "]" << "\n"
		<< "Box offset Left: [X: " << leftOffset.x << ", Y: " << leftOffset.y << "]" << "\n"
		<< "Duration: " << duration << "\n"
		<< "Animation: " << animation << "\n"
		<< "Vertical: " << vertical << "\n";

	return ss.str();
}
