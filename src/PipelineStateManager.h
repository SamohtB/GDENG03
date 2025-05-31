#pragma once
#include <unordered_map>
#include <string>
#include "Dx12Commons.h"
#include "InputLayouts.h"

enum class InputLayoutType : int;

struct PipelineMapKey 
{
	InputLayoutType layout;
	std::wstring shaderName;

	bool operator==(const PipelineMapKey& other) const 
	{
		return layout == other.layout && shaderName == other.shaderName;
	}
};

namespace std 
{
	template<>
	struct hash<PipelineMapKey> 
	{
		size_t operator()(const PipelineMapKey& key) const noexcept 
		{
			size_t h1 = std::hash<int>{}(static_cast<int>(key.layout));
			size_t h2 = std::hash<std::wstring>{}(key.shaderName);
			return h1 ^ (h2 << 1);
		}
	};
}

struct ShaderDesc 
{
	std::wstring vertexPath;
	std::string vertexEntry;
	std::string vertexTarget = "vs_5_0";

	std::wstring pixelPath;
	std::string pixelEntry;
	std::string pixelTarget = "ps_5_0";
};

class PipelineStateManager
{
public:
	using PipelineMap = std::unordered_map<PipelineMapKey, ComPtr<ID3D12PipelineState>>;

	PipelineStateManager(ID3D12Device* device);
	~PipelineStateManager() = default;

	ID3D12PipelineState* GetPipelineState(InputLayoutType layout, const std::wstring& shaderName) const;
	ID3D12RootSignature* GetRootSignature() const;

	void RegisterPipeline(ID3D12Device* device, InputLayoutType layout, const ShaderDesc& desc, const std::wstring& shaderName);

private:
	void CreateRootSignature(ID3D12Device* device);
	std::pair<ComPtr<ID3DBlob>, ComPtr<ID3DBlob>> LoadShaders(const ShaderDesc& desc);
	ComPtr<ID3D12PipelineState> CreatePipelineState(ID3D12Device* device, const std::vector<D3D12_INPUT_ELEMENT_DESC>& inputLayout, ComPtr<ID3DBlob> vs, ComPtr<ID3DBlob> ps);

	PipelineMap m_pipelineStates;
	ComPtr<ID3D12RootSignature> m_rootSignature;
};

