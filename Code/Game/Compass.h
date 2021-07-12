#pragma once

typedef enum Compass {
	COMPASS_NORTH,
	COMPASS_NORTH_EAST,
	COMPASS_EAST,
	COMPASS_SOUTH_EAST,
	COMPASS_SOUTH,
	COMPASS_SOUTH_WEST,
	COMPASS_WEST,
	COMPASS_NORTH_WEST
} Compass;

Compass CompassAnticlockwise(Compass Compass);
Compass CompassAnticlockwiseCardinal(Compass Compass);
Compass CompassAnticlockwiseOrdinal(Compass Compass);
Compass CompassClockwise(Compass Compass);
Compass CompassClockwiseCardinal(Compass Compass);
Compass CompassClockwiseOrdinal(Compass Compass);
Compass CompassOpposite(Compass Compass);
Vector2 CompassToVector2(Compass compass);
bool IsCompassCardinal(Compass Compass);
bool IsCompassOpposite(Compass c0, Compass c1);
bool IsCompassOrdinal(Compass Compass);
