#include "stdafx.h"
#include "Core.h"

//=============================================================================
// LOG
//=============================================================================
#if SE_PLATFORM_WINDOWS && SE_DEBUG
extern "C" __declspec(dllimport) void __stdcall OutputDebugStringA(const char*);
extern "C" __declspec(dllimport) int __stdcall MessageBoxA(void*, const char*, const char*, unsigned int);
extern "C" __declspec(dllimport) void* __stdcall GetStdHandle(unsigned long);
extern "C" __declspec(dllimport) int __stdcall SetConsoleTextAttribute(void*, unsigned short);
#endif
//-----------------------------------------------------------------------------
Log::~Log()
{
	m_stream.flush();
	m_stream.close();
}
//-----------------------------------------------------------------------------
void Log::Print(const char* text) noexcept
{
#if SE_PLATFORM_WINDOWS && SE_DEBUG
	OutputDebugStringA(text);
	OutputDebugStringA("\n");
#endif

	puts(text);

	m_stream << text << std::endl;
	m_stream.flush();

}
//-----------------------------------------------------------------------------
void Log::Error(const char* text) noexcept
{
	const std::string formatText = "ERROR: " + std::string(text);
#if SE_PLATFORM_WINDOWS && SE_DEBUG
	OutputDebugStringA(formatText.c_str());
	OutputDebugStringA("\n");
#endif
#if SE_PLATFORM_WINDOWS
	MessageBoxA(nullptr, text, "Error", 0x00000000L);
#endif

#if SE_PLATFORM_WINDOWS && SE_DEBUG
	static auto win32_console_handle = GetStdHandle(/*STD_OUTPUT_HANDLE*/((unsigned long)-11));
	SetConsoleTextAttribute(win32_console_handle, /*FOREGROUND_RED*/0x0004);
#endif
	puts(formatText.c_str());
#if SE_PLATFORM_WINDOWS && SE_DEBUG
	SetConsoleTextAttribute(win32_console_handle, /*FOREGROUND_RED*/0x0004 | /*FOREGROUND_GREEN*/0x0002 | /*FOREGROUND_BLUE*/0x0001);
#endif

	m_stream << formatText << std::endl;
	m_stream.flush();
}
//-----------------------------------------------------------------------------
bool Log::open(const char* fileName) noexcept
{
#if SE_PLATFORM_WINDOWS
	m_stream.open(fileName, std::ios::out);
	if (m_stream.fail())
		return false;
#endif
	return true;
}
//-----------------------------------------------------------------------------