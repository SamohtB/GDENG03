#include "ConstantBuffer.h"
#include "GraphicsEngine.h"
#include "RenderSystem.h"
#include "DescriptorHeapManager.h"
#include "Debug.h"

ConstantBuffer::ConstantBuffer(ComPtr<ID3D12Resource> buffer) : m_constantBuffer(buffer)
{
    CD3DX12_RANGE readRange(0, 0);
    Debug::ThrowIfFailed(m_constantBuffer->Map(0, &readRange, reinterpret_cast<void**>(&m_cbvDataBegin)));
}

void ConstantBuffer::Update(MaterialDescription mat)
{
    this->m_constantBufferData.diffuseHandleIndex = mat.albedoTextureIndex;
    this->m_constantBufferData.normalHandleIndex = mat.normalTextureIndex;
    this->m_constantBufferData.normalStr = mat.normalStr;
    this->m_constantBufferData.metalHandleIndex = mat.metallicTextureIndex;
    this->m_constantBufferData.metalStr = mat.metallicStr;
    this->m_constantBufferData.roughHandleIndex = mat.roughTextureIndex;
    this->m_constantBufferData.roughStr = mat.roughStr;
    this->m_constantBufferData.ambientOcclussionHandleIndex = mat.ambientOcclusionTextureIndex;
    this->m_constantBufferData.ambientOcclussionStr = mat.ambientOcclusionStr;
    this->m_constantBufferData.materialFlags = MaterialData::GetFlags(mat);

    memcpy(m_cbvDataBegin, &m_constantBufferData, sizeof(m_constantBufferData));
}