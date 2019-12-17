#include "StringHelper.h"
#include <algorithm>
#include <sstream>
#include <iomanip>


std::vector<std::string> StrH::TokenizeQuoted(const std::string& input)
{
	std::istringstream stream;
	stream.str(input);
	std::vector<std::string> tokens;
	std::string token;

	while (stream >> std::quoted(token))
	{
		tokens.push_back(std::move(token));
	}
	return tokens;
}

std::string StrH::ToNarrow(const std::wstring& wide)
{
	char narrow[512];
	wcstombs_s(nullptr, narrow, wide.c_str(), _TRUNCATE);
	return narrow;
}

std::wstring StrH::ToWide(const std::string& narrow)
{
	wchar_t wide[512];
	mbstowcs_s(nullptr, wide, narrow.c_str(), _TRUNCATE);
	return wide;
}

std::string StrH::GetDirectoryFromPath(const std::string& filepath)
{
	size_t off1 = filepath.find_last_of('\\');
	size_t off2 = filepath.find_last_of('/');
	if (off1 == std::string::npos && off2 == std::string::npos) //If no slash or backslash in path?
	{
		return "";
	}
	if (off1 == std::string::npos)
	{
		return filepath.substr(0, off2);
	}
	if (off2 == std::string::npos)
	{
		return filepath.substr(0, off1);
	}
	//If both exists, need to use the greater offset
	return filepath.substr(0, std::max(off1, off2));
}

std::string StrH::GetFileExtension(const std::string& filename)
{
	size_t off = filename.find_last_of('.');
	if (off == std::string::npos)			//max  size_t
	{
		return {};
	}
	return std::string(filename.substr(off + 1));
}

std::string StrH::GetShaderRootPath()
{
	std::string shaderfolder = "";
#pragma region DetermineShaderPath
	if (IsDebuggerPresent() == TRUE)
	{
#ifdef _DEBUG //Debug Mode
#ifdef _WIN64 //x64
		shaderfolder = "..\\Build\\x64\\Debug\\";
#else  //x86 (Win32)
		shaderfolder = "..\\Build\\Win32\\Debug\\";
#endif
#else //Release Mode
#ifdef _WIN64 //x64
		shaderfolder = "..\\Build\\x64\\Release\\";
#else  //x86 (Win32)
		shaderfolder = "..\\Build\\Win32\\Release\\";
#endif
#endif
	}
	return shaderfolder;
}
