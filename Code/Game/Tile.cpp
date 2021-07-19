#include "stdafx.h"
#include "Tile.h"
#include "IGameCamera.h"
#include "ResourceManager.h"
#include <Engine/DebugNew.h>
//-----------------------------------------------------------------------------
const std::map<TileType, std::string> textureFloorName =
{
	{TileType::Grass, "../data/temp/textures/map/outdoor/plains-ground2.png"},
	{TileType::Road,  "../data/temp/textures/map/outdoor/road.png"},
};
//-----------------------------------------------------------------------------
const std::map<TileDecorType, std::string> textureDecorName =
{
	{TileDecorType::None,  ""},
	{TileDecorType::Tree,  "../data/temp/textures/map/outdoor/plains-tree2.png"},
	{TileDecorType::Tree2, "../data/temp/textures/map/outdoor/plains-tree3.png"},
};
//-----------------------------------------------------------------------------
Tile Tile::Create(ResourceManager& resource, TileType type, TileDecorType decor)
{
	Tile tile;
	tile.type = type;
	tile.decor = decor;
	tile.textureTile = resource.GetTexture(textureFloorName.at(type));
	if (decor != TileDecorType::None)
		tile.textureDecor = resource.GetTexture(textureDecorName.at(decor));

	//if (textureFloorName != "")
	//	tile.textureTile = resource.GetTexture(textureFloorName);
	//if (decor != TileDecorType::None && textureDecorName != "")
	//	tile.textureDecor = resource.GetTexture(textureDecorName);

	return tile;
}
//-----------------------------------------------------------------------------
void Tile::Draw(IGameCamera* camera, const Vector2& pos)
{
	if (!textureTile) return;

	// Floor render	
	DrawCubeTexture(*textureTile, { pos.x, -0.5f, pos.y }, 1, 1, 1, WHITE);
	//DrawModel(model, Vector3{ (float)x, 0.0f, (float)y }, 0.5f, WHITE);

	// object render
	if (!textureDecor || decor == TileDecorType::None) return;
	DrawBillboard(camera->GetCamera(), *textureDecor, { pos.x, 1.0f, pos.y }, 2.0f, WHITE);
	//DrawCubeTexture(m_resourceMgr.textureTree, Vector3{ (float)x, 0.5f, (float)y }, 1, 1, 1, WHITE);
	//DrawCubeTexture(tx, Vector3{ (float)x, 1.5f, (float)y }, 1, 1, 1, GREEN);
	//DrawCubeTexture(tx, Vector3{ (float)x, 0.5f, (float)y }, 0.25f, 1, 0.25f, BROWN);
}
//-----------------------------------------------------------------------------