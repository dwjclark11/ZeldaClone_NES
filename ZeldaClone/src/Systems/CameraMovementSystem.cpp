#include "Systems/CameraMovementSystem.h"
#include "Components/BoxColliderComponent.h"
#include "Components/TransformComponent.h"
#include "Components/CameraFollowComponent.h"
#include "Components/RigidBodyComponent.h"
#include "Components/SpriteComponent.h"
#include "Components/PlayerComponent.h"
#include "Game/Game.h"
#include "Game/Player.h"
#include "Utilities/Utility.h"
#include "Utilities/GameData.h"
#include "Utilities/Camera.h"
#include <chrono>
#include <thread>
#include <SDL.h>

CameraMovementSystem::CameraMovementSystem()
	: m_Game{ Game::Instance() }
	, m_GameData{ GameData::GetInstance() }
	, m_CamSpeed{ 640 }
	, m_PrevCamX{ 7168 }
	, m_PrevCamY{ 4448 }
	, m_bStartEast{ false }
	, m_bStartWest{ false }
	, m_bStartNorth{ false }
	, m_bStartSouth{ false }
{
	RequiredComponent<CameraFollowComponent>();
	RequiredComponent<TransformComponent>();
}

void CameraMovementSystem::UpdatePlayerCam( Camera& camera, const float& dt )
{
	int window_width = m_Game.GetWindowWidth();
	int window_height = m_Game.GetWindowHeight();
	int level_width = m_GameData.GetLevelWidth();
	int level_height = m_GameData.GetLevelHeight();

	const auto& player = m_Game.GetPlayer();
	const auto& playerEnt = player->GetPlayer();
	const auto& transform = playerEnt.GetComponent<TransformComponent>();

	// Camera data
	const auto& cameraPos = camera.GetCameraPos();
	const auto& cameraWidth = camera.GetCameraWidth();
	const auto& cameraHeight = camera.GetCameraHeight();
	const auto& prevCamX = camera.GetPrevCamX();
	const auto& prevCamY = camera.GetPrevCamY();
	int new_cam_x = cameraPos.x;
	int new_cam_y = cameraPos.y;

	if ( !m_bStartWest && !m_bStartEast && !m_bStartNorth && !m_bStartSouth )
	{
		camera.SetCameraMoving( false );

		if ( transform.position.x - new_cam_x <= 0 )
			m_bStartWest = true;
		else if ( transform.position.x - new_cam_x >= window_width )
			m_bStartEast = true;
		else if ( transform.position.y - new_cam_y <= 256 )
			m_bStartNorth = true;
		else if ( transform.position.y - new_cam_y >= window_height )
			m_bStartSouth = true;
	}
	else
	{
		camera.SetCameraMoving( true );
	}

	if ( m_bStartNorth )
	{
		new_cam_y -= m_CamSpeed * dt;

		int newCamY = prevCamY - window_height + HUD_HEIGHT;

		if ( newCamY <= 0 )
			newCamY = 0;

		if ( new_cam_y <= newCamY )
		{
			new_cam_y = newCamY;
			camera.SetPrevCamPosition( prevCamX, newCamY );
			m_bStartNorth = false;
		}
	}
	else if ( m_bStartSouth )
	{
		new_cam_y += m_CamSpeed * dt;

		int newCamY = prevCamY + window_height - HUD_HEIGHT;

		if ( newCamY >= level_height )
			newCamY = level_height;

		if ( new_cam_y >= newCamY )
		{
			new_cam_y = newCamY;
			camera.SetPrevCamPosition( prevCamX, newCamY );
			m_bStartSouth = false;
		}
	}
	else if ( m_bStartWest )
	{
		new_cam_x -= m_CamSpeed * dt;

		int newCamX = prevCamX - window_width;

		if ( newCamX <= 0 )
			newCamX = 0;

		if ( new_cam_x <= newCamX )
		{
			new_cam_x = newCamX;
			camera.SetPrevCamPosition( newCamX, prevCamY );
			m_bStartWest = false;
		}
	}
	else if ( m_bStartEast )
	{
		new_cam_x += m_CamSpeed * dt;

		int newCamX = prevCamX + window_width;

		if ( newCamX >= level_width )
			newCamX = level_width;

		if ( new_cam_x >= newCamX )
		{
			new_cam_x = newCamX;
			camera.SetPrevCamPosition( newCamX, prevCamY );
			m_bStartEast = false;
		}
	}

	// Clamp the camera position
	if ( new_cam_x <= 0 )
		new_cam_x = 0;
	if ( new_cam_y <= -256 )
		new_cam_y = -256;
	if ( new_cam_x + cameraWidth >= level_width - cameraWidth )
		new_cam_x = level_width - cameraWidth;
	if ( new_cam_y + cameraHeight >= level_height )
		new_cam_y = level_height - cameraHeight;

	camera.SetCameraPosition( new_cam_x, new_cam_y );
}

void CameraMovementSystem::UpdateEditorCam( Camera& camera, const float& dt )
{
	const auto& cameraPos = camera.GetCameraPos();
	const auto& cameraWidth = camera.GetCameraWidth();
	const auto& cameraHeight = camera.GetCameraHeight();

	// Keep the camera rectange view inside the screen limits
	int new_cam_x = cameraPos.x;
	int new_cam_y = cameraPos.y;

	// If the camera.x is < 0 make it 0
	new_cam_x = new_cam_x < -100 ? 100 : new_cam_x;
	new_cam_y = new_cam_y < -100 ? 100 : new_cam_y;

	// if Camera.x is > than the width of the screen
	new_cam_x = ( new_cam_x + cameraWidth > 100000 ) ? 100000 - cameraWidth : new_cam_x;
	new_cam_y = ( new_cam_y + cameraHeight > 100000 ) ? 100000 - cameraHeight : new_cam_y;

	camera.SetCameraPosition( new_cam_x, new_cam_y );
}
