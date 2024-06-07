#pragma once
#include <SDL.h>
#include <sol/sol.hpp>
#include <glm/glm.hpp>
#include "Utilities/Timer.h"

/*
 *  Simple Camera class that gets bound to a lua usertype. This allows the user to
 *  Get a reference to the main engine camera and move and use the camera accoriding
 *  to their needs in the lua script. Currently the camera type only allows the Lua user
 *  to get an instance of the main camera and manipulate that. I might create another user
 *  type to allow for multiple camera creation from the lua script; however, I currently do
 *  no see the need for it.
 */
class Camera
{
  private:
	/*
	 *  The actual camera is just an SDL_Rect that is moved around and used to
	 *  offset any entities in the game world that are not fixed
	 */
	SDL_Rect m_Camera, m_LeftRect, m_RightRect, m_ScreenFlash;
	Uint8 m_FadeAlpha;
	float m_Zoom;
	bool m_bCameraMoving, m_bStartFadeIn, m_bStartFadeOut, m_bFadeFinished;
	bool m_bStartOpen, m_bStartClose, m_bStartScreenFlash, m_bCurtainClosed, m_bCurtainOpen;
	int m_FlashIndex, m_PrevCamX, m_PrevCamY;
	Timer m_FlashOnTimer, m_FlashOffTimer;

  public:
	Camera();
	Camera( int x, int y, int width, int height );
	~Camera() = default;

	/*
	 *  GetCameraWidth() returns the width of the SDL_Rect mCamera
	 *  as a const int&
	 */
	inline const int& GetCameraWidth() const { return m_Camera.w; }

	/*
	 *  GetCameraHeight() returns the height of the SDL_Rect mCamera
	 *  as a const int&
	 */
	inline const int& GetCameraHeight() const { return m_Camera.h; }

	/*
	 *  GetCameraPosition() returns a glm::vec2 to give both
	 *  your x and y coordinates of the mCamera SDL_Rect
	 */
	inline glm::vec2 GetCameraPos() const { return glm::vec2( m_Camera.x, m_Camera.y ); }
	/*
	 *  GetCameraRect() returns a reference to the actual
	 *  mCamera SDL_Rect
	 */
	inline SDL_Rect& GetCameraRect() { return m_Camera; }

	/*
	 *  Sets the width of the camera
	 *  @param - It takes an integer value width
	 */
	inline void SetCameraWidth( int width ) { m_Camera.w = width; }

	/*
	 *  Sets the height of the camera
	 *  @param - It takes an integer value height
	 */
	inline void SetCameraHeight( int height ) { m_Camera.h = height; }

	/*
	 *  This function allows the user to adjust or set the camera position
	 *  @param - It taks two integer values for the x and y coordinates
	 */
	inline void SetCameraPosition( int x, int y )
	{
		m_Camera.x = x;
		m_Camera.y = y;
	}
	void SetCameraPosFromSceneChange( int x, int y );

	/*
	 *	GetZoom()
	 *	@return float mZoom -- Returns the zoom of the camera.
	 */
	inline const float GetZoom() const { return m_Zoom; }

	/*
	 *	SetZoom()
	 *	@param float zoom -- Sets the zoom of the camera --> Affects all entities / Shapes etc
	 *	Unless those objects are fixed. Zoom is used in the draw functions
	 */
	inline void SetZoom( float zoom ) { m_Zoom = zoom; }
	void FadeScreen();
	void UpdateCurtain();
	void UpdateScreenFlash();

	inline void StartCurtainClose() { m_bStartClose = true; }
	inline void StartCurtainOpen() { m_bStartOpen = true; }
	inline void StartScreenFlash() { m_bStartScreenFlash = true; }

	inline const bool CurtainOpen() const { return m_bCurtainOpen; }
	inline const bool CurtainClosed() const { return m_bCurtainClosed; }

	inline const bool IsCameraMoving() const { return m_bCameraMoving; }
	inline void SetCameraMoving( bool moving ) { m_bCameraMoving = moving; }

	inline void SetFadeAlpha( Uint8 alpha ) { m_FadeAlpha = alpha; }
	inline const Uint8& GetFadeAlpha() const { return m_FadeAlpha; }

	inline const bool FadeInStarted() const { return m_bStartFadeIn; }
	inline void StartFadeIn( bool fade ) { m_bStartFadeIn = fade; }

	inline const bool FadeOutStarted() const { return m_bStartFadeOut; }
	inline void StartFadeOut( bool fade ) { m_bStartFadeOut = fade; }

	inline const bool FadeFinished() const { return m_bFadeFinished; }
	inline void SetFadeFinished( bool finished ) { m_bFadeFinished = finished; }

	inline void SetPrevCamPosition( int x, int y )
	{
		m_PrevCamX = x;
		m_PrevCamY = y;
	}
	inline const int GetPrevCamX() const { return m_PrevCamX; }
	inline const int GetPrevCamY() const { return m_PrevCamY; }
};
