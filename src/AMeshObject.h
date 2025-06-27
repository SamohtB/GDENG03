#pragma once
#include <memory>
#include <vector>
#include <string>

#include "ShaderNames.h"
#include "MaterialNames.h"

#include "AGameObject.h"
#include "IndexBuffer.h"
#include "VertexBuffer.h"
#include "ConstantBuffer.h"

class DeviceContext;

class AMeshObject : public AGameObject
{
public:
	virtual void Update(float deltaTime) override;
	virtual void Draw(DeviceContext* context) override;

protected:
	AMeshObject(String name, String shaderName = ShaderNames::LIT, String materialName = MaterialNames::DEFAULT);
	virtual ~AMeshObject() = default;

	void SetGeometry(std::vector<Vertex> vertices, std::vector<unsigned int> indices);
	void SetTopology(D3D12_PRIMITIVE_TOPOLOGY topology);

private:
	/* To DO: Move Buffers to a Mesh Manager and Create a primitve factory */
	std::unique_ptr<VertexBuffer> m_vertexBuffer;
	std::unique_ptr<IndexBuffer> m_indexBuffer;
	std::unique_ptr<ObjectConstantsBuffer> m_constantBuffer;
	UINT m_indicesSize;
	String m_shaderName;
	String m_materialName;
	D3D12_PRIMITIVE_TOPOLOGY m_topology;
};

