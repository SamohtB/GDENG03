#include "ConstantBuffer.h"
#include "Helper.h"
#include "GraphicsEngine.h"

ConstantBuffer::ConstantBuffer(ID3D12Device* device)
{
    const UINT totalBufferSize = BUFFER_SIZE * FRAME_COUNT;

    CD3DX12_HEAP_PROPERTIES uploadHeapProps(D3D12_HEAP_TYPE_UPLOAD);
    CD3DX12_RESOURCE_DESC resourceDesc = CD3DX12_RESOURCE_DESC::Buffer(totalBufferSize);

    ThrowIfFailed(device->CreateCommittedResource(
        &uploadHeapProps,
        D3D12_HEAP_FLAG_NONE,
        &resourceDesc,
        D3D12_RESOURCE_STATE_GENERIC_READ,
        nullptr,
        IID_PPV_ARGS(&m_constantBuffer)));

    // Map and initialize the constant buffer. We don't unmap this until the
    // app closes. Keeping things mapped for the lifetime of the resource is okay.
    CD3DX12_RANGE readRange(0, 0);
    ThrowIfFailed(m_constantBuffer->Map(0, &readRange, reinterpret_cast<void**>(&m_pCbvDataBegin)));
    memset(m_pCbvDataBegin, 0, totalBufferSize);
}

void ConstantBuffer::Update(float time, UINT currentFrameIndex)
{
    this->m_constantBufferData.time = time;
    void* dest = reinterpret_cast<BYTE*>(m_pCbvDataBegin) + (currentFrameIndex * BUFFER_SIZE);
    memcpy(dest, &m_constantBufferData, sizeof(m_constantBufferData));
}

D3D12_GPU_VIRTUAL_ADDRESS ConstantBuffer::GetVirtualAddress(UINT currentFrameIndex)
{
    D3D12_GPU_VIRTUAL_ADDRESS baseAddress = m_constantBuffer->GetGPUVirtualAddress();
    D3D12_GPU_VIRTUAL_ADDRESS cbAddress = baseAddress + (currentFrameIndex * BUFFER_SIZE);

    return cbAddress;
}
