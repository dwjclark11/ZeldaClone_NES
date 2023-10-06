#pragma once
#include <string>
#include <glm/glm.hpp>
#include <SDL.h>
#include "../Logger/Logger.h"
void ConvertNumberParser(std::string group, int num, int power);
void ConvertName(const std::string& name, int x, int y);
void ConvertLetter(const char letter, int x, int y);
void ConvertNumber(int number, int x, int y);
void ConvertNumber(const std::string& number, int x, int y);

void ChangePlayerAttrib(const glm::vec2& velocity, const glm::vec2& shieldOffset, 
	const glm::vec2& swordOffset, const int& shieldWidth, 
	const int& shieldHeight, const int& spriteWidth,  const int& swordWidth, const int& swordHeight);


struct SDL_Wrappers
{
	/*
	*  SDL_Destroyer, these are custom deleters that are needed
	*  to call the SDL_Destroy functions for the SDL pointer
	*  wrappers
	*/
	struct SDL_Destroyer
	{
		void operator()(SDL_Window* window) const
		{
			SDL_DestroyWindow(window);
			Logger::Log("Destroyed SDL_Window!");
		}
		void operator()(SDL_Renderer* renderer) const
		{
			SDL_DestroyRenderer(renderer);
			Logger::Log("Destroyed SDL_Renderer!");
		}
		void operator()(SDL_Texture* texture) const
		{
			SDL_DestroyTexture(texture);
			Logger::Log("Destroyed SDL_Texture!");
		}
		void operator()(SDL_GameController* controller) const
		{
			SDL_GameControllerClose(controller); // NOT WORKING!!
			controller = nullptr;
			Logger::Log("Closed SDL Game Controller!");
		}
	};
};

typedef std::unique_ptr<SDL_Window, SDL_Wrappers::SDL_Destroyer>			Window;
typedef std::unique_ptr<SDL_Renderer, SDL_Wrappers::SDL_Destroyer> 			Renderer;
typedef std::unique_ptr<SDL_Texture, SDL_Wrappers::SDL_Destroyer>			Texture;
typedef std::unique_ptr<SDL_GameController, SDL_Wrappers::SDL_Destroyer>	Controller;


constexpr int PANEL_WIDTH = 1024;
constexpr int PANEL_HEIGHT = 672;
constexpr int HUD_HEIGHT = 288;
