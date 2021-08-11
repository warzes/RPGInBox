#include "stdafx.h"
#include "NewGameScene.h"
#include "GameScenes.h"
#include "Player.h"
#include "Input.h"
#include <Engine/DebugNew.h>

const char* DefaultName = "ANON";

class NewGameMenu : public Menu
{
public:
	NewGameMenu();

	virtual MenuAction Update(Menu*& nextMenu);
	virtual void Draw(MenuDrawState state);

	void SetClass(int classId);
	void SetName(const char* name);

private:
	int selCol;
	int selRow;
	Player::PlayerInfo players[Players];

	std::shared_ptr<Texture2D> m_patchTexture = nullptr;
	std::shared_ptr<Texture> m_cursor = nullptr;
};

class ChooseClassMenu : public Menu
{
	NewGameMenu* parent;
	int selCol;
	int selRow;
	std::shared_ptr<Texture> m_cursor = nullptr;

public:
	ChooseClassMenu(NewGameMenu* parent);

	virtual MenuAction Update(Menu*& nextMenu);
	virtual void Draw(MenuDrawState state);

private:
	int GetSelectedClass();
};

class ChooseNameMenu : public Menu
{
	NewGameMenu* parent;
	int selCol;
	int selRow;
	int nameLength;
	char name[NameLength + 1];
	std::shared_ptr<Texture> m_cursor = nullptr;

public:
	ChooseNameMenu(NewGameMenu* parent, const char* name);

	virtual MenuAction Update(Menu*& nextMenu);
	virtual void Draw(MenuDrawState state);

private:
	void InsertChar();
	void DeleteChar();
};

//----------------------------------------------------------------------------
//  NewGameScene
//----------------------------------------------------------------------------

void NewGameScene::Init() noexcept
{
	NewGameMenu* newGameMenu = new NewGameMenu();
	m_mainMenu.InitHost(newGameMenu);
}

void NewGameScene::Update() noexcept
{
	m_mainMenu.Update();
}

void NewGameScene::Draw() noexcept
{
	m_mainMenu.Draw();
}

IPlayfield* NewGameScene::AsPlayfield() noexcept
{
	return nullptr;
}

IGameModule* NewGameScene::Make() noexcept
{
	NewGameScene* screen = new NewGameScene();
	screen->Init();
	return screen;
}

//----------------------------------------------------------------------------
//  NewGameMenu
//----------------------------------------------------------------------------
const int OptionBoxX = 128;
const int OptionBoxY = 24;

NewGameMenu::NewGameMenu()
	: selCol(0),
	selRow(0)
{
	players[0].Class = Class_Fighter;
	players[1].Class = Class_Thief;
	players[2].Class = Class_WhiteMage;
	players[3].Class = Class_BlackMage;

	for (int i = 0; i < Players; i++)
		strcpy_s(players[i].Name, DefaultName);

	m_patchTexture = ResourceManager::GetGlobalTexture("../data/ui/battleui_background.png");
	m_cursor = ResourceManager::GetGlobalTexture("images/cursor.png");
}

MenuAction NewGameMenu::Update(Menu*& nextMenu)
{
	if (Input::IsKeyPressing(KEY_RIGHT))
	{
		selCol = (selCol + 1) % 2;
		GameMenu::PlayCursor();
	}
	else if (Input::IsKeyPressing(KEY_LEFT))
	{
		selCol = (selCol - 1 + 2) % 2;
		GameMenu::PlayCursor();
	}
	else if (Input::IsKeyPressing(KEY_DOWN))
	{
		selRow = (selRow + 1) % Players;
		GameMenu::PlayCursor();
	}
	else if (Input::IsKeyPressing(KEY_UP))
	{
		selRow = (selRow - 1 + Players) % Players;
		GameMenu::PlayCursor();
	}
	else if (Input::IsKeyPressing(ConfirmKey))
	{
		GameMenu::PlayConfirm();
		if (selCol == 0)
		{
			nextMenu = new ChooseClassMenu(this);
			return Menu_Push;
		}
		else if (selCol == 1)
		{
			nextMenu = new ChooseNameMenu(this, players[selRow].Name);
			return Menu_Push;
		}
	}
	else if (Input::IsKeyPressing(CancelKey))
	{
		GameScenes::SwitchScene(SceneId_Title);
	}
	else if (Input::IsKeyPressing(MenuKey))
	{
		GameMenu::PlayConfirm();
		Player::InitFile(players);
		Point2 pos = Player::GetPlayerPos();
		GameScenes::SwitchToField(pos.x, pos.y);
	}

	return Menu_None;
}

