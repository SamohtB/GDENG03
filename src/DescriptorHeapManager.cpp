#include "DescriptorHeapManager.h"
#include "Helper.h"

DescriptorHeapManager::DescriptorHeapManager(ComPtr<ID3D12Device> device)
{
    /* Render Target View */
    D3D12_DESCRIPTOR_HEAP_DESC rtvHeapDesc = {};
    rtvHeapDesc.NumDescriptors = FRAME_COUNT;
    rtvHeapDesc.Type = D3D12_DESCRIPTOR_HEAP_TYPE_RTV;
    rtvHeapDesc.Flags = D3D12_DESCRIPTOR_HEAP_FLAG_NONE;
    ThrowIfFailed(device->CreateDescriptorHeap(&rtvHeapDesc, IID_PPV_ARGS(&m_rtvHeap)));

    /* Shader Resource View */
    D3D12_DESCRIPTOR_HEAP_DESC srvHeapDesc = {};
    srvHeapDesc.NumDescriptors = 1;
    srvHeapDesc.Type = D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV;
    srvHeapDesc.Flags = D3D12_DESCRIPTOR_HEAP_FLAG_SHADER_VISIBLE;
    ThrowIfFailed(device->CreateDescriptorHeap(&srvHeapDesc, IID_PPV_ARGS(&m_srvHeap)));

    m_rtvDescriptorSize = device->GetDescriptorHandleIncrementSize(D3D12_DESCRIPTOR_HEAP_TYPE_RTV);
}

D3D12_CPU_DESCRIPTOR_HANDLE DescriptorHeapManager::GetRTVHeapStart() const
{
	return m_rtvHeap->GetCPUDescriptorHandleForHeapStart();
}

UINT DescriptorHeapManager::GetRTVDescriptorSize() const
{
	return m_rtvDescriptorSize;
}

ComPtr<ID3D12DescriptorHeap> DescriptorHeapManager::GetRTVHeap() const
{
    return m_rtvHeap;
}

ComPtr<ID3D12DescriptorHeap> DescriptorHeapManager::GetSRVHeap() const
{
    return m_srvHeap;
}
