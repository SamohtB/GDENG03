#pragma once
#include <d3d12.h>
#include <wrl.h>
#include "MaterialTypes.h"

using Microsoft::WRL::ComPtr;

class ConstantBuffer
{
public:

	struct alignas(256) SceneConstantBuffer 
	{
		UINT diffuseHandleIndex = 0;
		UINT normalHandleIndex = 0;
		float normalStr = 0;
		UINT roughHandleIndex = 0;
		float roughStr = 0;
	};

	ConstantBuffer(ComPtr<ID3D12Resource> buffer);
	~ConstantBuffer() = default;

	void Update(MaterialDescription mat);

	static constexpr UINT BUFFER_SIZE = (sizeof(SceneConstantBuffer) + 255) & ~255;

private:
	ComPtr<ID3D12Resource> m_constantBuffer;
	SceneConstantBuffer m_constantBufferData {};
	UINT8* m_cbvDataBegin;
};