void NewGameMenu::Draw(MenuDrawState state)
{
	const int PicX = 120;
	const int NameX = 200;
	const int EntryY = 120;
	const int EntryHeight = 120;

	// caption
	Rectangle dest = { 30.0f, 0.0f, (float)GetScreenWidth() - dest.x * 2.0f, 40.0f };
	NPatchInfo ninePatchInfo = { { 0.0f, 0.0f, 64.0f, 64.0f }, 6, 6, 6, 6, NPATCH_NINE_PATCH };
	DrawTextureNPatch(*m_patchTexture, ninePatchInfo, dest, { 0.0f, 0.0f }, 0.0f, WHITE);
	DrawText("Create Party", 440, 8, 24, WHITE);

	// left
	dest = { 30.0f, 50.0f, (float)GetScreenWidth()/2.0f - dest.x * 2.0f, (float)GetScreenHeight() - 70.0f };
	DrawTextureNPatch(*m_patchTexture, ninePatchInfo, dest, { 0.0f, 0.0f }, 0.0f, WHITE);

	for (int i = 0; i < _countof(players); i++)
	{
		const char* className = Player::GetClassName(players[i].Class);
		int y = EntryY + EntryHeight * i;

		GameMenu::DrawClass(players[i].Class, PicX, y);
		DrawText(players[i].Name, NameX, y + 10, 24, WHITE);
		DrawText(className,       NameX, y + 40, 24, WHITE);
	}

	// right
	dest = { (float)GetScreenWidth() / 2.0f + 30.0f, 50.0f, (float)GetScreenWidth() / 2.0f - dest.x * 2.0f, (float)GetScreenHeight() - 70.0f };
	DrawTextureNPatch(*m_patchTexture, ninePatchInfo, dest, { 0.0f, 0.0f }, 0.0f, WHITE);

	static int numframe = 0;
	numframe++;
	if (state == MenuDraw_Active || (numframe % 4) < 2)
	{
		if (numframe > 10000) numframe = 0;
		float x = (selCol == 0 ? PicX : NameX) - 48;
		float y = EntryY + 20 + EntryHeight * selRow;
		DrawTextureEx(*m_cursor, { x, y }, 0.0f, 2.0f, WHITE);
	}
}

void NewGameMenu::SetClass(int classId)
{
	players[selRow].Class = classId;
}

void NewGameMenu::SetName(const char* name)
{
	strcpy_s(players[selRow].Name, name);
}

//----------------------------------------------------------------------------
//  ChooseClassMenu
//----------------------------------------------------------------------------

ChooseClassMenu::ChooseClassMenu(NewGameMenu* parent)
	: parent(parent),
	selCol(0),
	selRow(0)
{
	m_cursor = ResourceManager::GetGlobalTexture("images/cursor.png");
}

MenuAction ChooseClassMenu::Update(Menu*& nextMenu)
{
	if (Input::IsKeyPressing(KEY_RIGHT))
	{
		selCol = (selCol + 1) % 2;
		GameMenu::PlayCursor();
	}
	else if (Input::IsKeyPressing(KEY_LEFT))
	{
		selCol = (selCol - 1 + 2) % 2;
		GameMenu::PlayCursor();
	}
	else if (Input::IsKeyPressing(KEY_DOWN))
	{
		selRow = (selRow + 1) % 3;
		GameMenu::PlayCursor();
	}
	else if (Input::IsKeyPressing(KEY_UP))
	{
		selRow = (selRow - 1 + 3) % 3;
		GameMenu::PlayCursor();
	}
	else if (Input::IsKeyPressing(ConfirmKey))
	{
		int classId = GetSelectedClass();

		parent->SetClass(classId);
		GameMenu::PlayConfirm();

		return Menu_Pop;
	}
	else if (Input::IsKeyPressing(CancelKey))
	{
		return Menu_Pop;
	}

	return Menu_None;
}

void ChooseClassMenu::Draw(MenuDrawState state)
{
	if (prevMenu != nullptr)
		prevMenu->Draw(MenuDraw_Paused);

	const int EntryX = (float)GetScreenWidth() / 2.0f + 100;
	const int EntryY = 100;
	const int EntryWidth = 200;
	const int EntryHeight = 160;

	for (int r = 0; r < 3; r++)
	{
		for (int c = 0; c < 2; c++)
		{
			int x = EntryX + EntryWidth * c;
			int y = EntryY + EntryHeight * r;
			int classId = r * 2 + c;

			GameMenu::DrawClass(classId, x, y + 48);
		}
	}

	int selClassId = GetSelectedClass();
	const char* className = Player::GetClassName(selClassId);
	DrawText(className, EntryX + 70, 60, 24, WHITE);

	static int numframe = 0;
	numframe++;
	if (state == MenuDraw_Active || (numframe % 4) < 2)
	{
		if (numframe > 10000) numframe = 0;
		float x = EntryX + EntryWidth * selCol - 60;
		float y = EntryY + EntryHeight * selRow + 60;
		DrawTextureEx(*m_cursor, { x, y }, 0.0f, 2.0f, WHITE);
	}
}

