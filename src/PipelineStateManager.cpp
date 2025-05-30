#include "PipelineStateManager.h"
#include "DxException.h"

PipelineStateManager::PipelineStateManager(ID3D12Device* device)
{
	CreateRootSignature(device);

    ShaderDesc shaderDesc = 
    {
        L"Assets/Shaders/TexturedVertexShader.hlsl", "VSMain", "vs_5_0",
        L"Assets/Shaders/TexturedPixelShader.hlsl", "PSMain", "ps_5_0"
    };

    RegisterPipeline(device, InputLayoutType::Pos_Tex_Color, shaderDesc, L"Default");

    shaderDesc =
    {
        L"Assets/Shaders/Pos2Col2Vertex.hlsl", "VSMain", "vs_5_0",
        L"Assets/Shaders/Pos2Col2Pixel.hlsl", "PSMain", "ps_5_0"
    };

    RegisterPipeline(device, InputLayoutType::Pos_Pos_Col_Col, shaderDesc, L"Animated");
}

ID3D12PipelineState* PipelineStateManager::GetPipelineState(InputLayoutType layout, const std::wstring& shaderName) const
{
	PipelineMapKey key = { layout, shaderName };
	auto it = m_pipelineStates.find(key);
	return (it != m_pipelineStates.end()) ? it->second.Get() : nullptr;
}

ID3D12RootSignature* PipelineStateManager::GetRootSignature() const
{
	return m_rootSignature.Get();
}

void PipelineStateManager::RegisterPipeline(ID3D12Device* device, InputLayoutType layout, const ShaderDesc& desc, const std::wstring& shaderName)
{
	const auto& inputLayout = InputLayouts::Get(layout);

	auto [vs, ps] = LoadShaders(desc);
	auto pso = CreatePipelineState(device, inputLayout, vs, ps);

	m_pipelineStates[{layout, shaderName}] = pso;
}

void PipelineStateManager::CreateRootSignature(ID3D12Device* device)
{
    D3D12_FEATURE_DATA_ROOT_SIGNATURE featureData = {};
    featureData.HighestVersion = D3D_ROOT_SIGNATURE_VERSION_1_1;

    if (FAILED(device->CheckFeatureSupport(D3D12_FEATURE_ROOT_SIGNATURE, &featureData, sizeof(featureData))))
        featureData.HighestVersion = D3D_ROOT_SIGNATURE_VERSION_1_0;

    CD3DX12_DESCRIPTOR_RANGE1 srvRange(D3D12_DESCRIPTOR_RANGE_TYPE_SRV, 1, 0);

    CD3DX12_ROOT_PARAMETER1 rootParams[2];
    rootParams[0].InitAsDescriptorTable(1, &srvRange, D3D12_SHADER_VISIBILITY_PIXEL);
    rootParams[1].InitAsConstantBufferView(0, 0, D3D12_ROOT_DESCRIPTOR_FLAG_NONE, D3D12_SHADER_VISIBILITY_ALL);

    D3D12_STATIC_SAMPLER_DESC sampler = {};
    sampler.Filter = D3D12_FILTER_MIN_MAG_MIP_LINEAR;
    sampler.AddressU = D3D12_TEXTURE_ADDRESS_MODE_WRAP;
    sampler.AddressV = D3D12_TEXTURE_ADDRESS_MODE_WRAP;
    sampler.AddressW = D3D12_TEXTURE_ADDRESS_MODE_WRAP;
    sampler.MipLODBias = 0;
    sampler.MaxAnisotropy = 0;
    sampler.ComparisonFunc = D3D12_COMPARISON_FUNC_NEVER;
    sampler.BorderColor = D3D12_STATIC_BORDER_COLOR_OPAQUE_BLACK;
    sampler.MinLOD = 0.0f;
    sampler.MaxLOD = D3D12_FLOAT32_MAX;
    sampler.ShaderRegister = 0;
    sampler.RegisterSpace = 0;
    sampler.ShaderVisibility = D3D12_SHADER_VISIBILITY_PIXEL;

    CD3DX12_VERSIONED_ROOT_SIGNATURE_DESC rootSignatureDesc;
    rootSignatureDesc.Init_1_1(_countof(rootParams), rootParams, 1, &sampler, D3D12_ROOT_SIGNATURE_FLAG_ALLOW_INPUT_ASSEMBLER_INPUT_LAYOUT);

    ComPtr<ID3DBlob> signature;
    ComPtr<ID3DBlob> error;
    THROW_IF_FAILED(D3DX12SerializeVersionedRootSignature(&rootSignatureDesc, featureData.HighestVersion, &signature, &error));

    THROW_IF_FAILED(device->CreateRootSignature(0, signature->GetBufferPointer(), signature->GetBufferSize(), IID_PPV_ARGS(&m_rootSignature)),
        "Root Signature Creation Failed!");
}

