# ZeldaClone_NES

## Table of Contents
* [Introduction](#introduction)
* [Technologies](#technologies)
* [Game/Engine](#gameengine)

## Introduction
This game was created for educational purposes and to help develop my skills with C++, Lua, and understanding
of data management. 

It uses the Pikuma Game Engine from the course https://courses.pikuma.com/courses/2dgameengine. 
This was a great course and I recommend it to anyone wanting to start learning more on creating Game Engines. 

I have taken what I learned from that course and applied it to creating this Zelda Clone and Tilemap/Quest Editor. 
This is not a finished game and the quest editor does not really work; however, it was fun to make. 

There are also many changes that I would like to make if I had more time, but I have other projects that have a higher priority. 

# Build
----
Requires [CMake 3.26](https://cmake.org/) and [vcpkg](https://github.com/microsoft/vcpkg)
#### Get VCPKG:
```ps
git clone https://github.com/microsoft/vcpkg
./vcpkg/bootstrap-vcpkg.bat
```
#### Make sure the following environment variables are set:
```
VCPKG_ROOT=[path_to_vcpkg]
VCPKG_DEFAULT_TRIPLET=x64-windows
```
#### Install dependencies 
```
./vcpkg install glm sdl2[alsa] sdl2-mixer[mpg123] sdl2-ttf sdl2-image lua sol2
```
  * For sdl2_mixer you name need to use:
```
./vcpkg install sdl2-mixer[mpg123] --recurse
```
* I tested this on ```Ubuntu 24``` and the build works. Just adjust default triplet for VCPKG to ```VCPKG_DEFAULT_TRIPLET=x64-linux```

#### Clone the repository 
```
git clone https://github.com/dwjclark11/ZeldaClone_NES.git
cd ZeldaClone_NES
cmake -S . -B build
```
---- 
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

----
https://user-images.githubusercontent.com/63356975/163869073-acae6d9b-0948-48d3-aecb-b369309f7278.mp4

----
- Fairy Fountain


https://user-images.githubusercontent.com/63356975/164451922-1915021e-952e-4653-a596-64e2b1feb660.mp4

----

- Oldest Video

https://user-images.githubusercontent.com/63356975/153033167-88800d4c-b702-4609-9451-782682668e28.mp4

----

## Tilemap Editor Controls
       * Under Construction!!

 ----
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

----
# Issues
There are still tonnes of issues in order to have a proper working game. 
* Needs proper AI for enemies.
* Quest editor is currently broken.
* Needs proper animations when going into caves.
* Player can still move after getting triforce.
* Titlescreen does not reset properly and will eventually crash.

## Keep Coding and Take it Easy!

