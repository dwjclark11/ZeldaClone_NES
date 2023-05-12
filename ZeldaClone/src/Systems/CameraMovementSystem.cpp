#include "CameraMovementSystem.h"
#include "../Components/BoxColliderComponent.h"
#include "../Components/TransformComponent.h"
#include "../Components/CameraFollowComponent.h"
#include "../Components/RigidBodyComponent.h"
#include "../Components/SpriteComponent.h"
#include "../Components/PlayerComponent.h"
#include "../Game/Game.h"
#include "../Game/Player.h"
#include "../Utilities/GameData.h"
#include "../Utilities/Camera.h"
#include <chrono>
#include <thread>
#include <SDL.h>


CameraMovementSystem::CameraMovementSystem()
	: game(Game::Instance())
	, gameData(GameData::GetInstance())
	, m_CamSpeed{640}
	, m_PrevCamX{ 7168 }
	, m_PrevCamY{ 4448 }
	, m_bStartEast{false}
	, m_bStartWest{false}
	, m_bStartNorth{false}
	, m_bStartSouth{false}
{
	RequiredComponent<CameraFollowComponent>();
	RequiredComponent<TransformComponent>();

	// Initialize all the member variables
	first = false;
	titleScreenScroll_1 = false;
	titleScreenScroll_2 = false;
	titleScreenScroll_3 = false;
	timerStart = false;
	titleScreenScroll_Finished = false;
	scrollTimer = 0;

}

void CameraMovementSystem::UpdateTitleCam(Camera& camera, const float& dt)
{
	if (!timer.isStarted())
		timer.Start();
	// Camera data
	const auto& cameraPos = camera.GetCameraPos();
	const auto& cameraWidth = camera.GetCameraWidth();
	const auto& cameraHeight = camera.GetCameraHeight();
	int new_cam_x = cameraPos.x;
	int new_cam_y = cameraPos.y;
	
	/*
		This part of the system controls the scroll of the camera during the title screen animations
		The camera will wait for a certain amount of time and then the screen should appear to scroll
		in a upwards motion, pausing for 3 seconds at 960
	*/
	// Where to place the Origin of the camera during the first entrance of the state
	if (!first)
	{
		camera.SetCameraPosition(0, 0);
		first = true;
	}

	if (!titleScreenScroll_1 && !titleScreenScroll_2 && !camera.IsCameraMoving())
	{
		titleScreenScroll_1 = true;
		camera.SetCameraMoving(true);
	}

	if (titleScreenScroll_1)
	{
		new_cam_y += 100 * dt;
		std::this_thread::sleep_for(std::chrono::microseconds(5));

		if (new_cam_y >= 960)
		{
			new_cam_y = 960;
			titleScreenScroll_1 = false;
			camera.SetCameraMoving(true);
		}
	}

	if (!titleScreenScroll_1 && !titleScreenScroll_2 && !titleScreenScroll_3)
	{
		if (timer.GetTicks() > 18000)
			titleScreenScroll_2 = true;
	}

	if (titleScreenScroll_2)
	{
		new_cam_y += 100 * dt;

		if (new_cam_y >= 6720)
		{
			new_cam_y = 6720;
			titleScreenScroll_3 = true;
		}
	}
	if (titleScreenScroll_3)
	{
		//if (timer.isStarted()) Logger::Log("ticks: " + std::to_string(timer.GetTicks()));
		if (timer.GetTicks() > 120000)
		{
			titleScreenScroll_Finished = true;
			titleScreenScroll_3 = false;
			titleScreenScroll_2 = false;
			titleScreenScroll_1 = false;
			timer.Stop();
			new_cam_y = 0;
		}
	}
	// Keep the camera rectange view inside the screen limits
	new_cam_x = new_cam_x < 0 ? 0 : new_cam_x; // If the camera.x is < 0 make it 0
	new_cam_y = new_cam_y < 0 ? 0 : new_cam_y;

	camera.SetCameraPosition(new_cam_x, new_cam_y);
	//camera.SetCameraMoving(false);
}

