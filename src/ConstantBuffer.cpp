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
    /* Set Values */
    this->m_constantBufferData.diffuseHandleIndex = mat.albedo;
    this->m_constantBufferData.normalHandleIndex = mat.normal;
    this->m_constantBufferData.normalStr = mat.normalValue;
    this->m_constantBufferData.roughHandleIndex = mat.roughness;
    this->m_constantBufferData.roughStr = mat.roughnessValue;

    memcpy(m_cbvDataBegin, &m_constantBufferData, sizeof(m_constantBufferData));
}
