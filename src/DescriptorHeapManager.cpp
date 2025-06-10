#include "DescriptorHeapManager.h"
#include <directxtk12/DirectXHelpers.h>
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
    D3D12_DESCRIPTOR_HEAP_DESC  srvHeapDesc = {};
    srvHeapDesc.NumDescriptors = this->m_maxSRVCount;
    srvHeapDesc.Type = D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV;
    srvHeapDesc.Flags = D3D12_DESCRIPTOR_HEAP_FLAG_SHADER_VISIBLE;
    Debug::ThrowIfFailed(device->CreateDescriptorHeap(&srvHeapDesc, IID_PPV_ARGS(&this->m_srvHeap)));

    this->m_srvDescriptorSize = device->GetDescriptorHandleIncrementSize(D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV);

    /* Depth Stencil View */
    D3D12_DESCRIPTOR_HEAP_DESC dsvHeapDesc = {};
    dsvHeapDesc.NumDescriptors = 1;
    dsvHeapDesc.Type = D3D12_DESCRIPTOR_HEAP_TYPE_DSV;
    dsvHeapDesc.Flags = D3D12_DESCRIPTOR_HEAP_FLAG_NONE;
    Debug::ThrowIfFailed(device->CreateDescriptorHeap(&dsvHeapDesc, IID_PPV_ARGS(&m_dsvHeap)));

    /* add for binding */
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

D3D12_CPU_DESCRIPTOR_HANDLE DescriptorHeapManager::GetRTVCPUHandleAt(UINT frameIndex) const
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

UINT DescriptorHeapManager::AllocateSRVSlot()
{
    Debug::Assert(this->m_currentSRVOffset < this->m_maxSRVCount, "Exceeded SRV descriptor heap capacity");
    return this->m_currentSRVOffset++;
}

D3D12_CPU_DESCRIPTOR_HANDLE DescriptorHeapManager::GetSRVCPUHandleAt(UINT index) const
{
    return CD3DX12_CPU_DESCRIPTOR_HANDLE(
        this->m_srvHeap->GetCPUDescriptorHandleForHeapStart(),
        index,
        this->m_srvDescriptorSize
    );
}

D3D12_GPU_DESCRIPTOR_HANDLE DescriptorHeapManager::GetSRVGPUHandleAt(UINT index) const
{
    return CD3DX12_GPU_DESCRIPTOR_HANDLE(
        this->m_srvHeap->GetGPUDescriptorHandleForHeapStart(),
        index,
        this->m_srvDescriptorSize
    );
}

D3D12_CPU_DESCRIPTOR_HANDLE DescriptorHeapManager::GetDSVCPUHandle() const
{
    return this->m_dsvHeap->GetCPUDescriptorHandleForHeapStart();
}

