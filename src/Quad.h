#pragma once
#include <memory>
#include "stdafx.h"
#include "VertexTypes.h"

#include "AGameObject.h"
#include "VertexBuffer.h"
#include "TextureManager.h"

class VertexBuffer;

class Quad : public AGameObject
{
public:
	using String = std::string;

	Quad(int id, String name, XMFLOAT2 offset, std::vector<XMFLOAT4> colors);
	~Quad() = default;

	// Inherited via AGameObject
	void Update() override;
	void Draw(ID3D12GraphicsCommandList* cmdList) override;

private:
	std::vector<TexturedVertex3D> m_vertices;
	std::unique_ptr<VertexBuffer> m_vertexBuffer;
};

