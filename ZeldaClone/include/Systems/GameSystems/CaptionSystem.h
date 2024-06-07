#pragma once
#include "ECS/ECS.h"

class CaptionSystem : public System
{
  private:
	void AddCaption( const Entity& entity, int& spaceX, int& spaceY, char letter );
	int NextLine( std::string& str );

  public:
	CaptionSystem();
	~CaptionSystem() = default;

	void Update( const float& dt );
};