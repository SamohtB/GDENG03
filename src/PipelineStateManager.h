#pragma once
#include <unordered_map>
#include "stdafx.h"
#include "InputLayouts.h"

class PipelineStateManager
{
public:
	using PipelineMap = std::unordered_map<InputLayoutType, ComPtr<ID3D12PipelineState>>; 
	PipelineStateManager(ComPtr<ID3D12Device> device);
	~PipelineStateManager() = default;

	ComPtr<ID3D12PipelineState> GetPipelineState(InputLayoutType type) const;
	ComPtr<ID3D12RootSignature> GetRootSignature() const;

private:
	void CreateRootSignature(ComPtr<ID3D12Device> device);
	void LoadShaders();
	ComPtr<ID3D12PipelineState> CreatePipelineState(ComPtr<ID3D12Device> device, 
		const std::vector<D3D12_INPUT_ELEMENT_DESC>& inputLayout);

	PipelineMap m_pipelineStates;
	ComPtr<ID3D12RootSignature> m_rootSignature;

	/* Move Shaders to its own shader cache */
	ComPtr<ID3DBlob> vertexShader;
	ComPtr<ID3DBlob> pixelShader;
};

