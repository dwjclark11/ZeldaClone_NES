#include "Systems/EditorSystems/RenderEditorGUISystem.h"
#include "Systems/EditorSystems/RenderEditorSystem.h"
#include "Components/TransformComponent.h"
#include "Components/SecretComponent.h"

#include "Utilities/FileManagerUtils.h"
#include "Utilities/Camera.h"
#include "States/MenuState.h"
#include "States/GameStateMachine.h"
#include "Game/LevelLoader.h"

#include <filesystem>

RenderEditorGUISystem::RenderEditorGUISystem( MouseControlSystem& mouseControl )
	: m_Game( Game::Instance() )
	, m_MouseControl{ mouseControl }
	, m_Loader{ m_MouseControl }
{
	ImGuiSetup();
	ImGuiStyle& style = ImGui::GetStyle();
	style.TabRounding = 0.0f;
	style.FrameBorderSize = 1.0f;
	style.ScrollbarRounding = 0.0f;
	style.ScrollbarSize = 10.0f;
	ImVec4* colors = ImGui::GetStyle().Colors;
	colors[ ImGuiCol_Text ] = ImVec4( 0.95f, 0.95f, 0.95f, 1.00f );
	colors[ ImGuiCol_TextDisabled ] = ImVec4( 0.50f, 0.50f, 0.50f, 1.00f );
	colors[ ImGuiCol_WindowBg ] = ImVec4( 0.12f, 0.12f, 0.12f, 1.00f );
	colors[ ImGuiCol_ChildBg ] = ImVec4( 0.04f, 0.04f, 0.04f, 0.50f );
	colors[ ImGuiCol_PopupBg ] = ImVec4( 0.12f, 0.12f, 0.12f, 0.94f );
	colors[ ImGuiCol_Border ] = ImVec4( 0.25f, 0.25f, 0.27f, 0.50f );
	colors[ ImGuiCol_BorderShadow ] = ImVec4( 0.00f, 0.00f, 0.00f, 0.00f );
	colors[ ImGuiCol_FrameBg ] = ImVec4( 0.20f, 0.20f, 0.22f, 0.50f );
	colors[ ImGuiCol_FrameBgHovered ] = ImVec4( 0.25f, 0.25f, 0.27f, 0.75f );
	colors[ ImGuiCol_FrameBgActive ] = ImVec4( 0.30f, 0.30f, 0.33f, 1.00f );
	colors[ ImGuiCol_TitleBg ] = ImVec4( 0.04f, 0.04f, 0.04f, 1.00f );
	colors[ ImGuiCol_TitleBgActive ] = ImVec4( 0.04f, 0.04f, 0.04f, 1.00f );
	colors[ ImGuiCol_TitleBgCollapsed ] = ImVec4( 0.04f, 0.04f, 0.04f, 0.75f );
	colors[ ImGuiCol_MenuBarBg ] = ImVec4( 0.18f, 0.18f, 0.19f, 1.00f );
	colors[ ImGuiCol_ScrollbarBg ] = ImVec4( 0.24f, 0.24f, 0.26f, 0.75f );
	colors[ ImGuiCol_ScrollbarGrab ] = ImVec4( 0.41f, 0.41f, 0.41f, 0.75f );
	colors[ ImGuiCol_ScrollbarGrabHovered ] = ImVec4( 0.62f, 0.62f, 0.62f, 0.75f );
	colors[ ImGuiCol_ScrollbarGrabActive ] = ImVec4( 0.94f, 0.92f, 0.94f, 0.75f );
	colors[ ImGuiCol_CheckMark ] = ImVec4( 0.60f, 0.60f, 0.60f, 1.00f );
	colors[ ImGuiCol_SliderGrab ] = ImVec4( 0.41f, 0.41f, 0.41f, 0.75f );
	colors[ ImGuiCol_SliderGrabActive ] = ImVec4( 0.62f, 0.62f, 0.62f, 0.75f );
	colors[ ImGuiCol_Button ] = ImVec4( 0.20f, 0.20f, 0.22f, 1.00f );
	colors[ ImGuiCol_ButtonHovered ] = ImVec4( 0.25f, 0.25f, 0.27f, 1.00f );
	colors[ ImGuiCol_ButtonActive ] = ImVec4( 0.41f, 0.41f, 0.41f, 1.00f );
	colors[ ImGuiCol_Header ] = ImVec4( 0.18f, 0.18f, 0.19f, 1.00f );
	colors[ ImGuiCol_HeaderHovered ] = ImVec4( 0.25f, 0.25f, 0.27f, 1.00f );
	colors[ ImGuiCol_HeaderActive ] = ImVec4( 0.41f, 0.41f, 0.41f, 1.00f );
	colors[ ImGuiCol_Separator ] = ImVec4( 0.25f, 0.25f, 0.27f, 1.00f );
	colors[ ImGuiCol_SeparatorHovered ] = ImVec4( 0.41f, 0.41f, 0.41f, 1.00f );
	colors[ ImGuiCol_SeparatorActive ] = ImVec4( 0.62f, 0.62f, 0.62f, 1.00f );
	colors[ ImGuiCol_ResizeGrip ] = ImVec4( 0.30f, 0.30f, 0.33f, 0.75f );
	colors[ ImGuiCol_ResizeGripHovered ] = ImVec4( 0.41f, 0.41f, 0.41f, 0.75f );
	colors[ ImGuiCol_ResizeGripActive ] = ImVec4( 0.62f, 0.62f, 0.62f, 0.75f );
	colors[ ImGuiCol_Tab ] = ImVec4( 0.21f, 0.21f, 0.22f, 1.00f );
	colors[ ImGuiCol_TabHovered ] = ImVec4( 0.37f, 0.37f, 0.39f, 1.00f );
	colors[ ImGuiCol_TabActive ] = ImVec4( 0.30f, 0.30f, 0.33f, 1.00f );
	colors[ ImGuiCol_TabUnfocused ] = ImVec4( 0.12f, 0.12f, 0.12f, 0.97f );
	colors[ ImGuiCol_TabUnfocusedActive ] = ImVec4( 0.18f, 0.18f, 0.19f, 1.00f );
	colors[ ImGuiCol_PlotLines ] = ImVec4( 0.61f, 0.61f, 0.61f, 1.00f );
	colors[ ImGuiCol_PlotLinesHovered ] = ImVec4( 1.00f, 0.43f, 0.35f, 1.00f );
	colors[ ImGuiCol_PlotHistogram ] = ImVec4( 0.90f, 0.70f, 0.00f, 1.00f );
	colors[ ImGuiCol_PlotHistogramHovered ] = ImVec4( 1.00f, 0.60f, 0.00f, 1.00f );
	colors[ ImGuiCol_TextSelectedBg ] = ImVec4( 0.26f, 0.59f, 0.98f, 0.50f );
	colors[ ImGuiCol_DragDropTarget ] = ImVec4( 1.00f, 1.00f, 0.00f, 0.90f );
	colors[ ImGuiCol_NavHighlight ] = ImVec4( 0.26f, 0.59f, 0.98f, 1.00f );
	colors[ ImGuiCol_NavWindowingHighlight ] = ImVec4( 1.00f, 1.00f, 1.00f, 0.70f );
	colors[ ImGuiCol_NavWindowingDimBg ] = ImVec4( 0.80f, 0.80f, 0.80f, 0.20f );
	colors[ ImGuiCol_ModalWindowDimBg ] = ImVec4( 0.80f, 0.80f, 0.80f, 0.35f );
	style.WindowMenuButtonPosition = ImGuiDir_Right;
}

