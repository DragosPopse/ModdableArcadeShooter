# Bitflight

A moddable arcade-style airplane shooter. 

This is my programming assignment for the Breda University Creative Media and Game Technologies 2020 programme. The theme is "Waves", so the enemies come into waves throughout the level. The music is also designed to facilitate that. I created 2 levels for this assignment.

## Purpose
The main purpose of this project is to experiment with creating a Modding API in Lua for a C++ game. Another concept that I try to get familiar with while creating this game is makefiles and building from multiple PCs, thing that I do using Premake5.

## Future
I plan to create another level in space and release the game for free on itch.io and "market" it on different forums. My main goal is to get as much feedback as possible regarding the things I did (code, mechanics and level design).

## Build 
1) Click on one of the setup.bat's that matches your compiler (vs2019 or vs2017).
2) Once the solution is created, select the architecture and configuration (x86/x64 and Debug/Release)
3) Build 
4) The binaries created will be available in the bin folder

*Note: If you are one of the teachers, the sln file and binaries will already be available and there is no need for setup.bat.*

## Controls
*Note: all controls can be customized in-game*
* Movement: WASD
* Fire: Space
* Switch weapons: Q and E 
* Pause: Escape (*please pause the game because something cool happens :>*) 

## Libraries Used
* rapidjson (http://rapidjson.org/)
  * JSON library
  * Used for reading/writing JSON files for Player settings and high score. 
* sol2 (https://sol2.readthedocs.io/en/latest/)
  * Lua wrapper written in C++
  * Used for creating the Modding API
* SFML (https://www.sfml-dev.org/)
  * Multimedia library for handling Graphics/Audio
* Thor (https://bromeon.ch/libraries/thor/index.html)
  * SFML extension
  * Used mostly for particles  
* TGUI (https://tgui.eu/)
  * GUI library written for SFML
  
## Technologies Used
* C++17
  * The core of the game
* Lua 5.3.5
  * The scripting language used for modding
* Premake5
  * Cross platform makefile that has a friendly Lua interface
  
## Credits
*Note: in case of code taken from other sources the credits are available in the source files with the said code.*
* Book: SFML Game Development
  * This was the starting point of the game. 
  * A lot of techniques and design patterns tought there are used in this game, most notably Command, the scene graph and StateMachine-like SceneManager. 
* Music and SFX: Valeriu Andrei Ghebaru (youtube.com/channel/UC8mGNFGRp7b3z0NaGYteFeA)
  * The soundtrack and sound effects are created just for this game.
  * Every soundtrack was created carefully to match the theme of the level and the waves pattern.
* Textures: Ana Maria Dirica
  * Textures were created for this game. 
