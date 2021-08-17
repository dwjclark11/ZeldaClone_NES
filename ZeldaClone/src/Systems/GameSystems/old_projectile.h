if (ItemSelectKeyboardControlSystem::itemType == WOOD_BOW || ItemSelectKeyboardControlSystem::itemType == MAGIC_BOW)
		{
			if (event.symbol == SDLK_SPACE)
			{
				for (auto entity : GetSystemEntities())
				{
					if (entity.HasTag("player"))
					{
						const auto& projectileEmitter = entity.GetComponent<ProjectileEmitterComponent>();
						const auto& transform = entity.GetComponent<TransformComponent>();
						const auto& rigidbody = entity.GetComponent<RigidBodyComponent>();

						// If the entity has a sprite --> Position the projectile in the center
						glm::vec2 projectilePosition = transform.position;

						if (entity.HasComponent<SpriteComponent>())
						{
							auto sprite = entity.GetComponent<SpriteComponent>();
							projectilePosition.x += (transform.scale.x * sprite.width / 2);
							projectilePosition.y += (transform.scale.y * sprite.height / 2);
						}

						glm::vec2 projectileVelocity = projectileEmitter.projectileVelocity;
						int directionX = 0;
						int directionY = 0;
						int srcRectY = 64;
						int srcRectX = 0;
						
						// Change direction of the arrow based on the direction the player is facing
						if (KeyboardControlSystem::dir == UP)
						{
							directionY = -1;
							srcRectX = 0;
						}

						if (KeyboardControlSystem::dir == RIGHT)
						{
							directionX = 1;
							srcRectX = 16;
						}

						if (KeyboardControlSystem::dir == DOWN)
						{
							directionY = 1;
							srcRectX = 32;
						}

						if (KeyboardControlSystem::dir == LEFT)
						{
							directionX = -1;
							srcRectX = 48;
						}
						
						// Set the projectile velocity based on the direction 
						projectileVelocity.x = projectileEmitter.projectileVelocity.x * directionX;
						projectileVelocity.y = projectileEmitter.projectileVelocity.y * directionY;

						// Create new projectile entity and add it to the world
						Entity arrow = entity.registry->CreateEntity();
						arrow.Group("projectile");
						arrow.AddComponent<TransformComponent>(projectilePosition, glm::vec2(4, 4), 0.0);
						arrow.AddComponent<RigidBodyComponent>(projectileVelocity);
						arrow.AddComponent<SpriteComponent>("items", 16, 16, 2, false, srcRectX, srcRectY);
						arrow.AddComponent<ProjectileComponent>(projectileEmitter.isFriendly, projectileEmitter.hitPercentDamage, projectileEmitter.projectileDuration);
						arrow.AddComponent<GameComponent>();
						
						// Play the arrow shot sound
						Game::Instance()->GetSystem<SoundFXSystem>().PlaySoundFX(Game::Instance()->GetAssetManager(), "boomerang_arrow", 0, 1);
					}
				}
			}
		}
		// If the selected item is BOMBS
		else if (ItemSelectKeyboardControlSystem::itemType == BOMB)
		{
			// Make sure that the player has bombs in the inventory
			if (GameState::totalBombs > 0)
			{
				if (event.symbol == SDLK_SPACE && !KeyboardControlSystem::keyDown)
				{
					for (auto entity : GetSystemEntities())
					{
						if (entity.HasTag("player"))
						{
							const auto projectileEmitter = entity.GetComponent<ProjectileEmitterComponent>();
							const auto transform = entity.GetComponent<TransformComponent>();
							const auto rigidbody = entity.GetComponent<RigidBodyComponent>();

							// If the entity has a sprite --> Position the projectile in the center
							glm::vec2 bombPosition = transform.position;
							const auto sprite = entity.GetComponent<SpriteComponent>();
							glm::vec2 bombVelocity = projectileEmitter.projectileVelocity;

							int directionX = 0;
							int directionY = 0;
							int srcRectY = 112;
							int srcRectX = 64;
							
							// place the bombs ahead of the character based on the direction the player is facing
							if (KeyboardControlSystem::dir == UP)
							{
								bombPosition.x = (transform.position.x);
								bombPosition.y = (transform.position.y - sprite.height * 3);
							}

							if (KeyboardControlSystem::dir == RIGHT)
							{
								bombPosition.x = (transform.position.x + sprite.width * 3);
								bombPosition.y = (transform.position.y);
							}

							if (KeyboardControlSystem::dir == DOWN)
							{
								bombPosition.x = (transform.position.x);
								bombPosition.y = (transform.position.y + sprite.height * 3);
							}

							if (KeyboardControlSystem::dir == LEFT)
							{
								bombPosition.x = (transform.position.x - sprite.width * 3);
								bombPosition.y = (transform.position.y);
							}

							// Create new projectile entity and add it to the world
							Entity bomb = entity.registry->CreateEntity();
							bomb.Group("bomber");
							bomb.AddComponent<TransformComponent>(bombPosition, glm::vec2(4, 4), 0.0);
							bomb.AddComponent<SpriteComponent>("items", 16, 16, 2, false, srcRectX, srcRectY);
							bomb.AddComponent<ProjectileComponent>(projectileEmitter.isFriendly, projectileEmitter.hitPercentDamage, projectileEmitter.projectileDuration);
							bomb.AddComponent<GameComponent>();
							//bomb.AddComponent<BoxColliderComponent>(0, 0);
							
							// play the bomb drop sound --> explosion is played in the projectile life system
							Game::Instance()->GetSystem<SoundFXSystem>().PlaySoundFX(Game::Instance()->GetAssetManager(), "bomb_drop", 0, 1);
							
							// Take a bomb out of inventory after using one
							GameState::totalBombs -= 1;
							KeyboardControlSystem::keyDown = true;
						}
					}
				}
			}
		}
		// If the item selected is the CANDLE
		else if (ItemSelectKeyboardControlSystem::itemType == CANDLE)
		{
			if (event.symbol == SDLK_SPACE)
			{
				for (auto entity : GetSystemEntities())
				{
					if (entity.HasTag("player"))
					{
						const auto projectileEmitter = entity.GetComponent<ProjectileEmitterComponent>();
						const auto transform = entity.GetComponent<TransformComponent>();
						const auto rigidbody = entity.GetComponent<RigidBodyComponent>();
						auto& sprite = entity.GetComponent<SpriteComponent>();
						
						// If the entity has a sprite --> Position the projectile in the center
						glm::vec2 flamePosition = transform.position;
						glm::vec2 flameVelocity = projectileEmitter.projectileVelocity;
						
						int directionX = 0;
						int directionY = 0;
						int srcRectY = 96;
						int srcRectX = 0;
						
						// Use the fire ahead of the player in the direction the player is facing
						if (KeyboardControlSystem::dir == UP)
						{
							flamePosition.x = (transform.position.x);
							flamePosition.y = (transform.position.y - sprite.height * 3);
							directionY = -1;
						}

						if (KeyboardControlSystem::dir == RIGHT)
						{
							flamePosition.x = (transform.position.x + sprite.width * 3);
							flamePosition.y = (transform.position.y);
							directionX = 1;
						}

						if (KeyboardControlSystem::dir == DOWN)
						{
							flamePosition.x = (transform.position.x);
							flamePosition.y = (transform.position.y + sprite.height * 3);
							directionY = 1;
						}

						if (KeyboardControlSystem::dir == LEFT)
						{
							flamePosition.x = (transform.position.x - sprite.width * 3);
							flamePosition.y = (transform.position.y);
							directionX = -1;
						}
						
						// set the velocity/direction of the flame
						flameVelocity.x = projectileEmitter.projectileVelocity.x * directionX;
						flameVelocity.y = projectileEmitter.projectileVelocity.y * directionY;
						
						// Set the duration of the flame to be 200ms
						int flameDuration = 200;
						
						// Create new projectile entity and add it to the world
						Entity candleFlame = entity.registry->CreateEntity();
						candleFlame.Group("projectile");
						candleFlame.AddComponent<TransformComponent>(flamePosition, glm::vec2(4, 4), 0.0);
						candleFlame.AddComponent<RigidBodyComponent>(flameVelocity);
						candleFlame.AddComponent<SpriteComponent>("items", 16, 16, 2, false, srcRectX, srcRectY);
						candleFlame.AddComponent<ProjectileComponent>(projectileEmitter.isFriendly, projectileEmitter.hitPercentDamage, flameDuration);
						candleFlame.AddComponent<BoxColliderComponent>(14, 14, glm::vec2(5, 5));
						candleFlame.AddComponent<GameComponent>();
						
						// Play the Candle flame sound
						Game::Instance()->GetSystem<SoundFXSystem>().PlaySoundFX(Game::Instance()->GetAssetManager(), "candle", 0, 1);
					}
				}
			}
		}
		// If the item selected is the BOOMERANG
		else if (ItemSelectKeyboardControlSystem::itemType == BOOMERANG)
		{
			if (event.symbol == SDLK_SPACE && !KeyboardControlSystem::keyDown)
			{
				for (auto entity : GetSystemEntities())
				{
					if (entity.HasTag("player"))
					{
						if (!playerSet)
						{
							playerPosition = entity.GetComponent<TransformComponent>().position;
							playerSet = true;
						}

						const auto projectileEmitter = entity.GetComponent<ProjectileEmitterComponent>();
						const auto transform = entity.GetComponent<TransformComponent>();
						const auto rigidbody = entity.GetComponent<RigidBodyComponent>();

						// If the entity has a sprite --> Position the projectile in the center
						glm::vec2 projectilePosition = transform.position;


						const auto sprite = entity.GetComponent<SpriteComponent>();
						glm::vec2 projectileVelocity = projectileEmitter.projectileVelocity;
						
						int directionX = 0;
						int directionY = 0;
						int srcRectY = 112;
						int srcRectX = 0;
						int boxColliderX = 8;
						int boxColliderY = 8;
						glm::vec2 boxColliderOffset = glm::vec2(0);

						// Set the direction of the boomerang based on the direction the player is facing
						if (KeyboardControlSystem::dir == UP)
						{
							projectilePosition.x = (transform.position.x);
							projectilePosition.y = (transform.position.y - sprite.height * 3);
							directionY = -1;
							
							// Create the box collider offset for the boomerang
							boxColliderOffset.x = 14;
							boxColliderOffset.y = 16;
						}

						if (KeyboardControlSystem::dir == RIGHT)
						{
							projectilePosition.x = (transform.position.x + sprite.width * 3);
							projectilePosition.y = (transform.position.y);
							directionX = 1;
							
							// Create the box collider offset for the boomerang		
							boxColliderOffset.x = 14;
							boxColliderOffset.y = 16;
						}

						if (KeyboardControlSystem::dir == DOWN)
						{
							projectilePosition.x = (transform.position.x);
							projectilePosition.y = (transform.position.y + sprite.height * 3);
							directionY = 1;
							
							// Create the box collider offset for the boomerang
							boxColliderOffset.x = 14;
							boxColliderOffset.y = 16;
						}

						if (KeyboardControlSystem::dir == LEFT)
						{
							projectilePosition.x = (transform.position.x - sprite.width * 3);
							projectilePosition.y = (transform.position.y);
							directionX = -1;
							
							// Create the box collider offset for the boomerang
							boxColliderOffset.x = 14;
							boxColliderOffset.y = 16;
						}
						
						// Set the velocity of the boomerang
						projectileVelocity.x = projectileEmitter.projectileVelocity.x * directionX;
						projectileVelocity.y = projectileEmitter.projectileVelocity.y * directionY;
						
						// Set the life duration of the boomerang to 3 seconds
						int boomerangDuration = 3000;
						
						if (!boomerangReturned)
						{
							// Create new projectile entity and add it to the world
							Entity boomerang = entity.registry->CreateEntity();
							boomerang.Group("boomerang");
							boomerang.AddComponent<TransformComponent>(projectilePosition, glm::vec2(4, 4), 0.0);
							boomerang.AddComponent<RigidBodyComponent>(glm::vec2(projectileVelocity.x * 2, projectileVelocity.y * 2));
							boomerang.AddComponent<SpriteComponent>("items", 16, 16, 5, false, srcRectX, srcRectY);
							boomerang.AddComponent<AnimationComponent>(4, 20, false);
							boomerang.AddComponent<ProjectileComponent>(projectileEmitter.isFriendly, projectileEmitter.hitPercentDamage, boomerangDuration);
							boomerang.AddComponent<BoxColliderComponent>(boxColliderX, boxColliderY, boxColliderOffset);
							boomerang.AddComponent<ProjectileEmitterComponent>();
							boomerang.AddComponent<GameComponent>();
							
							// Play the boomerang sound
							Game::Instance()->GetSystem<SoundFXSystem>().PlaySoundFX(Game::Instance()->GetAssetManager(), "boomerang_arrow", 0, 1);
						}
					}
				}
				KeyboardControlSystem::keyDown = true;
			}
		}
		// If the selected item is the MAGIC ROD
		else if (ItemSelectKeyboardControlSystem::itemType == MAGIC_ROD)
		{
			if (event.symbol == SDLK_SPACE && !KeyboardControlSystem::keyDown)
			{
				for (auto entity : GetSystemEntities())
				{
					if (entity.HasTag("player"))
					{
						const auto projectileEmitter = entity.GetComponent<ProjectileEmitterComponent>();
						const auto transform = entity.GetComponent<TransformComponent>();
						const auto rigidbody = entity.GetComponent<RigidBodyComponent>();

						// If the entity has a sprite --> Position the projectile in the center
						glm::vec2 projectilePosition = transform.position;
						glm::vec2 rodPosition = transform.position;

						auto& sprite = entity.GetComponent<SpriteComponent>();

						glm::vec2 projectileVelocity = projectileEmitter.projectileVelocity;
						int directionX = 0;
						int directionY = 0;
						int srcRectYROD = 80;
						int srcRectYBEAM = 96;
						int srcRectXROD = 64;
						int srcRectXBEAM = 64;

						if (KeyboardControlSystem::dir == UP)
						{
							projectilePosition.x = (transform.position.x);
							projectilePosition.y = (transform.position.y - sprite.height * 4);
							rodPosition.x = (transform.position.x - 20);
							rodPosition.y = (transform.position.y - sprite.height * 4);
							directionY = -1;
							srcRectXROD = 64;
							srcRectXBEAM = 64;
							sprite.srcRect.y = sprite.height * 2;
						}

						if (KeyboardControlSystem::dir == RIGHT)
						{
							projectilePosition.x = ((transform.position.x - 15) + sprite.width * 4);
							projectilePosition.y = (transform.position.y);
							rodPosition.x = (transform.position.x + sprite.width * 4);
							rodPosition.y = (transform.position.y + 10);
							directionX = 1;
							srcRectXROD = 80;
							srcRectXBEAM = 80;
							sprite.srcRect.y = sprite.height * 2;
						}

						if (KeyboardControlSystem::dir == DOWN)
						{
							projectilePosition.x = (transform.position.x);
							projectilePosition.y = (transform.position.y + sprite.height * 4);
							rodPosition.x = (transform.position.x + 5);
							rodPosition.y = (transform.position.y + sprite.height * 4);
							directionY = 1;
							srcRectXROD = 96;
							srcRectXBEAM = 96;
							sprite.srcRect.y = sprite.height * 2;
						}

						if (KeyboardControlSystem::dir == LEFT)
						{
							projectilePosition.x = ((transform.position.x + 10) - sprite.width * 4);
							projectilePosition.y = (transform.position.y);
							rodPosition.x = ((transform.position.x + 5) - sprite.width * 4);
							rodPosition.y = (transform.position.y + 5);
							directionX = -1;
							srcRectXROD = 112;
							srcRectXBEAM = 112;
							sprite.srcRect.y = sprite.height * 2;
						}

						projectileVelocity.x = projectileEmitter.projectileVelocity.x * directionX;
						projectileVelocity.y = projectileEmitter.projectileVelocity.y * directionY;
						
						int beamDuration = 2000;
						int rodDuration = 200;

						// Create new projectile entity and add it to the world
						Entity magicRod = entity.registry->CreateEntity();
						magicRod.Group("projectile");
						magicRod.AddComponent<TransformComponent>(rodPosition, glm::vec2(4, 4), 0.0);
						magicRod.AddComponent<SpriteComponent>("items", 16, 16, 2, false, srcRectXROD, srcRectYROD);
						magicRod.AddComponent<ProjectileComponent>(projectileEmitter.isFriendly, projectileEmitter.hitPercentDamage, rodDuration);
						magicRod.AddComponent<GameComponent>();

						Entity beam = entity.registry->CreateEntity();
						beam.Group("projectile");
						beam.AddComponent<TransformComponent>(projectilePosition, glm::vec2(4, 4), 0.0);
						beam.AddComponent<RigidBodyComponent>(glm::vec2(projectileVelocity.x * 2, projectileVelocity.y * 2));
						beam.AddComponent<SpriteComponent>("items", 16, 16, 2, false, srcRectXBEAM, srcRectYBEAM);
						beam.AddComponent<ProjectileComponent>(projectileEmitter.isFriendly, projectileEmitter.hitPercentDamage, beamDuration);
						beam.AddComponent<BoxColliderComponent>(16, 16);
						beam.AddComponent<GameComponent>();
						
						// Play the beam sound
						Game::Instance()->GetSystem<SoundFXSystem>().PlaySoundFX(Game::Instance()->GetAssetManager(), "magic_rod", 0, 1);
					}
				}
				KeyboardControlSystem::keyDown = true;
			}
		}
		// Swing the sword
		if (event.symbol == SDLK_RSHIFT && !KeyboardControlSystem::keyDown)
		{
			for (auto entity : GetSystemEntities())
			{
				if (entity.HasTag("player"))
				{
					const auto projectileEmitter = entity.GetComponent<ProjectileEmitterComponent>();
					const auto transform = entity.GetComponent<TransformComponent>();
					const auto rigidbody = entity.GetComponent<RigidBodyComponent>();

					// If the entity has a sprite --> Position the projectile in the center
					glm::vec2 projectilePosition = transform.position;
					glm::vec2 rodPosition = transform.position;

					auto& sprite = entity.GetComponent<SpriteComponent>();
					//auto& transform = entity.GetComponent<TransformComponent>();

					glm::vec2 projectileVelocity = projectileEmitter.projectileVelocity;
					int directionX = 0;
					int directionY = 0;
					int srcRectYROD = 80;
					int srcRectYBEAM = 96;
					int srcRectXROD = 64;
					int srcRectXBEAM = 64;
					int boxColliderX = 4;
					int boxColliderY = 3;
					glm::vec2 boxColliderOffset = glm::vec2(0);

					if (KeyboardControlSystem::dir == UP)
					{
						//projectilePosition.x = (transform.position.x);
						rodPosition.y = (transform.position.y - sprite.height * 4);
						boxColliderY = 11;
						boxColliderOffset.x = 18;
						boxColliderOffset.y = 16;
						sprite.srcRect.y = sprite.height * 2;
						srcRectYROD = 48;
						srcRectXROD = 48;
					}

					if (KeyboardControlSystem::dir == RIGHT)
					{
						rodPosition.x = ((transform.position.x) + sprite.width * 4);

						boxColliderX = 11;
						boxColliderOffset.y = 32;
						srcRectXROD = 80;
						srcRectYROD = 48;
						sprite.srcRect.x = sprite.width * 4;
						sprite.srcRect.y = sprite.height * 3;
					}

					if (KeyboardControlSystem::dir == DOWN)
					{
						rodPosition.y = (transform.position.y + sprite.height * 4);
						boxColliderY = 11;
						boxColliderOffset.x = 26;
						boxColliderOffset.y = 2;
						srcRectXROD = 0;
						srcRectYROD = 64;
						sprite.srcRect.y = sprite.height * 3;
					}

					if (KeyboardControlSystem::dir == LEFT)
					{
						rodPosition.x = ((transform.position.x ) - sprite.width * 4);
						boxColliderX = 11;
						boxColliderOffset.y = 32;
						boxColliderOffset.x = 16;
						srcRectXROD = 16;
						srcRectYROD = 48;
						sprite.srcRect.x = sprite.width * 2;
						sprite.srcRect.y = sprite.height * 3;
					}

					projectileVelocity.x = projectileEmitter.projectileVelocity.x * directionX;
					projectileVelocity.y = projectileEmitter.projectileVelocity.y * directionY;
					int beamgDuration = 2000;
					int rodDuration = 500;

					// Create new projectile entity and add it to the world
					Entity sword = entity.registry->CreateEntity();
					sword.Group("projectile");
					sword.AddComponent<TransformComponent>(rodPosition, glm::vec2(4, 4), 0.0);
					sword.AddComponent<SpriteComponent>("Sprites", 16, 16, 2, false, srcRectXROD, srcRectYROD);
					sword.AddComponent<ProjectileComponent>(projectileEmitter.isFriendly, projectileEmitter.hitPercentDamage, rodDuration);
					sword.AddComponent<RigidBodyComponent>(glm::vec2(projectileVelocity.x * 2, projectileVelocity.y * 2));
					sword.AddComponent<BoxColliderComponent>(boxColliderX, boxColliderY, boxColliderOffset);
					sword.AddComponent<GameComponent>();
					sword.Tag("the_sword");

					//Entity beam = entity.registry->CreateEntity();
					//beam.Group("projectile");
					//beam.AddComponent<TransformComponent>(projectilePosition, glm::vec2(4, 4), 0.0);
					//beam.AddComponent<RigidBodyComponent>(glm::vec2(projectileVelocity.x * 2, projectileVelocity.y * 2));
					//beam.AddComponent<SpriteComponent>("items", 16, 16, 2, false, srcRectXBEAM, srcRectYBEAM);
					//beam.AddComponent<ProjectileComponent>(projectileEmitter.isFriendly, projectileEmitter.hitPercentDamage, beamgDuration);
					
					// Play the sword slash sound
					Game::Instance()->GetSystem<SoundFXSystem>().PlaySoundFX(Game::Instance()->GetAssetManager(), "sword_slash", 0, 1);

				}
			}
			KeyboardControlSystem::keyDown = true;
		}
	}

	void UpdateGamePad()
	{
		if (ItemSelectKeyboardControlSystem::itemType == WOOD_BOW || ItemSelectKeyboardControlSystem::itemType == MAGIC_BOW)
		{
			if (GamePadSystem::xPressed && !GamePadSystem::buttonDown)
			{
				for (auto entity : GetSystemEntities())
				{
					if (entity.HasTag("player"))
					{
						const auto& projectileEmitter = entity.GetComponent<ProjectileEmitterComponent>();
						const auto& transform = entity.GetComponent<TransformComponent>();
						const auto& rigidbody = entity.GetComponent<RigidBodyComponent>();

						// If the entity has a sprite --> Position the projectile in the center
						glm::vec2 projectilePosition = transform.position;

						if (entity.HasComponent<SpriteComponent>())
						{
							auto sprite = entity.GetComponent<SpriteComponent>();
							projectilePosition.x += (transform.scale.x * sprite.width / 2);
							projectilePosition.y += (transform.scale.y * sprite.height / 2);
						}

						glm::vec2 projectileVelocity = projectileEmitter.projectileVelocity;
						int directionX = 0;
						int directionY = 0;
						int srcRectY = 64;
						int srcRectX = 0;

						if (KeyboardControlSystem::dir == UP)
						{
							directionY = -1;
							srcRectX = 0;
						}

						if (KeyboardControlSystem::dir == RIGHT)
						{
							directionX = 1;
							srcRectX = 16;
						}

						if (KeyboardControlSystem::dir == DOWN)
						{
							directionY = 1;
							srcRectX = 32;
						}

						if (KeyboardControlSystem::dir == LEFT)
						{
							directionX = -1;
							srcRectX = 48;
						}

						projectileVelocity.x = projectileEmitter.projectileVelocity.x * directionX;
						projectileVelocity.y = projectileEmitter.projectileVelocity.y * directionY;

						// Create new projectile entity and add it to the world
						Entity projectile = entity.registry->CreateEntity();
						projectile.Group("projectile");
						projectile.AddComponent<TransformComponent>(projectilePosition, glm::vec2(4, 4), 0.0);
						projectile.AddComponent<RigidBodyComponent>(projectileVelocity);
						projectile.AddComponent<SpriteComponent>("items", 16, 16, 2, false, srcRectX, srcRectY);
						projectile.AddComponent<ProjectileComponent>(projectileEmitter.isFriendly, projectileEmitter.hitPercentDamage, projectileEmitter.projectileDuration);
						projectile.AddComponent<GameComponent>();

						Game::Instance()->GetSystem<SoundFXSystem>().PlaySoundFX(Game::Instance()->GetAssetManager(), "boomerang_arrow", 0, 1);
					}
				}
				GamePadSystem::buttonDown = true;
			}
		}
		else if (ItemSelectKeyboardControlSystem::itemType == BOMB)
		{
			if (GameState::totalBombs > 0)
			{
				if (GamePadSystem::xPressed && !GamePadSystem::buttonDown)
				{
					for (auto entity : GetSystemEntities())
					{
						if (entity.HasTag("player"))
						{
							const auto projectileEmitter = entity.GetComponent<ProjectileEmitterComponent>();
							const auto transform = entity.GetComponent<TransformComponent>();
							const auto rigidbody = entity.GetComponent<RigidBodyComponent>();

							// If the entity has a sprite --> Position the projectile in the center
							glm::vec2 bombPosition = transform.position;
							const auto sprite = entity.GetComponent<SpriteComponent>();
							glm::vec2 bombVelocity = projectileEmitter.projectileVelocity;

							int directionX = 0;
							int directionY = 0;
							int srcRectY = 112;
							int srcRectX = 64;

							if (KeyboardControlSystem::dir == UP)
							{
								bombPosition.x = (transform.position.x);
								bombPosition.y = (transform.position.y - sprite.height * 3);
							}

							if (KeyboardControlSystem::dir == RIGHT)
							{
								bombPosition.x = (transform.position.x + sprite.width * 3);
								bombPosition.y = (transform.position.y);
							}

							if (KeyboardControlSystem::dir == DOWN)
							{
								bombPosition.x = (transform.position.x);
								bombPosition.y = (transform.position.y + sprite.height * 3);
							}

							if (KeyboardControlSystem::dir == LEFT)
							{
								bombPosition.x = (transform.position.x - sprite.width * 3);
								bombPosition.y = (transform.position.y);
							}

							// Create new projectile entity and add it to the world
							Entity bomb = entity.registry->CreateEntity();
							bomb.Group("bomber");
							bomb.AddComponent<TransformComponent>(bombPosition, glm::vec2(4, 4), 0.0);
							bomb.AddComponent<SpriteComponent>("items", 16, 16, 2, false, srcRectX, srcRectY);
							bomb.AddComponent<ProjectileComponent>(projectileEmitter.isFriendly, projectileEmitter.hitPercentDamage, projectileEmitter.projectileDuration);
							bomb.AddComponent<GameComponent>();
							//bomb.AddComponent<BoxColliderComponent>(0, 0);


							Game::Instance()->GetSystem<SoundFXSystem>().PlaySoundFX(Game::Instance()->GetAssetManager(), "bomb_drop", 0, 1);

							GameState::totalBombs -= 1;
							KeyboardControlSystem::keyDown = true;
						}
					}
					GamePadSystem::buttonDown = true;
				}
			}
		}
		else if (ItemSelectKeyboardControlSystem::itemType == CANDLE)
		{

			if (GamePadSystem::xPressed && !GamePadSystem::buttonDown)
			{
				for (auto entity : GetSystemEntities())
				{
					if (entity.HasTag("player"))
					{
						const auto projectileEmitter = entity.GetComponent<ProjectileEmitterComponent>();
						const auto transform = entity.GetComponent<TransformComponent>();
						const auto rigidbody = entity.GetComponent<RigidBodyComponent>();
						auto& sprite = entity.GetComponent<SpriteComponent>();
						// If the entity has a sprite --> Position the projectile in the center
						glm::vec2 flamePosition = transform.position;

						glm::vec2 flameVelocity = projectileEmitter.projectileVelocity;
						int directionX = 0;
						int directionY = 0;
						int srcRectY = 96;
						int srcRectX = 0;

						if (KeyboardControlSystem::dir == UP)
						{
							flamePosition.x = (transform.position.x);
							flamePosition.y = (transform.position.y - sprite.height * 3);
							directionY = -1;
						}

						if (KeyboardControlSystem::dir == RIGHT)
						{
							flamePosition.x = (transform.position.x + sprite.width * 3);
							flamePosition.y = (transform.position.y);
							directionX = 1;
						}

						if (KeyboardControlSystem::dir == DOWN)
						{
							flamePosition.x = (transform.position.x);
							flamePosition.y = (transform.position.y + sprite.height * 3);
							directionY = 1;
						}

						if (KeyboardControlSystem::dir == LEFT)
						{
							flamePosition.x = (transform.position.x - sprite.width * 3);
							flamePosition.y = (transform.position.y);
							directionX = -1;
						}

						flameVelocity.x = projectileEmitter.projectileVelocity.x * directionX;
						flameVelocity.y = projectileEmitter.projectileVelocity.y * directionY;

						int flameDuration = 200;
						// Create new projectile entity and add it to the world
						Entity candleFlame = entity.registry->CreateEntity();
						candleFlame.Group("projectile");
						candleFlame.AddComponent<TransformComponent>(flamePosition, glm::vec2(4, 4), 0.0);
						candleFlame.AddComponent<RigidBodyComponent>(flameVelocity);
						candleFlame.AddComponent<SpriteComponent>("items", 16, 16, 2, false, srcRectX, srcRectY);
						candleFlame.AddComponent<ProjectileComponent>(projectileEmitter.isFriendly, projectileEmitter.hitPercentDamage, flameDuration);
						candleFlame.AddComponent<BoxColliderComponent>(14, 14, glm::vec2(5, 5));
						candleFlame.AddComponent<GameComponent>();

						Game::Instance()->GetSystem<SoundFXSystem>().PlaySoundFX(Game::Instance()->GetAssetManager(), "candle", 0, 1);
					}
				}
				GamePadSystem::buttonDown = true;
			}
		}
		else if (ItemSelectKeyboardControlSystem::itemType == BOOMERANG)
		{
			if (GamePadSystem::xPressed && !GamePadSystem::buttonDown)
			{
				for (auto entity : GetSystemEntities())
				{
					if (entity.HasTag("player"))
					{
						if (!playerSet)
						{
							playerPosition = entity.GetComponent<TransformComponent>().position;
							playerSet = true;
						}

						const auto projectileEmitter = entity.GetComponent<ProjectileEmitterComponent>();
						const auto transform = entity.GetComponent<TransformComponent>();
						const auto rigidbody = entity.GetComponent<RigidBodyComponent>();

						// If the entity has a sprite --> Position the projectile in the center
						glm::vec2 projectilePosition = transform.position;


						const auto sprite = entity.GetComponent<SpriteComponent>();

						glm::vec2 projectileVelocity = projectileEmitter.projectileVelocity;
						int directionX = 0;
						int directionY = 0;
						int srcRectY = 112;
						int srcRectX = 0;
						int boxColliderX = 8;
						int boxColliderY = 8;
						glm::vec2 boxColliderOffset = glm::vec2(0);


						if (KeyboardControlSystem::dir == UP)
						{
							projectilePosition.x = (transform.position.x);
							projectilePosition.y = (transform.position.y - sprite.height * 3);
							directionY = -1;

							boxColliderOffset.x = 14;
							boxColliderOffset.y = 16;
						}

						if (KeyboardControlSystem::dir == RIGHT)
						{
							projectilePosition.x = (transform.position.x + sprite.width * 3);
							projectilePosition.y = (transform.position.y);
							directionX = 1;

							boxColliderOffset.x = 14;
							boxColliderOffset.y = 16;
						}

						if (KeyboardControlSystem::dir == DOWN)
						{
							projectilePosition.x = (transform.position.x);
							projectilePosition.y = (transform.position.y + sprite.height * 3);
							directionY = 1;

							boxColliderOffset.x = 14;
							boxColliderOffset.y = 16;
						}

						if (KeyboardControlSystem::dir == LEFT)
						{
							projectilePosition.x = (transform.position.x - sprite.width * 3);
							projectilePosition.y = (transform.position.y);
							directionX = -1;

							boxColliderOffset.x = 14;
							boxColliderOffset.y = 16;
						}

						projectileVelocity.x = projectileEmitter.projectileVelocity.x * directionX;
						projectileVelocity.y = projectileEmitter.projectileVelocity.y * directionY;
						int boomerangDuration = 3000;

						if (!boomerangReturned)
						{
							// Create new projectile entity and add it to the world
							Entity projectile = entity.registry->CreateEntity();
							projectile.Group("boomerang");
							projectile.AddComponent<TransformComponent>(projectilePosition, glm::vec2(4, 4), 0.0);
							projectile.AddComponent<RigidBodyComponent>(glm::vec2(projectileVelocity.x * 2, projectileVelocity.y * 2));
							projectile.AddComponent<SpriteComponent>("items", 16, 16, 5, false, srcRectX, srcRectY);
							projectile.AddComponent<AnimationComponent>(4, 20, false);
							projectile.AddComponent<ProjectileComponent>(projectileEmitter.isFriendly, projectileEmitter.hitPercentDamage, boomerangDuration);
							projectile.AddComponent<BoxColliderComponent>(boxColliderX, boxColliderY, boxColliderOffset);
							projectile.AddComponent<ProjectileEmitterComponent>();
							projectile.AddComponent<GameComponent>();

							Game::Instance()->GetSystem<SoundFXSystem>().PlaySoundFX(Game::Instance()->GetAssetManager(), "boomerang_arrow", 0, 1);
						}
					}
				}
				GamePadSystem::buttonDown = true;
			}
		}
		else if (ItemSelectKeyboardControlSystem::itemType == MAGIC_ROD)
		{
			if (GamePadSystem::xPressed && !GamePadSystem::buttonDown)
			{
				for (auto entity : GetSystemEntities())
				{
					if (entity.HasTag("player"))
					{
						const auto projectileEmitter = entity.GetComponent<ProjectileEmitterComponent>();
						const auto transform = entity.GetComponent<TransformComponent>();
						const auto rigidbody = entity.GetComponent<RigidBodyComponent>();

						// If the entity has a sprite --> Position the projectile in the center
						glm::vec2 projectilePosition = transform.position;
						glm::vec2 rodPosition = transform.position;


						auto& sprite = entity.GetComponent<SpriteComponent>();

						glm::vec2 projectileVelocity = projectileEmitter.projectileVelocity;
						int directionX = 0;
						int directionY = 0;
						int srcRectYROD = 80;
						int srcRectYBEAM = 96;
						int srcRectXROD = 64;
						int srcRectXBEAM = 64;

						if (KeyboardControlSystem::dir == UP)
						{
							projectilePosition.x = (transform.position.x);
							projectilePosition.y = (transform.position.y - sprite.height * 4);
							rodPosition.x = (transform.position.x - 20);
							rodPosition.y = (transform.position.y - sprite.height * 4);
							directionY = -1;
							srcRectXROD = 64;
							srcRectXBEAM = 64;
							sprite.srcRect.y = sprite.height * 2;
						}

						if (KeyboardControlSystem::dir == RIGHT)
						{
							projectilePosition.x = ((transform.position.x - 15) + sprite.width * 4);
							projectilePosition.y = (transform.position.y);
							rodPosition.x = (transform.position.x + sprite.width * 4);
							rodPosition.y = (transform.position.y + 10);
							directionX = 1;
							srcRectXROD = 80;
							srcRectXBEAM = 80;
							sprite.srcRect.y = sprite.height * 2;
						}

						if (KeyboardControlSystem::dir == DOWN)
						{
							projectilePosition.x = (transform.position.x);
							projectilePosition.y = (transform.position.y + sprite.height * 4);
							rodPosition.x = (transform.position.x + 5);
							rodPosition.y = (transform.position.y + sprite.height * 4);
							directionY = 1;
							srcRectXROD = 96;
							srcRectXBEAM = 96;
							sprite.srcRect.y = sprite.height * 2;
						}

						if (KeyboardControlSystem::dir == LEFT)
						{
							projectilePosition.x = ((transform.position.x + 10) - sprite.width * 4);
							projectilePosition.y = (transform.position.y);
							rodPosition.x = ((transform.position.x + 5) - sprite.width * 4);
							rodPosition.y = (transform.position.y + 5);
							directionX = -1;
							srcRectXROD = 112;
							srcRectXBEAM = 112;
							sprite.srcRect.y = sprite.height * 2;
						}

						projectileVelocity.x = projectileEmitter.projectileVelocity.x * directionX;
						projectileVelocity.y = projectileEmitter.projectileVelocity.y * directionY;
						int beamDuration = 2000;
						int rodDuration = 200;

						// Create new projectile entity and add it to the world
						Entity magicRod = entity.registry->CreateEntity();
						magicRod.Group("projectile");
						magicRod.AddComponent<TransformComponent>(rodPosition, glm::vec2(4, 4), 0.0);
						magicRod.AddComponent<SpriteComponent>("items", 16, 16, 2, false, srcRectXROD, srcRectYROD);
						magicRod.AddComponent<ProjectileComponent>(projectileEmitter.isFriendly, projectileEmitter.hitPercentDamage, rodDuration);
						magicRod.AddComponent<GameComponent>();

						Entity beam = entity.registry->CreateEntity();
						beam.Group("projectile");
						beam.AddComponent<TransformComponent>(projectilePosition, glm::vec2(4, 4), 0.0);
						beam.AddComponent<RigidBodyComponent>(glm::vec2(projectileVelocity.x * 2, projectileVelocity.y * 2));
						beam.AddComponent<SpriteComponent>("items", 16, 16, 2, false, srcRectXBEAM, srcRectYBEAM);
						beam.AddComponent<ProjectileComponent>(projectileEmitter.isFriendly, projectileEmitter.hitPercentDamage, beamDuration);
						beam.AddComponent<BoxColliderComponent>(16, 16);
						beam.AddComponent<GameComponent>();

						Game::Instance()->GetSystem<SoundFXSystem>().PlaySoundFX(Game::Instance()->GetAssetManager(), "magic_rod", 0, 1);

					}
				}
				GamePadSystem::buttonDown = true;
			}
		}

		if (GamePadSystem::aPressed && !GamePadSystem::buttonDown)
		{
			for (auto entity : GetSystemEntities())
			{
				if (entity.HasTag("player"))
				{
					const auto projectileEmitter = entity.GetComponent<ProjectileEmitterComponent>();
					const auto transform = entity.GetComponent<TransformComponent>();
					const auto rigidbody = entity.GetComponent<RigidBodyComponent>();

					// If the entity has a sprite --> Position the projectile in the center
					glm::vec2 projectilePosition = transform.position;
					glm::vec2 rodPosition = transform.position;


					auto& sprite = entity.GetComponent<SpriteComponent>();
					//auto& transform = entity.GetComponent<TransformComponent>();

					glm::vec2 projectileVelocity = projectileEmitter.projectileVelocity;
					int directionX = 0;
					int directionY = 0;
					int srcRectYROD = 80;
					int srcRectYBEAM = 96;
					int srcRectXROD = 64;
					int srcRectXBEAM = 64;
					int boxColliderX = 4;
					int boxColliderY = 3;
					glm::vec2 boxColliderOffset = glm::vec2(0);

					if (KeyboardControlSystem::dir == UP)
					{
						//projectilePosition.x = (transform.position.x);
						rodPosition.y = (transform.position.y - sprite.height * 4);
						boxColliderY = 11;
						boxColliderOffset.x = 18;
						boxColliderOffset.y = 16;
						sprite.srcRect.y = sprite.height * 2;
						srcRectYROD = 48;
						srcRectXROD = 48;
					}

					if (KeyboardControlSystem::dir == RIGHT)
					{
						rodPosition.x = ((transform.position.x) + sprite.width * 4);

						boxColliderX = 11;
						boxColliderOffset.y = 32;
						srcRectXROD = 80;
						srcRectYROD = 48;
						sprite.srcRect.x = sprite.width * 4;
						sprite.srcRect.y = sprite.height * 3;
					}

					if (KeyboardControlSystem::dir == DOWN)
					{
						rodPosition.y = (transform.position.y + sprite.height * 4);
						boxColliderY = 11;
						boxColliderOffset.x = 26;
						boxColliderOffset.y = 2;
						srcRectXROD = 0;
						srcRectYROD = 64;
						sprite.srcRect.y = sprite.height * 3;
					}

					if (KeyboardControlSystem::dir == LEFT)
					{
						rodPosition.x = ((transform.position.x) - sprite.width * 4);
						boxColliderX = 11;
						boxColliderOffset.y = 32;
						boxColliderOffset.x = 16;
						srcRectXROD = 16;
						srcRectYROD = 48;
						sprite.srcRect.x = sprite.width * 2;
						sprite.srcRect.y = sprite.height * 3;
					}

					projectileVelocity.x = projectileEmitter.projectileVelocity.x * directionX;
					projectileVelocity.y = projectileEmitter.projectileVelocity.y * directionY;
					int beamgDuration = 2000;
					int rodDuration = 500;

					// Create new projectile entity and add it to the world
					Entity magicRod = entity.registry->CreateEntity();
					magicRod.Group("projectile");
					magicRod.AddComponent<TransformComponent>(rodPosition, glm::vec2(4, 4), 0.0);
					magicRod.AddComponent<SpriteComponent>("Sprites", 16, 16, 2, false, srcRectXROD, srcRectYROD);
					magicRod.AddComponent<ProjectileComponent>(projectileEmitter.isFriendly, projectileEmitter.hitPercentDamage, rodDuration);
					magicRod.AddComponent<RigidBodyComponent>(glm::vec2(projectileVelocity.x * 2, projectileVelocity.y * 2));
					magicRod.AddComponent<BoxColliderComponent>(boxColliderX, boxColliderY, boxColliderOffset);
					magicRod.AddComponent<GameComponent>();
					magicRod.Tag("the_sword");

					//Entity beam = entity.registry->CreateEntity();
					//beam.Group("projectile");
					//beam.AddComponent<TransformComponent>(projectilePosition, glm::vec2(4, 4), 0.0);
					//beam.AddComponent<RigidBodyComponent>(glm::vec2(projectileVelocity.x * 2, projectileVelocity.y * 2));
					//beam.AddComponent<SpriteComponent>("items", 16, 16, 2, false, srcRectXBEAM, srcRectYBEAM);
					//beam.AddComponent<ProjectileComponent>(projectileEmitter.isFriendly, projectileEmitter.hitPercentDamage, beamgDuration);

					Game::Instance()->GetSystem<SoundFXSystem>().PlaySoundFX(Game::Instance()->GetAssetManager(), "sword_slash", 0, 1);

				}
			}
			GamePadSystem::buttonDown = true;
		}
		else
		{
			
		}
	}

	void Update(Registry* registry) 
	{
		
		for (auto entity : GetSystemEntities())
		{	
			// Make the boomerang come back to the player and destroy it when it returns
			if (entity.BelongsToGroup("boomerang"))
			{	
				auto& rigidbody = entity.GetComponent<RigidBodyComponent>();

				if (KeyboardControlSystem::dir == UP)
				{
					if (entity.GetComponent<TransformComponent>().position.y < (playerPosition.y - 200) && !boomerangReturned)
					{
						rigidbody.velocity *= -1;
						boomerangReturned = true;
					}
					else if (entity.GetComponent<TransformComponent>().position.y >= (playerPosition.y -64) && boomerangReturned)
					{
						entity.Kill();
						boomerangReturned = false;
						playerSet = false;
					}
				}
				else if (KeyboardControlSystem::dir == RIGHT)
				{
					if (entity.GetComponent<TransformComponent>().position.x > (playerPosition.x + 200) && !boomerangReturned)
					{
						rigidbody.velocity *= -1;
						boomerangReturned = true;
					}
					else if (entity.GetComponent<TransformComponent>().position.x <= (playerPosition.x + 64) && boomerangReturned)
					{
						entity.Kill();
						boomerangReturned = false;
						playerSet = false;
					}
				}
				else if (KeyboardControlSystem::dir == DOWN)
				{
					if (entity.GetComponent<TransformComponent>().position.y > (playerPosition.y + 200) && !boomerangReturned)
					{
						rigidbody.velocity *= -1;
						boomerangReturned = true;
					}
					else if (entity.GetComponent<TransformComponent>().position.y <= (playerPosition.y + 64) && boomerangReturned)
					{
						entity.Kill();
						boomerangReturned = false;
						playerSet = false;
					}
				}
				else if (KeyboardControlSystem::dir == LEFT)
				{
					if (entity.GetComponent<TransformComponent>().position.x < (playerPosition.x - 200) && !boomerangReturned)
					{
						rigidbody.velocity *= -1;
						boomerangReturned = true;
					}
					else if (entity.GetComponent<TransformComponent>().position.x >= (playerPosition.x - 64) && boomerangReturned)
					{
						entity.Kill();
						boomerangReturned = false;
						playerSet = false;
					}
				}
			}
			
		//	auto& projectileEmitter = entity.GetComponent<ProjectileEmitterComponent>();
		//	const auto transform = entity.GetComponent<TransformComponent>(); 
		//	// This is needed to keep the entity shooting
		//	if (projectileEmitter.repeatFrequency == 0)
		//	{
		//		continue;
		//	}

		//	// Check if it is time to re emit a new projectile
		//	if (SDL_GetTicks() - projectileEmitter.lastEmissionTime > projectileEmitter.repeatFrequency)
		//	{
		//		glm::vec2 projectilePosition = transform.position;

		//		if (entity.HasComponent<SpriteComponent>())
		//		{
		//			const auto sprite = entity.GetComponent<SpriteComponent>();
		//			projectilePosition.x += (transform.scale.x * sprite.width / 2);
		//			projectilePosition.y += (transform.scale.y * sprite.height / 2);
		//		}

		//		int srcRectY = 64;
		//		int srcRectX = 0;
		//		const auto rigidBody = projectileEmitter.projectileVelocity;

		//		if (rigidBody.y < 0)
		//		{
		//			srcRectX = 0;
		//		}
		//		if (rigidBody.x > 0)
		//		{
		//			srcRectX = 16;
		//		}
		//		if (rigidBody.y > 0)
		//		{
		//			srcRectX = 32;
		//		}
		//		if (rigidBody.x < 0)
		//		{
		//			srcRectX = 48;
		//		}

		//		Entity projectile = registry->CreateEntity();
		//		projectile.Group("projectile");
		//		projectile.AddComponent<TransformComponent>(projectilePosition, glm::vec2(4, 4), 0.0);
		//		projectile.AddComponent<RigidBodyComponent>(projectileEmitter.projectileVelocity);
		//		projectile.AddComponent<SpriteComponent>("items", 16, 16, false, srcRectX, srcRectY);
		//		projectile.AddComponent<BoxColliderComponent>(16, 16);
		//		projectile.AddComponent<ProjectileComponent>(projectileEmitter.isFriendly, projectileEmitter.hitPercentDamage, projectileEmitter.projectileDuration);
		//	}
		}
	}
};