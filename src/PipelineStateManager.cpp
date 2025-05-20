#include "PipelineStateManager.h"
#include "Helper.h"


PipelineStateManager::PipelineStateManager(ComPtr<ID3D12Device> device)
{
	CreateRootSignature(device);
    LoadShaders();

    for (int i = 0; i < InputLayoutType_Count; i++)
    {
        InputLayoutType layoutType = static_cast<InputLayoutType>(i);
        auto layout = InputLayouts::Get(layoutType);
        ComPtr<ID3D12PipelineState> pso = CreatePipelineState(device, layout);
        m_pipelineStates[layoutType] = pso;
    }

}

ComPtr<ID3D12PipelineState> PipelineStateManager::GetPipelineState(InputLayoutType type) const
{
    return this->m_pipelineStates.at(type);
}

ComPtr<ID3D12RootSignature> PipelineStateManager::GetRootSignature() const
{
    return this->m_rootSignature;
}

/* What resources the shaders need access to */
void PipelineStateManager::CreateRootSignature(ComPtr<ID3D12Device> device)
{
    D3D12_FEATURE_DATA_ROOT_SIGNATURE featureData = {};

    featureData.HighestVersion = D3D_ROOT_SIGNATURE_VERSION_1_1;

    if (FAILED(device->CheckFeatureSupport(D3D12_FEATURE_ROOT_SIGNATURE, &featureData, sizeof(featureData))))
    {
        featureData.HighestVersion = D3D_ROOT_SIGNATURE_VERSION_1_0;
    }

    CD3DX12_DESCRIPTOR_RANGE1 ranges[1];
    ranges[0].Init(D3D12_DESCRIPTOR_RANGE_TYPE_SRV, // Type
        1,                                          // number of descriptors (num of resources)
        0,                                          // BaseShaderRegister: register(t0)
        0,                                          // RegisterSpace : space0
        D3D12_DESCRIPTOR_RANGE_FLAG_DATA_STATIC
    );

    CD3DX12_ROOT_PARAMETER1 rootParameters[1];

    /* index 0 exposes 1 texture in pixel shader */
    rootParameters[0].InitAsDescriptorTable(1, 
        &ranges[0], 
        D3D12_SHADER_VISIBILITY_PIXEL
    );

    /* Texture Sampler */
    D3D12_STATIC_SAMPLER_DESC sampler = {};
    sampler.Filter = D3D12_FILTER_MIN_MAG_MIP_POINT;
    sampler.AddressU = D3D12_TEXTURE_ADDRESS_MODE_BORDER;
    sampler.AddressV = D3D12_TEXTURE_ADDRESS_MODE_BORDER;
    sampler.AddressW = D3D12_TEXTURE_ADDRESS_MODE_BORDER;
    sampler.MipLODBias = 0;
    sampler.MaxAnisotropy = 0;
    sampler.ComparisonFunc = D3D12_COMPARISON_FUNC_NEVER;
    sampler.BorderColor = D3D12_STATIC_BORDER_COLOR_TRANSPARENT_BLACK;
    sampler.MinLOD = 0.0f;
    sampler.MaxLOD = D3D12_FLOAT32_MAX;
    sampler.ShaderRegister = 0;
    sampler.RegisterSpace = 0;
    sampler.ShaderVisibility = D3D12_SHADER_VISIBILITY_PIXEL;

    CD3DX12_VERSIONED_ROOT_SIGNATURE_DESC rootSignatureDesc;

	rootSignatureDesc.Init_1_1(
        _countof(rootParameters),                                       // Params Size
        rootParameters,                                                 // Params Data
        1,                                                              // Static Sampler Size
        &sampler,                                                       // Static Sampler Data
        D3D12_ROOT_SIGNATURE_FLAG_ALLOW_INPUT_ASSEMBLER_INPUT_LAYOUT    // Flags
    );

	ComPtr<ID3DBlob> signature;
	ComPtr<ID3DBlob> error;
	ThrowIfFailed(D3DX12SerializeVersionedRootSignature(&rootSignatureDesc, featureData.HighestVersion, &signature, &error));
	ThrowIfFailed(device->CreateRootSignature(0, signature->GetBufferPointer(), signature->GetBufferSize(), IID_PPV_ARGS(&m_rootSignature)));
}

void PipelineStateManager::LoadShaders()
{
    UINT compileFlags = 0;

    std::wstring shaderFile = GetFullAssetPath(L"Assets\\Shaders\\Shader.hlsl");

    ThrowIfFailed(D3DCompileFromFile(shaderFile.c_str(), nullptr, nullptr, "VSMain", "vs_5_0", compileFlags, 0, &vertexShader, nullptr));
    ThrowIfFailed(D3DCompileFromFile(shaderFile.c_str(), nullptr, nullptr, "PSMain", "ps_5_0", compileFlags, 0, &pixelShader, nullptr));
}


/* Input Layout and Specific Shader Combo */
ComPtr<ID3D12PipelineState> PipelineStateManager::CreatePipelineState(
    ComPtr<ID3D12Device> device, 
    const std::vector<D3D12_INPUT_ELEMENT_DESC>& inputLayout)
{
    // Describe and create the graphics pipeline state object (PSO).
    D3D12_GRAPHICS_PIPELINE_STATE_DESC psoDesc = {};
    psoDesc.InputLayout = { inputLayout.data(), static_cast<UINT>(inputLayout.size()) };
    psoDesc.pRootSignature = m_rootSignature.Get();
    psoDesc.VS = CD3DX12_SHADER_BYTECODE(vertexShader.Get());
    psoDesc.PS = CD3DX12_SHADER_BYTECODE(pixelShader.Get());
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
    ThrowIfFailed(device->CreateGraphicsPipelineState(&psoDesc, IID_PPV_ARGS(&pipelineState)));
    return pipelineState;
}
