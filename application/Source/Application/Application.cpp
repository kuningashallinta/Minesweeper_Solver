#include "Application.h"
#include <tchar.h>

namespace App {
    Application::Application() : m_hwnd(nullptr), m_isRunning(false) {
        m_renderer = std::make_unique<Renderer::DX11Renderer>();
        m_gui = std::make_unique<UI::ImGuiLayer>();
    }

    Application::~Application() {}

    bool Application::Initialize(HINSTANCE hInstance) {
        if (!CreateMainWindow(hInstance)) 
            return false;

        if (!m_renderer->Initialize(m_hwnd)) 
            return false;

        if (!m_gui->Initialize(m_hwnd, m_renderer.get())) 
            return false;

        m_isRunning = true;
        return true;
    }

    bool Application::CreateMainWindow(HINSTANCE hInstance) {
        WNDCLASSEX wc = {
            sizeof(WNDCLASSEX),
            CS_CLASSDC,
            WindowProc,
            0, 0,
            hInstance,
            nullptr, nullptr, nullptr, nullptr,
            _T("mkwc"),
            nullptr
        };

        RegisterClassEx(&wc);
        m_hwnd = CreateWindow(
            wc.lpszClassName,
            _T("Milkshake"),
            WS_OVERLAPPEDWINDOW,
            100, 100, 1280, 800,
            nullptr, nullptr,
            wc.hInstance,
            this
        );

        return m_hwnd != nullptr;
    }

    void Application::Run() {
        ShowWindow(m_hwnd, SW_SHOWDEFAULT);
        UpdateWindow(m_hwnd);

        MSG msg;
        ZeroMemory(&msg, sizeof(msg));

        while (m_isRunning) {
            while (PeekMessage(&msg, nullptr, 0U, 0U, PM_REMOVE)) {
                TranslateMessage(&msg);
                DispatchMessage(&msg);
                if (msg.message == WM_QUIT) {
                    m_isRunning = false;
                    break;
                }
            }

            if (!m_isRunning) break;

            m_renderer->BeginFrame();
            m_gui->BeginFrame();
            m_gui->Render();
            m_gui->EndFrame();
            m_renderer->EndFrame();
        }
    }

    LRESULT Application::WindowProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam) {
        if (ImGui_ImplWin32_WndProcHandler(hwnd, msg, wParam, lParam))
            return true;

        switch (msg) {
            case WM_GETMINMAXINFO: {
                LPMINMAXINFO lpMMI = (LPMINMAXINFO)lParam;
                lpMMI->ptMinTrackSize.x = 800;
                lpMMI->ptMinTrackSize.y = 600;
                return 0;
            }
            case WM_SIZE:
                if (auto* app = reinterpret_cast<Application*>(GetWindowLongPtr(hwnd, GWLP_USERDATA))) {
                    if (wParam != SIZE_MINIMIZED && app->m_renderer != nullptr) {
                        app->m_renderer->Resize((UINT)LOWORD(lParam), (UINT)HIWORD(lParam));
                    }
                }
                return 0;
            case WM_DESTROY:
                PostQuitMessage(0);
                return 0;
            case WM_CREATE: {
                LPCREATESTRUCT createStruct = reinterpret_cast<LPCREATESTRUCT>(lParam);
                Application* app = reinterpret_cast<Application*>(createStruct->lpCreateParams);
                SetWindowLongPtr(hwnd, GWLP_USERDATA, reinterpret_cast<LONG_PTR>(app));
                return 0;
            }
                          return 0;
        }
        return DefWindowProc(hwnd, msg, wParam, lParam);
    }
}