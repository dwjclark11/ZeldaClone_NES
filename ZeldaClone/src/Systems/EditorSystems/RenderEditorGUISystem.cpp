#include "RenderEditorGUISystem.h"
#include "RenderEditorSystem.h"
#include "../../Components/TransformComponent.h"

#include "../../Utilities/FileManagerUtils.h"
#include "../../States/MenuState.h"
#include "../../Game/LevelLoader.h"
#include <filesystem>

// Variable Declarations
int RenderEditorGUISystem::zIndex = 0;
int RenderEditorGUISystem::scaleX = Game::gameScale;
int RenderEditorGUISystem::scaleY = Game::gameScale;
int RenderEditorGUISystem::mouseRectX = 16;
int RenderEditorGUISystem::mouseRectY = 16;
int RenderEditorGUISystem::boxColliderWidth = 0;
int RenderEditorGUISystem::boxColliderHeight = 0;
int RenderEditorGUISystem::boxColliderOffsetX = 0;
int RenderEditorGUISystem::boxColliderOffsetY = 0;

bool RenderEditorGUISystem::mouseImageLoaded = false;
bool RenderEditorGUISystem::showLoadMap = false;
bool RenderEditorGUISystem::showCanvasProperties = false;
bool RenderEditorGUISystem::showImageBox = false;

int RenderEditorGUISystem::imageWidth = 0;
int RenderEditorGUISystem::imageHeight = 0;
int RenderEditorGUISystem::mapWidth = 0;
int RenderEditorGUISystem::mapHeight = 0;

std::string RenderEditorGUISystem::fileName = "";
std::string RenderEditorGUISystem::imageName = "";
std::string RenderEditorGUISystem::imageID = "";

bool RenderEditorGUISystem::fileLoaded = false;

