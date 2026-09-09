#pragma once
#include <d3d11.h>


namespace Renderer {
    class DX11Renderer {
    public:
        DX11Renderer();
        ~DX11Renderer();

        bool Initialize(HWND hwnd);
        void Cleanup();
        void BeginFrame();
        void EndFrame();
        void Resize(UINT width, UINT height);

        ID3D11Device* GetDevice() { return m_device; }
        ID3D11DeviceContext* GetContext() { return m_context; }

    private:
        bool CreateDeviceAndSwapChain(HWND hwnd);
        bool CreateRenderTarget();

        ID3D11Device* m_device;
        ID3D11DeviceContext* m_context;
        IDXGISwapChain* m_swapChain;
        ID3D11RenderTargetView* m_renderTargetView;
    };
}