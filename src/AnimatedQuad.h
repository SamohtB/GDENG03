#pragma once
#include "VertexTypes.h"
#include "AGameObject.h"
#include "VertexBuffer.h"
#include "TextureManager.h"

class VertexBuffer;

class AnimatedQuad : public AGameObject
{
public:
	using String = std::string;

	AnimatedQuad(int id, String name);
	~AnimatedQuad() = default;

	// Inherited via AGameObject
	void Update() override;
	void Draw(ID3D12GraphicsCommandList* cmdList) override;

private:
	std::vector<POS_POS_COL_COL> m_vertices;
	std::unique_ptr<VertexBuffer> m_vertexBuffer;
};

