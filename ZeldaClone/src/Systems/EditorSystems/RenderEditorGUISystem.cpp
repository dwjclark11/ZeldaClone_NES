#include "RenderEditorGUISystem.h"

// Variable Declarations
int RenderEditorGUISystem::zIndex = 0;
int RenderEditorGUISystem::scaleX = 1;
int RenderEditorGUISystem::scaleY = 1;
int RenderEditorGUISystem::mouseRectX = 16;
int RenderEditorGUISystem::mouseRectY = 16;
int RenderEditorGUISystem::boxColliderWidth = 0;
int RenderEditorGUISystem::boxColliderHeight = 0;
int RenderEditorGUISystem::boxColliderOffsetX = 0;
int RenderEditorGUISystem::boxColliderOffsetY = 0;
bool RenderEditorGUISystem::layer = false;

unsigned int RenderEditorGUISystem::imageWidth = 0;
unsigned int RenderEditorGUISystem::imageHeight = 0;

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
			ImGui::EndMenu();
		}

		if (ImGui::BeginMenu("Tools"))
		{
			if (ImGui::MenuItem("Tile Properties"))
			{
				// TODO: Show Tile ProperTies Window
			}

			ImGui::Spacing(); ImGui::Spacing(); ImGui::Spacing(); ImGui::Spacing();

			ImGui::Text("Select what to Create");

			if (ImGui::Checkbox("Create Tiles", &MouseControlSystem::createTile))
			{
				//MouseControlSystem::createTile = !MouseControlSystem::createTile;

				if (MouseControlSystem::createTile)
				{
					MouseControlSystem::createObstacles = false;
					MouseControlSystem::createBoxCollider = false;
					MouseControlSystem::createEnemy = false;
				}
			}

			if (ImGui::Checkbox("Create Obstacles", &MouseControlSystem::createObstacles))
			{

				if (MouseControlSystem::createObstacles)
				{
					MouseControlSystem::createTile = false;
					MouseControlSystem::createBoxCollider = false;
					MouseControlSystem::createEnemy = false;
				}
			}

			if (ImGui::Checkbox("Create Colliders", &MouseControlSystem::createBoxCollider))
			{

				if (MouseControlSystem::createBoxCollider)
				{
					MouseControlSystem::createTile = false;
					MouseControlSystem::createObstacles = false;
					MouseControlSystem::createEnemy = false;
				}
			}

			if (ImGui::Checkbox("Create Enemies", &MouseControlSystem::createEnemy))
			{
				if (MouseControlSystem::createEnemy)
				{
					MouseControlSystem::createTile = false;
					MouseControlSystem::createObstacles = false;
					MouseControlSystem::createBoxCollider = false;
				}
			}


			ImGui::EndMenu();
		}

		ImGui::Spacing(); ImGui::Spacing(); ImGui::Spacing(); ImGui::Spacing();

		if (MouseControlSystem::createTile) ImGui::TextColored(ImVec4(255, 255, 0, 1), "CURRENTLT CREATING TILES!");
		if (MouseControlSystem::createObstacles) ImGui::TextColored(ImVec4(255, 255, 0, 1), "CURRENTLT CREATING OBSTACLES!");
		if (MouseControlSystem::createBoxCollider) ImGui::TextColored(ImVec4(255, 255, 0, 1), "CURRENTLT CREATING BOX COLLIDERS!");
		if (MouseControlSystem::createEnemy) ImGui::TextColored(ImVec4(255, 255, 0, 1), "CURRENTLT CREATING ENEMIES!");

		ImGui::Spacing(); ImGui::Spacing(); ImGui::Spacing(); ImGui::Spacing();
		ImGui::Spacing(); ImGui::Spacing(); ImGui::Spacing(); ImGui::Spacing();

		ImGui::TextColored(ImVec4(0, 255, 0, 1), "Mouse X: %d", static_cast<int>(game.GetMouseBox().x + game.GetCamera().x) / MouseControlSystem::gridSize);
		ImGui::TextColored(ImVec4(0, 255, 0, 1), "Mouse Y : %d", static_cast<int>(game.GetMouseBox().y + game.GetCamera().y) / MouseControlSystem::gridSize);


		// Put Above this
		ImGui::EndMainMenuBar();
	}

	if (ImGui::Begin("Tile Properties"))
	{
		ShowMenuTile(assetManager, renderer);
		ImGui::End();
	}

	ImageBox(assetManager, renderer);
	EnemyProperties(assetManager, renderer);
	ImGui::Render();
	ImGuiSDL::Render(ImGui::GetDrawData());


	ImGuiIO& io = ImGui::GetIO();
	if (io.WantCaptureMouse)
	{
		Logger::Log("Over the window");
		MouseControlSystem::OverImGuiWindow = true;
	}
	else
	{
		MouseControlSystem::OverImGuiWindow = false;
	}

}


