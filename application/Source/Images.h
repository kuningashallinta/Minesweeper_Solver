#pragma once

#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"

#include <cstdio>
#include <cstdint>
#include <unordered_map>

#include "imgui.h"

#include <d3d11.h>
#include <Windows.h>

#include <iostream>

namespace Images
{
    typedef struct tagImage
    {
        ID3D11ShaderResourceView* Texture = nullptr;
        std::uint32_t width;
        std::uint32_t height;
    } Image;

    std::unordered_map<int, Image> Images;

    bool LoadTextureFromFile(ID3D11Device* device, const wchar_t* path, Image* im)
    {
        FILE* file = nullptr;
        _wfopen_s(&file, path, L"rb");

        if (not file)
        {
            return false;
        }

        int width, height, channels;
        stbi_uc* pixels = stbi_load_from_file(file, &width, &height, &channels, STBI_rgb_alpha);
        fclose(file);

        if (not pixels)
        {
            return false;
        }

        D3D11_TEXTURE2D_DESC desc = {};
        desc.Width = width;
        desc.Height = height;
        desc.MipLevels = 1;
        desc.ArraySize = 1;
        desc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
        desc.SampleDesc.Count = 1;
        desc.Usage = D3D11_USAGE_DEFAULT;
        desc.BindFlags = D3D11_BIND_SHADER_RESOURCE;

        D3D11_SUBRESOURCE_DATA subResource = {};
        subResource.pSysMem = pixels;
        subResource.SysMemPitch = width * 4;

        ID3D11Texture2D* texture = nullptr;
        HRESULT hr = device->CreateTexture2D(&desc, &subResource, &texture);
        stbi_image_free(pixels);

        if (FAILED(hr))
        {
            return false;
        }

        D3D11_SHADER_RESOURCE_VIEW_DESC srvDesc = {};
        srvDesc.Format = desc.Format;
        srvDesc.ViewDimension = D3D11_SRV_DIMENSION_TEXTURE2D;
        srvDesc.Texture2D.MipLevels = 1;
        hr = device->CreateShaderResourceView(texture, &srvDesc, &im->Texture);
        texture->Release();

        if (FAILED(hr))
        {
            return false;
        }

        im->width = width;
        im->height = height;

        return true;
    }

    void CleanupTexture(Image* im) {
        if (im->Texture) {
            im->Texture->Release();
            im->Texture = nullptr;
        }
    }
}
