#pragma once

#include <windows.h>
#include <d3d12.h>
#include <dxgi1_6.h>
#include <wrl.h>
#include <shellapi.h>
#include "d3dx12.h"

#include <memory>

using Microsoft::WRL::ComPtr;
static const UINT FRAME_COUNT = 2;
static const UINT SRV_MAX_COUNT = 128;
static const UINT CBV_MAX_COUNT = 64 * FRAME_COUNT;

enum RootDescriptorIndex : int {
	TEXTURES = 0,
	MAT_CONST = 1,
	GLOBAL_CONST
};
