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

class RenderEditorGUISystem : public System
{
private:
	std::string imageID;
	bool imageLoaded = false;
	bool fileLoaded = false;
	std::ofstream saveFile;
	EditorFileLoader loader;
	MouseControlSystem choices;

public:
	struct Funcs
	{
		static int MyResizeCallback(ImGuiInputTextCallbackData* data)
		{
			if (data->EventFlag == ImGuiInputTextFlags_CallbackResize)
			{
				ImVector<char>* my_str = (ImVector<char>*)data->UserData;
				IM_ASSERT(my_str->begin() == data->Buf);
				my_str->resize(data->BufSize); // NB: On resizing calls, generally data->BuFisze == data BufTextLen + 1
				data->Buf = my_str->begin();
			}
			return 0;
		}
		// Tip: Because ImGui is a namespace you would typically add your own function into the namespace in your own files.
		// For example, you may adda function called ImGui::InputText(const char* label, MyString* my_str);
		static bool MyInputTextMultiline(const char* label, ImVector<char>* my_str, const ImVec2& size = ImVec2(0, 0), ImGuiInputTextFlags flags = 0)
		{
			IM_ASSERT((flags & ImGuiInputTextFlags_CallbackResize) == 0);
			return ImGui::InputTextMultiline(label, my_str->begin(), (size_t)my_str->size(), size, flags | ImGuiInputTextFlags_CallbackResize, Funcs::MyResizeCallback, (void*)my_str);
		}
	};

	// Setup the IMGUI Style in the Constructor
	RenderEditorGUISystem()
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
		//colors[ImGuiCol_DockingPreview] = ImVec4(0.26f, 0.59f, 0.98f, 0.50f);
		//colors[ImGuiCol_DockingEmptyBg] = ImVec4(0.20f, 0.20f, 0.20f, 1.00f);
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

		////ImGui::ShowDemoWindow();

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
			imageName = dialog.OpenImage();

			choices.SetImageName(imageName);
			Logger::Log(choices.GetImageName());
			MouseControlSystem::imageID = loader.SetName(imageName);

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
				Logger::Log("Saved Level");
			}

			if (MouseControlSystem::createObstacles)
			{
				if (!fileName.empty())
				{
					loader.SaveObjectMap(fileName,  assetManager, renderer);
					fileLoaded = true;

				}
				Logger::Log("Saved Object");
			}
		}

		if (ImGui::MenuItem("Save As.."))
		{
			//std::string saveAsFile = dialog.SaveFile();

			if (MouseControlSystem::createTile)
			{
				std::string saveAsFile = dialog.SaveFile();
				if (!saveAsFile.empty())
				{
					Logger::Err("Saved Level");
					loader.SaveTilemap(saveAsFile, assetManager, renderer);
				}
			}

			if (MouseControlSystem::createObstacles)
			{
				std::string saveAsFile = dialog.SaveObjFile();
				if (!saveAsFile.empty())
				{
					Logger::Err("SavedObject");
					loader.SaveObjectMap(saveAsFile, assetManager, renderer);
				}
			}

			if (MouseControlSystem::createBoxCollider)
			{
				std::string saveAsFile = dialog.SaveBoxColliderFile();
				if (!saveAsFile.empty())
				{
					Logger::Err("SavedCollider");
					loader.SaveBoxColliderMap(saveAsFile, assetManager, renderer);
					loader.SaveBoxColliderMapToLuaFile(saveAsFile, assetManager, renderer);
				}
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
		if (ImGui::Button("Create Colliders"))
		{
			MouseControlSystem::createBoxCollider = !MouseControlSystem::createBoxCollider;

			if (MouseControlSystem::createTile || MouseControlSystem::createObstacles)
			{
				MouseControlSystem::createTile = false;
				MouseControlSystem::createObstacles = false;
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
			Game::Instance()->GetStateMachine()->PopState();
			Game::Instance()->GetStateMachine()->PushState(new MenuState());
		}

		ImGui::Spacing();
		ImGui::Spacing();
		ImGui::Spacing();
		ImGui::Spacing();
		if (MouseControlSystem::createTile) ImGui::Text("CURRENTLT CREATING TILES!");
		if (MouseControlSystem::createObstacles) ImGui::Text("CURRENTLT CREATING OBSTACLES!");
		if (MouseControlSystem::createBoxCollider) ImGui::Text("CURRENTLT CREATING BOX COLLIDERS!");
	}
	void ShowMenuTile( const std::unique_ptr<AssetManager>& assetManager, SDL_Renderer* renderer)
	{
		// Variable Declarations
		static int zIndex = 0;
		static int scaleX = 1;
		static int scaleY = 1;
		static int mouseRectX = 32;
		static int mouseRectY = 32;
		static int boxColliderWidth = 0;
		static int boxColliderHeight = 0;
		static int boxColliderOffsetX = 0;
		static int boxColliderOffsetY = 0;

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

		if (ImGui::CollapsingHeader("Music", ImGuiTreeNodeFlags_DefaultOpen));
		{
			//ImGui::Combo("Sprite Type", &item_type, sprite_names, IM_ARRAYSIZE(sprite_names), IM_ARRAYSIZE(sprite_names));
			ImGui::SliderInt("Volume", &volume, 1, 100);
			
			if (ImGui::Button("  Pause Music  "))
			{
				Mix_PauseMusic();
			}
			if (ImGui::Button(" Resume Music  "))
			{
				Mix_ResumeMusic();
			}
			if (ImGui::Button("   Stop Music  "))
			{
				Mix_HaltMusic();
			}
			
			Mix_VolumeMusic(volume);
		}

		ImGui::Spacing();

		if (ImGui::CollapsingHeader("Box Collider Components", ImGuiTreeNodeFlags_DefaultOpen))
		{
			ImGui::SliderInt("Box Width", &boxColliderWidth, 1, 96);
			ImGui::SliderInt("Box Height", &boxColliderHeight, 1, 96);
			ImGui::SliderInt("Box OffsetX", &boxColliderOffsetX, -96, 96);
			ImGui::SliderInt("Box OffsetY", &boxColliderOffsetY, -96, 96);
		}
		if (ImGui::CollapsingHeader("Mouse Box Size", ImGuiTreeNodeFlags_DefaultOpen))
		{

			ImGui::SliderInt("Mouse Rect X", &mouseRectX, 8, 128);
			ImGui::SliderInt("Mouse Rect Y", &mouseRectY, 8, 128);
		}
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

		// Warning Messages if Enemy Creation Fails
		if (noScaleX) ImGui::Text("Please Put in X Scale");
		if (noScaleY) ImGui::Text("Please Put in Y Scale");
	}
	void ImageBox( const std::unique_ptr<AssetManager>& assetManager, SDL_Renderer* renderer)
	{
		assetManager->GetTexture(MouseControlSystem::imageID);
		static int  my_image_width = 256 * 3;
		static int my_image_height = 192 * 3;
		const ImVec2 vec1(10, 10);
		const ImVec2 vec2(30, 30);
		ImGui::Begin("Texture");
		ImGui::Image(assetManager->GetTexture(MouseControlSystem::imageID), ImVec2(my_image_width, my_image_height));


		auto draw_list = ImGui::GetWindowDrawList();
		if (ImGui::IsItemHovered()) {
			if (ImGui::IsMouseClicked(0))
			{
				Logger::Log(std::to_string(ImGui::GetMousePos().x) + std::to_string(ImGui::GetMousePos().y));
			}

		}
		////ImDrawList::AddLine
		ImGui::End();
	}
};