void RenderEditorGUISystem::ShowMenuFile(const std::unique_ptr<AssetManager>& assetManager, SDL_Renderer* renderer)
{
	static std::string fileName;
	static std::string imageName;
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
		Logger::Log(choices.GetImageName());
	}

	if (ImGui::MenuItem("Load Mouse Image"))
	{
		// Open a file dialog and assign the return directory to the imageName
		imageName = dialog.OpenImage();

		choices.SetImageName(imageName);
		Logger::Log(choices.GetImageName());

		// Strip the image name of the directory strings leaving just the name 
		MouseControlSystem::imageID = loader.SetName(imageName);

		// Read the image PNG file 
		std::ifstream image(imageName);
		// The width is 16 bytes in and is 4 bytes long the height comes 
		// right after at 20. Also 4 bytes
		image.seekg(16);
		// Assign the values of the File header to the given varaibles
		image.read((char*)&imageWidth, 4);
		image.read((char*)&imageHeight, 4);

		// We must now rearrange the varialbes from Network byte order to host byte order.
		// Big-Endian to Little-Endian so we can use the values.
		imageWidth = ntohl(imageWidth);
		imageHeight = ntohl(imageHeight);


		// Check to see if the String is Empty!!
		if (!imageName.empty())
		{
			if (!assetManager->HasTexture(MouseControlSystem::imageID))
			{
				Logger::Err("Added new image with the ID  -- " + MouseControlSystem::imageID);
				assetManager->AddTextures(renderer, MouseControlSystem::imageID, choices.GetImageName());
				Logger::Err(choices.GetImageName());
			}
			else
			{
				Logger::Err("This Image ID Already Exists! -- " + MouseControlSystem::imageID);
				assetManager->GetTexture(MouseControlSystem::imageID);
			}
			imageLoaded = true;
		}
		else
		{
			Logger::Err("Did Not Load Image");
			return;
		}
	}

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
	if (ImGui::MenuItem("Open Collider Map", "Ctrl+O"))
	{
		fileName = dialog.OpenFile();

		loader.SetFileName(fileName);

		// Check to see if the String is Empty!!
		if (!fileName.empty())
		{
			loader.LoadBoxColliderMap(assetManager, renderer);
			fileLoaded = true;
		}
	}

	if (ImGui::MenuItem("Save", "Ctrl+S"))
	{
		if (MouseControlSystem::createTile)
		{
			if (!fileName.empty())
			{
				loader.SaveTilemap(fileName, assetManager, renderer);
				fileLoaded = true;

			}
		}

		if (MouseControlSystem::createObstacles)
		{
			if (!fileName.empty())
			{
				loader.SaveObjectMap(fileName, assetManager, renderer);
				fileLoaded = true;

			}
		}
	}

	if (ImGui::MenuItem("Save As.."))
	{
		if (MouseControlSystem::createTile)
		{
			std::string saveAsFile = dialog.SaveFile();
			if (!saveAsFile.empty())
			{
				loader.SaveTilemap(saveAsFile, assetManager, renderer);
			}
		}
		else if (MouseControlSystem::createObstacles)
		{
			std::string saveAsFile = dialog.SaveObjFile();
			if (!saveAsFile.empty())
			{
				loader.SaveObjectMap(saveAsFile, assetManager, renderer);
			}
		}
		else if (MouseControlSystem::createBoxCollider)
		{
			std::string saveAsFile = dialog.SaveBoxColliderFile();
			if (!saveAsFile.empty())
			{
				loader.SaveBoxColliderMapToLuaFile(saveAsFile);
			}
		}
		else if (MouseControlSystem::createEnemy)
		{
			std::string saveAsFile = dialog.SaveBoxColliderFile();
			if (!saveAsFile.empty())
			{
				loader.SaveEnemiesToLuaFile(saveAsFile);
			}

		}
	}

	// Change the canvas size 
	if (ImGui::CollapsingHeader("Canvas Size", ImGuiTreeNodeFlags_DefaultOpen))
	{
		// Test to see if the canvas slider will be a divided evenly by 16
		if (ImGui::SliderInt("Canvas Width", &MouseControlSystem::CanvasWidth, 256, 20000))
		{
			MouseControlSystem::CanvasWidth /= 16;
			MouseControlSystem::CanvasWidth *= 16;
		}
		if (ImGui::SliderInt("Canvas Height", &MouseControlSystem::CanvasHeight, 240, 20000))
		{
			MouseControlSystem::CanvasHeight /= 16;
			MouseControlSystem::CanvasHeight *= 16;
		}
	}

	ImGui::Spacing();
	ImGui::Spacing();

	if (MouseControlSystem::createObstacles)
	{
		ImGui::Checkbox("GridSnap", &MouseControlSystem::gridSnap);
		if (MouseControlSystem::gridSnap)
		{
			ImGui::SliderInt("GridSize", &MouseControlSystem::gridSize, 1, 192);
		}
	}

	if (ImGui::Button("Quit Editor"))
	{
		game.GetStateMachine()->PopState();
		game.GetStateMachine()->PushState(new MenuState());
	}

	ImGui::Spacing(); ImGui::Spacing(); ImGui::Spacing(); ImGui::Spacing();

}

