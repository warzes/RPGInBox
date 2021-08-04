#pragma once

class Stats
{
public:
	Stats() = default;
	Stats(int num) { Set(num); }

	void Set(int num)
	{
		current = max = num;
	}

	void Bonus(int num)
	{
		current = std::min(current + num, max); // не больше max
	}

	void Restore()
	{
		current = max;
	}

	Stats& operator=(int num) { Set(num); return *this; }

	int current = 0;
	int max = 0;
};