#include "stdafx.h"
#include "Directions.h"

Directions::Directions()
{
}

Directions::Value Directions::stringToValue(std::string str)
{
	if (str == "west") return Value::West;
	else if (str == "east") return Value::East;
	else if (str == "north") return Value::North;
	else if (str == "south") return Value::South;
	else return Value::North;
}

Directions::Value Directions::getLeft(Value& facing)
{
	switch (facing)
	{
	case Value::East:
		return Value::North;

	case Value::North:
		return Value::West;

	case Value::West:
		return Value::South;

	case Value::South:
		return Value::East;
	}
}

Directions::Value Directions::getRight(Value& facing)
{
	switch (facing)
	{
	case Value::East:
		return Value::South;

	case Value::North:
		return Value::East;

	case Value::West:
		return Value::North;

	case Value::South:
		return Value::West;
	}
}

Directions::Value Directions::getReverse(Value& facing)
{
	switch (facing)
	{
	case Value::East:
		return Value::West;

	case Value::North:
		return Value::South;

	case Value::South:
		return Value::North;

	case Value::West:
		return Value::East;
	}
}

Vector3 Directions::getVectorFloat(Value& facing)
{
	switch (facing)
	{
	case Directions::Value::North:
		return { 0,0,1 };

	case Directions::Value::South:
		return { 0,0,-1 };

	case Directions::Value::East:
		return { 1,0,0 };

	case Directions::Value::West:
		return { -1,0,0 };
	}
}

//irr::core::vector3di Directions::getVectorInt(Value& facing)
//{
//	switch (facing)
//	{
//	case Directions::Value::North:
//		return { 0,0,1 };
//
//	case Directions::Value::South:
//		return { 0,0,-1 };
//
//	case Directions::Value::East:
//		return { 1,0,0 };
//
//	case Directions::Value::West:
//		return { -1,0,0 };
//	}
//}