#pragma once
#include <memory>
#include "stdafx.h"
#include "AGameObject.h"
#include "VertexTypes.h"

class VertexBuffer;

class Triangle : public AGameObject
{
public:
	using String = std::string;

	Triangle(int id, String name, ComPtr<ID3D12Device> device);
	~Triangle() = default;

	// Inherited via AGameObject
	void Update() override;
	void Draw(ID3D12GraphicsCommandList* cmdList) override;

private:
	std::vector<Vertex> m_vertices;
	std::unique_ptr<VertexBuffer> m_vertexBuffer;
};

