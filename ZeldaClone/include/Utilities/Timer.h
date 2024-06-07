#pragma once
#include <chrono>
#include <thread>
#include <SDL.h>

class Timer
{
  public:
	Timer();

	// Various clock actions
	void Start();
	void Stop();
	void Pause();
	void UnPause();

	// Get's the timer's time
	Uint32 GetTicks();

	// Checks the status of the timer
	bool isStarted();
	bool isPaused();
	void Update();

  private:
	// The clock time when the timer started
	Uint32 m_StartTicks;
	// The ticks stored when the timer was paused
	Uint32 m_PausedTicks;
	// The Timer Status
	bool m_bPaused;
	bool m_bStarted;
};