#include "Utilities/Timer.h"
#include "Logger/Logger.h"

#include <SDL.h>

using namespace std::literals::chrono_literals;

Timer::Timer()
	: m_StartTicks( 0 )
	, m_PausedTicks( 0 )
	, m_bPaused( false )
	, m_bStarted( false )
{
	// Timer Created
}

// Various clock actions
void Timer::Start()
{
	// Logger::Log("Timer Started");
	//  Start the timer
	m_bStarted = true;
	// Unpause if paused
	m_bPaused = false;

	// Get the current clock time
	m_StartTicks = SDL_GetTicks();
	m_PausedTicks = 0;
}

void Timer::Stop()
{
	// Stop the timer
	m_bStarted = false;

	// UnPause the timer
	m_bPaused = false;
	// Clear the tick variables
	m_StartTicks = 0;
	m_PausedTicks = 0;
}

void Timer::Pause()
{
	// If the timer is running and isn't already paused
	if ( m_bStarted && !m_bPaused )
	{
		// Pause the timer
		m_bPaused = true;
		// Calculated the paused ticks
		m_PausedTicks = SDL_GetTicks() - m_StartTicks;
		m_StartTicks = 0;
	}
}

void Timer::UnPause()
{
	// If the timer is running and paused;
	if ( m_bStarted && m_bPaused )
	{
		// Unpause the timer
		m_bPaused = false;
		// Reset the Start ticks
		m_StartTicks = SDL_GetTicks() - m_PausedTicks;
		// Reset the paused ticks
		m_PausedTicks = 0;
	}
}

// Get's the timer's time
Uint32 Timer::GetTicks()
{
	// The actual timer time
	Uint32 time = 0;
	// If the timer is running
	if ( m_bStarted )
	{
		// If the timer is paused
		if ( m_bPaused )
		{
			// Return the number of ticks when the timer was paused
			time = m_PausedTicks;
		}
		else
		{
			// Return the currnet time minus the start time
			time = SDL_GetTicks() - m_StartTicks;
		}
	}
	return time;
}

// Checks the status of the timer
bool Timer::isStarted()
{
	return m_bStarted;
}

bool Timer::isPaused()
{
	return m_bPaused;
}

void Timer::Update()
{
	auto start = std::chrono::high_resolution_clock::now();

	std::this_thread::sleep_for( 1s );

	auto end = std::chrono::high_resolution_clock::now();

	std::chrono::duration<float> duration = end - start;
	std::cout << duration.count() << std::endl;
	std::cin.get();
}