void RenderEditorGUISystem::Update(const std::unique_ptr<AssetManager>& assetManager, SDL_Renderer* renderer)
{
	ImGui::NewFrame();

	if (ImGui::BeginMainMenuBar())
	{
		if (ImGui::BeginMenu("File"))
		{
			ShowMenuFile(assetManager, renderer);
			ImGui::EndMenu();
		}

		if (ImGui::BeginMenu("Edit"))
		{
			if (ImGui::MenuItem("Canvas Size"))
				showCanvasProperties = true;

			ImGui::EndMenu();
		}

		if (ImGui::BeginMenu("Tools"))
		{
			ImGui::Text("Select what to Create");
			ImGui::Spacing(); ImGui::Spacing(); ImGui::Spacing(); ImGui::Spacing();
			if (ImGui::Checkbox("Create Tiles", &MouseControlSystem::createTile))
			{
				if (MouseControlSystem::createTile)
				{
					MouseControlSystem::createObstacles = false;
					MouseControlSystem::createBoxCollider = false;
					MouseControlSystem::createEnemy = false;
					MouseControlSystem::createTrigger = false;
				}
			}

			if (ImGui::Checkbox("Create Obstacles", &MouseControlSystem::createObstacles))
			{
				if (MouseControlSystem::createObstacles)
				{
					MouseControlSystem::createTile = false;
					MouseControlSystem::createBoxCollider = false;
					MouseControlSystem::createEnemy = false;
					MouseControlSystem::createTrigger = false;
				}
			}

			if (ImGui::Checkbox("Create Colliders", &MouseControlSystem::createBoxCollider))
			{
				if (MouseControlSystem::createBoxCollider)
				{
					MouseControlSystem::createTile = false;
					MouseControlSystem::createObstacles = false;
					MouseControlSystem::createEnemy = false;
					MouseControlSystem::createTrigger = false;
				}
			}

			if (ImGui::Checkbox("Create Triggers", &MouseControlSystem::createTrigger))
			{
				if (MouseControlSystem::createTrigger)
				{
					MouseControlSystem::createTile = false;
					MouseControlSystem::createObstacles = false;
					MouseControlSystem::createEnemy = false;
					MouseControlSystem::createBoxCollider = false;
				}
			}

			if (ImGui::Checkbox("Create Enemies", &MouseControlSystem::createEnemy))
			{
				if (MouseControlSystem::createEnemy)
				{
					MouseControlSystem::createTile = false;
					MouseControlSystem::createObstacles = false;
					MouseControlSystem::createBoxCollider = false;
					MouseControlSystem::createTrigger = false;
				}
			}
			// Set the mouse image to snap to the grid 
			ImGui::Checkbox("Grip Snap", &MouseControlSystem::gridSnap);

			ImGui::EndMenu();
		}

		ImGui::Spacing(); ImGui::Spacing(); ImGui::Spacing(); ImGui::Spacing();

		if (MouseControlSystem::createTile) ImGui::TextColored(ImVec4(255, 255, 0, 1), "CURRENTLY CREATING TILES!");
		if (MouseControlSystem::createObstacles) ImGui::TextColored(ImVec4(255, 255, 0, 1), "CURRENTLY CREATING OBSTACLES!");
		if (MouseControlSystem::createBoxCollider) ImGui::TextColored(ImVec4(255, 255, 0, 1), "CURRENTLY CREATING BOX COLLIDERS!");
		if (MouseControlSystem::createEnemy) ImGui::TextColored(ImVec4(255, 255, 0, 1), "CURRENTLY CREATING ENEMIES!");
		if (MouseControlSystem::createTrigger) ImGui::TextColored(ImVec4(255, 255, 0, 1), "CURRENTLY CREATING TRIGGERS!");

		ImGui::Spacing(); ImGui::Spacing(); ImGui::Spacing(); ImGui::Spacing();
		ImGui::Spacing(); ImGui::Spacing(); ImGui::Spacing(); ImGui::Spacing();

		ImGui::TextColored(ImVec4(0, 255, 0, 1), "Grid X: %d", static_cast<int>(game.GetMouseBox().x + game.GetCamera().x) / MouseControlSystem::gridSize);
		ImGui::TextColored(ImVec4(0, 255, 0, 1), "Grid Y : %d", static_cast<int>(game.GetMouseBox().y + game.GetCamera().y) / MouseControlSystem::gridSize);
		ImGui::Spacing(); ImGui::Spacing(); ImGui::Spacing(); ImGui::Spacing();
		ImGui::TextColored(ImVec4(0, 255, 0, 1), "[Mouse X: %d", static_cast<int>(game.GetMouseBox().x + game.GetCamera().x));
		ImGui::TextColored(ImVec4(0, 255, 0, 1), "Mouse Y : %d]", static_cast<int>(game.GetMouseBox().y + game.GetCamera().y));

		// Put Above this
		ImGui::EndMainMenuBar();
	}

	if (MouseControlSystem::createTile)
	{
		ImageBox(assetManager, renderer);
		
		if (ImGui::Begin("Tile Properties", &MouseControlSystem::createTile))
		{
			ShowMenuTile();
			if (ImGui::Button("Set Tile Attributes"))
			{
				MouseControlSystem::layer = zIndex;
				MouseControlSystem::Scale.x = scaleX;
				MouseControlSystem::Scale.y = scaleY;
				MouseControlSystem::boxColliderWidth = boxColliderWidth;
				MouseControlSystem::boxColliderHeight = boxColliderHeight;
				MouseControlSystem::boxColliderOffset.x = boxColliderOffsetX;
				MouseControlSystem::boxColliderOffset.y = boxColliderOffsetY;
				MouseControlSystem::mouseRectX = mouseRectX;
				MouseControlSystem::mouseRectY = mouseRectY;
			}
			ImGui::End();
		}
	}

	if (showLoadMap)
		ShowMapLoaderWindow(assetManager, renderer);

	if (showCanvasProperties)
		CanvasSize();
	
	if (MouseControlSystem::createEnemy)
		EnemyProperties(assetManager, renderer);
	if (MouseControlSystem::createTrigger)
		TriggerProperties(assetManager, renderer);
	if (MouseControlSystem::createBoxCollider)
		ColliderProperties(assetManager, renderer);

	ImGui::Render();
	ImGuiSDL::Render(ImGui::GetDrawData());

	ImGuiIO& io = ImGui::GetIO();
	if (io.WantCaptureMouse)
		MouseControlSystem::OverImGuiWindow = true;
	else
		MouseControlSystem::OverImGuiWindow = false;
}


