#include "ConstantBuffer.h"
#include "Debug.h"


ConstantBuffer::ConstantBuffer(ID3D12Device* device)
{
    /* We make 2 for each RTV */
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

    CD3DX12_RANGE readRange(0, 0);
    Debug::ThrowIfFailed(m_constantBuffer->Map(0, &readRange, reinterpret_cast<void**>(&m_pCbvDataBegin)));
    memset(m_pCbvDataBegin, 0, totalBufferSize);
}

void ConstantBuffer::Update(DirectX::SimpleMath::Vector3 matValues, UINT currentFrameIndex)
{
    /* Set Values */
    this->m_constantBufferData.normal = matValues.x;
    this->m_constantBufferData.roughness = matValues.y;
    this->m_constantBufferData.metalness = matValues.z;

    void* dest = reinterpret_cast<BYTE*>(m_pCbvDataBegin) + (currentFrameIndex * BUFFER_SIZE);
    memcpy(dest, &m_constantBufferData, sizeof(m_constantBufferData));
}

D3D12_GPU_VIRTUAL_ADDRESS ConstantBuffer::GetVirtualAddress(UINT currentFrameIndex)
{
    D3D12_GPU_VIRTUAL_ADDRESS baseAddress = m_constantBuffer->GetGPUVirtualAddress();
    D3D12_GPU_VIRTUAL_ADDRESS cbAddress = baseAddress + (currentFrameIndex * BUFFER_SIZE);

    return cbAddress;
}
