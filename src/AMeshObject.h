#pragma once
#include <memory>
#include <vector>
#include <string>

#include "VertexTypes.h"
#include "TextureTypes.h"
#include "ShaderTypes.h"
#include "MaterialTypes.h"

#include "AGameObject.h"
#include "IndexBuffer.h"
#include "VertexBuffer.h"
#include "Colors.h"

class DeviceContext;

class AMeshObject : public AGameObject
{
public:
	using String = std::string;
	AMeshObject(String name, Vector3 color = ColorPalette::White, ShaderType shader = ShaderType::DEFAULT_SHADER);
	virtual ~AMeshObject() = default;

	virtual void Update(float deltaTime) override final;
	virtual void Draw(DeviceContext* context) override;

protected:
	template<typename T>
	void SetGeometry(std::vector<T> vertices, std::vector<unsigned int> indices);
	void SetTopology(D3D12_PRIMITIVE_TOPOLOGY topology);
	virtual void OnUpdate(float deltaTime) = 0;

private:
	Vector3 m_color = ColorPalette::White; // Default color for the primitive

	/* To DO: Move Buffers to a Mesh Manager and Create a primitve factory */
	std::unique_ptr<VertexBuffer> m_vertexBuffer;
	std::unique_ptr<IndexBuffer> m_indexBuffer;
	UINT m_indicesSize = 0;
	ShaderType m_shader = ShaderType::DEFAULT_SHADER;
	D3D12_PRIMITIVE_TOPOLOGY m_topology = D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST;
};

