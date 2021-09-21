#pragma once

class Log final : NonCopyable, NonMovable
{
public:
	Log(const char* fileName) noexcept;
	~Log();

	void Print(const char* text) noexcept;
	void Error(const char* text) noexcept;

private:	
	void outputDebugString(const char* text) noexcept;
	void printIntFile(const char* text) noexcept;

#if SE_PLATFORM_DESKTOP
	std::fstream m_stream;
#endif
};