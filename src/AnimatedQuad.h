#pragma once
#include <memory>
#include <vector>

#include "VertexTypes.h"
#include "ShaderTypes.h"

#include "AGameObject.h"
#include "IndexBuffer.h"
#include "VertexBuffer.h"

class DeviceContext;

class AnimatedQuad : public AGameObject
{
public:
	using String = std::string;

	AnimatedQuad(int id, String name, Vector2 offset);
	~AnimatedQuad() = default;

	// Inherited via AGameObject
	void Update() override;
	void Draw(DeviceContext* dvcContext) override;

private:
	UINT m_indicesSize = 0;
	ShaderType m_shader = ShaderType::ANIMATED;

	std::unique_ptr<VertexBuffer> m_vertexBuffer;
	std::unique_ptr<IndexBuffer> m_indexBuffer;
};