void RenderEditorGUISystem::ShowMenuTile(const std::unique_ptr<AssetManager>& assetManager, SDL_Renderer* renderer)
{
	// Music Variables
	static int volume = 0;

	// Value check booleans
	static bool noScaleX = false;
	static bool noScaleY = false;

	if (ImGui::CollapsingHeader("Transform Components", ImGuiTreeNodeFlags_DefaultOpen))
	{
		ImGui::SliderInt("X Scale", &scaleX, 1, 10);
		ImGui::SliderInt("Y Scale", &scaleY, 1, 10);
	}
	ImGui::Spacing();
	ImGui::Spacing();

	ImGui::Checkbox("Box Collider", &MouseControlSystem::isCollision);
	if (ImGui::CollapsingHeader("Box Collider Components", ImGuiTreeNodeFlags_DefaultOpen))
	{
		ImGui::SliderInt("Box Width", &boxColliderWidth, 1, 96);
		ImGui::SliderInt("Box Height", &boxColliderHeight, 1, 96);
		ImGui::SliderInt("Box OffsetX", &boxColliderOffsetX, -96, 96);
		ImGui::SliderInt("Box OffsetY", &boxColliderOffsetY, -96, 96);
	}

	if (MouseControlSystem::createEnemy)
	{
		ImGui::Checkbox("Animation", &MouseControlSystem::animation);

		if (MouseControlSystem::animation)
		{
			if (ImGui::CollapsingHeader("Animation", ImGuiTreeNodeFlags_DefaultOpen))
			{
				if (ImGui::InputInt("Frame Rate Speed", &MouseControlSystem::animationFrameRateSpeed, 0, 1));
				if (MouseControlSystem::layer < 0) MouseControlSystem::layer = 0;
				if (MouseControlSystem::layer > 20) MouseControlSystem::layer = 20;
				if (ImGui::InputInt("Number of Frames", &MouseControlSystem::animationNumFrames, 0, 1));
				if (MouseControlSystem::layer < 0) MouseControlSystem::animationNumFrames = 0;
				if (MouseControlSystem::layer > 20) MouseControlSystem::animationNumFrames = 20;
				if (ImGui::InputInt("Frame Offset", &MouseControlSystem::animationFrameOffset, 0, 1));

				ImGui::Checkbox("Vertical", &MouseControlSystem::animationVerticalScroll);
				ImGui::SameLine();
				ImGui::Checkbox("Looped", &MouseControlSystem::animationLooped);
			}
		}
	}

	if (ImGui::CollapsingHeader("Mouse Box Size", ImGuiTreeNodeFlags_DefaultOpen))
	{
		ImGui::SliderInt("Mouse Rect X", &mouseRectX, 8, 128);
		ImGui::SliderInt("Mouse Rect Y", &mouseRectY, 8, 128);
	}

	if (ImGui::InputInt("Z-Index", &MouseControlSystem::layer, 0, 1));
	if (MouseControlSystem::layer < 0) MouseControlSystem::layer = 0;
	if (MouseControlSystem::layer > 10) MouseControlSystem::layer = 10;

	ImGui::Spacing();
	ImGui::Spacing();

	if (ImGui::Button("Set Tile Attributes"))
	{
		if (scaleX == 0) noScaleX = true;
		else noScaleX = false;
		if (scaleY == 0) noScaleY = true;
		else noScaleY = false;
		MouseControlSystem::layer = zIndex;
		MouseControlSystem::tileScaleX = scaleX;
		MouseControlSystem::tileScaleY = scaleY;
		MouseControlSystem::boxColliderWidth = boxColliderWidth;
		MouseControlSystem::boxColliderHeight = boxColliderHeight;
		MouseControlSystem::boxColliderOffsetX = boxColliderOffsetX;
		MouseControlSystem::boxColliderOffsetY = boxColliderOffsetY;
		MouseControlSystem::mouseRectX = mouseRectX;
		MouseControlSystem::mouseRectY = mouseRectY;

	}

	/*const char* triggers[] = { "NO_TRIGGER", "SECRET_AREA",  "ENTER_DUNGEON", "BURN_BUSHES", "PUSH_ROCKS", "CAMERA_RIGHT", "TRAP", "HIDDEN_SWITCH", "HIDDEN_OBJECT" };
	const char* current_item = nullptr;

	if (ImGui::BeginCombo("Trigger Types", current_item))
	{
		for (int n = 0; n < IM_ARRAYSIZE(triggers); n++)
		{
			bool is_selected = (current_item == triggers[n]);
			if (ImGui::Selectable(triggers[n], is_selected))
			{
				current_item = triggers[n];
			}
			if (is_selected)
			{
				ImGui::SetItemDefaultFocus();
			}
			if (current_item == "NO_TRIGGER")
			{
				MouseControlSystem::triggerType = NO_TRIGGER;
			}
			else if (current_item == "SECRET_AREA")
			{
				MouseControlSystem::triggerType = SECRET_AREA;
			}
			else if (current_item == "ENTER_DUNGEON")
			{
				MouseControlSystem::triggerType = ENTER_DUNGEON;
			}
			else if (current_item == "BURN_BUSHES")
			{
				MouseControlSystem::triggerType = BURN_BUSHES;
			}
			else if (current_item == "PUSH_ROCKS")
			{
				MouseControlSystem::triggerType = PUSH_ROCKS;
			}
			else if (current_item == "COLLECT_ITEM")
			{
				MouseControlSystem::triggerType = COLLECT_ITEM;
			}
			else if (current_item == "TRAP")
			{
				MouseControlSystem::triggerType = BOMB_SECRET;
			}
			else if (current_item == "HIDDEN_SWITCH")
			{
				MouseControlSystem::triggerType = HIDDEN_SWITCH;
			}
			else if (current_item == "HIDDEN_OBJECT")
			{
				MouseControlSystem::triggerType = HIDDEN_OBJECT;
			}
		}
	}ImGui::EndCombo();*/
}

