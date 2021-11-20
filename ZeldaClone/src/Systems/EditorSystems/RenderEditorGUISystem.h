#pragma once
#include "../ECS/ECS.h"
#include "MouseControlSystem.h"
#include "../Utilities/FileManagerUtils.h"
#include "../Editor/EditorFileLoader.h"
#include "../States/MenuState.h"
#include <imgui/imgui.h>
#include <imgui/imgui_sdl.h>
#include <imgui/imgui_stdlib.h>
#include <fstream>
#include "../Game/Game.h"
//#include <winsock.h>
#include <fstream>
#include <iostream>

class RenderEditorGUISystem : public System
{
private:
	std::string imageID;
	bool imageLoaded = false;
	bool fileLoaded = false;
	std::ofstream saveFile;
	EditorFileLoader loader;
	MouseControlSystem choices;
	static unsigned int imageWidth, imageHeight;
	class Game& game;
	
public:

	// Setup the IMGUI Style in the Constructor --> Change to it's own style function?
	RenderEditorGUISystem()
		: game(*Game::Instance())
	{
		ImGuiStyle& style = ImGui::GetStyle();
		style.TabRounding = 0.0f;
		style.FrameBorderSize = 1.0f;
		style.ScrollbarRounding = 0.0f;
		style.ScrollbarSize = 10.0f;
		ImVec4* colors = ImGui::GetStyle().Colors;
		colors[ImGuiCol_Text] = ImVec4(0.95f, 0.95f, 0.95f, 1.00f);
		colors[ImGuiCol_TextDisabled] = ImVec4(0.50f, 0.50f, 0.50f, 1.00f);
		colors[ImGuiCol_WindowBg] = ImVec4(0.12f, 0.12f, 0.12f, 1.00f);
		colors[ImGuiCol_ChildBg] = ImVec4(0.04f, 0.04f, 0.04f, 0.50f);
		colors[ImGuiCol_PopupBg] = ImVec4(0.12f, 0.12f, 0.12f, 0.94f);
		colors[ImGuiCol_Border] = ImVec4(0.25f, 0.25f, 0.27f, 0.50f);
		colors[ImGuiCol_BorderShadow] = ImVec4(0.00f, 0.00f, 0.00f, 0.00f);
		colors[ImGuiCol_FrameBg] = ImVec4(0.20f, 0.20f, 0.22f, 0.50f);
		colors[ImGuiCol_FrameBgHovered] = ImVec4(0.25f, 0.25f, 0.27f, 0.75f);
		colors[ImGuiCol_FrameBgActive] = ImVec4(0.30f, 0.30f, 0.33f, 1.00f);
		colors[ImGuiCol_TitleBg] = ImVec4(0.04f, 0.04f, 0.04f, 1.00f);
		colors[ImGuiCol_TitleBgActive] = ImVec4(0.04f, 0.04f, 0.04f, 1.00f);
		colors[ImGuiCol_TitleBgCollapsed] = ImVec4(0.04f, 0.04f, 0.04f, 0.75f);
		colors[ImGuiCol_MenuBarBg] = ImVec4(0.18f, 0.18f, 0.19f, 1.00f);
		colors[ImGuiCol_ScrollbarBg] = ImVec4(0.24f, 0.24f, 0.26f, 0.75f);
		colors[ImGuiCol_ScrollbarGrab] = ImVec4(0.41f, 0.41f, 0.41f, 0.75f);
		colors[ImGuiCol_ScrollbarGrabHovered] = ImVec4(0.62f, 0.62f, 0.62f, 0.75f);
		colors[ImGuiCol_ScrollbarGrabActive] = ImVec4(0.94f, 0.92f, 0.94f, 0.75f);
		colors[ImGuiCol_CheckMark] = ImVec4(0.60f, 0.60f, 0.60f, 1.00f);
		colors[ImGuiCol_SliderGrab] = ImVec4(0.41f, 0.41f, 0.41f, 0.75f);
		colors[ImGuiCol_SliderGrabActive] = ImVec4(0.62f, 0.62f, 0.62f, 0.75f);
		colors[ImGuiCol_Button] = ImVec4(0.20f, 0.20f, 0.22f, 1.00f);
		colors[ImGuiCol_ButtonHovered] = ImVec4(0.25f, 0.25f, 0.27f, 1.00f);
		colors[ImGuiCol_ButtonActive] = ImVec4(0.41f, 0.41f, 0.41f, 1.00f);
		colors[ImGuiCol_Header] = ImVec4(0.18f, 0.18f, 0.19f, 1.00f);
		colors[ImGuiCol_HeaderHovered] = ImVec4(0.25f, 0.25f, 0.27f, 1.00f);
		colors[ImGuiCol_HeaderActive] = ImVec4(0.41f, 0.41f, 0.41f, 1.00f);
		colors[ImGuiCol_Separator] = ImVec4(0.25f, 0.25f, 0.27f, 1.00f);
		colors[ImGuiCol_SeparatorHovered] = ImVec4(0.41f, 0.41f, 0.41f, 1.00f);
		colors[ImGuiCol_SeparatorActive] = ImVec4(0.62f, 0.62f, 0.62f, 1.00f);
		colors[ImGuiCol_ResizeGrip] = ImVec4(0.30f, 0.30f, 0.33f, 0.75f);
		colors[ImGuiCol_ResizeGripHovered] = ImVec4(0.41f, 0.41f, 0.41f, 0.75f);
		colors[ImGuiCol_ResizeGripActive] = ImVec4(0.62f, 0.62f, 0.62f, 0.75f);
		colors[ImGuiCol_Tab] = ImVec4(0.21f, 0.21f, 0.22f, 1.00f);
		colors[ImGuiCol_TabHovered] = ImVec4(0.37f, 0.37f, 0.39f, 1.00f);
		colors[ImGuiCol_TabActive] = ImVec4(0.30f, 0.30f, 0.33f, 1.00f);
		colors[ImGuiCol_TabUnfocused] = ImVec4(0.12f, 0.12f, 0.12f, 0.97f);
		colors[ImGuiCol_TabUnfocusedActive] = ImVec4(0.18f, 0.18f, 0.19f, 1.00f);
		colors[ImGuiCol_PlotLines] = ImVec4(0.61f, 0.61f, 0.61f, 1.00f);
		colors[ImGuiCol_PlotLinesHovered] = ImVec4(1.00f, 0.43f, 0.35f, 1.00f);
		colors[ImGuiCol_PlotHistogram] = ImVec4(0.90f, 0.70f, 0.00f, 1.00f);
		colors[ImGuiCol_PlotHistogramHovered] = ImVec4(1.00f, 0.60f, 0.00f, 1.00f);
		colors[ImGuiCol_TextSelectedBg] = ImVec4(0.26f, 0.59f, 0.98f, 0.50f);
		colors[ImGuiCol_DragDropTarget] = ImVec4(1.00f, 1.00f, 0.00f, 0.90f);
		colors[ImGuiCol_NavHighlight] = ImVec4(0.26f, 0.59f, 0.98f, 1.00f);
		colors[ImGuiCol_NavWindowingHighlight] = ImVec4(1.00f, 1.00f, 1.00f, 0.70f);
		colors[ImGuiCol_NavWindowingDimBg] = ImVec4(0.80f, 0.80f, 0.80f, 0.20f);
		colors[ImGuiCol_ModalWindowDimBg] = ImVec4(0.80f, 0.80f, 0.80f, 0.35f);
		style.WindowMenuButtonPosition = ImGuiDir_Right;
	}

