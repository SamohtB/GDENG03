#pragma once

#include <directxtk12/SimpleMath.h>
using namespace DirectX::SimpleMath;

struct IVertexType {};

struct POS_COL : IVertexType
{
	Vector3 position;
	Vector3 color;

	POS_COL() = default;
	POS_COL(const Vector3& pos, const Vector3& col)
		: position(pos), color(col) {
	}
};

struct POS_TEX_COL : IVertexType
{
	Vector3 position;
	Vector2 uv;
	Vector3 color;

	POS_TEX_COL() = default;
	POS_TEX_COL(const Vector3& pos, const Vector2& tex, const Vector3& col)
		: position(pos), uv(tex), color(col) {
	}
};

struct POS_TEX_NOR_TAN_BIT : IVertexType
{
	Vector3 position;
	Vector2 uv;
	Vector3 normal;
	Vector3 tangent;
	Vector3 bitangent;

	POS_TEX_NOR_TAN_BIT() = default;
	POS_TEX_NOR_TAN_BIT(const Vector3& pos, const Vector2& tex, const Vector3& nor, const Vector3& tan, const Vector3& bit)
		: position(pos), uv(tex), normal(nor), tangent(tan), bitangent(bit){
	}
};

struct POS_POS_COL_COL : IVertexType
{
	Vector3 position1;
	Vector3 position2;
	Vector3 color1;
	Vector3 color2;

	POS_POS_COL_COL() = default;
	POS_POS_COL_COL(const Vector3& pos1, const Vector3& pos2, const Vector3& col1, const Vector3& col2)
		: position1(pos1), position2(pos2), color1(col1), color2(col2) {
	}
};