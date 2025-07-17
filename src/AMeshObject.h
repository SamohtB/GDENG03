#pragma once
#include "FrameConstants.h"

#include "TextureTypes.h"
#include "ShaderTypes.h"
#include "MaterialTypes.h"

#include "AGameObject.h"
#include "IndexBuffer.h"
#include "VertexBuffer.h"

class DeviceContext;

class AMeshObject : public AGameObject
{
public:
	AMeshObject(String name, String mesh, String material = MaterialType::DEFAULT);
	virtual ~AMeshObject() = default;

	virtual void Update(float deltaTime) override;
	virtual void Draw(DeviceContext* context);

	void SetMaterial(String material);
	String GetMaterial() const;

	void SetGPUAddress(UINT frameIndex, D3D12_GPU_VIRTUAL_ADDRESS address);

protected:
	void SetTopology(D3D12_PRIMITIVE_TOPOLOGY topology);

private:
	std::vector<D3D12_GPU_VIRTUAL_ADDRESS> m_gpuAddresses;

	String m_material;
	String m_mesh;
	D3D12_PRIMITIVE_TOPOLOGY m_topology;
};