#pragma once
#include <memory>
#include <vector>

#include "VertexTypes.h"
#include "AGameObject.h"
#include "VertexBuffer.h"

class DeviceContext;
class VertexBuffer;

class Quad : public AGameObject
{
public:
	using String = std::string;

	Quad(int id, String name, XMFLOAT2 offset);
	~Quad() = default;

	// Inherited via AGameObject
	void Update() override;
	void Draw(DeviceContext* dvcContext) override;

private:
	std::vector<POS_COL> m_vertices;
	std::vector<UINT> m_indices;
	std::unique_ptr<VertexBuffer> m_vertexBuffer;
};