void CameraMovementSystem::UpdatePlayerCam(Camera& camera, const float& dt)
{
	int window_width = game.GetWindowWidth();
	int window_height = game.GetWindowHeight();
	int level_width = gameData.GetLevelWidth();
	int level_height = gameData.GetLevelHeight();

	const auto& player = game.GetPlayer();
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

	if (!m_bStartWest && !m_bStartEast && !m_bStartNorth && !m_bStartSouth)
	{
		camera.SetCameraMoving(false);

		if (transform.position.x - new_cam_x <= 0)
			m_bStartWest = true;
		else if (transform.position.x - new_cam_x >= window_width)
			m_bStartEast = true;
		else if (transform.position.y - new_cam_y <= 256)
			m_bStartNorth = true;
		else if (transform.position.y - new_cam_y >= window_height)
			m_bStartSouth = true;
	}
	else
	{
		camera.SetCameraMoving(true);
	}

	//Logger::Log("CamY: " + std::to_string(camera.y));
	if (m_bStartNorth)
	{
		new_cam_y -= m_CamSpeed * dt;

		int newCamY = prevCamY - window_height + 288;

		if (newCamY <= 0) 
			newCamY = 0;

		if (new_cam_y <= newCamY)
		{
			new_cam_y = newCamY;
			camera.SetPrevCamPosition(prevCamX, newCamY);
			m_bStartNorth = false;
		}
	}
	else if (m_bStartSouth)
	{
		new_cam_y += m_CamSpeed * dt;

		int newCamY = prevCamY + window_height - 288;

		if (newCamY >= level_height)
			newCamY = level_height;

		if (new_cam_y >= newCamY)
		{
			new_cam_y = newCamY;
			camera.SetPrevCamPosition(prevCamX, newCamY);
			m_bStartSouth = false;
		}
	}
	else if (m_bStartWest)
	{
		new_cam_x -= m_CamSpeed * dt;

		int newCamX = prevCamX - window_width;

		if (newCamX <= 0)
			newCamX = 0;

		if (new_cam_x <= newCamX)
		{
			new_cam_x = newCamX;
			camera.SetPrevCamPosition(newCamX, prevCamY);
			m_bStartWest = false;
		}
	}
	else if (m_bStartEast)
	{
		new_cam_x += m_CamSpeed * dt;

		int newCamX = prevCamX + window_width;

		if (newCamX >= level_width)
			newCamX = level_width;

		if (new_cam_x >= newCamX)
		{
			new_cam_x = newCamX;
			camera.SetPrevCamPosition(newCamX, prevCamY);
			m_bStartEast = false;
		}
	}

	// Clamp the camera position
	if (new_cam_x <= 0) new_cam_x = 0;
	if (new_cam_y <= -256) new_cam_y = -256;
	if (new_cam_x + cameraWidth >= level_width - cameraWidth) new_cam_x = level_width - cameraWidth;
	if (new_cam_y + cameraHeight >= level_height) new_cam_y = level_height - cameraHeight;

	camera.SetCameraPosition(new_cam_x, new_cam_y);
}

void CameraMovementSystem::UpdateEditorCam(Camera& camera, const float& dt)
{
	const auto& cameraPos = camera.GetCameraPos();
	const auto& cameraWidth = camera.GetCameraWidth();
	const auto& cameraHeight = camera.GetCameraHeight();
	
	if (!first)
	{
		camera.SetCameraPosition(0, 0);
		first = true;
	}

	// Keep the camera rectange view inside the screen limits
	int new_cam_x = cameraPos.x;
	int new_cam_y = cameraPos.y;

	// If the camera.x is < 0 make it 0
	new_cam_x = new_cam_x < -100 ? 100 : new_cam_x; 
	new_cam_y = new_cam_y < -100 ? 100 : new_cam_y;
	
	// if Camera.x is > than the width of the screen
	new_cam_x = (new_cam_x + cameraWidth > 100000) ? 100000 - cameraWidth : new_cam_x; 
	new_cam_y = (new_cam_y + cameraHeight > 100000) ? 100000 - cameraHeight : new_cam_y;

	camera.SetCameraPosition(new_cam_x, new_cam_y);
}

void CameraMovementSystem::OnExit()
{
	first = false;
	scrollTimer = 0;
}