void RenderEditorGUISystem::ShowMenuFile(const std::unique_ptr<AssetManager>& assetManager, SDL_Renderer* renderer)
{
	FileDialogs dialog;
	ImGui::MenuItem("(File Menu)", NULL, false, false);

	// Destroy All Entities and reset all variables
	if (ImGui::MenuItem("New"))
	{
		if (!GetSystemEntities().empty())
		{
			for (auto entity : GetSystemEntities())
			{
				entity.Kill();
				imageName = "";
				fileName = "";
			}
		}
	}

	if (ImGui::MenuItem("Load Image"))
	{
		imageName = dialog.OpenImage();
		choices.SetImageName(imageName);
		loader.SetImageName(imageName);
		//MouseControlSystem::imageID = loader.SetName(imageName);
		// Check to see if the String is Empty!!
		if (!imageName.empty())
		{
			imageLoaded = true;
		}
		else
		{
			Logger::Err("Did Not Load Image");
			return;
		}
		Logger::Log("This name " + choices.GetImageName());
	}

	if (ImGui::MenuItem("Load Tileset"))
	{
		LoadNewImage(assetManager, renderer, MouseControlSystem::imageID, imageWidth, imageHeight);
		mouseImageLoaded = true;
	}

	if (ImGui::MenuItem("Load Sprite Map", "Creates a map from one texture"))
	{
		showLoadMap = true;
	}

	if (MouseControlSystem::createTile)
	{
		if (ImGui::MenuItem("Open Tilemap", "Ctrl+O"))
		{
			if (!imageName.empty())
			{
				fileName = dialog.OpenFile();

				loader.SetFileName(fileName);

				// Check to see if the String is Empty!!
				if (!fileName.empty())
				{
					loader.LoadTilemap(assetManager, renderer);
					fileLoaded = true;
				}
			}
		}
	}

	if (MouseControlSystem::createObstacles)
	{
		if (ImGui::MenuItem("Open Object Map", "Ctrl+O"))
		{
			if (!imageName.empty())
			{
				fileName = dialog.OpenFile();

				loader.SetFileName(fileName);

				// Check to see if the String is Empty!!
				if (!fileName.empty())
				{
					loader.LoadObjectMap(assetManager, renderer);
					fileLoaded = true;
				}
			}
		}
	}
	if (MouseControlSystem::createBoxCollider)
	{
		if (ImGui::MenuItem("Open Collider Map", "Ctrl+O"))
		{
			fileName = dialog.OpenFile();

			std::string extension = std::filesystem::path(fileName).extension().string();

			Logger::Log("Extension: " + extension);

			loader.SetFileName(fileName);

			// Check to see if the String is Empty!!
			if (!fileName.empty() && extension == ".map")
			{
				loader.LoadBoxColliderMap(assetManager, renderer, fileName);
				fileLoaded = true;
			}
		}
	}

	if (MouseControlSystem::createTrigger)
	{
		if (ImGui::MenuItem("Open Trigger Map", "Ctrl+O"))
		{
			LevelLoader load;
			fileName = dialog.OpenFile();

			std::string extension = std::filesystem::path(fileName).extension().string();
			std::string stem = std::filesystem::path(fileName).stem().string();
			Logger::Log("Extension: " + extension);

			loader.SetFileName(fileName);

			// Check to see if the String is Empty!!
			if (!fileName.empty() && extension == ".lua")
			{
				load.LoadTriggers(game.GetLuaState(), stem);
				fileLoaded = true;
			}
		}
	}


	if (ImGui::MenuItem("Save", "Ctrl+S"))
	{
		std::string extension = std::filesystem::path(fileName).extension().string();
		if (!fileName.empty())
		{
			if (MouseControlSystem::createTile)
			{
				if (extension == ".map")
					loader.SaveTilemap(fileName, assetManager, renderer);
				fileLoaded = true;
			}

			if (MouseControlSystem::createObstacles)
			{
				if (extension == ".obj")
					loader.SaveObjectMap(fileName, assetManager, renderer);
				fileLoaded = true;
			}
			if (MouseControlSystem::createBoxCollider)
			{
				if (extension == ".map")
					loader.SaveBoxColliderMap(fileName, assetManager, renderer);
				else if (extension == ".lua")
					loader.SaveBoxColliderMapToLuaFile(fileName);

				fileLoaded = true;
			}
			if (MouseControlSystem::createTrigger)
			{
				if (extension == ".lua")
					loader.SaveTriggersToLuaFile(fileName);
			}
		}
	}

	if (ImGui::MenuItem("Save As.."))
	{
		std::string saveAsFile = dialog.SaveFile();
		std::string extension = std::filesystem::path(saveAsFile).extension().string();

		if (!saveAsFile.empty())
		{
			if (MouseControlSystem::createTile)
			{

				if (extension == ".map")
					loader.SaveTilemap(saveAsFile, assetManager, renderer);
			}

			else if (MouseControlSystem::createObstacles)
			{
				if (extension == ".obj")
					loader.SaveObjectMap(saveAsFile, assetManager, renderer);
				
			}
			else if (MouseControlSystem::createBoxCollider)
			{
				if (extension == ".lua")
					loader.SaveBoxColliderMapToLuaFile(saveAsFile);
				else if (extension == ".map")
					loader.SaveBoxColliderMap(saveAsFile, game.GetAssetManager(), game.GetRenderer());	
			}
			else if (MouseControlSystem::createEnemy)
			{
				if (extension == ".lua")
					loader.SaveEnemiesToLuaFile(saveAsFile);
			}
			else if (MouseControlSystem::createTrigger)
			{
				if (extension == ".lua")
					loader.SaveTriggersToLuaFile(saveAsFile);
			}
		}
	}

	ImGui::Spacing(); ImGui::Spacing(); ImGui::Spacing(); ImGui::Spacing();
	if (ImGui::MenuItem("Play Game!!"))
	{
		game.GetStateMachine()->PopState();
		game.GetStateMachine()->PushState(new MenuState());
	}

	ImGui::Spacing(); ImGui::Spacing(); ImGui::Spacing(); ImGui::Spacing();

	if (ImGui::MenuItem("Quit"))
	{
		game.GetGameRunning() = false;
	}
}

