#include "BaseException.h"
#include <sstream>
BaseException::BaseException(int line, const char* file) noexcept
	:
	line(line),
	file(file)
{
}

const char* BaseException::what() const noexcept
{
	std::ostringstream oss;
	oss << GetType() << std::endl
		<< GetOriginString();
	whatBuffer = oss.str();
	return whatBuffer.c_str();
}

const char* BaseException::GetType() const noexcept
{
	return "Base Exception";
}

int BaseException::GetLine() const noexcept
{
	return line;
}

const std::string& BaseException::GetFile() const noexcept
{
	return file;
}

std::string BaseException::GetOriginString() const noexcept
{
	std::ostringstream oss;
	oss << "[File] " << file << std::endl
		<< "[Line] " << line;
	return oss.str();
}
