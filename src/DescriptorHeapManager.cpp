#include "DescriptorHeapManager.h"
#include "Helper.h"

DescriptorHeapManager::DescriptorHeapManager(ComPtr<ID3D12Device> device)
{
    D3D12_DESCRIPTOR_HEAP_DESC rtvHeapDesc = {};
    rtvHeapDesc.NumDescriptors = FRAME_COUNT;
    rtvHeapDesc.Type = D3D12_DESCRIPTOR_HEAP_TYPE_RTV;
    rtvHeapDesc.Flags = D3D12_DESCRIPTOR_HEAP_FLAG_NONE;
    ThrowIfFailed(device->CreateDescriptorHeap(&rtvHeapDesc, IID_PPV_ARGS(&m_rtvHeap)));

    m_rtvDescriptorSize = device->GetDescriptorHandleIncrementSize(D3D12_DESCRIPTOR_HEAP_TYPE_RTV);
}

D3D12_CPU_DESCRIPTOR_HANDLE DescriptorHeapManager::GetRTVHeapStart()
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