void RenderEditorGUISystem::ShowMenuTile()
{
	ImGui::Text("Transform Component");
	ImGui::SliderInt("X Scale", &scaleX, 1, 10);
	ImGui::SliderInt("Y Scale", &scaleY, 1, 10);
	
	ImGui::Spacing(); ImGui::Spacing();

	ImGui::Checkbox("Box Collider", &MouseControlSystem::isCollision);

	ImGui::Text("Box Collider Components");
	ImGui::Spacing();
	ImGui::TextColored(ImVec4(255, 255, 0, 1), "Box Collider values are multiples of 8");

	if (ImGui::InputInt("Box Width", &boxColliderWidth, 8, 8))
	{
		boxColliderWidth = (boxColliderWidth / 8) * 8;
		if (boxColliderWidth <= 0)
			boxColliderWidth = 0;
	}
	
	if (ImGui::InputInt("Box Height", &boxColliderHeight, 8, 8))
	{
		boxColliderHeight = (boxColliderHeight / 8) * 8;
		if (boxColliderHeight <= 0)
			boxColliderHeight = 0;
	}

	if (ImGui::InputInt("Box OffsetX", &boxColliderOffsetX, 8, 8))
		boxColliderOffsetX = (boxColliderOffsetX / 8) * 8;
	
	if (ImGui::InputInt("Box OffsetY", &boxColliderOffsetY, 8, 8))
		boxColliderOffsetY = (boxColliderOffsetY / 8) * 8;
	
	ImGui::Spacing();
	ImGui::TextColored(ImVec4(255, 255, 0, 1), "Mouse values are multiples of 8");
	ImGui::Spacing();
	ImGui::Text("Mouse Box Size");
	if (ImGui::InputInt("Mouse Rect X", &mouseRectX, 8, 8))
	{
		mouseRectX = (mouseRectX / 8) * 8;
		// Clamp mouse Rect X at zero
		if (mouseRectX <= 0)
			mouseRectX = 0;
	}
		
	if (ImGui::InputInt("Mouse Rect Y", &mouseRectY, 8, 8))
	{
		mouseRectY = (mouseRectY / 8) * 8;
		// Clamp mouse Rect Y at zero
		if (mouseRectY <= 0)
			mouseRectY = 0;
	}
	ImGui::TextColored(ImVec4(255, 255, 0, 255), "Current Src Rect [X: %d, Y: %d]", MouseControlSystem::imageSrcX, MouseControlSystem::imageSrcY);
	// There is no need for the layer if not a tile or enemy
	if (!MouseControlSystem::createBoxCollider && !MouseControlSystem::createTrigger)
	{
		ImGui::Spacing();
		ImGui::Text("Set the layer of the tile");
		ImGui::SliderInt("Z-Index", &MouseControlSystem::layer, 0, 10);

		if (MouseControlSystem::layer < 0) MouseControlSystem::layer = 0;
		if (MouseControlSystem::layer > 10) MouseControlSystem::layer = 10;
	}
	ImGui::Spacing(); ImGui::Spacing();
}

