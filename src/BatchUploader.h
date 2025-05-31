#pragma once
#include <memory>
#include "Dx12Commons.h"
#include <directxtk12/ResourceUploadBatch.h>

using namespace DirectX;

struct VertexBufferInfo 
{
	ComPtr<ID3D12Resource> buffer;
	D3D12_VERTEX_BUFFER_VIEW view;
};

struct IndexBufferInfo
{
	ComPtr<ID3D12Resource> buffer;
	D3D12_INDEX_BUFFER_VIEW view;
};

class BatchUploader
{
public:
	BatchUploader(ComPtr<ID3D12Device> device);
	~BatchUploader() = default;

	void StartUpload();
	void StopAndWaitUpload();

	IndexBufferInfo SchedIndexBuffer(const std::vector<unsigned int>& indices);

	template<typename VertexType>
	VertexBufferInfo SchedVertexBuffer(const std::vector<VertexType>& vertices);

private:
	ComPtr<ID3D12Device> m_device;
	ComPtr<ID3D12CommandQueue> m_commandQueue;
	std::unique_ptr<ResourceUploadBatch> m_resourceUploader;
	
	bool m_uploadStarted = false;

};
