#include "Renderer.h"

namespace Renderer {
    DX11Renderer::DX11Renderer() :
        m_device(nullptr),
        m_context(nullptr),
        m_swapChain(nullptr),
        m_renderTargetView(nullptr) {
    }

    DX11Renderer::~DX11Renderer() {
        Cleanup();
    }

    bool DX11Renderer::Initialize(HWND hwnd) {
        if (!CreateDeviceAndSwapChain(hwnd)) return false;
        if (!CreateRenderTarget()) return false;
        return true;
    }

    bool DX11Renderer::CreateDeviceAndSwapChain(HWND hwnd) {
        DXGI_SWAP_CHAIN_DESC sd;
        ZeroMemory(&sd, sizeof(sd));
        sd.BufferCount = 2;
        sd.BufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
        sd.BufferDesc.RefreshRate.Numerator = 60;
        sd.BufferDesc.RefreshRate.Denominator = 1;
        sd.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
        sd.OutputWindow = hwnd;
        sd.SampleDesc.Count = 1;
        sd.SampleDesc.Quality = 0;
        sd.Windowed = TRUE;
        sd.SwapEffect = DXGI_SWAP_EFFECT_DISCARD;

        UINT createDeviceFlags = 0;
#ifdef _DEBUG
        createDeviceFlags |= D3D11_CREATE_DEVICE_DEBUG;
#endif

        D3D_FEATURE_LEVEL featureLevel;
        const D3D_FEATURE_LEVEL featureLevelArray[2] = {
            D3D_FEATURE_LEVEL_11_0,
            D3D_FEATURE_LEVEL_10_0,
        };

        IDXGISwapChain* swapChain;
        ID3D11Device* device;
        ID3D11DeviceContext* context;

        HRESULT hr = D3D11CreateDeviceAndSwapChain(
            nullptr, D3D_DRIVER_TYPE_HARDWARE, nullptr,
            createDeviceFlags, featureLevelArray, 2,
            D3D11_SDK_VERSION, &sd,
            &swapChain, &device, &featureLevel, &context
        );

        if (FAILED(hr)) return false;

        m_swapChain = swapChain;
        m_device = device;
        m_context = context;

        return true;
    }

    bool DX11Renderer::CreateRenderTarget() {
        ID3D11Texture2D* backBuffer = nullptr;
        if (FAILED(m_swapChain->GetBuffer(0, __uuidof(ID3D11Texture2D), (void**)&backBuffer)))
            return false;

        HRESULT hr = m_device->CreateRenderTargetView(backBuffer, nullptr, &m_renderTargetView);
        backBuffer->Release();

        if (FAILED(hr)) return false;
        return true;
    }

    void DX11Renderer::BeginFrame() {
        const float clearColor[4] = { 0.45f, 0.55f, 0.60f, 1.00f };
        m_context->OMSetRenderTargets(1, &m_renderTargetView, nullptr);
        m_context->ClearRenderTargetView(m_renderTargetView, clearColor);
    }

    void DX11Renderer::EndFrame() {
        m_swapChain->Present(1, 0);
    }

    void DX11Renderer::Resize(UINT width, UINT height) {
        if (!m_device) return;

        if (m_renderTargetView) {
            m_renderTargetView->Release();
            m_renderTargetView = nullptr;
        }
        m_swapChain->ResizeBuffers(0, width, height, DXGI_FORMAT_UNKNOWN, 0);
        CreateRenderTarget();
    }

    void DX11Renderer::Cleanup() {
        if (m_renderTargetView) { m_renderTargetView->Release(); m_renderTargetView = nullptr; }
        if (m_swapChain) { m_swapChain->Release(); m_swapChain = nullptr; }
        if (m_context) { m_context->Release(); m_context = nullptr; }
        if (m_device) { m_device->Release(); m_device = nullptr; }
    }
}
