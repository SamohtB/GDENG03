#pragma once
#include <d3d12.h>
#include <wrl.h>
#include "MaterialTypes.h"

using Microsoft::WRL::ComPtr;

class MaterialBuffer
{
public:

	struct alignas(256) SceneConstantBuffer
	{
		uint32_t diffuseHandleIndex = 0;
		uint32_t normalHandleIndex = 0;
		float normalStr = 0;
		uint32_t metalHandleIndex = 0;
		float metalStr = 0;
		uint32_t roughHandleIndex = 0;
		float roughStr = 0;
		uint32_t ambientOcclussionHandleIndex = 0;
		float ambientOcclussionStr = 0;
		uint32_t materialFlags = 0;
	};


	MaterialBuffer(ComPtr<ID3D12Resource> buffer);
	~MaterialBuffer() = default;

	void Update(MaterialDescription mat);

	static constexpr UINT BUFFER_SIZE = (sizeof(SceneConstantBuffer) + 255) & ~255;

private:
	ComPtr<ID3D12Resource> m_constantBuffer;
	SceneConstantBuffer m_constantBufferData {};
	UINT8* m_cbvDataBegin;
};