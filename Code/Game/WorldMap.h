#pragma once

#include "Directions.h"

struct Vector2DI
{
	int x = 0;
	int y = 0;
};

class WorldMap
{
public:

	// 2d vectors for each layer
	std::vector<std::vector<int>> walls;
	std::vector<std::vector<int>> floor;
	std::vector<std::vector<int>> ceiling;

	// coordinate related
	Vector2DI startPosition;
	Vector3 camStartPosition;
	Directions::Value startFacing;
	int width, height;
	float cameraHeight;
	Vector3 scaling;

	// player action speeds
	float turnSpeed, moveSpeed, wallBumpSpeed;
	float wallBumpDistance;

	// texture paths
	std::string texturePath, textureExtension, skyboxPath, skyboxExtension;

	// parse the dungeon file and setup the walls, floor, and ceiling vectors
	void loadDungeonFile(const std::string& mapPath);

	WorldMap(const std::string& mapPath);

	// initialize the scene, add nodes to the scenemanager
	void initializeScene();

	// get the start position of the player
	Vector2DI getStartPosition();

	// get the start position of the camera
	Vector3 getCamStartPosition();

	// get the direction the player starts facing
	Directions::Value getStartFacing();

	// get the scaling of the dungeon
	Vector3 getScaling();

	// get the camera position for a given point on the map
	Vector3 getPosition(int x, int y);

	// checks if a position in the walls grid is empty 
	bool isWallsPositionEmpty(Vector2 position);

	// get the move speed
	float getMoveSpeed();

	// get the turn speed
	float getTurnSpeed();

	// get the wallbump distance
	float getBumpDistance();

	// get the wallbump speed
	float getBumpSpeed();
};