void RenderEditorGUISystem::EnemyProperties(const std::unique_ptr<AssetManager>& assetManager, SDL_Renderer* renderer)
{
	if (ImGui::Begin("Enemy Properties", &MouseControlSystem::createEnemy))
	{
		std::string enemies[] = { "octorok", "moblin", "darknut", "leever", "tektite", "peahat", "armos", "ghini", "lynel", "zora" };
		static std::string current_enemy = "";

		ComboLoop("Enemy Type", current_enemy, enemies, sizeof(enemies) / sizeof(enemies[0]));
		LevelLoader loader;

		
		MouseControlSystem::enemyType = loader.ConvertStringToEnemyType(std::string(current_enemy));

		Logger::Log(std::string(current_enemy) + ": " + std::to_string(MouseControlSystem::enemyType));

		ImGui::Checkbox("Animation", &MouseControlSystem::animation);

		if (MouseControlSystem::animation)
		{
			if (ImGui::CollapsingHeader("Animation", ImGuiTreeNodeFlags_DefaultOpen))
			{
				if (ImGui::InputInt("Frame Rate Speed", &MouseControlSystem::animationFrameRateSpeed, 0, 1));
				if (ImGui::InputInt("Number of Frames", &MouseControlSystem::animationNumFrames, 0, 1));
				if (ImGui::InputInt("Frame Offset", &MouseControlSystem::animationFrameOffset, 0, 1));

				ImGui::Checkbox("Vertical", &MouseControlSystem::animationVerticalScroll);
				ImGui::SameLine();
				ImGui::Checkbox("Looped", &MouseControlSystem::animationLooped);
			}
		}

		ImGui::SliderInt("Scale X", &scaleX, 1, 10);
		ImGui::SliderInt("Scale Y", &scaleY, 1, 10);


		if (ImGui::Button("Set Tile Attributes"))
		{
			MouseControlSystem::Scale.x = scaleX;
			MouseControlSystem::Scale.y = scaleY;
			MouseControlSystem::mouseRectX = mouseRectX;
			MouseControlSystem::mouseRectY = mouseRectY;
		}
		ImGui::End();
	}
}
void RenderEditorGUISystem::TriggerProperties(const std::unique_ptr<AssetManager>& assetManager, SDL_Renderer* renderer)
{
	if (ImGui::Begin("Trigger Properties", &MouseControlSystem::createTrigger))
	{
		static std::string levelMusic{ "stop" };
		static std::string assetFile{ "no_file" };
		static std::string enemyFile{ "no_file" };
		static std::string colliderFile{ "no_file" };
		static std::string tileMapName{ "no_file" };
		static std::string tileMapImageName{ "no_file" };
		static std::string entityFileName{ "no_file" };
		static std::string triggerFile{ "no_file" };

		static std::string spriteAssetID = "";

		static int spriteWidth = 0;
		static int spriteHeight = 0;
		static int spriteSrcX = 0;
		static int spriteSrcY = 0;

		static int newSpriteWidth = 0;
		static int newSpriteHeight = 0;
		static int newSpriteSrcX = 0;
		static int newSpriteSrcY = 0;
		static std::string locationID { "" };
		static std::string newSpriteAssetID { "" };
		static std::string new_trigger { "" };

		const std::string triggers[] = { "NO_TRIGGER", "SECRET_AREA",  "TRANSPORT", "BURN_BUSHES", "PUSH_ROCKS", "BOMB_SECRET", "HIDDEN_SWITCH", "HIDDEN_OBJECT" };
		static std::string current_item = "";

		ComboLoop("Trigger Types", current_item, triggers, sizeof(triggers) / sizeof(triggers[0]));
		
		// Set Trigger Types
		if (current_item == "NO_TRIGGER")
			MouseControlSystem::triggerType = NO_TRIGGER;
		else if (current_item == "SECRET_AREA")
			MouseControlSystem::triggerType = SECRET_AREA;
		else if (current_item == "TRANSPORT")
			MouseControlSystem::triggerType = TRANSPORT;
		else if (current_item == "BURN_BUSHES")
			MouseControlSystem::triggerType = BURN_BUSHES;
		else if (current_item == "PUSH_ROCKS")
			MouseControlSystem::triggerType = PUSH_ROCKS;
		else if (current_item == "COLLECT_ITEM")
			MouseControlSystem::triggerType = COLLECT_ITEM;
		else if (current_item == "BOMB_SECRET")
			MouseControlSystem::triggerType = BOMB_SECRET;
		else if (current_item == "HIDDEN_SWITCH")
			MouseControlSystem::triggerType = HIDDEN_SWITCH;
		else if (current_item == "HIDDEN_OBJECT")
			MouseControlSystem::triggerType = HIDDEN_OBJECT;
		else
			MouseControlSystem::triggerType = NO_TRIGGER;
		
		ImGui::Spacing(); ImGui::Spacing(); ImGui::Spacing();
		if (ImGui::InputFloat("Transport Offset X", &MouseControlSystem::transportOffset.x, 1, 100));
		if (ImGui::InputFloat("Transport Offset Y", &MouseControlSystem::transportOffset.y, 1, 100));
		if (ImGui::InputFloat("Camera Offset X", &MouseControlSystem::cameraOffset.x, 1, 100));
		if (ImGui::InputFloat("Camera Offset Y", &MouseControlSystem::cameraOffset.y, 1, 100));
		
		ImGui::TextColored(ImVec4(255, 255, 0, 255), "Set the music to the song ID or \"stop\"");
		ImFuncs::MyInputText("Level Music", &levelMusic);
		ImGui::TextColored(ImVec4(255, 255, 0, 255), "Put in the file name or leave as \"no_file\"");
		ImFuncs::MyInputText("Asset File", &assetFile);
		ImFuncs::MyInputText("Enemy File", &enemyFile);
		ImFuncs::MyInputText("Collider File", &colliderFile);
		ImFuncs::MyInputText("Tilemap Name", &tileMapName);
		ImFuncs::MyInputText("Tilemap Image", &tileMapImageName);
		ImFuncs::MyInputText("Entity File", &entityFileName);
		ImFuncs::MyInputText("Trigger File", &triggerFile);

		ImGui::Spacing(); ImGui::Spacing(); ImGui::Spacing();
		ImGui::TextColored(ImVec4(255, 255, 0, 255), "Is the trigger also a collider?");
		ImGui::Checkbox("Collider", &MouseControlSystem::triggerCollider);
		ImGui::Spacing(); ImGui::Spacing(); ImGui::Spacing();

		ShowMenuTile();
		ImGui::Checkbox("Sprite Component", &MouseControlSystem::spriteSelected);
		if (MouseControlSystem::spriteSelected) ImageBox(assetManager, renderer);


		ImGui::Checkbox("Secret", &MouseControlSystem::secretSelected);
		
		if (MouseControlSystem::secretSelected)
		{
			ComboLoop("New Secret Trigger", new_trigger, triggers, sizeof(triggers) / sizeof(triggers[0]));

			ImFuncs::MyInputText("Location ID", &locationID);
			ImFuncs::MyInputText("New Sprite Asset ID", &newSpriteAssetID);

			if (ImGui::InputInt("New Sprite Width", &newSpriteWidth, 16, 16))
			{
				newSpriteWidth = (newSpriteWidth / 16) * 16;
				if (newSpriteWidth <= 0)
					newSpriteWidth = 0;
			}
			
			if (ImGui::InputInt("New Sprite Height", &newSpriteHeight, 16, 16))
			{
				newSpriteHeight = (newSpriteHeight / 16) * 16;
				if (newSpriteHeight <= 0)
					newSpriteHeight = 0;
			}

			if (ImGui::InputInt("New Sprite Src X", &newSpriteSrcX, 16, 16))
			{
				newSpriteSrcX = (newSpriteSrcX / 16) * 16;
				if (newSpriteSrcX <= 0)
					newSpriteSrcX = 0;
			}

			if (ImGui::InputInt("New Sprite Src Y", &newSpriteSrcY, 16, 16))
			{
				newSpriteSrcY = (newSpriteSrcY / 16) * 16;
				if (newSpriteSrcY <= 0)
					newSpriteSrcY = 0;
			}

			// Set Trigger Types
			if (new_trigger == "NO_TRIGGER")
				MouseControlSystem::newTriggerType = "no_trigger";
			else if (new_trigger == "SECRET_AREA")
				MouseControlSystem::newTriggerType = "secret_area";
			else if (new_trigger == "BURN_BUSHES")
				MouseControlSystem::newTriggerType = "burn_bushes";
			else if (new_trigger == "PUSH_ROCKS")
				MouseControlSystem::newTriggerType = "push_rocks";
			else if (new_trigger == "COLLECT_ITEM")
				MouseControlSystem::newTriggerType = "collect_item";
			else if (new_trigger == "BOMB_SECRET")
				MouseControlSystem::newTriggerType = "bomb_secret";
			else if (new_trigger == "HIDDEN_SWITCH")
				MouseControlSystem::newTriggerType = "hidder_switch";
			else if (new_trigger == "HIDDEN_OBJECT")
				MouseControlSystem::newTriggerType = "hidden_object";
			else
				MouseControlSystem::newTriggerType = "no_trigger";
		}

		if (ImGui::Button("Set Trigger Attributes"))
		{
			MouseControlSystem::Scale.x = scaleX;
			MouseControlSystem::Scale.y = scaleY;
			MouseControlSystem::boxColliderHeight = boxColliderHeight;
			MouseControlSystem::boxColliderWidth = boxColliderWidth;
			MouseControlSystem::boxColliderOffset.x = boxColliderOffsetX;
			MouseControlSystem::boxColliderOffset.y = boxColliderOffsetY;
			MouseControlSystem::mouseRectX = mouseRectX;
			MouseControlSystem::mouseRectY = mouseRectY;

			MouseControlSystem::levelMusic = levelMusic;
			MouseControlSystem::assetFile = assetFile;
			MouseControlSystem::enemyNewFile = enemyFile;
			MouseControlSystem::colliderFile = colliderFile;
			MouseControlSystem::TileMapName = tileMapName;
			MouseControlSystem::TileMapImageName = tileMapImageName;
			MouseControlSystem::entityFileName = entityFileName;
			MouseControlSystem::triggerFile = triggerFile;

			// Set Secret Attributes
			if (MouseControlSystem::secretSelected)
			{
				MouseControlSystem::newSpriteWidth = newSpriteWidth;
				MouseControlSystem::newSpriteHeight = newSpriteHeight;

				MouseControlSystem::imageSrcX = newSpriteSrcX;
				MouseControlSystem::imageSrcY = newSpriteSrcY;
			}

			if (MouseControlSystem::spriteSelected)
			{
				MouseControlSystem::spriteComponent.assetID = MouseControlSystem::imageID;
				MouseControlSystem::spriteComponent.width = MouseControlSystem::mouseRectX;
				MouseControlSystem::spriteComponent.height = MouseControlSystem::mouseRectY;
				MouseControlSystem::spriteComponent.layer = MouseControlSystem::layer;
				MouseControlSystem::spriteComponent.isFixed = false;
				MouseControlSystem::spriteComponent.srcRect.x = MouseControlSystem::imageSrcX;
				MouseControlSystem::spriteComponent.srcRect.y = MouseControlSystem::imageSrcY;
			}
		}

		ImGui::End();
	}
}
void RenderEditorGUISystem::ColliderProperties(const std::unique_ptr<AssetManager>& assetManager, SDL_Renderer* renderer)
{
	if (ImGui::Begin("Collider Properties", &MouseControlSystem::createBoxCollider))
	{
		ShowMenuTile();

		if (ImGui::Button("Set Collider Attributes"))
		{
			MouseControlSystem::Scale.x = scaleX;
			MouseControlSystem::Scale.y = scaleY;
			MouseControlSystem::boxColliderWidth = boxColliderWidth;
			MouseControlSystem::boxColliderHeight = boxColliderHeight;
			MouseControlSystem::boxColliderOffset.x = boxColliderOffsetX;
			MouseControlSystem::boxColliderOffset.y = boxColliderOffsetY;
			
			MouseControlSystem::mouseRectX = mouseRectX;
			MouseControlSystem::mouseRectY = mouseRectY;
		}
		ImGui::End();
	}
}
void RenderEditorGUISystem::ImageBox(const std::unique_ptr<AssetManager>& assetManager, SDL_Renderer* renderer)
{
	// Set the image Width/Height based on the obtain values from the file and the current game scale
	if (ImGui::Begin("Texture"), &mouseImageLoaded)
	{
		int my_image_width = imageWidth * 2;
		int my_image_height = imageHeight * 2;

		ImGui::Image(assetManager->GetTexture(MouseControlSystem::imageID), ImVec2(my_image_width, my_image_height));

		// This is for the mouse Offset for the image --> Still needs to be refined --> Currently Hackish
		//Logger::Log("x: " + std::to_string(ImGui::GetMousePos().x) + "y: " + std::to_string(ImGui::GetMousePos().y));
		int mouseposX = ImGui::GetMousePos().x - ImGui::GetWindowPos().x;
		int mouseposY = ImGui::GetMousePos().y - ImGui::GetWindowPos().y - 26; // It's a hack I know

		// Clamp --> To make sure that we are not dividing by zero value!
		if (my_image_width > 0 && my_image_height > 0 && MouseControlSystem::mouseRectX > 0 && MouseControlSystem::mouseRectY > 0)
		{
			int rows = (my_image_height / (MouseControlSystem::mouseRectY * 2));
			int col = (my_image_width / (MouseControlSystem::mouseRectX * 2));

			for (int i = 0; i < col; i++)
			{
				for (int j = 0; j < rows; j++)
				{
					auto draw_list = ImGui::GetWindowDrawList();
					// Check to see if we are in the area of the desired 2D tile/object
					if (mouseposX >= (my_image_width / col) * i && mouseposX <= (my_image_width / col) + ((my_image_width / col) * i))
					{
						if (mouseposY >= (my_image_height / rows) * j && mouseposY <= (my_image_height / rows) + ((my_image_height / rows) * j))
						{
							if (ImGui::IsItemHovered())
							{
								if (ImGui::IsMouseClicked(0))
								{
									//Logger::Log("X: " + std::to_string(i) + ", Y : " + std::to_string(j));
									MouseControlSystem::imageSrcX = i * MouseControlSystem::mouseRectX; 
									MouseControlSystem::imageSrcY = j * MouseControlSystem::mouseRectY;
								}
							}
						}
					}
				}
			}
		}
	}
	ImGui::End();
}

