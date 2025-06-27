#pragma once
#include "pch.h"
#include <dxcapi.h>
#include "GraphicsCommons.h"
#include "ShaderLoader.h"
#include "ShaderNames.h"

class PipelineStateManager
{
public:
	using PipelineMap = std::unordered_map<std::string, ComPtr<ID3D12PipelineState>>;

	PipelineStateManager(ID3D12Device* device);
	~PipelineStateManager() = default;

	ID3D12PipelineState* GetPipelineState(const std::string& shaderName) const;
	ID3D12RootSignature* GetRootSignature() const;

	void RegisterPipeline(ID3D12Device* device, const std::string& shaderName, const ShaderDesc& vertexDesc, const ShaderDesc& pixelDesc);

private:
	void CreateRootSignature(ID3D12Device* device);
	ComPtr<ID3D12PipelineState> CreatePipelineState(ID3D12Device* device, ComPtr<IDxcBlob> vs, ComPtr<IDxcBlob> ps);

	PipelineMap m_pipelineStates;
	ComPtr<ID3D12RootSignature> m_rootSignature;
	std::unique_ptr<ShaderLoader> m_shaderLoader;
};