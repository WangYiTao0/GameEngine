#pragma once
#include <string>
#include "WindowHelper.h"

class StringHelper
{
public:
	static std::wstring StringToWide(std::string str);
	static std::string GetDirectoryFromPath(const std::string& filepath);
	static std::string GetFileExtension(const std::string& filename);

	static std::string GetShaderRootPath();
};