int ChooseClassMenu::GetSelectedClass()
{
	return selRow * 2 + selCol;
}

//----------------------------------------------------------------------------
//  ChooseNameMenu
//----------------------------------------------------------------------------

const int LineCount = 8;
const int LineLength = 19;
const int LineCols = 10;

const char chars[LineCount][LineLength + 1] =
{
"A B C D E F G H I J",
"K L M N O P Q R S T",
"U V W X Y Z        ",
"a b c d e f g h i j",
"k l m n o p q r s t",
"u v w x y z        ",
"0 1 2 3 4 5 6 7 8 9",
"                END"
};


ChooseNameMenu::ChooseNameMenu(NewGameMenu* parent, const char* name)
	: parent(parent),
	selCol(0),
	selRow(0),
	nameLength(0)
{
	if (strcmp(name, DefaultName) == 0)
	{
		this->name[0] = '\0';
	}
	else
	{
		strcpy_s(this->name, name);
		nameLength = strlen(name);
	}

	m_cursor = ResourceManager::GetGlobalTexture("images/cursor.png");
}

MenuAction ChooseNameMenu::Update(Menu*& nextMenu)
{
	if (Input::IsKeyPressing(KEY_RIGHT))
	{
		selCol = (selCol + 1) % LineCols;

		if (selRow == LineCount - 1 && selCol == LineCols - 1)
			selCol = 0;

		GameMenu::PlayCursor();
	}
	else if (Input::IsKeyPressing(KEY_LEFT))
	{
		selCol = (selCol - 1 + LineCols) % LineCols;
		GameMenu::PlayCursor();
	}
	else if (Input::IsKeyPressing(KEY_DOWN))
	{
		selRow = (selRow + 1) % LineCount;
		GameMenu::PlayCursor();
	}
	else if (Input::IsKeyPressing(KEY_UP))
	{
		selRow = (selRow - 1 + LineCount) % LineCount;
		GameMenu::PlayCursor();
	}
	else if (Input::IsKeyPressing(ConfirmKey))
	{
		GameMenu::PlayConfirm();

		if (selRow == LineCount - 1 && selCol == LineCols - 2)
		{
			parent->SetName(name);
			return Menu_Pop;
		}

		InsertChar();
	}
	else if (Input::IsKeyPressing(CancelKey))
	{
		if (nameLength == 0)
			return Menu_Pop;

		DeleteChar();
	}

	if (selRow == LineCount - 1 && selCol == LineCols - 1)
		selCol--;

	return Menu_None;
}

void ChooseNameMenu::InsertChar()
{
	int index = selCol * 2;
	char ch = chars[selRow][index];

	if (nameLength < NameLength)
	{
		name[nameLength] = ch;
		nameLength++;
		name[nameLength] = '\0';
	}
	else
	{
		// replace the last char, if name is full
		name[nameLength - 1] = ch;
	}
}

void ChooseNameMenu::DeleteChar()
{
	if (nameLength > 0)
	{
		nameLength--;
		name[nameLength] = '\0';
	}
}

void ChooseNameMenu::Draw(MenuDrawState state)
{
	if (prevMenu != nullptr)
		prevMenu->Draw(MenuDraw_Paused);

	const int BoxX = (float)GetScreenWidth() / 2.0f + 60;
	const int BoxY = 100;
	const int NameX = BoxX + 24;
	const int NameY = BoxY + 24;
	const int CharsX = BoxX + 30;
	const int CharsY = BoxY + 48;

	DrawText(name, NameX, NameY, 24, WHITE);

	for (int i = 0; i < LineCount; i++)
		DrawText(chars[i], CharsX, CharsY + i * 24, 24, WHITE);

	static int numframe = 0;
	numframe++;
	if (state == MenuDraw_Active || (numframe % 4) < 2)
	{
		if (numframe > 10000) numframe = 0;
		float x = CharsX + 24 * selCol - 30;
		float y = CharsY + 24 * selRow;
		DrawTextureEx(*m_cursor, { x, y }, 0.0f, 2.0f, WHITE);
	}
}