void RenderEditorGUISystem::EnemyProperties(const std::unique_ptr<AssetManager>& assetManager, SDL_Renderer* renderer)
{
	if (MouseControlSystem::createEnemy)
	{
		if (ImGui::Begin("Enemy Properties"))
		{
			const char* enemies[] = { "octorok", "moblin", "darknut", "leever", "tektite", "peahat", "armos", "ghini", "lynel", "zora" };
			static const char* current_enemy = NULL;

			if (ImGui::BeginCombo("Enemy Types", current_enemy))
			{
				for (int n = 0; n < IM_ARRAYSIZE(enemies); n++)
				{
					bool is_selected = (current_enemy == enemies[n]);
					if (ImGui::Selectable(enemies[n], is_selected))
					{
						current_enemy = enemies[n];
					}
					if (is_selected)
					{
						ImGui::SetItemDefaultFocus();
					}
				}
				ImGui::EndCombo();
			}

			// Check to see what enemy type is selected from the combo-box
			if (current_enemy == "octorok")
				MouseControlSystem::enemyType = MouseControlSystem::EditorEnemyType::OCTOROK;
			else if (current_enemy == "moblin")
				MouseControlSystem::enemyType = MouseControlSystem::EditorEnemyType::MOBLIN;
			else if (current_enemy == "darknut")
				MouseControlSystem::enemyType = MouseControlSystem::EditorEnemyType::DARKNUT;
			else if (current_enemy == "leever")
				MouseControlSystem::enemyType = MouseControlSystem::EditorEnemyType::LEEVER;
			else if (current_enemy == "tektite")
				MouseControlSystem::enemyType = MouseControlSystem::EditorEnemyType::TEKTITE;
			else if (current_enemy == "peahat")
				MouseControlSystem::enemyType = MouseControlSystem::EditorEnemyType::PEAHAT;
			else if (current_enemy == "armos")
				MouseControlSystem::enemyType = MouseControlSystem::EditorEnemyType::ARMOS;
			else if (current_enemy == "ghini")
				MouseControlSystem::enemyType = MouseControlSystem::EditorEnemyType::GHINI;
			else if (current_enemy == "lynel")
				MouseControlSystem::enemyType = MouseControlSystem::EditorEnemyType::LYNEL;
			else if (current_enemy == "zora")
				MouseControlSystem::enemyType = MouseControlSystem::EditorEnemyType::ZORA;
			else
				MouseControlSystem::enemyType = MouseControlSystem::EditorEnemyType::OCTOROK;


			//static bool secret = false;
			//static std::string secret_type = "";
			//if (ImGui::Checkbox("Add Secret", &secret))
			//{
			//	ImGui::InputText("Secret Type", &secret_type);
			//}


			//Logger::Log(secret_type);

			// Warning Messages if Enemy Creation Fails
			//if (noScaleX) ImGui::Text("Please Put in X Scale");
			//if (noScaleY) ImGui::Text("Please Put in Y Scale");

			if (ImGui::Button("Set Tile Attributes"))
			{
				MouseControlSystem::tileScaleX = scaleX;
				MouseControlSystem::tileScaleY = scaleY;
				ImGui::End();
			}

			ImGui::End();
		}
	}
}
void RenderEditorGUISystem::ImageBox(const std::unique_ptr<AssetManager>& assetManager, SDL_Renderer* renderer)
{
	// Get the Desired texture
	assetManager->GetTexture(MouseControlSystem::imageID);

	// Set the image Width/Height based on the obtain values from the file and the current game scale
	int  my_image_width = imageWidth * 2;//* Game::gameScale;
	int my_image_height = imageHeight * 2; //* Game::gameScale;

	if (ImGui::Begin("Texture"))
	{
		ImGui::Image(assetManager->GetTexture(MouseControlSystem::imageID), ImVec2(my_image_width, my_image_height));

		// This is for the mouse Offset for the image --> Still needs to be refined --> Currently Hackish
		//Logger::Log("x: " + std::to_string(ImGui::GetMousePos().x) + "y: " + std::to_string(ImGui::GetMousePos().y));

		int mouseposX = ImGui::GetMousePos().x - ImGui::GetWindowPos().x;
		int mouseposY = ImGui::GetMousePos().y - ImGui::GetWindowPos().y - 26; // It's a hack I know

		// Clamp --> To make sure that we are not dividing by zero value!
		if (my_image_width > 0 && my_image_height > 0 && MouseControlSystem::mouseRectX > 0 && MouseControlSystem::mouseRectY > 0)
		{
			int rows = (my_image_height / (MouseControlSystem::mouseRectY * 2)); //* Game::gameScale));
			int col = (my_image_width / (MouseControlSystem::mouseRectX * 2));//* Game::gameScale));

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
									Logger::Log("X: " + std::to_string(i) + ", Y : " + std::to_string(j));
									MouseControlSystem::imageSrcX = i * MouseControlSystem::mouseRectX; // The actual size of the sprite [16 x 16],[32 x 32] etc
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