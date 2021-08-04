#pragma once

class Stats
{
public:
	Stats() = default;
	Stats(int num) { Set(num); }

	Stats& operator=(int num) { Set(num); return *this; }

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

	std::string GetFullText() { return (std::to_string(current) + "/" + std::to_string(max)); }



	int current = 0;
	int max = 0;
};