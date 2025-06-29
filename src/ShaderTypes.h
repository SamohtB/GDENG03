#pragma once
#include <string>

class ShaderNames
{
public:
	inline static const std::string UNLIT = "UNLIT";
	inline static const std::string LIT = "LIT";
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
