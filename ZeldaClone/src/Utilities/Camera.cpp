#include "Utilities/Camera.h"
#include "Logger/Logger.h"
#include "Game/Game.h"

Camera::Camera()
	: Camera( 0, 0, 1024, 960 )
{
	Logger::Log( "Camera Constructor called!" );
}

Camera::Camera( int x, int y, int width, int height )
	: m_Camera{ .x = x, .y = y, .w = width, .h = height }
	, m_LeftRect{ 0, 256, 512, 704 }
	, m_RightRect{ 512, 256, 512, 704 }
	, m_ScreenFlash{ 0, 256, 1024, 704 }
	, m_FadeAlpha{ 255 }
	, m_Zoom{ 1.f }
	, m_bCameraMoving{ false }
	, m_bStartFadeIn{ false }
	, m_bStartFadeOut{ false }
	, m_bStartOpen{ false }
	, m_bStartClose{ false }
	, m_bCurtainOpen{ false }
	, m_bCurtainClosed{ false }
	, m_bFadeFinished{ true }
	, m_bStartScreenFlash{ false }
	, m_FlashIndex{ 0 }
	, m_PrevCamX{ 7168 }
	, m_PrevCamY{ 4448 }
	, m_FlashOnTimer{}
	, m_FlashOffTimer{}
{
}

void Camera::SetCameraPosFromSceneChange( int x, int y )
{
	if ( x < 1 || y < 1 )
	{
		Logger::Err( "Failed to set Camera Pos during scene change! - x or y must be >= 1!" );
		assert( true && "Failed to set Camera Pos during scene change! - x or y must be >= 1!" );
		return;
	}

	m_Camera.x = m_Camera.w * ( x - 1 );
	m_Camera.y = m_Camera.h * ( y - 1 );
}

void Camera::FadeScreen()
{
	if ( m_bStartFadeIn )
	{
		if ( m_FadeAlpha < 255 )
		{
			m_FadeAlpha += 15;
		}
		else
		{
			m_bStartFadeIn = false;
		}
	}

	if ( m_bStartFadeOut )
	{
		if ( m_FadeAlpha > 0 )
		{
			m_FadeAlpha -= 15;
		}
		else
		{
			m_bStartFadeOut = false;
		}
	}
}

void Camera::UpdateCurtain()
{
	auto& game = Game::Instance();
	if ( !m_bStartClose && !m_bStartOpen )
		return;

	// Render all HUD objects
	SDL_SetRenderDrawColor( game.GetRenderer(), 0, 0, 0, 255 );
	SDL_RenderFillRect( game.GetRenderer(), &m_LeftRect );
	SDL_RenderDrawRect( game.GetRenderer(), &m_LeftRect );
	SDL_SetRenderDrawColor( game.GetRenderer(), 0, 0, 0, 255 );

	// Render all HUD objects
	SDL_SetRenderDrawColor( game.GetRenderer(), 0, 0, 0, 255 );
	SDL_RenderFillRect( game.GetRenderer(), &m_RightRect );
	SDL_RenderDrawRect( game.GetRenderer(), &m_RightRect );
	SDL_SetRenderDrawColor( game.GetRenderer(), 0, 0, 0, 255 );

	if ( m_bStartOpen )
	{
		if ( m_LeftRect.x > -512 && m_RightRect.x < 1536 )
		{
			m_LeftRect.x -= 16;
			m_RightRect.x += 16;
			std::this_thread::sleep_for( std::chrono::microseconds( 5 ) );
		}
		else
		{
			m_bStartOpen = false;
			m_bCurtainOpen = true;
			m_bCurtainClosed = false;
		}
	}
	else if ( m_bStartClose )
	{
		if ( m_LeftRect.x != 0 && m_RightRect.x != 512 )
		{
			m_LeftRect.x += 16;
			m_RightRect.x -= 16;
			std::this_thread::sleep_for( std::chrono::microseconds( 5 ) );
		}
		else
		{
			m_bStartClose = false;
			m_bCurtainOpen = false;
			m_bCurtainClosed = true;
		}
	}
}

void Camera::UpdateScreenFlash()
{
	auto& game = Game::Instance();

	if ( m_bStartScreenFlash )
	{
		if ( !m_FlashOnTimer.isStarted() && !m_FlashOffTimer.isStarted() )
			m_FlashOnTimer.Start();

		if ( m_FlashOnTimer.isStarted() )
		{
			// Render all HUD objects
			SDL_SetRenderDrawColor( game.GetRenderer(), 255, 255, 255, 225 );
			SDL_RenderFillRect( game.GetRenderer(), &m_ScreenFlash );
			SDL_RenderDrawRect( game.GetRenderer(), &m_ScreenFlash );
			SDL_SetRenderDrawColor( game.GetRenderer(), 255, 255, 255, 225 );
		}

		if ( m_FlashOnTimer.GetTicks() >= 100 && !m_FlashOffTimer.isStarted() )
		{
			m_FlashOffTimer.Start();
			m_FlashOnTimer.Stop();
		}

		if ( m_FlashOffTimer.GetTicks() >= 100 && !m_FlashOnTimer.isStarted() )
		{
			m_FlashOnTimer.Start();
			m_FlashOffTimer.Stop();
			m_FlashIndex++;
		}

		if ( m_FlashIndex >= 15 )
		{
			m_bStartScreenFlash = false;
			m_FlashIndex = 0;
			m_FlashOnTimer.Stop();
			m_FlashOffTimer.Stop();
		}
	}
}