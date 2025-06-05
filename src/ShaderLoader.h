#pragma once
#include "Dx12Commons.h"
#include <dxcapi.h>
#include <string>

struct ShaderDesc 
{
	std::wstring shaderFilePath;
	std::wstring shaderEntry;
	std::wstring shaderTarget;

	ShaderDesc() = default;
	ShaderDesc(std::wstring path, std::wstring entry, std::wstring target) :
		shaderFilePath(path), shaderEntry(entry), shaderTarget(target) {
	}
};

using Microsoft::WRL::ComPtr;

class ShaderLoader
{
public:
	ShaderLoader();
	~ShaderLoader() = default;

	ComPtr<IDxcBlob> CompileShader(ShaderDesc desc);

private:
	ComPtr<IDxcCompiler3> m_compiler;
	ComPtr<IDxcUtils> m_utils;
	ComPtr<IDxcIncludeHandler> m_includeHandler;
};