bool RenderEditorGUISystem::LoadNewImage(const std::unique_ptr<AssetManager>& assetManager, SDL_Renderer* renderer, std::string& image_id, int& image_width, int& image_height)
{
	FileDialogs dialog;
	// Open a file dialog and assign the return directory to the imageName
	std::string image_name = dialog.OpenImage();
	//imageName = dialog.OpenImage();

	choices.SetImageName(image_name);
	Logger::Log(choices.GetImageName());

	// Strip the image name of the directory strings leaving just the name 
	image_id = loader.SetName(image_name);

	// Check to see if the String is Empty!!
	if (!image_name.empty())
	{
		if (!assetManager->HasTexture(image_id))
		{
			Logger::Err("Added new image with the ID  -- " + image_id);
			assetManager->AddTextures(renderer, image_id, choices.GetImageName());
			Logger::Err(choices.GetImageName());
		}
		else
		{
			Logger::Err("This Image ID Already Exists! -- " + image_id);
			assetManager->GetTexture(image_id);
		}
		imageLoaded = true;
	}
	else
	{
		Logger::Err("Did Not Load Image");
		return false;
	}

	// Get the image width and Height!! --> TEST
	if (SDL_QueryTexture(assetManager->GetTexture(image_id), NULL, NULL, &image_width, &image_height) == 0)
		return true;
	else
	{
		const char* message = SDL_GetError();
		Logger::Err("EDITOR: __FUNC: LoadImage: " + std::string(message));
		return false;
	}
}

