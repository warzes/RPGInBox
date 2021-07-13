#include "stdafx.h"
#include "ResourceManager.h"
//-----------------------------------------------------------------------------
ResourceManager::~ResourceManager()
{
	UnloadTexture(textureGrass);
	UnloadTexture(textureRoad);
	UnloadTexture(textureTree);
	UnloadTexture(textureTown);
}
//-----------------------------------------------------------------------------
bool ResourceManager::Init()
{
	textureGrass = LoadTexture("../data/temp/textures/map/outdoor/plains-ground.png");
	textureRoad = LoadTexture("../data/temp/textures/map/outdoor/road.png");
	
	textureTree = LoadTexture("../data/temp/textures/map/outdoor/plains-tree.png");
	textureTown = LoadTexture("../data/temp/textures/map/outdoor/town.png");
	return true;
}
//-----------------------------------------------------------------------------