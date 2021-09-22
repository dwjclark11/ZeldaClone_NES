#include "Timer.h"
#include <SDL.h>
#include <iostream>
#include "../Logger/Logger.h"
using namespace std::literals::chrono_literals; // Needed to get the 's' in 1s for seconds


Timer::Timer() 
	: mStartTicks(0)
	, mPausedTicks(0)
	, mPaused(false)
	, mStarted(false)
{
	// Timer Created
}
	
// Various clock actions
void Timer::Start()
{
	//Logger::Log("Timer Started");
	// Start the timer
	mStarted = true;
	// Unpause if paused
	mPaused = false;
	
	// Get the current clock time
	mStartTicks = SDL_GetTicks();
	mPausedTicks = 0;
}

void Timer::Stop()
{
	// Stop the timer
	mStarted = false;
	
	// UnPause the timer
	mPaused = false;
	// Clear the tick variables
	mStartTicks = 0;
	mPausedTicks = 0;
}

void Timer::Pause()
{
	// If the timer is running and isn't already paused
	if (mStarted && !mPaused)
	{
		// Pause the timer
		mPaused = true;
		// Calculated the paused ticks
		mPausedTicks = SDL_GetTicks() - mStartTicks;
		mStartTicks = 0;
	}
}
	
void Timer::UnPause()
{
	// If the timer is running and paused;
	if (mStarted && mPaused)
	{
		// Unpause the timer
		mPaused = false;
		// Reset the Start ticks
		mStartTicks = SDL_GetTicks() - mPausedTicks;
		// Reset the paused ticks
		mPausedTicks = 0;
	}
}

// Get's the timer's time
Uint32 Timer::GetTicks()
{
	// The actual timer time
	Uint32 time = 0;
	// If the timer is running
	if (mStarted)
	{
		// If the timer is paused
		if (mPaused)
		{
			// Return the number of ticks when the timer was paused
			time = mPausedTicks;
		}
		else
		{
			// Return the currnet time minus the start time
			time = SDL_GetTicks() - mStartTicks;
		}
	}
	return time;
}
	
// Checks the status of the timer
bool Timer::isStarted()
{
	return mStarted;
}

bool Timer::isPaused()
{
	return mPaused;
}

void Timer::Update()
{
	auto start = std::chrono::high_resolution_clock::now();

	std::this_thread::sleep_for(1s);

	auto end = std::chrono::high_resolution_clock::now();


	std::chrono::duration<float> duration = end - start;
	std::cout << duration.count() << std::endl;
	std::cin.get();
}
