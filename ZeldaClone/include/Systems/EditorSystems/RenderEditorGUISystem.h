#pragma once
#include "ECS/ECS.h"
#include "Game/Game.h"
#include "Editor/EditorFileLoader.h"
#include "Systems/EditorSystems/MouseControlSystem.h"

#include <imgui.h>
#include <imgui_sdl.h>
#include <imgui_stdlib.h>

#include <fstream>
#include <iostream>

class RenderEditorGUISystem : public System
{
  private:
	class Game& m_Game;
	MouseControlSystem& m_MouseControl;
	EditorFileLoader m_Loader;

	bool imageLoaded{ false };
	std::ofstream saveFile;

	int m_ImageWidth{ 0 }, m_ImageHeight{ 0 };
	int m_MapWidth{ 0 }, m_MapHeight{ 0 };

	// Variable Declarations
	int m_ZIndex{ 0 }, m_ScaleX{ 4 }, m_ScaleY{ 4 };
	int m_MouseRectX{ 16 }, m_MouseRectY{ 16 };
	int m_BoxColliderWidth{ 0 }, m_BoxColliderHeight{ 0 }, m_BoxColliderOffsetX{ 0 }, m_BoxColliderOffsetY{ 0 };

	bool m_bMouseImageLoaded{ false }, m_bShowLoadMap{ false }, m_bShowTileProperties{ false };
	bool m_bShowCanvasProperties{ false }, m_bShowImageBox{ false }, m_bFileLoaded{ false };

	std::string m_sFileName{ "" }, m_sImageName{ "" }, m_sImageID{ "" };

  public:
	RenderEditorGUISystem( MouseControlSystem& mouseControl );
	~RenderEditorGUISystem() = default;

	void Update( const std::unique_ptr<AssetManager>& assetManager, SDL_Renderer* renderer );

	void ShowMenuFile( const std::unique_ptr<AssetManager>& assetManager, SDL_Renderer* renderer );
	void ShowMenuTile();
	void EnemyProperties( const std::unique_ptr<AssetManager>& assetManager, SDL_Renderer* renderer );
	void TileProperties( const std::unique_ptr<AssetManager>& assetManager, SDL_Renderer* renderer );
	void TriggerProperties( const std::unique_ptr<AssetManager>& assetManager, SDL_Renderer* renderer );
	void ColliderProperties();
	void ShowMapLoaderWindow( const std::unique_ptr<AssetManager>& assetManager, SDL_Renderer* renderer );
	void CanvasSize();

	void ImGuiSetup();
	void ImageBox( const std::unique_ptr<AssetManager>& assetManager, SDL_Renderer* renderer );
	bool LoadEntireMapSprite( const std::unique_ptr<AssetManager>& assetManager, SDL_Renderer* renderer );
	bool LoadNewImage( const std::unique_ptr<AssetManager>& assetManager, SDL_Renderer* renderer, std::string& imageID,
					   int& image_width, int& image_height );

	void ComboLoop( std::string label, std::string& current_item, const std::string handinArray[], int size );

	struct ImFuncs
	{
		static int MyResizeCallback( ImGuiInputTextCallbackData* data )
		{
			if ( data->EventFlag == ImGuiInputTextFlags_CallbackResize )
			{
				std::string* my_str = (std::string*)data->UserData;
				// IM_ASSERT(my_str->begin() == data->Buf);
				my_str->resize( data->BufSize );
				data->Buf = (char*)my_str;
			}
			return 0;
		}

		static bool MyInputText( const char* label, std::string* my_str, const ImVec2& size = ImVec2( 0, 0 ),
								 ImGuiInputTextFlags flags = 0 )
		{
			IM_ASSERT( ( flags & ImGuiInputTextFlags_CallbackResize ) == 0 );
			return ImGui::InputText( label,
									 my_str,
									 flags /* | ImGuiInputTextFlags_CallbackResize*/,
									 ImFuncs::MyResizeCallback,
									 (void*)my_str );
		}
	};
};
