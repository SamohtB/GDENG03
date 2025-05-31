#include "DescriptorHeapManager.h"
#include "Debug.h"

DescriptorHeapManager::DescriptorHeapManager(ID3D12Device* device, UINT maxRTVCount, UINT maxSRVCount) 
    : m_maxRTVCount(maxRTVCount), m_maxSRVCount(maxSRVCount), m_currentSRVOffset(0)
{
    /* Render Target View */
    D3D12_DESCRIPTOR_HEAP_DESC rtvHeapDesc = {};
    rtvHeapDesc.NumDescriptors = this->m_maxRTVCount;
    rtvHeapDesc.Type = D3D12_DESCRIPTOR_HEAP_TYPE_RTV;
    rtvHeapDesc.Flags = D3D12_DESCRIPTOR_HEAP_FLAG_NONE;
    Debug::ThrowIfFailed(device->CreateDescriptorHeap(&rtvHeapDesc, IID_PPV_ARGS(&this->m_rtvHeap)));
   
    this->m_rtvDescriptorSize = device->GetDescriptorHandleIncrementSize(D3D12_DESCRIPTOR_HEAP_TYPE_RTV);

    /* Shader Resource View */
    D3D12_DESCRIPTOR_HEAP_DESC srvHeapDesc = {};
    srvHeapDesc.NumDescriptors = this->m_maxSRVCount;
    srvHeapDesc.Type = D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV;
    srvHeapDesc.Flags = D3D12_DESCRIPTOR_HEAP_FLAG_SHADER_VISIBLE;
    Debug::ThrowIfFailed(device->CreateDescriptorHeap(&srvHeapDesc, IID_PPV_ARGS(&this->m_srvHeap)));

    this->m_srvDescriptorSize = device->GetDescriptorHandleIncrementSize(D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV);

    this->m_activeHeaps.push_back(m_srvHeap.Get());
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

D3D12_CPU_DESCRIPTOR_HANDLE DescriptorHeapManager::GetRTVHandleFromFrame(UINT frameIndex) const
{
    return CD3DX12_CPU_DESCRIPTOR_HANDLE(
        m_rtvHeap->GetCPUDescriptorHandleForHeapStart(),
        frameIndex,
        m_rtvDescriptorSize
    );
}

UINT DescriptorHeapManager::GetSRVDescriptorSize() const
{
    return this->m_srvDescriptorSize;
}

ID3D12DescriptorHeap* DescriptorHeapManager::GetSRVHeap() const
{
    return this->m_srvHeap.Get();
}

D3D12_CPU_DESCRIPTOR_HANDLE DescriptorHeapManager::GetNextFreeSRV_CPUHandle() const
{
    return CD3DX12_CPU_DESCRIPTOR_HANDLE(
        this->m_srvHeap->GetCPUDescriptorHandleForHeapStart(),
        this->m_currentSRVOffset,
        this->m_srvDescriptorSize
    );
}

D3D12_GPU_DESCRIPTOR_HANDLE DescriptorHeapManager::GetSRVGPUHandleFromIndex(UINT index) const
{
    return CD3DX12_GPU_DESCRIPTOR_HANDLE(
        this->m_srvHeap->GetGPUDescriptorHandleForHeapStart(),
        index,
        this->m_srvDescriptorSize
    );
}

/* Allocate First and use returned UINT to create SRV */
UINT DescriptorHeapManager::AllocateSRVSlot()
{
    Debug::Assert(this->m_currentSRVOffset < this->m_maxSRVCount, "Exceeded SRV descriptor heap capacity");
    return this->m_currentSRVOffset++;
}
