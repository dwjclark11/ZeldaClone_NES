#include "Utility.h"
//#include <string>
#include <cmath>
#include "../ECS/ECS.h"
#include "../Components/SpriteComponent.h"
#include "../Components/TransformComponent.h"
#include "../Components/HUDComponent.h"

void ConvertNumberParser(std::string group, int num, int power)
{
	{
		int digits = 0;
		int temp_num = num;
		int tens = 0;
		int space = 0;
		int temp_num2 = num;
		
		// check for a value of zero
		if (num <= 0)
		{
			// Do Nothing --> The srcRect is set to 0 by default
		}
		else
		{
			if (power == 2)
			{
				temp_num /= pow(10, power);
			}
			else if (power == 1)
			{
				if (num > 99)
				{
					temp_num /= pow(10, 2);
					temp_num *= 100;
					temp_num2 -= temp_num;
				}

				temp_num2 /= 10;
				temp_num = temp_num2;

			}
			else if (power == 0)
			{
				int temp_num3 = 0;

				if (num > 99)
				{
					temp_num /= pow(10, 2);
					temp_num *= 100;
					temp_num2 -= temp_num;
					temp_num3 = temp_num2;

					if (temp_num3 > 9)
					{
						temp_num2 /= pow(10, 1);
						temp_num2 *= 10;
						temp_num3 -= temp_num2;
					}
					temp_num = temp_num3;
				}
				else if (num > 9)
				{
					temp_num /= pow(10, 1);
					temp_num *= 10;
					temp_num2 -= temp_num;
					temp_num = temp_num2;
				}
			}
			else
			{
				Logger::Err("PLEASE PICK A POWER SMALLER BETWEEN 0 - 2");

			}
			
			// Create a variable entity and have it equal the given tag
			auto entity = Registry::Instance()->GetEntityByTag(group);

			// After Calulations --> Change the srcRectX location based on outcome
			switch (temp_num)
			{
			case 0: // Zero is the Default value
				entity.GetComponent<SpriteComponent>().srcRect.x = 0;
				entity.GetComponent<SpriteComponent>().srcRect.y = 0;
				//std::cout << temp_num << std::endl;
				break;

			case 1:
				entity.GetComponent<SpriteComponent>().srcRect.x = 8;
				entity.GetComponent<SpriteComponent>().srcRect.y = 0;
				//std::cout << temp_num << std::endl;
				break;

			case 2:
				entity.GetComponent<SpriteComponent>().srcRect.x = 16;
				entity.GetComponent<SpriteComponent>().srcRect.y = 0;
				// std::cout << temp_num << std::endl;
				break;

			case 3:
				entity.GetComponent<SpriteComponent>().srcRect.x = 24;
				entity.GetComponent<SpriteComponent>().srcRect.y = 0;
				// std::cout << temp_num << std::endl;
				break;

			case 4:
				entity.GetComponent<SpriteComponent>().srcRect.x = 32;
				entity.GetComponent<SpriteComponent>().srcRect.y = 0;
				// std::cout << temp_num << std::endl;
				break;

			case 5:
				entity.GetComponent<SpriteComponent>().srcRect.x = 40;
				entity.GetComponent<SpriteComponent>().srcRect.y = 0;
				// std::cout << temp_num << std::endl;
				break;

			case 6:
				entity.GetComponent<SpriteComponent>().srcRect.x = 48;
				entity.GetComponent<SpriteComponent>().srcRect.y = 0;
				// std::cout << temp_num << std::endl;
				break;

			case 7:
				entity.GetComponent<SpriteComponent>().srcRect.x = 56;
				entity.GetComponent<SpriteComponent>().srcRect.y = 0;
				// std::cout << temp_num << std::endl;
				break;

			case 8:
				entity.GetComponent<SpriteComponent>().srcRect.x = 64;
				entity.GetComponent<SpriteComponent>().srcRect.y = 0;
				// std::cout << temp_num << std::endl;
				break;

			case 9:
				entity.GetComponent<SpriteComponent>().srcRect.x = 72;
				entity.GetComponent<SpriteComponent>().srcRect.y = 0;
				// std::cout << temp_num << std::endl;
				break;
			default:
				break;
			}
		}
	}
}


