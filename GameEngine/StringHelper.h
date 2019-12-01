#pragma once
#include <string>
#include <vector>
#include "WindowHelper.h"

class StringHelper
{
public:

	static std::vector<std::string> TokenizeQuoted(const std::string& input);
	static std::string ToNarrow(const std::wstring& wide);
	static std::wstring ToWide(const std::string& narrow);
	static std::string GetDirectoryFromPath(const std::string& filepath);
	static std::string GetFileExtension(const std::string& filename);

	static std::string GetShaderRootPath();
};