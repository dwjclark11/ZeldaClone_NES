#pragma once
#include "../../ECS/ECS.h"

class CaptionSystem : public System
{
private:
	
	void AddCaption(const Entity& entity, int& spaceX, int& spaceY, char letter);
	int NextLine(std::string& str);

	const int LETTER_SPACE = 32;

public:
	CaptionSystem();
	void Update(const float& dt);
};