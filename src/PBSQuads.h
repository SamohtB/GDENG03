#pragma once
#include <memory>
#include <vector>

#include "FrameConstants.h"
#include "VertexTypes.h"
#include "TextureTypes.h"
#include "ShaderTypes.h"
#include "MaterialTypes.h"

#include "AGameObject.h"
#include "IndexBuffer.h"
#include "VertexBuffer.h"

class DeviceContext;

class PBSQuads : public AGameObject
{
public:
	using String = std::string;

	PBSQuads(String name, Vector2 offset, MaterialType matType);
	~PBSQuads() = default;

	// Inherited via AGameObject
	void Update(float deltaTime) override;
	void Draw(DeviceContext* dvcContext) override;

private:
	UINT m_indicesSize = 0;
	ShaderType m_shader = ShaderType::DEFAULT_SHADER;
	MaterialType m_material = MaterialType::DEFAULT_MAT;

	std::unique_ptr<VertexBuffer> m_vertexBuffer;
	std::unique_ptr<IndexBuffer> m_indexBuffer;
};