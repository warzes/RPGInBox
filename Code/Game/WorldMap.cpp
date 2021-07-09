#include "stdafx.h"
#include "WorldMap.h"
#include "MyJson.h"

WorldMap::WorldMap(const std::string& mapPath)
{
	loadDungeonFile(mapPath);
}

void WorldMap::loadDungeonFile(const std::string& mapPath)
{
	// TODO if you feel like doing something tedious and sort of unnecessary
	// you can replace these Myjson::checkValid calls with MyJson::get

	std::ifstream file(mapPath, std::ifstream::binary);
	Json::Value root;
	file >> root;
	file.close();

	width = MyJson::get(root, "width").asInt();
	height = MyJson::get(root, "height").asInt();

	float scalingXZ = MyJson::get(root, "scalingXZ").asFloat();
	float scalingY = MyJson::get(root, "scalingY").asFloat();
	scaling = { scalingXZ, scalingY, scalingXZ };
	cameraHeight = MyJson::get(root, "cameraHeight").asFloat();

	texturePath = MyJson::get(root, "texturePath").asString();
	textureExtension = MyJson::get(root, "textureExtension").asString();

	skyboxPath = MyJson::get(root, "skyboxPath").asString();
	skyboxExtension = MyJson::get(root, "skyboxExtension").asString();

	turnSpeed = MyJson::get(root, "turnSpeed").asFloat();
	moveSpeed = MyJson::get(root, "moveSpeed").asFloat();

	wallBumpSpeed = MyJson::get(root, "wallBumpSpeed").asFloat();
	wallBumpDistance = MyJson::get(root, "wallBumpDistance").asFloat();

	int startPositionX = MyJson::get(root, "startPositionX").asInt();
	int startPositionY = MyJson::get(root, "startPositionY").asInt();

	startPosition = Vector2DI(startPositionX, startPositionY);
	camStartPosition = { scaling.x * startPositionX, 0, -scaling.z * startPositionY };

	Directions directions;
	startFacing = directions.stringToValue(MyJson::get(root, "startFacing").asString());

	// fill in the vectors based on the width and height
	//DEBUG_MODE(MyJson::checkValid(root, "floor"));
	for (int r = 0, index = 0; r < height; r++)
	{
		floor.push_back(std::vector<int>());

		for (int c = 0; c < width; c++)
		{
			floor[r].push_back(root["floor"][index].asInt());
			index++;
		}
	}

	//DEBUG_MODE(MyJson::checkValid(root, "ceiling"));
	for (int r = 0, index = 0; r < height; r++)
	{
		ceiling.push_back(std::vector<int>());

		for (int c = 0; c < width; c++)
		{
			ceiling[r].push_back(root["ceiling"][index].asInt());
			index++;
		}
	}

	//DEBUG_MODE(MyJson::checkValid(root, "walls"));
	for (int r = 0, index = 0; r < height; r++)
	{
		walls.push_back(std::vector<int>());

		for (int c = 0; c < width; c++)
		{
			walls[r].push_back(root["walls"][index].asInt());
			index++;
		}
	}

}

void WorldMap::initializeScene()
{
	// NOTE/TODO this method assumes that the width and height are the same for all three layer vectors.
	// if that changes this needs to be adapted (though there any plans to change that really)

	// make the scaling more adjustable and put it in the json

	for (int r = 0; r < width; r++)
	{
		for (int c = 0; c < height; c++)
		{
			if (walls[r][c] != 0)
			{
				//ISceneNode* tmp = device->getSceneManager()->addCubeSceneNode();
				//tmp->setPosition({ c * scaling.X, 0, r * -scaling.Z });
				//tmp->setScale({ scaling.X / 10, 2 * scaling.Y / 10, scaling.Z / 10 });
				//tmp->setMaterialFlag(video::EMF_LIGHTING, false);

				//core::stringw img = texturePath.c_str();
				//img += walls[r][c];
				//img += textureExtension.c_str();

				//tmp->setMaterialTexture(0, device->getVideoDriver()->getTexture(img));


				////temp
				//// TODO figure out why textures are blurry at odd angles and distances
				//tmp->getMaterial(0).TextureLayer[0].LODBias = 2.f;

			}

			if (floor[r][c] != 0)
			{
				//ISceneNode* tmp = device->getSceneManager()->addCubeSceneNode();
				//tmp->setPosition({ c * scaling.X, -scaling.Y, r * -scaling.Z });
				//tmp->setScale({ scaling.X / 10, scaling.Y / 10, scaling.Z / 10 });
				//tmp->setMaterialFlag(video::EMF_LIGHTING, false);

				//core::stringw img = texturePath.c_str();
				//img += floor[r][c];
				//img += textureExtension.c_str();

				//tmp->setMaterialTexture(0, device->getVideoDriver()->getTexture(img));

				////temp
				//// TODO figure out why textures are blurry at odd angles and distances
				//tmp->getMaterial(0).TextureLayer[0].LODBias = 2.f;
			}
		}
	}

	// add the skybox
	// TODO im not sure if the "top left back front right left" stuff should be hardcoded
	// though its probably not worth working around in this case
	//irr::scene::ISceneNode* skybox = device->getSceneManager()->addSkyBoxSceneNode(
	//	device->getVideoDriver()->getTexture((skyboxPath + "top" + skyboxExtension).c_str()),
	//	device->getVideoDriver()->getTexture((skyboxPath + "bottom" + skyboxExtension).c_str()),
	//	device->getVideoDriver()->getTexture((skyboxPath + "left" + skyboxExtension).c_str()),
	//	device->getVideoDriver()->getTexture((skyboxPath + "right" + skyboxExtension).c_str()),
	//	device->getVideoDriver()->getTexture((skyboxPath + "front" + skyboxExtension).c_str()),
	//	device->getVideoDriver()->getTexture((skyboxPath + "back" + skyboxExtension).c_str())
	//);
}

Vector2DI WorldMap::getStartPosition()
{
	return startPosition;
}

Vector3 WorldMap::getCamStartPosition()
{
	return camStartPosition;
}

Directions::Value WorldMap::getStartFacing()
{
	return startFacing;
}

Vector3 WorldMap::getScaling()
{
	return scaling;
}

bool WorldMap::isWallsPositionEmpty(Vector2 position)
{
	uint32_t row = uint32_t(round(-position.y / scaling.x));
	uint32_t col = uint32_t(round(position.x / scaling.z));

	if (row >= walls.size() || col >= walls[0].size())
	{
		//DEBUG_MODE(std::cerr << "out of bounds position in isWallsPositionEmpty\n");
		return false;
	}
	else
	{
		if (walls[row][col] != 0)
		{
			return false;
		}
		else
		{
			return true;
		}
	}
}

Vector3 WorldMap::getPosition(int x, int y)
{
	return { scaling.x * x, 0, -scaling.z * y };
}

float WorldMap::getTurnSpeed()
{
	return turnSpeed;
}

float WorldMap::getMoveSpeed()
{
	return moveSpeed;
}

float WorldMap::getBumpDistance()
{
	return wallBumpDistance;
}

float WorldMap::getBumpSpeed()
{
	return wallBumpSpeed;
}