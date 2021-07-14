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
	textureGrass = LoadTexture("../data/temp/textures/map/outdoor/plains-ground2.png");
	textureRoad = LoadTexture("../data/temp/textures/map/outdoor/road.png");
	
	textureTree = LoadTexture("../data/temp/textures/map/outdoor/plains-tree2.png");
	textureTown = LoadTexture("../data/temp/textures/map/outdoor/plains-tree3.png");
	return true;
}
//-----------------------------------------------------------------------------