#pragma once

#include "IGameModule.h"
#include "Global.h"

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
	virtual MenuAction Update(Menu*& nextMenu) = 0;
	virtual void Draw(MenuDrawState state) = 0;

	Menu* prevMenu;
};

class GameMenu final : public IGameModule
{
public:
	GameMenu() noexcept;
	~GameMenu() noexcept;

	void InitHost(Menu* firstMenu) noexcept;

	void Update() noexcept final;
	void Draw() noexcept final;

	IPlayfield* AsPlayfield() noexcept final;

	static void DrawClass(int classId, float x, float y) noexcept;
	static void DrawOrb(int index, bool lit, float x, float y) noexcept;

	static void PlayError() noexcept;
	static void PlayCursor() noexcept;
	static void PlayConfirm() noexcept;

private:
	void init() noexcept;
	void push(Menu* nextMenu) noexcept;
	void pop() noexcept;
	void popAll() noexcept;

	static const int MenuStrings = 64;
	static const int Shops = 71;
	static const int ShopStrings = 38;

	static GameMenu* instance;
	Menu* m_activeMenu = nullptr;

	uint8_t m_itemTarget[(256 + 7) / 8];
	uint8_t m_magicTarget[(64 + 7) / 8];
	uint8_t m_shopTypes[Shops];

	Table<char, MenuStrings> m_menuText;
	Table<uint8_t, Shops> m_shopStock;
	Table<char, ShopStrings> m_shopText;

	std::shared_ptr<Texture> m_playerBmp;
	std::shared_ptr<Texture> m_menuBmp;
};