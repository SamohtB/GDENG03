#pragma once
#include <string>

enum ShaderType : int
{
    DEFAULT_SHADER = 0,
    TEXTURED = 1,
    PBS = 2,
    ANIMATED = 3
};

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