void RenderEditorGUISystem::ComboLoop(std::string label, std::string& current_item, const std::string handinArray[], int size)
{
	if (ImGui::BeginCombo(label.c_str(), current_item.c_str()))
	{
		for (int n = 0; n < size; n++)
		{
			bool is_selected = (current_item == handinArray[n]);
			if (ImGui::Selectable(handinArray[n].c_str(), is_selected))
			{
				current_item = handinArray[n];
			}
			if (is_selected)
			{
				ImGui::SetItemDefaultFocus();
			}
		}
		ImGui::EndCombo();
	}
}

void RenderEditorGUISystem::ImGuiSetup()
{
	ImGuiIO& io = ImGui::GetIO();
	io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;

	// Setup ImGui Keys
	io.KeyMap[ImGuiKey_Backspace] = SDL_SCANCODE_BACKSPACE;
	io.KeyMap[ImGuiKey_Enter] = SDL_SCANCODE_RETURN;
	io.KeyMap[ImGuiKey_Space] = SDL_SCANCODE_SPACE;
	io.KeyMap[ImGuiKey_Tab] = SDL_SCANCODE_TAB;
	io.KeyMap[ImGuiKey_LeftArrow] = SDL_SCANCODE_LEFT;
	io.KeyMap[ImGuiKey_RightArrow] = SDL_SCANCODE_RIGHT;
	io.KeyMap[ImGuiKey_UpArrow] = SDL_SCANCODE_UP;
	io.KeyMap[ImGuiKey_DownArrow] = SDL_SCANCODE_DOWN;
	io.KeyMap[ImGuiKey_Home] = SDL_SCANCODE_HOME;
	io.KeyMap[ImGuiKey_End] = SDL_SCANCODE_END;
	io.KeyMap[ImGuiKey_Escape] = SDL_SCANCODE_ESCAPE;
	
	io.KeyMap[ImGuiKey_A] = SDL_SCANCODE_A;
	io.KeyMap[ImGuiKey_C] = SDL_SCANCODE_C;
	io.KeyMap[ImGuiKey_V] = SDL_SCANCODE_V;
	io.KeyMap[ImGuiKey_X] = SDL_SCANCODE_X;
	io.KeyMap[ImGuiKey_Y] = SDL_SCANCODE_Y;
	io.KeyMap[ImGuiKey_Z] = SDL_SCANCODE_Z;
	
}

