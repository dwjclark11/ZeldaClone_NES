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
	bool imageLoaded = false;
	
	std::ofstream saveFile;
	EditorFileLoader loader;
	MouseControlSystem choices;
	static int imageWidth, imageHeight;
	static int mapWidth, mapHeight;
	// static std::string mapAssetID;
	class Game& game;

	// Variable Declarations
	static int zIndex;
	static int scaleX;
	static int scaleY;
	static int mouseRectX;
	static int mouseRectY;
	static int boxColliderWidth;
	static int boxColliderHeight;
	static int boxColliderOffsetX;
	static int boxColliderOffsetY;

	static bool mouseImageLoaded;
	static bool showLoadMap;
	static bool showTileProperties;
	static bool showCanvasProperties;
	static bool showImageBox;

public:
	static std::string fileName;
	static std::string imageName;
	static std::string imageID;

	static bool fileLoaded;

	// Setup the IMGUI Style in the Constructor --> Change to it's own style function?
	RenderEditorGUISystem()
		: game(Game::Instance())
	{
		ImGuiSetup();
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
