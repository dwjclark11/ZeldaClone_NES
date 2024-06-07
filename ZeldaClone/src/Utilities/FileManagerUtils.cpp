#include "Utilities/FileManagerUtils.h"
#include "Logger/Logger.h"

#include <optional>
#include <iostream>
#include <string>
#include <sstream>

#ifdef _WIN32
#include <Windows.h>
#else
// TODO: Include for other OS
#endif

std::string FileDialogs::OpenFile( const char* filter )
{
#ifdef _WIN32
	OPENFILENAMEA ofn;			// Common dialog box structure
	CHAR szFile[ 260 ] = { 0 }; // If using tChar MACROS

	// Initialize OPENFILENAME
	ZeroMemory( &ofn, sizeof( OPENFILENAME ) );

	ofn.lStructSize = sizeof( OPENFILENAME );
	ofn.hwndOwner = NULL;
	ofn.lpstrFilter = filter;
	ofn.lpstrFile = szFile;
	ofn.nMaxFile = sizeof( szFile );
	ofn.nFilterIndex = 1;
	ofn.Flags = OFN_PATHMUSTEXIST | OFN_FILEMUSTEXIST | OFN_NOCHANGEDIR;

	if ( GetOpenFileNameA( &ofn ) )
	{
		return ofn.lpstrFile;
	}

	return std::string();
#else
	// TODO: This needs to be done in a better way
	return std::string();
#endif
}

std::string FileDialogs::OpenImage( const char* filter )
{
#ifdef _WIN32
	OPENFILENAMEA ofn;			// Common dialog box structure
	CHAR szFile[ 260 ] = { 0 }; // If using tChar MACROS

	// Initialize OPENFILENAME
	ZeroMemory( &ofn, sizeof( OPENFILENAME ) );

	ofn.lStructSize = sizeof( OPENFILENAME );
	ofn.hwndOwner = NULL;
	ofn.lpstrFilter = filter;
	ofn.lpstrFile = szFile;
	ofn.nMaxFile = sizeof( szFile );
	ofn.nFilterIndex = 1;
	ofn.Flags = OFN_PATHMUSTEXIST | OFN_FILEMUSTEXIST | OFN_NOCHANGEDIR;

	if ( GetOpenFileNameA( &ofn ) )
	{
		return ofn.lpstrFile;
	}

	return std::string();
#else
	return std::string();
#endif
}

std::string FileDialogs::SaveFile( const char* filter )
{
#ifdef _WIN32
	OPENFILENAMEA ofn;			// Common dialog box structure
	CHAR szFile[ 260 ] = { 0 }; // If using tChar MACROS

	// Initialize OPENFILENAME
	ZeroMemory( &ofn, sizeof( OPENFILENAME ) );

	ofn.lStructSize = sizeof( OPENFILENAME );
	ofn.hwndOwner = NULL;
	ofn.lpstrFilter = filter;
	ofn.lpstrFile = szFile;
	ofn.nMaxFile = sizeof( szFile );
	ofn.nFilterIndex = 1;
	ofn.Flags = OFN_PATHMUSTEXIST | OFN_FILEMUSTEXIST | OFN_NOCHANGEDIR;

	if ( GetSaveFileNameA( &ofn ) )
	{
		return ofn.lpstrFile;
	}

	return std::string();
#else
	return std::string();
#endif
}

std::string FileDialogs::SaveObjFile( const char* filter )
{
#ifdef _WIN32
	OPENFILENAMEA ofn;			// Common dialog box structure
	CHAR szFile[ 260 ] = { 0 }; // If using tChar MACROS

	// Initialize OPENFILENAME
	ZeroMemory( &ofn, sizeof( OPENFILENAME ) );

	ofn.lStructSize = sizeof( OPENFILENAME );
	ofn.hwndOwner = NULL;
	ofn.lpstrFilter = filter;
	ofn.lpstrFile = szFile;
	ofn.nMaxFile = sizeof( szFile );
	ofn.nFilterIndex = 1;
	ofn.Flags = OFN_PATHMUSTEXIST | OFN_FILEMUSTEXIST | OFN_NOCHANGEDIR;

	if ( GetSaveFileNameA( &ofn ) )
	{
		return ofn.lpstrFile;
	}

	return std::string();
#else
	return std::string();
#endif
}

std::string FileDialogs::SaveBoxColliderFile( const char* filter )
{
#ifdef _WIN32
	OPENFILENAMEA ofn;			// Common dialog box structure
	CHAR szFile[ 260 ] = { 0 }; // If using tChar MACROS

	// Initialize OPENFILENAME
	ZeroMemory( &ofn, sizeof( OPENFILENAME ) );

	ofn.lStructSize = sizeof( OPENFILENAME );
	ofn.hwndOwner = NULL;
	ofn.lpstrFilter = filter;
	ofn.lpstrFile = szFile;
	ofn.nMaxFile = sizeof( szFile );
	ofn.nFilterIndex = 1;
	ofn.Flags = OFN_PATHMUSTEXIST | OFN_FILEMUSTEXIST | OFN_NOCHANGEDIR;

	if ( GetSaveFileNameA( &ofn ) )
	{
		return ofn.lpstrFile;
	}

	return std::string();
#else
	return std::string();
#endif
}
