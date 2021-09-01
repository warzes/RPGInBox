#pragma once

// TODO: временные бонусы и менеджер подсчета времени

class Stats final
{
public:
	constexpr Stats() noexcept = default;
	constexpr Stats(const int num) noexcept { Set(num); }

	constexpr Stats& operator=(const int num) noexcept { Set(num); return *this; }

	constexpr Stats& operator+=(const int num) noexcept { Modify(num); return *this; }
	constexpr Stats& operator-=(const int num) noexcept { Modify(-num); return *this; }

	constexpr void Set(int num) noexcept
	{
		m_current = m_max = std::min(num, MaxNumberState);
	}

	constexpr void Modify(int num) noexcept
	{
		m_current = std::min(m_current + num, m_max); // не больше max
	}

	constexpr void Restore() noexcept
	{
		m_current = m_max;
	}

	constexpr std::string GetFullText() noexcept { return (std::to_string(m_current) + "/" + std::to_string(m_max)); }

	constexpr int GetCurrent() const noexcept { return m_current; }
	constexpr int GetMax() const noexcept { return m_max; }
	constexpr bool IsZero() const noexcept { return m_current == 0; }

private:
	int m_current = 0;
	int m_max = 0;
};