#pragma once
#include <memory>
#include <vector>

#include "VertexTypes.h"
#include "TextureTypes.h"
#include "ShaderTypes.h"
#include "MaterialTypes.h"

#include "AGameObject.h"
#include "IndexBuffer.h"
#include "VertexBuffer.h"

class DeviceContext;

class Cube : public AGameObject
{
public:
	using String = std::string;

	Cube(String name);
	~Cube() = default;

	// Inherited via AGameObject
	void Update(float deltaTime) override;
	void Draw(DeviceContext* dvcContext) override;

private:
	UINT m_indicesSize = 0;
	ShaderType m_shader = ShaderType::DEFAULT_SHADER;

	std::unique_ptr<VertexBuffer> m_vertexBuffer;
	std::unique_ptr<IndexBuffer> m_indexBuffer;
};

