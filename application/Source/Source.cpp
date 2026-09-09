#include "Application/Application.h"

#include <Windows.h>
#include <filesystem>
#include <iostream>

INT WINAPI wWinMain(
    HINSTANCE hInstance,
    HINSTANCE hPrevInstance,
    LPWSTR lpCmdLine,
    int nShowCmd
)
{
    if (0)
    {
        AllocConsole();
        FILE* fp;
        freopen_s(&fp, "CONOUT$", "w", stdout);
    }
    
    App::Application app;

    if (!app.Initialize(GetModuleHandle(nullptr)))
        return 1;

    app.Run();

    return 0;
}