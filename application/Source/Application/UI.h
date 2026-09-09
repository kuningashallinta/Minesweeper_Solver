

#pragma once
#include "imgui.h"
#include "Renderer.h"

namespace UI {
    class ImGuiLayer {
    public:
        ImGuiLayer();
        ~ImGuiLayer();

        bool Initialize(HWND hwnd, Renderer::DX11Renderer* renderer);
        void Cleanup();
        void BeginFrame();
        void EndFrame();
        void Render();

    private:
        void RenderMainWindow();
        Renderer::DX11Renderer* m_renderer;
    };
}