	void Update(const std::unique_ptr<AssetManager>& assetManager, SDL_Renderer* renderer)
	{
		ImGui::NewFrame();

		if (ImGui::Begin("File Menu"))
		{
			ShowMenuFile( assetManager, renderer);
		} ImGui::End();

		if (ImGui::Begin("Tile Properties"))
		{
			ShowMenuTile( assetManager, renderer);
		}ImGui::End();

		ImageBox(assetManager, renderer);

		ImGui::Render();
		ImGuiSDL::Render(ImGui::GetDrawData());
	}

	void ShowMenuFile( const std::unique_ptr<AssetManager>& assetManager, SDL_Renderer* renderer)
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
					loader.LoadTilemap( assetManager, renderer);
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
					loader.LoadObjectMap( assetManager, renderer);
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
					loader.SaveTilemap(fileName,  assetManager, renderer);
					fileLoaded = true;

				}
			}

			if (MouseControlSystem::createObstacles)
			{
				if (!fileName.empty())
				{
					loader.SaveObjectMap(fileName,  assetManager, renderer);
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
					loader.SaveBoxColliderMapToLuaFile(saveAsFile, assetManager, renderer);
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


		if (ImGui::Button("Create Tiles"))
		{
			MouseControlSystem::createTile = !MouseControlSystem::createTile;

			if (MouseControlSystem::createObstacles || MouseControlSystem::createBoxCollider)
			{
				MouseControlSystem::createObstacles = false;
				MouseControlSystem::createBoxCollider = false;
			}
		}

		if (ImGui::Button("Create Obstacles"))
		{
			MouseControlSystem::createObstacles = !MouseControlSystem::createObstacles;

			if (MouseControlSystem::createTile || MouseControlSystem::createBoxCollider)
			{
				MouseControlSystem::createTile = false;
				MouseControlSystem::createBoxCollider = false;
			}
		}
		if (ImGui::Button("Create Colliders/Triggers"))
		{
			MouseControlSystem::createBoxCollider = !MouseControlSystem::createBoxCollider;

			if (MouseControlSystem::createTile || MouseControlSystem::createObstacles)
			{
				MouseControlSystem::createTile = false;
				MouseControlSystem::createObstacles = false;
			}
		}

		if (ImGui::Button("Create Enemies"))
		{
			MouseControlSystem::createEnemy = !MouseControlSystem::createEnemy;

			if (MouseControlSystem::createEnemy)
			{
				MouseControlSystem::createTile = false;
				MouseControlSystem::createObstacles = false;
				MouseControlSystem::createBoxCollider = false;
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

		ImGui::Spacing();
		ImGui::Spacing();
		ImGui::Spacing();
		ImGui::Spacing();
		if (MouseControlSystem::createTile) ImGui::Text("CURRENTLT CREATING TILES!");
		if (MouseControlSystem::createObstacles) ImGui::Text("CURRENTLT CREATING OBSTACLES!");
		if (MouseControlSystem::createBoxCollider) ImGui::Text("CURRENTLT CREATING BOX COLLIDERS!");
		if (MouseControlSystem::createEnemy) ImGui::Text("CURRENTLT CREATING ENEMIES!");
	}
	void ShowMenuTile( const std::unique_ptr<AssetManager>& assetManager, SDL_Renderer* renderer)
	{
		// Variable Declarations
		static int zIndex = 0;
		static int scaleX = 1;
		static int scaleY = 1;
		static int mouseRectX = 16;
		static int mouseRectY = 16;
		static int boxColliderWidth = 0;
		static int boxColliderHeight = 0;
		static int boxColliderOffsetX = 0;
		static int boxColliderOffsetY = 0;
		static bool layer = false;
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

		if(ImGui::InputInt("Z-Index", &MouseControlSystem::layer, 0, 1));
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

		const char* triggers[] = { "NO_TRIGGER", "SECRET_AREA",  "ENTER_DUNGEON", "BURN_BUSHES", "PUSH_ROCKS", "CAMERA_RIGHT", "TRAP", "HIDDEN_SWITCH", "HIDDEN_OBJECT" };
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
					MouseControlSystem::triggerType = TRAP;
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
		}ImGui::EndCombo();
		
		// Warning Messages if Enemy Creation Fails
		if (noScaleX) ImGui::Text("Please Put in X Scale");
		if (noScaleY) ImGui::Text("Please Put in Y Scale");
	}
	
	// The image Box is the holder of the Tilemap/Object selector
	// The functionality is you click on the image and it determines the height/width/scale/offset based on the 
	// Selections made above!
	void ImageBox( const std::unique_ptr<AssetManager>& assetManager, SDL_Renderer* renderer)
	{
		// Get the Desired texture
		assetManager->GetTexture(MouseControlSystem::imageID);
		
		// Set the image Width/Height based on the obtain values from the file and the current game scale
		int  my_image_width = imageWidth * Game::gameScale;
		int my_image_height = imageHeight * Game::gameScale;
		
		ImGui::Begin("Texture");
		ImGui::Image(assetManager->GetTexture(MouseControlSystem::imageID), ImVec2(my_image_width, my_image_height));
		
		// This is for the mouse Offset for the image --> Still needs to be refined --> Currently Hackish
		
		/*
			TODO: Make the ImGui Window changes sizes based on the image size * game Scale values
			TODO: Rather get the mouse Pos Relative to the current ImGui window!
		*/
		
		int mouseposX = ImGui::GetMousePos().x - 376;
		int mouseposY = ImGui::GetMousePos().y - 31;
		
		// Clamp --> To make sure that we are not dividing by zero value!
		if (my_image_width > 0 && my_image_height > 0 && MouseControlSystem::mouseRectX > 0 && MouseControlSystem::mouseRectY > 0)
		{
			int rows = (my_image_height / (MouseControlSystem::mouseRectY * Game::gameScale));
			int col = (my_image_width / (MouseControlSystem::mouseRectX * Game::gameScale));

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
};

unsigned int RenderEditorGUISystem::imageWidth = 0;
unsigned int RenderEditorGUISystem::imageHeight = 0;