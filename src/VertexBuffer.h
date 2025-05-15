#pragma once
#include "VertexTypes.h"
#include "stdafx.h"

class VertexBuffer
{
public:
    VertexBuffer(ComPtr<ID3D12Device> device, const std::vector<Vertex>& vertices);
	~VertexBuffer() = default;

    D3D12_VERTEX_BUFFER_VIEW* GetVertexBufferViewPointer();

private:
    ComPtr<ID3D12Resource> m_vertexBuffer;
    D3D12_VERTEX_BUFFER_VIEW m_vertexBufferView;
};