void RenderEditorGUISystem::CanvasSize()
{
	static int numTilesWidth = 1;
	static int numTilesHeight = 1;
	static int spriteSize = 16;
	if (ImGui::Begin("Canvas Size"))
	{
		ImGui::InputInt("Num Tiles Width", &numTilesWidth);
		ImGui::InputInt("Num Tiles Height", &numTilesHeight);
		if (ImGui::SliderInt("Sprite Size", &spriteSize, 16, 128))
		{

		}

		if (ImGui::Button("Set Canvas"))
		{
			showCanvasProperties = false;
		}

		ImGui::SameLine();

		if (ImGui::Button("Cancel"))
			showCanvasProperties = false;
		
		ImGui::End();
	}
}

bool RenderEditorGUISystem::LoadEntireMapSprite(const std::unique_ptr<AssetManager>& assetManager, SDL_Renderer* renderer)
{
	std::string mapAssetID = "";
	// Remove all Entities from the Editor
	game.GetSystem<RenderEditorSystem>().OnExit();
	game.GetSystem<RenderCollisionSystem>().OnExit();

	// Load the image to the asset manager and query the width and height
	if (!LoadNewImage(assetManager, renderer, mapAssetID, mapWidth, mapHeight))
	{
		Logger::Err("EDITOR: __FUNC: LoadEntireMapSprite: Could not load the Map sprite!");
		return false;
	}

	// If the image has been loaded successfully, Create a new map entity
	auto newMap = Registry::Instance().CreateEntity();
	newMap.Group("map");
	newMap.AddComponent<TransformComponent>(glm::vec2(0), glm::vec2(scaleX, scaleY), 0);
	newMap.AddComponent<SpriteComponent>(mapAssetID, mapWidth, mapHeight, 0, false, 0, 0);
	newMap.AddComponent<EditorComponent>();

	Logger::Log("Created Map!!");
}

void RenderEditorGUISystem::ShowMapLoaderWindow(const std::unique_ptr<AssetManager>& assetManager, SDL_Renderer* renderer)
{
	static bool mapLoadPopUp = false;

	if (ImGui::Begin("Load Map Sprite"))
	{
		ImGui::SliderInt("X Scale", &scaleX, 1, 10);
		ImGui::SliderInt("Y Scale", &scaleY, 1, 10);

		if (ImGui::Button("Next"))
		{
			mapLoadPopUp = true;
		}
		ImGui::SameLine();
		if (ImGui::Button("cancel"))
		{
			showLoadMap = false;
		}
		ImGui::End();
	}

	if (mapLoadPopUp)
	{
		if (ImGui::Begin("Load"), &mapLoadPopUp)
		{
			ImGui::Text("This will erase all of you have done so far!");
			ImGui::Text("Are you sure?");

			if (ImGui::Button("Yes"))
			{
				LoadEntireMapSprite(assetManager, renderer);
				mapLoadPopUp = false;
				showLoadMap = false;
			}

			ImGui::SameLine();

			// Close the pop-up and go back to the Load window
			if (ImGui::Button("Cancel"))
				mapLoadPopUp = false;

			ImGui::End();
		}
	}
}