void RenderEditorGUISystem::Update( const std::unique_ptr<AssetManager>& assetManager, SDL_Renderer* renderer )
{
	ImGui::NewFrame();

	if ( ImGui::BeginMainMenuBar() )
	{
		if ( ImGui::BeginMenu( "File" ) )
		{
			ShowMenuFile( assetManager, renderer );
			ImGui::EndMenu();
		}

		if ( ImGui::BeginMenu( "Edit" ) )
		{
			if ( ImGui::MenuItem( "Canvas Size" ) )
				m_bShowCanvasProperties = true;

			ImGui::EndMenu();
		}

		if ( ImGui::BeginMenu( "Tools" ) )
		{
			ImGui::Text( "Select what to Create" );
			ImGui::Spacing();
			ImGui::Spacing();
			ImGui::Spacing();
			ImGui::Spacing();

			static bool bCreateTile{ false };
			if ( ImGui::Checkbox( "Create Tiles", &bCreateTile ) )
				m_MouseControl.SetCreateTile( bCreateTile );

			static bool bCreateObstacle{ false };
			if ( ImGui::Checkbox( "Create Obstacles", &bCreateObstacle ) )
				m_MouseControl.SetCreateObstacle( bCreateObstacle );

			static bool bCreateCollider{ false };
			if ( ImGui::Checkbox( "Create Colliders", &bCreateCollider ) )
				m_MouseControl.SetCreateBoxCollider( bCreateCollider );

			static bool bCreateTrigger{ false };
			if ( ImGui::Checkbox( "Create Triggers", &bCreateTrigger ) )
				m_MouseControl.SetCreateTrigger( bCreateTrigger );

			static bool bCreateEnemy{ false };
			if ( ImGui::Checkbox( "Create Enemies", &bCreateEnemy ) )
				m_MouseControl.SetCreateEnemy( bCreateEnemy );

			// Set the mouse image to snap to the grid
			static bool bGridSnap{ false };
			if ( ImGui::Checkbox( "Grip Snap", &bGridSnap ) )
				m_MouseControl.SetGridSnap( bGridSnap );

			ImGui::EndMenu();
		}

		ImGui::Spacing();
		ImGui::Spacing();
		ImGui::Spacing();
		ImGui::Spacing();

		if ( m_MouseControl.IsCreatingTiles() )
			ImGui::TextColored( ImVec4( 255, 255, 0, 1 ), "CURRENTLY CREATING TILES!" );
		else if ( m_MouseControl.IsCreatingObstacles() )
			ImGui::TextColored( ImVec4( 255, 255, 0, 1 ), "CURRENTLY CREATING OBSTACLES!" );
		else if ( m_MouseControl.IsCreatingColliders() )
			ImGui::TextColored( ImVec4( 255, 255, 0, 1 ), "CURRENTLY CREATING BOX COLLIDERS!" );
		if ( m_MouseControl.IsCreatingEnemies() )
			ImGui::TextColored( ImVec4( 255, 255, 0, 1 ), "CURRENTLY CREATING ENEMIES!" );
		if ( m_MouseControl.IsCreatingTriggers() )
			ImGui::TextColored( ImVec4( 255, 255, 0, 1 ), "CURRENTLY CREATING TRIGGERS!" );

		ImGui::Spacing();
		ImGui::Spacing();
		ImGui::Spacing();
		ImGui::Spacing();
		ImGui::Spacing();
		ImGui::Spacing();
		ImGui::Spacing();
		ImGui::Spacing();

		if ( m_MouseControl.GridSize() > 0 )
		{
			ImGui::TextColored(
				ImVec4( 0, 255, 0, 1 ),
				"Grid X: %d",
				static_cast<int>( Game::Instance().GetMouseBox().x + Game::Instance().GetCamera().GetCameraPos().x ) /
					m_MouseControl.GridSize() );

			ImGui::TextColored(
				ImVec4( 0, 255, 0, 1 ),
				"Grid Y : %d",
				static_cast<int>( Game::Instance().GetMouseBox().y + Game::Instance().GetCamera().GetCameraPos().y ) /
					m_MouseControl.GridSize() );
		}

		ImGui::Spacing();
		ImGui::Spacing();
		ImGui::Spacing();
		ImGui::Spacing();
		ImGui::TextColored(
			ImVec4( 0, 255, 0, 1 ),
			"[Mouse X: %d",
			static_cast<int>( Game::Instance().GetMouseBox().x + Game::Instance().GetCamera().GetCameraPos().x ) );

		ImGui::TextColored(
			ImVec4( 0, 255, 0, 1 ),
			"Mouse Y : %d]",
			static_cast<int>( Game::Instance().GetMouseBox().y + Game::Instance().GetCamera().GetCameraPos().y ) );

		// Put Above this
		ImGui::EndMainMenuBar();
	}

	bool bCreateingTiles = m_MouseControl.IsCreatingTiles();
	if ( bCreateingTiles )
	{
		ImageBox( assetManager, renderer );

		if ( ImGui::Begin( "Tile Properties", &bCreateingTiles ) )
		{
			ShowMenuTile();

			if ( ImGui::Button( "Set Tile Attributes" ) )
			{
				m_MouseControl.SetSpriteLayer( m_ZIndex );
				m_MouseControl.SetTransformScale( m_ScaleX, m_ScaleY );
				m_MouseControl.SetBoxCollider( BoxColliderComponent{
					m_BoxColliderWidth, m_BoxColliderHeight, glm::vec2{ m_BoxColliderOffsetX, m_BoxColliderOffsetY } } );
				m_MouseControl.SetMouseRect( m_MouseRectX, m_MouseRectY );
			}

			ImGui::End();
		}
	}

	if ( m_bShowLoadMap )
		ShowMapLoaderWindow( assetManager, renderer );

	if ( m_bShowCanvasProperties )
		CanvasSize();

	if ( m_MouseControl.IsCreatingEnemies() )
		EnemyProperties( assetManager, renderer );
	if ( m_MouseControl.IsCreatingTriggers() )
		TriggerProperties( assetManager, renderer );
	if ( m_MouseControl.IsCreatingColliders() )
		ColliderProperties();

	ImGui::Render();
	ImGuiSDL::Render( ImGui::GetDrawData() );

	ImGuiIO& io = ImGui::GetIO();
	if ( io.WantCaptureMouse )
		m_MouseControl.SetOverImGui( true );
	else
		m_MouseControl.SetOverImGui( false );
}

