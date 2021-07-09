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

	std::fstream m_stream;
};

//=============================================================================
// Timer
//=============================================================================

class Timer final : NonCopyable, NonMovable
{
public:
	static constexpr double MicrosecondsToSeconds = 1.0 / 1000000.0;

	Timer() noexcept { Reset(); }

	void Reset() noexcept { m_startTime = m_clock.now(); }

	inline uint64_t GetHours() const noexcept { return time<std::chrono::hours>(); }
	inline uint64_t GetMinutes() const noexcept { return time<std::chrono::minutes>(); }
	inline uint64_t GetSeconds() const noexcept { return time<std::chrono::seconds>(); }
	inline uint64_t GetMilliseconds() const noexcept { return time<std::chrono::milliseconds>(); }
	inline uint64_t GetMicroseconds() const noexcept { return time<std::chrono::microseconds>(); }

private:
	template<typename T>
	inline uint64_t time() const noexcept
	{
		const auto delta = m_clock.now() - m_startTime;
		return static_cast<uint64_t>(std::chrono::duration_cast<T>(delta).count());
	}

	std::chrono::high_resolution_clock m_clock;
	std::chrono::time_point<std::chrono::high_resolution_clock> m_startTime;
};

//=============================================================================
// StringConversion
//=============================================================================
// convert a string to a wstring
//std::wstring stringToWString(std::string s);
//
//// convert a wstring to a string
//std::string wStringToString(std::wstring s);