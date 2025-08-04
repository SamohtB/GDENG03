#pragma once
#include "AComponent.h"
#include "FrameConstants.h"

#include "TextureTypes.h"
#include "ShaderTypes.h"
#include "MaterialTypes.h"

#include "IndexBuffer.h"
#include "VertexBuffer.h"

class DeviceContext;

class MeshComponent : public AComponent
{
public:
	MeshComponent(String name, String mesh, std::weak_ptr<AGameObject> owner);
	~MeshComponent() = default;

	void SetDeviceContext(DeviceContext* context);

	void Perform() override;
	void DrawUI() override;

	void SetMaterial(String material);
	String GetMaterial() const;

	void SetGPUAddress(UINT frameIndex, D3D12_GPU_VIRTUAL_ADDRESS address);
	void SetTopology(D3D12_PRIMITIVE_TOPOLOGY topology);

	String GetMeshType() const;
private:
	std::vector<D3D12_GPU_VIRTUAL_ADDRESS> m_gpuAddresses;

	DeviceContext* m_context;
	String m_material;
	String m_mesh;
	D3D12_PRIMITIVE_TOPOLOGY m_topology;

};

