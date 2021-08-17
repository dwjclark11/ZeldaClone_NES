#pragma once
#include <Windows.h>
#include <string>

/*
	This class is for the Tilemap editor and it allows the user to open a File Dialog to save/load files
	using the native Windows functions. These will only load/save to the types of files that have been declared 
	in the functions --> .lua, .txt, .map, etc.
*/

class FileDialogs
{
public:
	static std::string OpenFile(const char* filter = "Map Files (*.map) | *.map\0*.map\0 Text Files (*.txt) |*.txt\0*.txt\0", HWND owner = NULL);
	static std::string OpenImage(const char* filter = "Image Files (*.png) |*.png\0*.png\0", HWND owner = NULL);
	static std::string SaveFile(const char* filter = "Map Files (*.map) | *.map\0*.map\0 Text Files (*.txt) |*.txt\0*.txt\0", HWND owner = NULL);
	static std::string SaveObjFile(const char* filter = "Map Files (*.map) | *.map\0*.map\0 Text Files (*.txt) |*.txt\0*.txt\0", HWND owner = NULL);
	static std::string SaveBoxColliderFile(const char* filter = "Map Files (*.map) | *.map\0*.map\0 Text Files (*.txt) |*.txt\0*.txt\0 Lua Files (*.lua) | *.lua\0*.lua\0", HWND owner = NULL);
};