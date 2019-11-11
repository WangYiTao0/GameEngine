#pragma once
#include <exception>
#include <string>

class BaseException : public std::exception
{
public:
	BaseException(int line, const char* file)noexcept;
	const char* what() const noexcept override;
	virtual const char* GetType() const noexcept;
	int GetLine() const noexcept;
	const std::string& GetFile()const noexcept;
	std::string GetOriginString()const noexcept;
private:
	int line;
	std::string file;
protected:
	//what() is declared const, we need make whatbuffer
	//mutable in order to set it from within what
	mutable std::string whatBuffer;
};