std::pair<ComPtr<ID3DBlob>, ComPtr<ID3DBlob>> PipelineStateManager::LoadShaders(const ShaderDesc& desc)
{
    UINT compileFlags = 0;

    ComPtr<ID3DBlob> vs;
    ComPtr<ID3DBlob> ps;
    ComPtr<ID3DBlob> err;
    const char* errorMsg;

    auto hr = D3DCompileFromFile(desc.vertexPath.c_str(), nullptr, nullptr, desc.vertexEntry.c_str(), 
        desc.vertexTarget.c_str(), compileFlags, 0, &vs, &err);
    errorMsg = static_cast<const char*>(err->GetBufferPointer());

    THROW_IF_FAILED(hr, errorMsg);

    auto hr = D3DCompileFromFile(desc.pixelPath.c_str(), nullptr, nullptr, desc.pixelEntry.c_str(), 
        desc.pixelTarget.c_str(), compileFlags, 0, &ps, &err);
    errorMsg = static_cast<const char*>(err->GetBufferPointer());

    THROW_IF_FAILED(hr, errorMsg);

    return { vs, ps };
}

ComPtr<ID3D12PipelineState> PipelineStateManager::CreatePipelineState(ID3D12Device* device, const std::vector<D3D12_INPUT_ELEMENT_DESC>& inputLayout, ComPtr<ID3DBlob> vs, ComPtr<ID3DBlob> ps)
{
    D3D12_GRAPHICS_PIPELINE_STATE_DESC psoDesc = {};
    psoDesc.InputLayout = { inputLayout.data(), static_cast<UINT>(inputLayout.size()) };
    psoDesc.pRootSignature = m_rootSignature.Get();
    psoDesc.VS = CD3DX12_SHADER_BYTECODE(vs.Get());
    psoDesc.PS = CD3DX12_SHADER_BYTECODE(ps.Get());
    psoDesc.RasterizerState = CD3DX12_RASTERIZER_DESC(D3D12_DEFAULT);
    psoDesc.BlendState = CD3DX12_BLEND_DESC(D3D12_DEFAULT);
    psoDesc.DepthStencilState.DepthEnable = FALSE;
    psoDesc.DepthStencilState.StencilEnable = FALSE;
    psoDesc.SampleMask = UINT_MAX;
    psoDesc.PrimitiveTopologyType = D3D12_PRIMITIVE_TOPOLOGY_TYPE_TRIANGLE;
    psoDesc.NumRenderTargets = 1;
    psoDesc.RTVFormats[0] = DXGI_FORMAT_R8G8B8A8_UNORM;
    psoDesc.SampleDesc.Count = 1;

    ComPtr<ID3D12PipelineState> pipelineState;
    THROW_IF_FAILED(device->CreateGraphicsPipelineState(&psoDesc, IID_PPV_ARGS(&pipelineState)), "PSO Creation Failed!");

    return pipelineState;
}
