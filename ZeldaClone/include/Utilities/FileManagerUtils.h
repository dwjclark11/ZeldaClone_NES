#pragma once
#include <string>

class FileDialogs
{
  public:
	static std::string OpenFile( const char* filter = "Tilemap Files (*.map) | *.map\0*.map\0 Text Files (*.txt) "
													  "|*.txt\0*.txt\0 Lua Files (*.lua) |*.lua\0*.lua\0" );
	static std::string OpenImage( const char* filter = "Image Files (*.png) |*.png\0*.png\0" );
	static std::string SaveFile(
		const char* filter =
			"Map Files (*.map) | *.map\0*.map\0 Text Files (*.txt) |*.txt\0*.txt\0 Lua Files (*.lua) |*.lua\0*.lua\0" );
	static std::string SaveObjFile(
		const char* filter = "Map Files (*.map) | *.map\0*.map\0 Text Files (*.txt) |*.txt\0*.txt\0" );
	static std::string SaveBoxColliderFile(
		const char* filter = "Map Files (*.map) | *.map\0*.map\0 Text Files (*.txt) |*.txt\0*.txt\0 Lua Files (*.lua) "
							 "| *.lua\0*.lua\0" );
};
