#include "stdafx.h"
#include "Log.h"
#include "DebugNew.h"
//-----------------------------------------------------------------------------
#if SE_PLATFORM_WINDOWS && SE_DEBUG
extern "C" __declspec(dllimport) void __stdcall OutputDebugStringA(const char*);
#endif
#if SE_PLATFORM_WINDOWS
extern "C" __declspec(dllimport) void* __stdcall GetStdHandle(unsigned long);
extern "C" __declspec(dllimport) int __stdcall SetConsoleTextAttribute(void*, unsigned short);
extern "C" __declspec(dllimport) int __stdcall MessageBoxA(void*, const char*, const char*, unsigned int);
#	ifndef STD_OUTPUT_HANDLE 
#		define STD_OUTPUT_HANDLE ((unsigned long)-11)
#	endif
#	ifndef FOREGROUND_BLUE 
#		define FOREGROUND_BLUE 0x0001
#	endif
#	ifndef FOREGROUND_GREEN 
#		define FOREGROUND_GREEN 0x0002
#	endif
#	ifndef FOREGROUND_RED 
#		define FOREGROUND_RED 0x0004
#	endif
#endif // SE_PLATFORM_WINDOWS
//-----------------------------------------------------------------------------
Log::Log(const char* fileName) noexcept
{
#if SE_PLATFORM_DESKTOP
	m_stream.open(fileName, std::ios::out);
#endif
}
//-----------------------------------------------------------------------------
Log::~Log()
{
#if SE_PLATFORM_DESKTOP
	if (m_stream.good())
	{
		m_stream.flush();
		m_stream.close();
	}
#endif
}
//-----------------------------------------------------------------------------
void Log::Print(const char* text) noexcept
{
	outputDebugString(text);
	puts(text);
	printIntFile(text);
}
//-----------------------------------------------------------------------------
void Log::Error(const char* text) noexcept
{
	const std::string formatText = "ERROR: " + std::string(text);
	outputDebugString(formatText.c_str());
#if SE_PLATFORM_WINDOWS
	MessageBoxA(nullptr, text, "Error", 0x00000000L);
#endif

#if SE_PLATFORM_WINDOWS
	static auto win32ConsoleHandle = GetStdHandle(STD_OUTPUT_HANDLE);
	SetConsoleTextAttribute(win32ConsoleHandle, FOREGROUND_RED);
#endif
	puts(formatText.c_str());
#if SE_PLATFORM_WINDOWS
	SetConsoleTextAttribute(win32ConsoleHandle, FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE);
#endif

	printIntFile(formatText.c_str());
}
//-----------------------------------------------------------------------------
void Log::outputDebugString(const char* text) noexcept
{
#if SE_PLATFORM_WINDOWS && SE_DEBUG
	OutputDebugStringA(text);
	OutputDebugStringA("\n");
#endif
}
//-----------------------------------------------------------------------------
void Log::printIntFile(const char* text) noexcept
{
#if SE_PLATFORM_DESKTOP
	if (m_stream.good())
	{
		m_stream << text << std::endl;
		m_stream.flush();
	}
#endif
}
//-----------------------------------------------------------------------------