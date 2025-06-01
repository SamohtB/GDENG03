#pragma once
#include <memory>
#include <vector>

#include "VertexTypes.h"
#include "TextureTypes.h"
#include "ShaderTypes.h"

#include "AGameObject.h"
#include "IndexBuffer.h"
#include "VertexBuffer.h"

class DeviceContext;

class PBSQuads : public AGameObject
{
public:
	using String = std::string;

	PBSQuads(int id, String name, Vector2 offset);
	~PBSQuads() = default;

	// Inherited via AGameObject
	void Update() override;
	void Draw(DeviceContext* dvcContext) override;

private:
	std::vector<POS_TEX_NOR_TAN_BIT> m_vertices;
	std::vector<unsigned int> m_indices;
	ShaderType m_shader = ShaderType::DEFAULT;
	TextureType m_texture = TextureType::UNSET;
	TextureType m_normal = TextureType::UNSET;

	std::unique_ptr<VertexBuffer> m_vertexBuffer;
	std::unique_ptr<IndexBuffer> m_indexBuffer;
};