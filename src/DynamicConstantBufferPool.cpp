#include "DynamicConstantBufferPool.h"
#include "FrameConstants.h"
#include "Debug.h"

DynamicConstantBufferPool::DynamicConstantBufferPool(ID3D12Device* device, size_t bufferSizePerFrame)
{
    auto size = Align(bufferSizePerFrame);
	m_bufferSizePerFrame = size;
	m_currentFrameIndex = 0;
	m_currentOffset = 0;
    CreateBuffers(device);
}

DynamicConstantBufferPool::~DynamicConstantBufferPool()
{
    for (UINT i = 0; i < FRAME_COUNT; ++i)
    {
        if (m_mappedPtrs[i] != nullptr && m_buffers[i] != nullptr)
        {
            D3D12_RANGE emptyRange = {};
            m_buffers[i]->Unmap(0, &emptyRange);
            m_mappedPtrs[i] = nullptr;
        }

        if (m_buffers[i])
        {
            m_buffers[i]->Release();
            m_buffers[i] = nullptr;
        }
    }
}

void DynamicConstantBufferPool::BeginFrame(uint32_t frameIndex)
{
	m_currentFrameIndex = frameIndex;
	m_currentOffset = 0;
}

DynamicConstantBufferPool::Allocation DynamicConstantBufferPool::Allocate(size_t size)
{
    size = Align(size);

    Debug::Assert(m_currentOffset + size <= m_bufferSizePerFrame, "DynamicConstantBufferPool overflow");

    uint8_t* cpuBase = reinterpret_cast<uint8_t*>(m_mappedPtrs[m_currentFrameIndex]);
    D3D12_GPU_VIRTUAL_ADDRESS gpuBase = m_buffers[m_currentFrameIndex]->GetGPUVirtualAddress();

    Allocation result;
    result.cpuPtr = cpuBase + m_currentOffset;
    result.gpuAddr = gpuBase + m_currentOffset;

    m_currentOffset += size;
    return result;
}

ID3D12Resource* DynamicConstantBufferPool::GetCurrentResource() const
{
    return m_buffers[m_currentFrameIndex].Get();
}

void DynamicConstantBufferPool::CreateBuffers(ID3D12Device* device)
{
    m_buffers.resize(FRAME_COUNT);
    m_mappedPtrs.resize(FRAME_COUNT);

    for (uint32_t i = 0; i < FRAME_COUNT; ++i) 
    {
        CD3DX12_HEAP_PROPERTIES heapProps(D3D12_HEAP_TYPE_UPLOAD);
        CD3DX12_RESOURCE_DESC bufferDesc = CD3DX12_RESOURCE_DESC::Buffer(m_bufferSizePerFrame);

        Debug::ThrowIfFailed(device->CreateCommittedResource(
            &heapProps,
            D3D12_HEAP_FLAG_NONE,
            &bufferDesc,
            D3D12_RESOURCE_STATE_GENERIC_READ,
            nullptr,
            IID_PPV_ARGS(m_buffers[i].GetAddressOf())
        ));

        void* mappedPtr = nullptr;
        D3D12_RANGE readRange = { 0, 0 };
        m_buffers[i]->Map(0, &readRange, &mappedPtr);
        m_mappedPtrs[i] = mappedPtr;
    }
}
