#pragma once
#include "../../ECS/ECS.h"
#include <imgui/imgui.h>
#include <imgui/imgui_sdl.h>
#include <imgui/imgui_stdlib.h>

#include "../../Game/Game.h"
#include "../../Editor/EditorFileLoader.h"
#include "MouseControlSystem.h"
#include <fstream>
#include <iostream>

class RenderEditorGUISystem : public System
{
private:
	class Game& game;
	MouseControlSystem& m_MouseControl;
	EditorFileLoader m_Loader;
	
	bool imageLoaded{ false };
	std::ofstream saveFile;

	int imageWidth{ 0 }, imageHeight{ 0 };
	int mapWidth{ 0 }, mapHeight{ 0 };
	
	// Variable Declarations
	int zIndex{ 0 }, scaleX{ 4 }, scaleY{ 4 };
	int mouseRectX{ 16 }, mouseRectY{ 16 };
	int boxColliderWidth{ 0 }, boxColliderHeight{ 0 }, boxColliderOffsetX{ 0 }, boxColliderOffsetY{ 0 };
	
	bool mouseImageLoaded{ false }, showLoadMap{ false }, showTileProperties{ false };
	bool showCanvasProperties{ false }, showImageBox{ false }, fileLoaded{ false };

	std::string fileName{""}, imageName{""}, imageID{""};

public:
	RenderEditorGUISystem(MouseControlSystem& mouseControl);
	~RenderEditorGUISystem() = default;

	void Update(const std::unique_ptr<AssetManager>& assetManager, SDL_Renderer* renderer);

	void ShowMenuFile(const std::unique_ptr<AssetManager>& assetManager, SDL_Renderer* renderer);
	void ShowMenuTile();
	void EnemyProperties(const std::unique_ptr<AssetManager>& assetManager, SDL_Renderer* renderer);
	void TileProperties(const std::unique_ptr<AssetManager>& assetManager, SDL_Renderer* renderer);
	void TriggerProperties(const std::unique_ptr<AssetManager>& assetManager, SDL_Renderer* renderer);
	void ColliderProperties();
	void ShowMapLoaderWindow(const std::unique_ptr<AssetManager>& assetManager, SDL_Renderer* renderer);
	void CanvasSize();
	
	void ImGuiSetup();
	void ImageBox(const std::unique_ptr<AssetManager>& assetManager, SDL_Renderer* renderer);
	bool LoadEntireMapSprite(const std::unique_ptr<AssetManager>& assetManager, SDL_Renderer* renderer);
	bool LoadNewImage(const std::unique_ptr<AssetManager>& assetManager, SDL_Renderer* renderer, std::string& imageID, int& image_width, int& image_height);

	void ComboLoop(std::string label, std::string& current_item, const std::string handinArray[], int size);

	struct ImFuncs
	{
		static int MyResizeCallback(ImGuiInputTextCallbackData* data)
		{
			if (data->EventFlag == ImGuiInputTextFlags_CallbackResize)
			{
				std::string* my_str = (std::string*)data->UserData;
				//IM_ASSERT(my_str->begin() == data->Buf);
				my_str->resize(data->BufSize);
				data->Buf = (char*)my_str;
			}
			return 0;
		}
		
		static bool MyInputText(const char* label, std::string* my_str, const ImVec2& size = ImVec2(0, 0), ImGuiInputTextFlags flags = 0)
		{
			IM_ASSERT((flags & ImGuiInputTextFlags_CallbackResize) == 0);
			return ImGui::InputText(label, my_str,  flags/* | ImGuiInputTextFlags_CallbackResize*/, ImFuncs::MyResizeCallback, (void*)my_str);
		}
	};
};
