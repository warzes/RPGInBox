#include "stdafx.h"
#include "Compass.h"

Compass CompassAnticlockwise(Compass compass)
{
	switch (compass) {
	case COMPASS_NORTH:
		return COMPASS_NORTH_WEST;
	case COMPASS_NORTH_EAST:
		return COMPASS_NORTH;
	case COMPASS_EAST:
		return COMPASS_NORTH_EAST;
	case COMPASS_SOUTH_EAST:
		return COMPASS_EAST;
	case COMPASS_SOUTH:
		return COMPASS_SOUTH_EAST;
	case COMPASS_SOUTH_WEST:
		return COMPASS_SOUTH;
	case COMPASS_WEST:
		return COMPASS_SOUTH_WEST;
	case COMPASS_NORTH_WEST:
		return COMPASS_WEST;
	default:
		return COMPASS_NORTH;
	}
}

Compass CompassAnticlockwiseCardinal(Compass compass)
{
	switch (compass) {
	case COMPASS_NORTH:
	case COMPASS_NORTH_WEST:
		return COMPASS_WEST;
	case COMPASS_NORTH_EAST:
	case COMPASS_EAST:
		return COMPASS_NORTH;
	case COMPASS_SOUTH_EAST:
	case COMPASS_SOUTH:
		return COMPASS_EAST;
	case COMPASS_SOUTH_WEST:
	case COMPASS_WEST:
		return COMPASS_SOUTH;
	default:
		return COMPASS_NORTH;
	}
}

Compass CompassAnticlockwiseOrdinal(Compass compass)
{
	switch (compass) {
	case COMPASS_NORTH:
	case COMPASS_NORTH_EAST:
		return COMPASS_NORTH_WEST;
	case COMPASS_EAST:
	case COMPASS_SOUTH_EAST:
		return COMPASS_NORTH_EAST;
	case COMPASS_SOUTH:
	case COMPASS_SOUTH_WEST:
		return COMPASS_SOUTH_EAST;
	case COMPASS_WEST:
	case COMPASS_NORTH_WEST:
		return COMPASS_SOUTH_WEST;
	default:
		return COMPASS_NORTH;
	}
}

Compass CompassClockwise(Compass compass)
{
	switch (compass) {
	case COMPASS_NORTH:
		return COMPASS_NORTH_EAST;
	case COMPASS_NORTH_EAST:
		return COMPASS_EAST;
	case COMPASS_EAST:
		return COMPASS_SOUTH_EAST;
	case COMPASS_SOUTH_EAST:
		return COMPASS_SOUTH;
	case COMPASS_SOUTH:
		return COMPASS_SOUTH_WEST;
	case COMPASS_SOUTH_WEST:
		return COMPASS_WEST;
	case COMPASS_WEST:
		return COMPASS_NORTH_WEST;
	case COMPASS_NORTH_WEST:
		return COMPASS_NORTH;
	default:
		return COMPASS_NORTH;
	}
}

Compass CompassClockwiseCardinal(Compass compass)
{
	switch (compass) {
	case COMPASS_NORTH:
	case COMPASS_NORTH_EAST:
		return COMPASS_EAST;
	case COMPASS_EAST:
	case COMPASS_SOUTH_EAST:
		return COMPASS_SOUTH;
	case COMPASS_SOUTH:
	case COMPASS_SOUTH_WEST:
		return COMPASS_WEST;
	case COMPASS_WEST:
	case COMPASS_NORTH_WEST:
		return COMPASS_NORTH;
	default:
		return COMPASS_NORTH;
	}
}

Compass CompassClockwiseOrdinal(Compass compass)
{
	switch (compass) {
	case COMPASS_NORTH_WEST:
	case COMPASS_NORTH:
		return COMPASS_NORTH_EAST;
	case COMPASS_NORTH_EAST:
	case COMPASS_EAST:
		return COMPASS_SOUTH_EAST;
	case COMPASS_SOUTH_EAST:
	case COMPASS_SOUTH:
		return COMPASS_SOUTH_WEST;
	case COMPASS_SOUTH_WEST:
	case COMPASS_WEST:
		return COMPASS_NORTH_WEST;
	default:
		return COMPASS_NORTH;
	}
}

Compass CompassOpposite(Compass compass)
{
	switch (compass) {
	case COMPASS_NORTH:
		return COMPASS_SOUTH;
	case COMPASS_NORTH_EAST:
		return COMPASS_SOUTH_WEST;
	case COMPASS_EAST:
		return COMPASS_WEST;
	case COMPASS_SOUTH_EAST:
		return COMPASS_NORTH_WEST;
	case COMPASS_SOUTH:
		return COMPASS_NORTH;
	case COMPASS_SOUTH_WEST:
		return COMPASS_NORTH_EAST;
	case COMPASS_WEST:
		return COMPASS_EAST;
	case COMPASS_NORTH_WEST:
		return COMPASS_SOUTH_EAST;
	default:
		return COMPASS_NORTH;
	}
}

Vector2 CompassToVector2(Compass compass)
{
	// Compass is relative to screen (where 0,0 is top,left)
	switch (compass) {
	case COMPASS_NORTH:      return Vector2 { 0, -1 };
	case COMPASS_NORTH_EAST: return Vector2 { 1, -1 };
	case COMPASS_EAST:       return Vector2 { 1, 0 };
	case COMPASS_SOUTH_EAST: return Vector2 { 1, 1 };
	case COMPASS_SOUTH:      return Vector2 { 0, 1 };
	case COMPASS_SOUTH_WEST: return Vector2 { -1, 1 };
	case COMPASS_WEST:       return Vector2 { -1, 0 };
	case COMPASS_NORTH_WEST: return Vector2 { -1, -1 };
	default:
		// CRASH: Implementation failure
		TraceLog(LOG_ERROR, TextFormat("COMPASS: CompassToVector2() case %d is not implemented", compass));
		return Vector2 { 0, 0 };
	}
}

bool IsCompassCardinal(Compass compass)
{
	switch (compass) {
	case COMPASS_NORTH:
	case COMPASS_EAST:
	case COMPASS_SOUTH:
	case COMPASS_WEST:
		return 1;
	default:
		return 0;
	}
}

bool IsCompassOpposite(Compass c0, Compass c1)
{
	return CompassOpposite(c0) == c1;
}

bool IsCompassOrdinal(Compass compass)
{
	switch (compass) {
	case COMPASS_NORTH_EAST:
	case COMPASS_SOUTH_EAST:
	case COMPASS_SOUTH_WEST:
	case COMPASS_NORTH_WEST:
		return 1;
	default:
		return 0;
	}
}