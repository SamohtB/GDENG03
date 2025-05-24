#pragma once
#include <memory>
#include "stdafx.h"
#include "AGameObject.h"
#include "VertexTypes.h"

#include "VertexBuffer.h"
#include "TextureManager.h"

class VertexBuffer;

class Triangle : public AGameObject
{
public:
	using String = std::string;
	using TextureType = TextureManager::TextureType;

	Triangle(int id, String name);
	~Triangle() = default;

	// Inherited via AGameObject
	void Update() override;
	void Draw(ID3D12GraphicsCommandList* cmdList) override;

private:
	std::vector<TexturedVertex3D> m_vertices;
	std::unique_ptr<VertexBuffer> m_vertexBuffer;
	TextureType m_texture = TextureType::CRATE;
};

