#pragma once

#include "IGameModule.h"

enum MenuAction
{
	Menu_None,
	Menu_Push,
	Menu_Pop,
	Menu_SwitchTop,
};

enum MenuDrawState
{
	MenuDraw_Active,
	MenuDraw_Paused,
};

enum ShopType
{
	ShopType_Weapon,
	ShopType_Armor,
	ShopType_WhiteMagic,
	ShopType_BlackMagic,
	ShopType_Church,
	ShopType_Inn,
	ShopType_Item,
	ShopType_Caravan,
};

const int PlayerPicX = 24;
const int PlayerPicY = 16;
const int PlayerEntryHeight = 56;

class Menu
{
public:
	Menu* prevMenu;

	virtual MenuAction Update(Menu*& nextMenu) = 0;
	virtual void Draw(MenuDrawState state) = 0;
};

class GameMenu : public IGameModule
{
public:
	static void PlayError();
	static void PlayCursor();
	static void PlayConfirm();
};