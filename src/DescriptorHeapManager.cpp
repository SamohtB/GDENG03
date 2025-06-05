#include "DescriptorHeapManager.h"
#include <directxtk12/DirectXHelpers.h>
#include "Debug.h"

DescriptorHeapManager::DescriptorHeapManager(ID3D12Device* device, UINT maxRTVCount, UINT maxSRVCount, UINT maxCBVCount)
    : m_maxRTVCount(maxRTVCount), m_maxSRVCount(maxSRVCount), m_maxCBVCount(maxCBVCount), 
    m_currentSRVOffset(0), m_currentCBVOffset(maxSRVCount), m_cbvEnd(maxSRVCount + maxCBVCount)
{
    /* Render Target View */
    D3D12_DESCRIPTOR_HEAP_DESC rtvHeapDesc = {};
    rtvHeapDesc.NumDescriptors = this->m_maxRTVCount;
    rtvHeapDesc.Type = D3D12_DESCRIPTOR_HEAP_TYPE_RTV;
    rtvHeapDesc.Flags = D3D12_DESCRIPTOR_HEAP_FLAG_NONE;
    Debug::ThrowIfFailed(device->CreateDescriptorHeap(&rtvHeapDesc, IID_PPV_ARGS(&this->m_rtvHeap)));
   
    this->m_rtvDescriptorSize = device->GetDescriptorHandleIncrementSize(D3D12_DESCRIPTOR_HEAP_TYPE_RTV);

    /* Shader Resource View + Constant Buffer View */
    D3D12_DESCRIPTOR_HEAP_DESC srvHeapDesc = {};
    srvHeapDesc.NumDescriptors = m_cbvEnd;
    srvHeapDesc.Type = D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV;
    srvHeapDesc.Flags = D3D12_DESCRIPTOR_HEAP_FLAG_SHADER_VISIBLE;
    Debug::ThrowIfFailed(device->CreateDescriptorHeap(&srvHeapDesc, IID_PPV_ARGS(&this->m_shaderVisibleHeap)));

    this->m_shaderVisibleDescriptorSize = device->GetDescriptorHandleIncrementSize(D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV);

    /* add for binding */
    this->m_activeHeaps.push_back(m_shaderVisibleHeap.Get());
}

const std::vector<ID3D12DescriptorHeap*>& DescriptorHeapManager::GetActiveHeaps() const
{
    return this->m_activeHeaps;
}

UINT DescriptorHeapManager::GetRTVDescriptorSize() const
{
	return this->m_rtvDescriptorSize;
}

ID3D12DescriptorHeap* DescriptorHeapManager::GetRTVHeap() const
{
    return this->m_rtvHeap.Get();
}

D3D12_CPU_DESCRIPTOR_HANDLE DescriptorHeapManager::GetRTVCPUHandleAt(UINT frameIndex) const
{
    return CD3DX12_CPU_DESCRIPTOR_HANDLE(
        m_rtvHeap->GetCPUDescriptorHandleForHeapStart(),
        frameIndex,
        m_rtvDescriptorSize
    );
}

UINT DescriptorHeapManager::GetShaderVisibleDescriptorSize() const
{
    return this->m_shaderVisibleDescriptorSize;
}

UINT DescriptorHeapManager::AllocateSRVSlot()
{
    Debug::Assert(this->m_currentSRVOffset < this->m_maxSRVCount, "Exceeded SRV descriptor heap capacity");
    return this->m_currentSRVOffset++;
}

UINT DescriptorHeapManager::AllocateCBVSlot()
{
    Debug::Assert(this->m_currentCBVOffset < m_cbvEnd, "Exceeded CBV descriptor heap capacity");
    return this->m_currentCBVOffset++;
}

D3D12_CPU_DESCRIPTOR_HANDLE DescriptorHeapManager::GetShaderVisibleCPUHandleAt(UINT index) const
{
    return CD3DX12_CPU_DESCRIPTOR_HANDLE(
        this->m_shaderVisibleHeap->GetCPUDescriptorHandleForHeapStart(),
        index,
        this->m_shaderVisibleDescriptorSize
    );
}

D3D12_GPU_DESCRIPTOR_HANDLE DescriptorHeapManager::GetShaderVisibleGPUHandleAt(UINT index) const
{
    return CD3DX12_GPU_DESCRIPTOR_HANDLE(
        this->m_shaderVisibleHeap->GetGPUDescriptorHandleForHeapStart(),
        index,
        this->m_shaderVisibleDescriptorSize
    );
}