void RenderEditorGUISystem::ShowMenuFile( const std::unique_ptr<AssetManager>& assetManager, SDL_Renderer* renderer )
{
	FileDialogs dialog;
	ImGui::MenuItem( "(File Menu)", NULL, false, false );

	// Destroy All Entities and reset all variables
	if ( ImGui::MenuItem( "New" ) )
	{
		if ( !GetSystemEntities().empty() )
		{
			for ( auto& entity : GetSystemEntities() )
			{
				entity.Kill();
				m_sImageName = "";
				m_sFileName = "";
			}
		}
	}

	if ( ImGui::MenuItem( "Load Image" ) )
	{
		m_sImageName = dialog.OpenImage();
		m_MouseControl.SetImageName( m_sImageName );
		m_Loader.SetImageName( m_sImageName );

		// Check to see if the String is Empty!!
		if ( !m_sImageName.empty() )
		{
			imageLoaded = true;
		}
		else
		{
			Logger::Err( "Did Not Load Image" );
			return;
		}
		Logger::Log( "This name " + m_MouseControl.GetImageName() );
	}

	if ( ImGui::MenuItem( "Load Tileset" ) )
	{
		std::string sAssetID{ m_MouseControl.GetSpriteComponent().assetID };
		LoadNewImage( assetManager, renderer, sAssetID, m_ImageWidth, m_ImageHeight );
		m_MouseControl.SetSpriteAssetID( sAssetID );
		m_bMouseImageLoaded = true;
	}

	if ( ImGui::MenuItem( "Load Sprite Map", "Creates a map from one texture" ) )
	{
		m_bShowLoadMap = true;
	}

	if ( m_MouseControl.IsCreatingTiles() )
	{
		if ( ImGui::MenuItem( "Open Tilemap", "Ctrl+O" ) )
		{
			if ( !m_sImageName.empty() )
			{
				m_sFileName = dialog.OpenFile();

				m_Loader.SetFileName( m_sFileName );

				// Check to see if the String is Empty!!
				if ( !m_sFileName.empty() )
				{
					m_Loader.LoadTilemap( assetManager, renderer );
					m_bFileLoaded = true;
				}
			}
		}
	}

	if ( m_MouseControl.IsCreatingObstacles() )
	{
		if ( ImGui::MenuItem( "Open Object Map", "Ctrl+O" ) )
		{
			if ( !m_sImageName.empty() )
			{
				m_sFileName = dialog.OpenFile();

				m_Loader.SetFileName( m_sFileName );

				// Check to see if the String is Empty!!
				if ( !m_sFileName.empty() )
				{
					m_Loader.LoadObjectMap( assetManager, renderer );
					m_bFileLoaded = true;
				}
			}
		}
	}
	if ( m_MouseControl.IsCreatingColliders() )
	{
		if ( ImGui::MenuItem( "Open Collider Map", "Ctrl+O" ) )
		{
			m_sFileName = dialog.OpenFile();

			std::string extension = std::filesystem::path( m_sFileName ).extension().string();

			Logger::Log( "Extension: " + extension );

			m_Loader.SetFileName( m_sFileName );

			// Check to see if the String is Empty!!
			if ( !m_sFileName.empty() && extension == ".map" )
			{
				m_Loader.LoadBoxColliderMap( assetManager, renderer, m_sFileName );
				m_bFileLoaded = true;
			}
		}
	}

	if ( m_MouseControl.IsCreatingTriggers() )
	{
		if ( ImGui::MenuItem( "Open Trigger Map", "Ctrl+O" ) )
		{
			LevelLoader load;
			m_sFileName = dialog.OpenFile();

			std::string extension = std::filesystem::path( m_sFileName ).extension().string();
			std::string stem = std::filesystem::path( m_sFileName ).stem().string();

			m_Loader.SetFileName( m_sFileName );

			// Check to see if the String is Empty!!
			if ( !m_sFileName.empty() && extension == ".lua" )
			{
				load.LoadTriggers( m_Game.GetLuaState(), stem );
				m_bFileLoaded = true;
			}
		}
	}

	if ( m_MouseControl.IsCreatingEnemies() )
	{
		if ( ImGui::MenuItem( "Open Enemy Map", "Ctrl+O" ) )
		{
			LevelLoader load;
			m_sFileName = dialog.OpenFile();

			std::string extension = std::filesystem::path( m_sFileName ).extension().string();
			std::string stem = std::filesystem::path( m_sFileName ).stem().string();

			m_Loader.SetFileName( m_sFileName );

			// Check to see if the String is Empty!!
			if ( !m_sFileName.empty() && extension == ".lua" )
			{
				load.LoadEnemiesFromLuaTable( m_Game.GetLuaState(), stem );
				m_bFileLoaded = true;
			}
		}
	}

	if ( ImGui::MenuItem( "Save", "Ctrl+S" ) )
	{
		std::string extension = std::filesystem::path( m_sFileName ).extension().string();
		if ( !m_sFileName.empty() )
		{
			if ( m_MouseControl.IsCreatingTiles() )
			{
				if ( extension == ".map" )
					m_Loader.SaveTilemap( m_sFileName, assetManager, renderer );
				m_bFileLoaded = true;
			}

			if ( m_MouseControl.IsCreatingObstacles() )
			{
				if ( extension == ".obj" )
					m_Loader.SaveObjectMap( m_sFileName, assetManager, renderer );
				m_bFileLoaded = true;
			}
			if ( m_MouseControl.IsCreatingColliders() )
			{
				if ( extension == ".map" )
					m_Loader.SaveBoxColliderMap( m_sFileName, assetManager, renderer );
				else if ( extension == ".lua" )
					m_Loader.SaveBoxColliderMapToLuaFile( m_sFileName );

				m_bFileLoaded = true;
			}
			if ( m_MouseControl.IsCreatingTriggers() )
			{
				if ( extension == ".lua" )
					m_Loader.SaveTriggersToLuaFile( m_sFileName );
			}
		}
	}

	if ( ImGui::MenuItem( "Save As.." ) )
	{
		std::string saveAsFile = dialog.SaveFile();
		std::string extension = std::filesystem::path( saveAsFile ).extension().string();

		if ( !saveAsFile.empty() )
		{
			if ( m_MouseControl.IsCreatingTiles() )
			{

				if ( extension == ".map" )
					m_Loader.SaveTilemap( saveAsFile, assetManager, renderer );
			}

			else if ( m_MouseControl.IsCreatingObstacles() )
			{
				if ( extension == ".obj" )
					m_Loader.SaveObjectMap( saveAsFile, assetManager, renderer );
			}
			else if ( m_MouseControl.IsCreatingColliders() )
			{
				if ( extension == ".lua" )
					m_Loader.SaveBoxColliderMapToLuaFile( saveAsFile );
				else if ( extension == ".map" )
					m_Loader.SaveBoxColliderMap( saveAsFile, m_Game.GetAssetManager(), m_Game.GetRenderer() );
			}
			else if ( m_MouseControl.IsCreatingEnemies() )
			{
				if ( extension == ".lua" )
					m_Loader.SaveEnemiesToLuaFile( saveAsFile );
			}
			else if ( m_MouseControl.IsCreatingTriggers() )
			{
				if ( extension == ".lua" )
					m_Loader.SaveTriggersToLuaFile( saveAsFile );
			}
		}
	}

	ImGui::Spacing();
	ImGui::Spacing();
	ImGui::Spacing();
	ImGui::Spacing();
	if ( ImGui::MenuItem( "Play Game!!" ) )
	{
		m_Game.GetStateMachine()->PushState( std::make_unique<MenuState>() );
	}

	ImGui::Spacing();
	ImGui::Spacing();
	ImGui::Spacing();
	ImGui::Spacing();

	if ( ImGui::MenuItem( "Quit" ) )
	{
		m_Game.SetGameRunning( false );
	}
}

