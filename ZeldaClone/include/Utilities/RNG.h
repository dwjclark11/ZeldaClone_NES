#pragma once
#include <random>

class RNG
{
  private:
	std::mt19937 m_RandomEngine;
	std::uniform_int_distribution<std::mt19937::result_type> m_Distribution;
	/*
	 *	Init() - Initialize the random engine
	 */
	void Init();

  public:
	RNG();
	~RNG(){};

	/*
	 *	Float() - Gives a random number between 0 - 1
	 */
	float Float();
};