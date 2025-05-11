#pragma once
#include "stdafx.h"

class PipelineStateManager
{
public:
	PipelineStateManager(ComPtr<ID3D12Device> device);
	~PipelineStateManager() = default;

	ComPtr<ID3D12PipelineState> GetPipelineState();
	ComPtr<ID3D12RootSignature> GetRootSignature();

private:
	void CreateRootSignature(ComPtr<ID3D12Device> device, 
		std::vector<CD3DX12_ROOT_PARAMETER>& rootParams,
		D3D12_ROOT_SIGNATURE_FLAGS flags);

	void CreatePipelineState(ComPtr<ID3D12Device> device);

	ComPtr<ID3D12PipelineState> m_pipelineState;
	ComPtr<ID3D12RootSignature> m_rootSignature;
};

