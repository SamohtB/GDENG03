#pragma once
#include <string>
#include "Dx12Commons.h"

class Texture
{
public:
    Texture(ComPtr<ID3D12Resource> buffer, UINT index);
    ~Texture() = default;

private:
    ComPtr<ID3D12Resource> m_textureResource;
    UINT m_srvHandleIndex;
};

