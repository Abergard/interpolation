////////////////////////////////////////////////////////////////////////////////
// Filename: main.cpp
////////////////////////////////////////////////////////////////////////////////
#define WIN32_LEAN_AND_MEAN

#include <windows.h>

#include <filesystem>
#include <iostream>

#include "systemclass.h"

// int WINAPI WinMain(HINSTANCE hInstance,
//                    HINSTANCE hPrevInstance,
//                    PSTR pScmdline,
//                    int iCmdshow)
int main(int, char* argv[])
{
    auto app_path = std::filesystem::path{argv[0]}.parent_path();
    auto shaders_path = app_path / "shaders";

    SystemClass system{app_path};
    if (auto result = system.Initialize())
    {
        system.Run();
    }

    system.Shutdown();
}
