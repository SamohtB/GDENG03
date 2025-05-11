#pragma once
#include "stdafx.h"

using namespace DirectX;

class VertexBuffer
{
public:
	VertexBuffer(ComPtr<ID3D12Device> device, float aspectRatio);
	~VertexBuffer() = default;

    D3D12_VERTEX_BUFFER_VIEW* GetVertexBufferViewPointer();


    struct Vertex
    {
        XMFLOAT3 position;
        XMFLOAT4 color;
    };

private:
    ComPtr<ID3D12Resource> m_vertexBuffer;
    D3D12_VERTEX_BUFFER_VIEW m_vertexBufferView;
};

