#pragma once
#include <string>

struct ShaderNames
{
	static inline const std::string LIT = "LIT";
	static inline const std::string UNLIT = "UNLIT";
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