void RenderEditorGUISystem::ShowMenuTile()
{
	ImGui::Text( "Transform Component" );
	ImGui::SliderInt( "X Scale", &m_ScaleX, 1, 10 );
	ImGui::SliderInt( "Y Scale", &m_ScaleY, 1, 10 );

	ImGui::Spacing();
	ImGui::Spacing();

	static bool bIsCollider{ false };
	if ( ImGui::Checkbox( "Box Collider", &bIsCollider ) )
		m_MouseControl.SetCollider( bIsCollider );

	ImGui::Text( "Box Collider Components" );
	ImGui::Spacing();
	ImGui::TextColored( ImVec4( 255, 255, 0, 1 ), "Box Collider values are multiples of 8" );

	if ( ImGui::InputInt( "Box Width", &m_BoxColliderWidth, 8, 8 ) )
	{
		m_BoxColliderWidth = ( m_BoxColliderWidth / 8 ) * 8;
		if ( m_BoxColliderWidth <= 0 )
			m_BoxColliderWidth = 0;
	}

	if ( ImGui::InputInt( "Box Height", &m_BoxColliderHeight, 8, 8 ) )
	{
		m_BoxColliderHeight = ( m_BoxColliderHeight / 8 ) * 8;
		if ( m_BoxColliderHeight <= 0 )
			m_BoxColliderHeight = 0;
	}

	if ( ImGui::InputInt( "Box OffsetX", &m_BoxColliderOffsetX, 8, 8 ) )
		m_BoxColliderOffsetX = ( m_BoxColliderOffsetX / 8 ) * 8;

	if ( ImGui::InputInt( "Box OffsetY", &m_BoxColliderOffsetY, 8, 8 ) )
		m_BoxColliderOffsetY = ( m_BoxColliderOffsetY / 8 ) * 8;

	ImGui::Spacing();
	ImGui::TextColored( ImVec4( 255, 255, 0, 1 ), "Mouse values are multiples of 8" );
	ImGui::Spacing();
	ImGui::Text( "Mouse Box Size" );
	if ( ImGui::InputInt( "Mouse Rect X", &m_MouseRectX, 8, 8 ) )
	{
		m_MouseRectX = ( m_MouseRectX / 8 ) * 8;
		// Clamp mouse Rect X at zero
		if ( m_MouseRectX <= 0 )
			m_MouseRectX = 0;
	}

	if ( ImGui::InputInt( "Mouse Rect Y", &m_MouseRectY, 8, 8 ) )
	{
		m_MouseRectY = ( m_MouseRectY / 8 ) * 8;
		// Clamp mouse Rect Y at zero
		if ( m_MouseRectY <= 0 )
			m_MouseRectY = 0;
	}
	ImGui::TextColored( ImVec4( 255, 255, 0, 255 ),
						"Current Src Rect [X: %d, Y: %d]",
						m_MouseControl.GetSpriteComponent().srcRect.x,
						m_MouseControl.GetSpriteComponent().srcRect.y );

	ImGui::Spacing();
	ImGui::Text( "Set the layer of the tile" );
	ImGui::SliderInt( "Z-Index", &m_ZIndex, 0, 10 );

	if ( m_ZIndex < 0 )
		m_ZIndex = 0;
	if ( m_ZIndex > 10 )
		m_ZIndex = 10;

	ImGui::Spacing();
	ImGui::Spacing();
}

