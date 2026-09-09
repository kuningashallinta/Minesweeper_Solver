#pragma once
#include <windows.h>
#include <d3d11.h>
#include <memory>
#include "Renderer.h"
#include "UI.h"

extern IMGUI_IMPL_API LRESULT ImGui_ImplWin32_WndProcHandler(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);

namespace App {
    class Application {
    public:
        Application();
        ~Application();
        bool Initialize(HINSTANCE hInstance);
        void Run();

    private:
        static LRESULT CALLBACK WindowProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam);
        bool CreateMainWindow(HINSTANCE hInstance);

        HWND m_hwnd;
        std::unique_ptr<Renderer::DX11Renderer> m_renderer;
        std::unique_ptr<UI::ImGuiLayer> m_gui;
        bool m_isRunning;
    };
}
