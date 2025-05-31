#include "BatchUploader.h"
#include <directxtk12/BufferHelpers.h>
#include "VertexTypes.h"
#include "Debug.h"

BatchUploader::BatchUploader(ComPtr<ID3D12Device> device) : m_device(device)
{
    D3D12_COMMAND_QUEUE_DESC queueDesc = {};
    queueDesc.Flags = D3D12_COMMAND_QUEUE_FLAG_NONE;
    queueDesc.Type = D3D12_COMMAND_LIST_TYPE_DIRECT;

    Debug::ThrowIfFailed(this->m_device->CreateCommandQueue(&queueDesc, IID_PPV_ARGS(&m_commandQueue)), 
        "Command Queue creation failed!");

    this->m_resourceUploader = std::make_unique<ResourceUploadBatch>(this->m_device);
}

void BatchUploader::StartUpload()
{
    if (this->m_uploadStarted) 
    {
        Debug::LogWarning("Upload Already In Progress!");
        return;
    }
        
    this->m_uploadStarted = true;
    this->m_resourceUploader->Begin();
}

void BatchUploader::StopAndWaitUpload()
{
    auto future = this->m_resourceUploader->End(this->m_commandQueue.Get());
    future.wait();

    this->m_uploadStarted = false;
}

IndexBufferInfo BatchUploader::SchedIndexBuffer(const std::vector<unsigned int>& indices)
{
    Debug::Assert(this->m_uploadStarted, "Upload Not Yet Started! | Invalid Shedule!");

    size_t indexBufferSize = indices.size() * sizeof(unsigned int);
    IndexBufferInfo indexBufferInfo;

    Debug::ThrowIfFailed(CreateStaticBuffer(this->m_device, this->m_resourceUploader, indices.data(), indices.size(),
        D3D12_RESOURCE_STATE_INDEX_BUFFER, &indexBufferInfo.buffer));

    indexBufferInfo.view.BufferLocation = indexBufferInfo.buffer->GetGPUVirtualAddress();
    indexBufferInfo.view.SizeInBytes = static_cast<UINT>(indexBufferSize);
    indexBufferInfo.view.Format = DXGI_FORMAT_R32_UINT;

    return indexBufferInfo;
}

template<typename VertexType>
VertexBufferInfo BatchUploader::SchedVertexBuffer(const std::vector<VertexType>& vertices)
{
    Debug::Assert(this->m_uploadStarted, "Upload Not Yet Started! | Invalid Shedule!");

    size_t vertexBufferSize = vertices.size() * sizeof(VertexType);
    VertexBufferInfo vertexBufferInfo;

    Debug::ThrowIfFailed(CreateStaticBuffer(this->m_device, this->m_resourceUploader, vertices.data(), vertices.size(),
        D3D12_RESOURCE_STATE_VERTEX_AND_CONSTANT_BUFFER, &vertexBufferInfo.buffer));

    vertexBufferInfo.view.BufferLocation = vertexBufferInfo.buffer->GetGPUVirtualAddress();
    vertexBufferInfo.view.StrideInBytes = sizeof(VertexType);
    vertexBufferInfo.view.SizeInBytes = static_cast<UINT>(vertexBufferSize);

    return vertexBufferInfo;
}

template VertexBufferInfo BatchUploader::SchedVertexBuffer(const std::vector<POS_COL>&);
template VertexBufferInfo BatchUploader::SchedVertexBuffer(const std::vector<POS_TEX_COL>&);
template VertexBufferInfo BatchUploader::SchedVertexBuffer(const std::vector<POS_POS_COL_COL>&);
