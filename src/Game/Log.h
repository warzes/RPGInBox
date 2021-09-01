#pragma once

class Log final : NonCopyable, NonMovable
{
	friend class Engine;
public:
	~Log();
	void Print(const char* text) noexcept;
	void Error(const char* text) noexcept;

private:
	Log() = default;
	bool open(const char* fileName) noexcept;
	void outputDebugString(const char* text) noexcept;
	void printIntFile(const char* text) noexcept;

#if SE_PLATFORM_DESKTOP
	std::fstream m_stream;
#endif
};