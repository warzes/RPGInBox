#pragma once

struct Directions
{
	enum class Value
	{
		North, South, East, West
	};

	Directions();

	// get the direction to the left of facing
	Value getLeft(Value& facing);

	// get the direction to the right of facing
	Value getRight(Value& facing);

	// get the opposite direction
	Value getReverse(Value& facing);

	// returns a unit vector in the direction of the facing parameter
	Vector3 getVectorFloat(Value& facing);

	// returns a unit vector in the direction of the facing parameter
	//irr::core::vector3di getVectorInt(Value& facing);

	// convert a string to a Directions::Value
	Value stringToValue(std::string str);
};