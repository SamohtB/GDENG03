#pragma once

#include <DirectXMath.h>
using namespace DirectX;

struct IVertexType {};

struct POS_COL : IVertexType
{
	XMFLOAT3 position;
	XMFLOAT3 color;

	POS_COL() = default;
	POS_COL(const XMFLOAT3& pos, const XMFLOAT3& col)
		: position(pos), color(col) {
	}
};

struct POS_TEX_COL : IVertexType
{
	XMFLOAT3 position;
	XMFLOAT2 uv;
	XMFLOAT3 color;

	POS_TEX_COL() = default;
	POS_TEX_COL(const XMFLOAT3& pos, const XMFLOAT2& tex, const XMFLOAT3& col)
		: position(pos), uv(tex), color(col) {
	}
};

struct POS_POS_COL_COL : IVertexType
{
	XMFLOAT3 position1;
	XMFLOAT3 position2;
	XMFLOAT3 color1;
	XMFLOAT3 color2;

	POS_POS_COL_COL() = default;
	POS_POS_COL_COL(const XMFLOAT3& pos1, const XMFLOAT3& pos2, const XMFLOAT3& col1, const XMFLOAT3& col2)
		: position1(pos1), position2(pos2), color1(col1), color2(col2) {
	}
};