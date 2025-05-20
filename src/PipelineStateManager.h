#pragma once
#include <unordered_map>
#include "stdafx.h"
#include "InputLayouts.h"

class PipelineStateManager
{
public:
	using PipelineMap = std::unordered_map<InputLayoutType, ComPtr<ID3D12PipelineState>>; 
	PipelineStateManager(ID3D12Device* device);
	~PipelineStateManager() = default;

	ID3D12PipelineState* GetPipelineState(InputLayoutType type) const;
	ID3D12RootSignature* GetRootSignature() const;

private:
	void CreateRootSignature(ID3D12Device* device);
	void LoadShaders();
	ComPtr<ID3D12PipelineState> CreatePipelineState(ID3D12Device* device, 
		const std::vector<D3D12_INPUT_ELEMENT_DESC>& inputLayout);

	PipelineMap m_pipelineStates;
	ComPtr<ID3D12RootSignature> m_rootSignature;

	/* Move Shaders to its own shader cache */
	ComPtr<ID3DBlob> vertexShader;
	ComPtr<ID3DBlob> pixelShader;
};

