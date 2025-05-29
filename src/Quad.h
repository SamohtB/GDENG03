#pragma once
#include <memory>
#include "stdafx.h"
#pragma once
#include "VertexTypes.h"
#include "AGameObject.h"
#include "VertexBuffer.h"
#include "TextureManager.h"

class VertexBuffer;

class Quad : public AGameObject
{
public:
	using String = std::string;

	Quad(int id, String name, XMFLOAT2 offset);
	~Quad() = default;

	// Inherited via AGameObject
	void Update() override;
	void Draw(ID3D12GraphicsCommandList* cmdList) override;

private:
	std::vector<POS_TEX_COL> m_vertices;
	std::unique_ptr<VertexBuffer> m_vertexBuffer;
};