void RenderEditorGUISystem::EnemyProperties( const std::unique_ptr<AssetManager>& assetManager, SDL_Renderer* renderer )
{
	if ( ImGui::Begin( "Enemy Properties" ) )
	{
		std::string enemies[] = {
			"octorok", "moblin", "darknut", "leever", "tektite", "peahat", "armos", "ghini", "lynel", "zora" };
		static std::string current_enemy = "";

		ComboLoop( "Enemy Type", current_enemy, enemies, sizeof( enemies ) / sizeof( enemies[ 0 ] ) );
		LevelLoader m_Loader;

		// MouseControlSystem::enemyType = m_Loader.ConvertStringToEnemyType(std::string(current_enemy));

		////Logger::Log(std::string(current_enemy) + ": " + std::to_string(MouseControlSystem::enemyType));

		// ImGui::Checkbox("Animation", &MouseControlSystem::animation);

		// if (MouseControlSystem::animation)
		//{
		//	if (ImGui::CollapsingHeader("Animation", ImGuiTreeNodeFlags_DefaultOpen))
		//	{
		//		if (ImGui::InputInt("Frame Rate Speed", &MouseControlSystem::animationComponent.frameSpeedRate, 0, 1));
		//		if (ImGui::InputInt("Number of Frames", &MouseControlSystem::animationComponent.numFrames, 0, 1));
		//		if (ImGui::InputInt("Frame Offset", &MouseControlSystem::animationComponent.frameOffset, 0, 1));

		//		ImGui::Checkbox("Vertical", &MouseControlSystem::animationComponent.vertical);
		//		ImGui::SameLine();
		//		ImGui::Checkbox("Looped", &MouseControlSystem::animationComponent.isLooped);
		//	}
		//}

		// ImGui::SliderInt("Scale X", &m_ScaleX, 1, 10);
		// ImGui::SliderInt("Scale Y", &m_ScaleY, 1, 10);

		// if (ImGui::Button("Set Tile Attributes"))
		//{
		//	MouseControlSystem::transformComponent.scale.x = m_ScaleX;
		//	MouseControlSystem::transformComponent.scale.y = m_ScaleY;
		//	MouseControlSystem::m_MouseRectX = m_MouseRectX;
		//	MouseControlSystem::m_MouseRectY = m_MouseRectY;
		// }
		// ImGui::End();
	}
}
void RenderEditorGUISystem::TriggerProperties( const std::unique_ptr<AssetManager>& assetManager,
											   SDL_Renderer* renderer )
{
	// if (ImGui::Begin("Trigger Properties", &MouseControlSystem::createTrigger))
	//{
	//	static std::string levelMusic{ "stop" };
	//	static std::string assetFile{ "no_file" };
	//	static std::string enemyFile{ "no_file" };
	//	static std::string colliderFile{ "no_file" };
	//	static std::string tileMapName{ "no_file" };
	//	static std::string tileMapImageName{ "no_file" };
	//	static std::string entityFileName{ "no_file" };
	//	static std::string triggerFile{ "no_file" };

	//	static std::string spriteAssetID = "";

	//	static int spriteWidth = 0;
	//	static int spriteHeight = 0;
	//	static int spriteSrcX = 0;
	//	static int spriteSrcY = 0;
	//	static int spriteLayer = 0;

	//	static int newSpriteWidth = 0;
	//	static int newSpriteHeight = 0;
	//	static int newSpriteSrcX = 0;
	//	static int newSpriteSrcY = 0;
	//	static std::string locationID { "" };
	//	static std::string newSpriteAssetID { "" };
	//	static std::string new_trigger { "" };

	//	const std::string triggers[] = { "no_trigger", "secret_area",  "transport", "burn_bushes", "push_rocks",
	//"bomb_secret", "locked_door", "hidden_object" }; 	static std::string current_trigger = "";

	//	ComboLoop("Trigger Types", current_trigger, triggers, sizeof(triggers) / sizeof(triggers[0]));
	//
	//	ImGui::Spacing(); ImGui::Spacing(); ImGui::Spacing();
	//	ImGui::InputFloat("Transport Offset X", &MouseControlSystem::triggerBox.transportOffset.x, 1, 100);
	//	ImGui::InputFloat("Transport Offset Y", &MouseControlSystem::triggerBox.transportOffset.y, 1, 100);
	//	ImGui::InputFloat("Camera Offset X", &MouseControlSystem::triggerBox.cameraOffset.x, 1, 100);
	//	ImGui::InputFloat("Camera Offset Y", &MouseControlSystem::triggerBox.cameraOffset.y, 1, 100);
	//
	//	ImGui::TextColored(ImVec4(255, 255, 0, 255), "Set the music to the song ID or \"stop\"");
	//	ImFuncs::MyInputText("Level Music", &levelMusic);
	//	ImGui::TextColored(ImVec4(255, 255, 0, 255), "Put in the file name or leave as \"no_file\"");

	//	ImFuncs::MyInputText("Asset File", &assetFile);
	//	ImFuncs::MyInputText("Enemy File", &enemyFile);
	//	ImFuncs::MyInputText("Collider File", &colliderFile);
	//	ImFuncs::MyInputText("Tilemap Name", &tileMapName);
	//	ImFuncs::MyInputText("Tilemap Image", &tileMapImageName);
	//	ImFuncs::MyInputText("Entity File", &entityFileName);
	//	ImFuncs::MyInputText("Trigger File", &triggerFile);

	//	ImGui::Spacing(); ImGui::Spacing(); ImGui::Spacing();
	//	ImGui::TextColored(ImVec4(255, 255, 0, 255), "Is the trigger also a collider?");
	//	ImGui::Checkbox("Collider", &MouseControlSystem::triggerBox.collider);
	//	ImGui::Spacing(); ImGui::Spacing(); ImGui::Spacing();

	//	ShowMenuTile();
	//
	//	ImGui::Checkbox("Sprite Component", &MouseControlSystem::spriteSelected);
	//	if (MouseControlSystem::spriteSelected)
	//	{
	//		ImageBox(assetManager, renderer);
	//
	//		if (ImGui::InputInt("Sprite Layer", &spriteLayer, 1, 1))
	//		{
	//			if (spriteLayer >= 10) spriteLayer = 10;
	//			if (spriteLayer <= 0) spriteLayer = 0;
	//		}
	//	}

	//	ImGui::Checkbox("Secret", &MouseControlSystem::secretSelected);
	//
	//	if (MouseControlSystem::secretSelected)
	//	{
	//		ComboLoop("New Secret Trigger", new_trigger, triggers, sizeof(triggers) / sizeof(triggers[0]));

	//		ImFuncs::MyInputText("Location ID", &locationID);
	//
	//		//ImFuncs::MyInputText("New Sprite Asset ID", &newSpriteAssetID);
	//
	//		if (ImGui::BeginCombo("New Sprite Asset ID", newSpriteAssetID.c_str()))
	//		{
	//			std::vector<std::string> mapKey;
	//			for (auto& spriteID : assetManager->GetTextureMap())
	//			{
	//				mapKey.push_back(spriteID.first);
	//			}

	//			for (int i = 0; i < mapKey.size(); i++)
	//			{
	//				bool isSelectable = (newSpriteAssetID == mapKey[i]);
	//				if (ImGui::Selectable(mapKey[i].c_str(), isSelectable))
	//					newSpriteAssetID = mapKey[i];
	//				if (isSelectable)
	//					ImGui::SetItemDefaultFocus();
	//			}

	//			ImGui::EndCombo();
	//		}

	//		if (ImGui::InputInt("New Sprite Width", &newSpriteWidth, 16, 16))
	//		{
	//			newSpriteWidth = (newSpriteWidth / 16) * 16;
	//			if (newSpriteWidth <= 0)
	//				newSpriteWidth = 0;
	//		}
	//
	//		if (ImGui::InputInt("New Sprite Height", &newSpriteHeight, 16, 16))
	//		{
	//			newSpriteHeight = (newSpriteHeight / 16) * 16;
	//			if (newSpriteHeight <= 0)
	//				newSpriteHeight = 0;
	//		}

	//		if (ImGui::InputInt("New Sprite Src X", &newSpriteSrcX, 16, 16))
	//		{
	//			newSpriteSrcX = (newSpriteSrcX / 16) * 16;
	//			if (newSpriteSrcX <= 0)
	//				newSpriteSrcX = 0;
	//		}

	//		if (ImGui::InputInt("New Sprite Src Y", &newSpriteSrcY, 16, 16))
	//		{
	//			newSpriteSrcY = (newSpriteSrcY / 16) * 16;
	//			if (newSpriteSrcY <= 0)
	//				newSpriteSrcY = 0;
	//		}
	//	}

	//	if (ImGui::Button("Set Trigger Attributes"))
	//	{
	//		MouseControlSystem::transformComponent.scale.x = m_ScaleX;
	//		MouseControlSystem::transformComponent.scale.y = m_ScaleY;
	//		MouseControlSystem::boxColliderComponent.height = m_BoxColliderHeight;
	//		MouseControlSystem::boxColliderComponent.width = m_BoxColliderWidth;
	//		MouseControlSystem::boxColliderComponent.offset.x = m_BoxColliderOffsetX;
	//		MouseControlSystem::boxColliderComponent.offset.y = m_BoxColliderOffsetY;
	//		MouseControlSystem::m_MouseRectX = m_MouseRectX;
	//		MouseControlSystem::m_MouseRectY = m_MouseRectY;

	//		//MouseControlSystem::triggerBox.levelMusic = levelMusic;
	//		//MouseControlSystem::triggerBox.assetFile = assetFile;
	//		//MouseControlSystem::triggerBox.enemyFile = enemyFile;
	//		//MouseControlSystem::triggerBox.colliderFile = colliderFile;
	//		//MouseControlSystem::triggerBox.tileMapName= tileMapName;
	//		//MouseControlSystem::triggerBox.tileImageName = tileMapImageName;
	//		//MouseControlSystem::triggerBox.entityFileName = entityFileName;
	//		//MouseControlSystem::triggerBox.triggerFile = triggerFile;

	//		// Set Secret Attributes
	//		if (MouseControlSystem::secretSelected)
	//		{
	//			MouseControlSystem::secretComponent.spriteWidth = newSpriteWidth;
	//			MouseControlSystem::secretComponent.spriteHeight = newSpriteHeight;
	//			MouseControlSystem::secretComponent.newSpriteAssetID = newSpriteAssetID; // Change this to the loaded
	//Images

	//			MouseControlSystem::secretComponent.spriteSrcX = newSpriteSrcX;
	//			MouseControlSystem::secretComponent.spriteSrcY = newSpriteSrcY;

	//			// Set Trigger Types
	//			MouseControlSystem::secretComponent.newTrigger = new_trigger;

	//		}

	//		if (MouseControlSystem::spriteSelected)
	//		{
	//			//MouseControlSystem::spriteComponent.assetID = spriteAssetID;
	//			MouseControlSystem::spriteComponent.width = MouseControlSystem::m_MouseRectX;
	//			MouseControlSystem::spriteComponent.height	= MouseControlSystem::m_MouseRectY;
	//			MouseControlSystem::spriteComponent.layer	= 1;
	//			MouseControlSystem::spriteComponent.bIsFixed = false;
	//			//MouseControlSystem::spriteComponent.srcRect.x = spriteSrcX;
	//			//MouseControlSystem::spriteComponent.srcRect.y = spriteSrcY;
	//		}

	//		// Set Trigger Types
	//		if (current_trigger == "no_trigger")
	//			MouseControlSystem::triggerBox.triggerType = TriggerBoxComponent::TriggerType::NO_TRIGGER;
	//		else if (current_trigger == "scene_change")
	//			MouseControlSystem::triggerBox.triggerType = TriggerBoxComponent::TriggerType::SCENE_CHANGE;
	//		else if (current_trigger == "transport")
	//			MouseControlSystem::triggerBox.triggerType = TriggerBoxComponent::TriggerType::TRANSPORT;
	//		else if (current_trigger == "burn_bushes")
	//			MouseControlSystem::triggerBox.triggerType = TriggerBoxComponent::TriggerType::BURN_BUSHES;
	//		else if (current_trigger == "push_rocks")
	//			MouseControlSystem::triggerBox.triggerType = TriggerBoxComponent::TriggerType::PUSH_ROCKS;
	//		else if (current_trigger == "collect_item")
	//			MouseControlSystem::triggerBox.triggerType = TriggerBoxComponent::TriggerType::COLLECT_ITEM;
	//		else if (current_trigger == "bomb_secret")
	//			MouseControlSystem::triggerBox.triggerType = TriggerBoxComponent::TriggerType::BOMB_SECRET;
	//		else if (current_trigger == "locked_door")
	//			MouseControlSystem::triggerBox.triggerType = TriggerBoxComponent::TriggerType::LOCKED_DOOR;
	//		else if (current_trigger == "hidden_object")
	//			MouseControlSystem::triggerBox.triggerType = TriggerBoxComponent::TriggerType::HIDDEN_OBJECT;
	//		else
	//			MouseControlSystem::triggerBox.triggerType = TriggerBoxComponent::TriggerType::NO_TRIGGER;
	//	}

	//	ImGui::End();
	//}
}
void RenderEditorGUISystem::ColliderProperties()
{
	if ( ImGui::Begin( "Collider Properties" ) )
	{
		ShowMenuTile();

		if ( ImGui::Button( "Set Collider Attributes" ) )
		{
			m_MouseControl.SetTransformScale( m_ScaleX, m_ScaleY );
			m_MouseControl.SetBoxCollider( BoxColliderComponent{
				m_BoxColliderWidth, m_BoxColliderHeight, glm::vec2{ m_BoxColliderOffsetX, m_BoxColliderOffsetY } } );
			m_MouseControl.SetMouseRect( m_MouseRectX, m_MouseRectY );
		}

		ImGui::End();
	}
}
void RenderEditorGUISystem::ImageBox( const std::unique_ptr<AssetManager>& assetManager, SDL_Renderer* renderer )
{
	// Set the image Width/Height based on the obtain values from the file and the current m_Game scale
	if ( ImGui::Begin( "Texture" ), &m_bMouseImageLoaded )
	{
		int my_image_width = m_ImageWidth * 2;
		int my_image_height = m_ImageHeight * 2;

		ImGui::Image( assetManager->GetTexture( m_MouseControl.GetSpriteComponent().assetID ),
					  ImVec2( my_image_width, my_image_height ) );

		// This is for the mouse Offset for the image --> Still needs to be refined --> Currently Hackish
		// Logger::Log("x: " + std::to_string(ImGui::GetMousePos().x) + "y: " + std::to_string(ImGui::GetMousePos().y));
		int mouseposX = ImGui::GetMousePos().x - ImGui::GetWindowPos().x;
		int mouseposY = ImGui::GetMousePos().y - ImGui::GetWindowPos().y - 26; // It's a hack I know

		int m_MouseRectX{ m_MouseControl.GetMouseRectX() };
		int m_MouseRectY{ m_MouseControl.GetMouseRectY() };

		// Clamp --> To make sure that we are not dividing by zero value!
		if ( my_image_width > 0 && my_image_height > 0 && m_MouseRectX > 0 && m_MouseRectY > 0 )
		{
			int rows = ( my_image_height / ( m_MouseRectY * 2 ) );
			int col = ( my_image_width / ( m_MouseRectX * 2 ) );

			for ( int i = 0; i < col; i++ )
			{
				for ( int j = 0; j < rows; j++ )
				{
					auto draw_list = ImGui::GetWindowDrawList();
					// Check to see if we are in the area of the desired 2D tile/object
					if ( ( mouseposX >= ( my_image_width / col ) * i &&
						   mouseposX <= ( my_image_width / col ) + ( ( my_image_width / col ) * i ) ) &&
						 ( mouseposY >= ( my_image_height / rows ) * j &&
						   mouseposY <= ( my_image_height / rows ) + ( ( my_image_height / rows ) * j ) ) )
					{
						if ( ImGui::IsItemHovered() )
						{
							if ( ImGui::IsMouseClicked( 0 ) )
							{
								m_MouseControl.SetSpriteSrcRect( i * m_MouseRectX, j * m_MouseRectY );
							}
						}
					}
				}
			}
		}
	}
	ImGui::End();
}

