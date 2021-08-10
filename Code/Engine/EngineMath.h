#pragma once

class Point2 final
{
public:
	int x = 0;
	int y = 0;
};

inline bool operator==(const Point2& left, const Point2& right)
{
	return left.x == right.x && left.y == right.y;
}

inline bool operator!=(const Point2& left, const Point2& right)
{
	return !(left == right);
}

class Size2 final
{
public:
	int width = 0;
	int height = 0;
};