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

class Quad : public AGameObject
{
public:
	using String = std::string;

	Quad(int id, String name, Vector2 offset);
	~Quad() = default;

	// Inherited via AGameObject
	void Update() override;
	void Draw(DeviceContext* dvcContext) override;

private:
	std::vector<POS_TEX_COL> m_vertices;
	std::vector<unsigned int> m_indices;
	ShaderType m_shader = ShaderType::DEFAULT_SHADER;
	TextureType m_texture = TextureType::DEFAULT_TEXTURE;

	std::unique_ptr<VertexBuffer> m_vertexBuffer;
	std::unique_ptr<IndexBuffer> m_indexBuffer;
};