bool RenderEditorGUISystem::LoadNewImage( const std::unique_ptr<AssetManager>& assetManager, SDL_Renderer* renderer,
										  std::string& image_id, int& image_width, int& image_height )
{
	FileDialogs dialog;
	// Open a file dialog and assign the return directory to the m_sImageName
	std::string image_name = dialog.OpenImage();
	// m_sImageName = dialog.OpenImage();

	m_MouseControl.SetImageName( image_name );
	Logger::Log( m_MouseControl.GetImageName() );

	// Strip the image name of the directory strings leaving just the name
	image_id = m_Loader.SetName( image_name );

	// Check to see if the String is Empty!!
	if ( !image_name.empty() )
	{
		if ( !assetManager->HasTexture( image_id ) )
		{
			Logger::Err( "Added new image with the ID  -- " + image_id );
			assetManager->AddTextures( renderer, image_id, m_MouseControl.GetImageName() );
			Logger::Err( m_MouseControl.GetImageName() );
		}
		else
		{
			Logger::Err( "This Image ID Already Exists! -- " + image_id );
			assetManager->GetTexture( image_id );
		}
		imageLoaded = true;
	}
	else
	{
		Logger::Err( "Did Not Load Image" );
		return false;
	}

	// Get the image width and Height!! --> TEST
	if ( SDL_QueryTexture( assetManager->GetTexture( image_id ), NULL, NULL, &image_width, &image_height ) == 0 )
		return true;
	else
	{
		const char* message = SDL_GetError();
		Logger::Err( "EDITOR: __FUNC: LoadImage: " + std::string( message ) );
		return false;
	}
}

