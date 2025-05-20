#pragma once
#include "stdafx.h"
#include "Helper.h"

class VertexBuffer
{
public:
    VertexBuffer() = default;
	~VertexBuffer() = default;

    template <typename VertexType>
    VertexBuffer(ID3D12Device* device, const std::vector<VertexType>& vertices);

    D3D12_VERTEX_BUFFER_VIEW* GetVertexBufferViewPointer();

private:
    ComPtr<ID3D12Resource> m_vertexBuffer;
    D3D12_VERTEX_BUFFER_VIEW m_vertexBufferView;
};

