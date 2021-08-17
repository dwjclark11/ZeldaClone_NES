#include "FileManagerUtils.h"
#include "../Logger/Logger.h"
#include <optional>
#include <iostream>
#include <string>
#include <sstream>

std::string FileDialogs::OpenFile(const char* filter, HWND owner)
{ 
    OPENFILENAMEA ofn; // Common dialog box structure
    CHAR szFile[260] = { 0 }; // If using tChar MACROS

    // Initialize OPENFILENAME
    ZeroMemory(&ofn, sizeof(OPENFILENAME));

    ofn.lStructSize = sizeof(OPENFILENAME);
    ofn.hwndOwner = owner;
    ofn.lpstrFilter = filter;
    ofn.lpstrFile = szFile;
    ofn.nMaxFile = sizeof(szFile);
    ofn.nFilterIndex = 1;
    ofn.Flags = OFN_PATHMUSTEXIST | OFN_FILEMUSTEXIST | OFN_NOCHANGEDIR;

    std::string fileNameStr;

    if (GetOpenFileNameA(&ofn))
    {
        return ofn.lpstrFile;
    }

    return std::string();
}

std::string FileDialogs::OpenImage(const char* filter, HWND owner)
{
    OPENFILENAMEA ofn; // Common dialog box structure
    CHAR szFile[260] = { 0 }; // If using tChar MACROS

    // Initialize OPENFILENAME
    ZeroMemory(&ofn, sizeof(OPENFILENAME));

    ofn.lStructSize = sizeof(OPENFILENAME);
    ofn.hwndOwner = owner;
    ofn.lpstrFilter = filter;
    ofn.lpstrFile = szFile;
    ofn.nMaxFile = sizeof(szFile);
    ofn.nFilterIndex = 1;
    ofn.Flags = OFN_PATHMUSTEXIST | OFN_FILEMUSTEXIST | OFN_NOCHANGEDIR;

    std::string fileNameStr;

    if (GetOpenFileNameA(&ofn))
    {
        return ofn.lpstrFile;
    }

    return std::string();
}

std::string FileDialogs::SaveFile(const char* filter, HWND owner)
{
    OPENFILENAMEA ofn; // Common dialog box structure
    CHAR szFile[260] = { 0 }; // If using tChar MACROS

    // Initialize OPENFILENAME
    ZeroMemory(&ofn, sizeof(OPENFILENAME));

    ofn.lStructSize = sizeof(OPENFILENAME);
    ofn.hwndOwner = owner;
    ofn.lpstrFilter = filter;
    ofn.lpstrFile = szFile;
    ofn.nMaxFile = sizeof(szFile);
    ofn.nFilterIndex = 1;
    ofn.Flags = OFN_PATHMUSTEXIST | OFN_FILEMUSTEXIST | OFN_NOCHANGEDIR;

    std::string fileNameStr;

    if (GetSaveFileNameA(&ofn))
    {
        return ofn.lpstrFile;
    }

    return std::string();
}

std::string FileDialogs::SaveObjFile(const char* filter, HWND owner)
{
    OPENFILENAMEA ofn; // Common dialog box structure
    CHAR szFile[260] = { 0 }; // If using tChar MACROS

    // Initialize OPENFILENAME
    ZeroMemory(&ofn, sizeof(OPENFILENAME));

    ofn.lStructSize = sizeof(OPENFILENAME);
    ofn.hwndOwner = owner;
    ofn.lpstrFilter = filter;
    ofn.lpstrFile = szFile;
    ofn.nMaxFile = sizeof(szFile);
    ofn.nFilterIndex = 1;
    ofn.Flags = OFN_PATHMUSTEXIST | OFN_FILEMUSTEXIST | OFN_NOCHANGEDIR;

    std::string fileNameStr;

    if (GetSaveFileNameA(&ofn))
    {
        return ofn.lpstrFile;
    }

    return std::string();
}

std::string FileDialogs::SaveBoxColliderFile(const char* filter, HWND owner)
{
    OPENFILENAMEA ofn; // Common dialog box structure
    CHAR szFile[260] = { 0 }; // If using tChar MACROS

    // Initialize OPENFILENAME
    ZeroMemory(&ofn, sizeof(OPENFILENAME));

    ofn.lStructSize = sizeof(OPENFILENAME);
    ofn.hwndOwner = owner;
    ofn.lpstrFilter = filter;
    ofn.lpstrFile = szFile;
    ofn.nMaxFile = sizeof(szFile);
    ofn.nFilterIndex = 1;
    ofn.Flags = OFN_PATHMUSTEXIST | OFN_FILEMUSTEXIST | OFN_NOCHANGEDIR;

    std::string fileNameStr;

    if (GetSaveFileNameA(&ofn))
    {
        return ofn.lpstrFile;
    }

    return std::string();
}
