#pragma once

//=============================================================================
// LOG
//=============================================================================

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

#if SE_PLATFORM_DESKTOP
	std::fstream m_stream;
#endif
};