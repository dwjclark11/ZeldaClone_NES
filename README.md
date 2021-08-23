# ZeldaClone_NES

## Table of Contents
* [Introduction](#introduction)
* [Technologies](#technologies)
* [Game/Engine](#game/engine)
* [Desired Additions/Changes](#desired)

## Introduction
This game was inspired by the 2D Game Engine course from Pikuma.com. Big thanks to Gustavo Pezzi for the inspiration. I have taken what I 
learned from that course to help me develop this Zelda Clone. 
This game was created for educational purposes and to help develop my skills with C++, Lua, and understanding
of data management. 
## Technologies
This project is created with:
* C/C++ 
* Lua 5.3
* SDL2
## Game/Engine
This game/engine includes the following:
* An ECS (Entity Component System) to help manage entities, components, and systems that are in the game.
* A functioning Tilemap Editor that can create:
        * Tilemaps 
        * colliders
        * triggers (transport/secret areas/ traps/ etc.)
        * Uses Dear ImGui as the user interface
        

![Clone_Editor](https://user-images.githubusercontent.com/63356975/130303677-08d6fdc8-6f66-490b-ba8a-df98e0a7192a.png)

![zelda_clone](https://user-images.githubusercontent.com/63356975/130304810-ad7e5efe-b8a2-4fd1-ab23-6c624646fd57.png)

## Desired Additions/Changes
Some of the desired changes are:
* Change from using the SDL2 renderer to a OpenGL Graphics Pipeline 
* Add an animation editor to the tilemap editor that allows the ability to load sprites and change animation speeds on the spot
* Add enemies to any maps using the editor --> currently working on this!
* Add a functional AI state machine with for entities/enemies done in lua

## There are still a lot of bugs that must be fixed to have a properly working demo

