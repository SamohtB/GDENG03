#include "GlobalBuffer.h"
#include "Debug.h"

GlobalBuffer::GlobalBuffer(ID3D12Device* device)
{
    const UINT totalBufferSize = BUFFER_SIZE * FRAME_COUNT;

    CD3DX12_HEAP_PROPERTIES uploadHeapProps(D3D12_HEAP_TYPE_UPLOAD);
    CD3DX12_RESOURCE_DESC resourceDesc = CD3DX12_RESOURCE_DESC::Buffer(totalBufferSize);

    Debug::ThrowIfFailed(device->CreateCommittedResource(
        &uploadHeapProps,
        D3D12_HEAP_FLAG_NONE,
        &resourceDesc,
        D3D12_RESOURCE_STATE_GENERIC_READ,
        nullptr,
        IID_PPV_ARGS(&m_constantBuffer)));

    // Map and initialize the constant buffer. We don't unmap this until the
    // app closes. Keeping things mapped for the lifetime of the resource is okay.
    CD3DX12_RANGE readRange(0, 0);
    Debug::ThrowIfFailed(m_constantBuffer->Map(0, &readRange, reinterpret_cast<void**>(&m_cbvDataBegin)));
    memset(m_cbvDataBegin, 0, totalBufferSize);
}

void GlobalBuffer::Update(float time, UINT currentFrame)
{
    this->m_constantBufferData.time = time;
    void* dest = reinterpret_cast<BYTE*>(m_cbvDataBegin) + (currentFrame * BUFFER_SIZE);
    memcpy(dest, &m_constantBufferData, sizeof(m_constantBufferData));
}

D3D12_GPU_VIRTUAL_ADDRESS GlobalBuffer::GetVirtualAddress(UINT currentFrameIndex)
{
    D3D12_GPU_VIRTUAL_ADDRESS baseAddress = m_constantBuffer->GetGPUVirtualAddress();
    D3D12_GPU_VIRTUAL_ADDRESS cbAddress = baseAddress + (currentFrameIndex * BUFFER_SIZE);

    return cbAddress;
}