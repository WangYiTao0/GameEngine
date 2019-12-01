#pragma once
#include <exception>
#include <string>
#include <comdef.h>
#include "StringHelper.h"

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



#define COM_ERROR_IF_FAILED( hr, msg ) if( FAILED( hr ) ) throw COMException( hr, msg, __FILE__, __FUNCTION__, __LINE__ )

class COMException
{
public:
	COMException(HRESULT hr, const std::string& msg, const std::string& file, const std::string& function, int line)
	{
		_com_error error(hr);
		whatmsg = L"Msg: " + StringHelper::ToWide(std::string(msg)) + L"\n";
		auto m = error.ErrorMessage();
		auto s = std::to_wstring(*m);
		whatmsg += s;
		whatmsg += L"\nFile: " + StringHelper::ToWide(file);
		whatmsg += L"\nFunction: " + StringHelper::ToWide(function);
		whatmsg += L"\nLine: " + StringHelper::ToWide(std::to_string(line));
	}

	const wchar_t* what() const
	{
		return whatmsg.c_str();
	}
private:
	std::wstring whatmsg;
};