void RenderEditorGUISystem::ComboLoop( std::string label, std::string& current_item, const std::string handinArray[],
									   int size )
{
	if ( ImGui::BeginCombo( label.c_str(), current_item.c_str() ) )
	{
		for ( int n = 0; n < size; n++ )
		{
			bool is_selected = ( current_item == handinArray[ n ] );
			if ( ImGui::Selectable( handinArray[ n ].c_str(), is_selected ) )
			{
				current_item = handinArray[ n ];
			}
			if ( is_selected )
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
	io.KeyMap[ ImGuiKey_Backspace ] = SDL_SCANCODE_BACKSPACE;
	io.KeyMap[ ImGuiKey_Enter ] = SDL_SCANCODE_RETURN;
	io.KeyMap[ ImGuiKey_Space ] = SDL_SCANCODE_SPACE;
	io.KeyMap[ ImGuiKey_Tab ] = SDL_SCANCODE_TAB;
	io.KeyMap[ ImGuiKey_LeftArrow ] = SDL_SCANCODE_LEFT;
	io.KeyMap[ ImGuiKey_RightArrow ] = SDL_SCANCODE_RIGHT;
	io.KeyMap[ ImGuiKey_UpArrow ] = SDL_SCANCODE_UP;
	io.KeyMap[ ImGuiKey_DownArrow ] = SDL_SCANCODE_DOWN;
	io.KeyMap[ ImGuiKey_Home ] = SDL_SCANCODE_HOME;
	io.KeyMap[ ImGuiKey_End ] = SDL_SCANCODE_END;
	io.KeyMap[ ImGuiKey_Escape ] = SDL_SCANCODE_ESCAPE;

	io.KeyMap[ ImGuiKey_A ] = SDL_SCANCODE_A;
	io.KeyMap[ ImGuiKey_C ] = SDL_SCANCODE_C;
	io.KeyMap[ ImGuiKey_V ] = SDL_SCANCODE_V;
	io.KeyMap[ ImGuiKey_X ] = SDL_SCANCODE_X;
	io.KeyMap[ ImGuiKey_Y ] = SDL_SCANCODE_Y;
	io.KeyMap[ ImGuiKey_Z ] = SDL_SCANCODE_Z;
}

void RenderEditorGUISystem::CanvasSize()
{
	static int numTilesWidth = 1;
	static int numTilesHeight = 1;
	static int spriteSize = 16;
	if ( ImGui::Begin( "Canvas Size" ) )
	{
		ImGui::InputInt( "Num Tiles Width", &numTilesWidth );
		ImGui::InputInt( "Num Tiles Height", &numTilesHeight );
		if ( ImGui::SliderInt( "Sprite Size", &spriteSize, 16, 128 ) )
		{
		}

		if ( ImGui::Button( "Set Canvas" ) )
		{
			m_bShowCanvasProperties = false;
		}

		ImGui::SameLine();

		if ( ImGui::Button( "Cancel" ) )
			m_bShowCanvasProperties = false;

		ImGui::End();
	}
}

bool RenderEditorGUISystem::LoadEntireMapSprite( const std::unique_ptr<AssetManager>& assetManager,
												 SDL_Renderer* renderer )
{
	std::string mapAssetID = "";
	// Remove all Entities from the Editor
	Registry::Instance().GetSystem<RenderEditorSystem>().OnExit();
	Registry::Instance().GetSystem<RenderCollisionSystem>().OnExit();

	// Load the image to the asset manager and query the width and height
	if ( !LoadNewImage( assetManager, renderer, mapAssetID, m_MapWidth, m_MapHeight ) )
	{
		Logger::Err( "EDITOR: __FUNC: LoadEntireMapSprite: Could not load the Map sprite!" );
		return false;
	}

	// If the image has been loaded successfully, Create a new map entity
	auto newMap = Registry::Instance().CreateEntity();
	newMap.Group( "map" );
	newMap.AddComponent<TransformComponent>( glm::vec2( 0 ), glm::vec2( m_ScaleX, m_ScaleY ), 0 );
	newMap.AddComponent<SpriteComponent>( SpriteComponent{ .assetID = mapAssetID,
														   .width = m_MapWidth,
														   .height = m_MapHeight,
														   .layer = 0,
														   .bIsFixed = false,
														   .srcRect = SDL_Rect{ 0, 0, m_MapWidth, m_MapHeight } } );

	newMap.AddComponent<EditorComponent>();

	Logger::Log( "Created Map!!" );
	return true;
}

void RenderEditorGUISystem::ShowMapLoaderWindow( const std::unique_ptr<AssetManager>& assetManager,
												 SDL_Renderer* renderer )
{
	static bool mapLoadPopUp = false;

	if ( ImGui::Begin( "Load Map Sprite" ) )
	{
		ImGui::SliderInt( "X Scale", &m_ScaleX, 1, 10 );
		ImGui::SliderInt( "Y Scale", &m_ScaleY, 1, 10 );

		if ( ImGui::Button( "Next" ) )
		{
			mapLoadPopUp = true;
		}
		ImGui::SameLine();
		if ( ImGui::Button( "cancel" ) )
		{
			m_bShowLoadMap = false;
		}
		ImGui::End();
	}

	if ( mapLoadPopUp )
	{
		if ( ImGui::Begin( "Load" ), &mapLoadPopUp )
		{
			ImGui::Text( "This will erase all of you have done so far!" );
			ImGui::Text( "Are you sure?" );

			if ( ImGui::Button( "Yes" ) )
			{
				LoadEntireMapSprite( assetManager, renderer );
				mapLoadPopUp = false;
				m_bShowLoadMap = false;
			}

			ImGui::SameLine();

			// Close the pop-up and go back to the Load window
			if ( ImGui::Button( "Cancel" ) )
				mapLoadPopUp = false;

			ImGui::End();
		}
	}
}