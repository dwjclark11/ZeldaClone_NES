# ZeldaClone_NES

## Table of Contents
* [Introduction](#introduction)
* [Technologies](#technologies)
* [Game/Engine](#gameengine)
* [Desired Additions/Changes](#desired-additionschanges)

## Introduction
This game was created for educational purposes and to help develop my skills with C++, Lua, and understanding
of data management. It uses the Pikuma Game Engine from the course https://courses.pikuma.com/courses/2dgameengine. 
This was a great course and I recommend it to anyone wanting to start learning more on creating Game Engines. 
I have taken what I learned from that course and applied it to creating this Zelda Clone and Tilemap/Quest Editor. 
I am still working on the functionality and this is a beautiful step into my journey to become a Software Developer!

## Technologies
This project is created with:
* C++ 
* Lua 5.3
* SDL2
* ImGui

## Game Controls
| Key |   Function  |
| --- | ----------- |
|  W  | Move Up     | 
|  D  | Move Right  | 
|  S  | Move Down   | 
|  A  | Move Left   |
|  Q  | Pause Menu  |
| ESC | Quit Game   | 
| RShift | Sword Attack |
| SpaceBar | Special Item |
* I want to make the controls changable in the future. Currently Hard Coded
## User Experience
* This is a small video where I demonstrate the current status of the game/editor, I will update this as development continues.
https://user-images.githubusercontent.com/63356975/153033167-88800d4c-b702-4609-9451-782682668e28.mp4

## Tilemap Editor Controls
       * Under Construction!!
## Game/Engine
This game/engine includes the following:
* An ECS (Entity Component System) to help manage entities, components, and systems that are in the game.
* #### A functioning Tilemap Editor that can create:
        * Tilemaps 
        * colliders
        * triggers (transport/secret areas/ traps/ etc.)
        * Uses Dear ImGui as the user interface
        * The tilemap editor still has some bugs; however, it does work
        

![Editor_Image](https://user-images.githubusercontent.com/63356975/153037159-f5e464fb-9853-49f6-97a1-916651dd8f92.png)


![gameplayPic](https://user-images.githubusercontent.com/63356975/133914504-5e2f6078-f494-4db0-890c-bf3d42004f6f.png)

## How to Build
       * Under Construction --> Premake file to come soon!

## Desired Additions/Changes
Some of the desired changes are:
* Change from using the SDL2 renderer to a OpenGL Graphics Pipeline 
* Add an animation editor to the tilemap editor that allows the ability to load sprites and change animation speeds on the spot
* Add enemies to any maps using the editor
* Add a functional AI state machine with for entities/enemies done in lua
       ** Currently this is done in C++, Not easy to make small changes!
* Ability for user to change keys/gamepad controls (Currently Hard Coded)


## There are still a lot of bugs that must be fixed to have a properly working demo
Some of the bugs are as follows:
* ~~SDL Window crashes when mouse goes outside of the zero boundaries -> Works fine otherwise~~ Fixed.
* ~~When changing scenes, there are issues removing mass entities.~~ Fixed.
* ~~Game is really slow in Debug mode. Release mode runs pretty good.~~ Fixed
* ~~Game does not really work properly. You can kill enemies and collect items; however, the removal of entities when changing scenes is having undesired results.~~ Fixed.

## Don't Forget to Star the Repository if you like what you see and where this is going. Thanks!
## Dustin 

