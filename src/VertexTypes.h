#pragma once

#include <DirectXMath.h>
using namespace DirectX;

struct IVertexType {};

struct Vertex3D : IVertexType
{
	XMFLOAT3 position;
	XMFLOAT4 color;

	Vertex3D() = default;
	Vertex3D(const XMFLOAT3& pos, const XMFLOAT4& col)
		: position(pos), color(col) {
	}
};

struct TexturedVertex3D : IVertexType
{
	XMFLOAT3 position;
	XMFLOAT2 uv;
	XMFLOAT4 color;

	TexturedVertex3D() = default;
	TexturedVertex3D(const XMFLOAT3& pos, const XMFLOAT2& tex, const XMFLOAT4& col)
		: position(pos), uv(tex), color(col) {
	}
};