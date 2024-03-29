# ZeldaClone_NES

## Table of Contents
* [Introduction](#introduction)
* [Technologies](#technologies)
* [Game/Engine](#gameengine)

## Introduction
This game was created for educational purposes and to help develop my skills with C++, Lua, and understanding
of data management. It uses the Pikuma Game Engine from the course https://courses.pikuma.com/courses/2dgameengine. 
This was a great course and I recommend it to anyone wanting to start learning more on creating Game Engines. 
I have taken what I learned from that course and applied it to creating this Zelda Clone and Tilemap/Quest Editor. 
The game is not yet finished; There is still plenty of work to be done!

## Technologies
This project is created with:
* C++ 
* Lua 5.3
* SDL2
* ImGui
* Sprites were created/edited using Aseprite!

## Game Controls
| Key |  Button  |   Function  |  
| --- |  ------- | ----------- | 
|  W  |  DPAD UP | Move Up  | 
|  D  |  DPAD RIGHT |Move Right  | 
|  S  |  DPAD DOWN |Move Down   | 
|  A  |  DPAD LEFT |Move Left   |
|  Q  |  START |Pause Menu  |
| ESC |  N/A |Quit Game   | 
| RShift | A BTN | Sword Attack |
| SpaceBar |  X BTN | Special Item |

![xbox_controller](https://user-images.githubusercontent.com/63356975/153439752-0862e46e-48c1-461a-a79b-481d6e00307c.png)
* GamePad Buttons are based on the SDL_GameControllerButtons == X BOX Style Gamepad.
* These are default keys that are now changable inside of the settings state. Changes the Action/Key/Btn Bindings.

## User Experience
* This is a small video where I demonstrate the current status of the game/editor, I will update this as development continues.
- New Video - Basic Gameplay


https://user-images.githubusercontent.com/63356975/163869073-acae6d9b-0948-48d3-aecb-b369309f7278.mp4


- Fairy Fountain


https://user-images.githubusercontent.com/63356975/164451922-1915021e-952e-4653-a596-64e2b1feb660.mp4



- Oldest Video

https://user-images.githubusercontent.com/63356975/153033167-88800d4c-b702-4609-9451-782682668e28.mp4

## Tilemap Editor Controls
       * Under Construction!!
## Game/Engine
This game/engine includes the following:
* An ECS (Entity Component System) to help manage entities, components, and systems that are in the game.
* #### A functioning Tilemap/Quest Editor that can create:
    * Tilemaps 
    * colliders
    * triggers (transport/secret areas/ traps/ etc.)
    * Uses Dear ImGui as the user interface
    * The Tilemap/Quest Editor still has some bugs; however, it does work
  
        

![Editor_Image](https://user-images.githubusercontent.com/63356975/153037159-f5e464fb-9853-49f6-97a1-916651dd8f92.png)


![gameplayPic](https://user-images.githubusercontent.com/63356975/133914504-5e2f6078-f494-4db0-890c-bf3d42004f6f.png)

## How to Build -- **Build my be broken due to most recent changes to c++20 and other changes. I will update the build shortly**
This project currently uses Visual Studio 2022 Community and has not been tested on different systems/IDEs.
* Step 1)
    *  Clone the repo
* Step 2)
    * Run the GenerateSolution.bat file
    * This should run the premake5.lua file and create a visual studio .sln
* Step 3)
    *  Open the solution and build and run. It should work from there.

## Keep Coding and Take it Easy!

