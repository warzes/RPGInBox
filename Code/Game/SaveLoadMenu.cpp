#include "stdafx.h"
#include "SaveLoadMenu.h"
#include "Input.h"
#include "Player.h"
#include "GameScenes.h"
#include <Engine/DebugNew.h>

const int BoxY = 16;
const int BoxHeight = 56;
const int TopLineYOff = 8;
const int MidLineYOff = 24;
const int BottomLineYOff = 40;

//----------------------------------------------------------------------------
//  SaveLoadMenu
//----------------------------------------------------------------------------

SaveLoadMenu::SaveLoadMenu(Action action)
	: action(action),
	selIndex(0),
	topRow(0),
	hasError(false)
{
	memset(&summaries, 0, sizeof summaries);
	hasError = !SaveFolder::ReadSummaries(summaries);

	m_patchTexture = ResourceManager::GetGlobalTexture("../data/ui/battleui_background.png");
	m_cursor = ResourceManager::GetGlobalTexture("images/cursor.png");
}

MenuAction SaveLoadMenu::Update(Menu*& nextMenu)
{
	if (hasError)
	{
		if (Input::IsKeyPressing(ConfirmKey) || Input::IsKeyPressing(CancelKey))
			return Close();

		return Menu_None;
	}

	if (Input::IsKeyPressing(KEY_DOWN))
	{
		if (selIndex < Slots - 1)
		{
			selIndex++;
			GameMenu::PlayCursor();
		}
	}
	else if (Input::IsKeyPressing(KEY_UP))
	{
		if (selIndex > 0)
		{
			selIndex--;
			GameMenu::PlayCursor();
		}
	}
	else if (Input::IsKeyPressing(ConfirmKey))
	{
		if (action == Save)
		{
			GameMenu::PlayConfirm();

			hasError = !SaveFolder::SaveFile(selIndex);
			if (!hasError)
				return Menu_Pop;
		}
		else if (action == Load)
		{
			if (summaries.Summaries[selIndex].State == 1)
			{
				GameMenu::PlayConfirm();

				hasError = !SaveFolder::LoadFile(selIndex);
				if (!hasError)
				{
					Point2 pos = Player::GetPlayerPos();
					GameScenes::SwitchToField(pos.x, pos.y);
				}
			}
			else
				GameMenu::PlayError();
		}
	}
	else if (Input::IsKeyPressing(CancelKey))
	{
		return Close();
	}

	if (selIndex < topRow)
	{
		topRow = selIndex;
	}
	else if (selIndex >= topRow + 4)
	{
		topRow = selIndex - 3;
	}

	return Menu_None;
}

MenuAction SaveLoadMenu::Close()
{
	if (action == Save)
	{
		return Menu_Pop;
	}
	else if (action == Load)
	{
		GameScenes::SwitchScene(SceneId_Title);
	}

	return Menu_None;
}

void SaveLoadMenu::Draw(MenuDrawState state)
{
	const char* title = (action == Save) ? "SAVE" : "LOAD";

	for (int i = 0; i < 4; i++)
	{
		int index = topRow + i;
		int boxY = BoxY + BoxHeight * i;

		DrawFile(index, boxY);
	}

	Rectangle dest = { 176.0f, 0.0f, 80.0f, 24.0f };
	NPatchInfo ninePatchInfo = { { 0.0f, 0.0f, 64.0f, 64.0f }, 6, 6, 6, 6, NPATCH_NINE_PATCH };
	DrawTextureNPatch(*m_patchTexture, ninePatchInfo, dest, { 0.0f, 0.0f }, 0.0f, WHITE);
	
	DrawText(title, 192, 8, 24, WHITE);

	if (hasError)
		DrawErrorMessage();

	static int numframe = 0;
	numframe++;
	if (state == MenuDraw_Active || (numframe % 4) < 2)
	{
		float y = BoxY + MidLineYOff + BoxHeight * (selIndex - topRow);
		DrawTextureEx(*m_cursor, { 0.0f, y }, 0.0f, 2.0f, WHITE);
	}
}

void SaveLoadMenu::DrawFile(int slot, int boxY)
{
	char str[32];
	auto& summary = summaries.Summaries[slot];

	Rectangle dest = { 0, boxY, 256, BoxHeight };
	NPatchInfo ninePatchInfo = { { 0.0f, 0.0f, 64.0f, 64.0f }, 6, 6, 6, 6, NPATCH_NINE_PATCH };
	DrawTextureNPatch(*m_patchTexture, ninePatchInfo, dest, { 0.0f, 0.0f }, 0.0f, WHITE);

	sprintf_s(str, "%d", slot + 1);
	DrawText(str, 16, boxY + MidLineYOff, 24, WHITE);

	if (summary.State == 0)
	{
		DrawText("EMPTY", 48, boxY + MidLineYOff, 24, WHITE);
		return;
	}

	DrawText(summary.Name, 40, boxY + TopLineYOff, 24, WHITE);

	for (int j = 0; j < Players; j++)
		GameMenu::DrawClass(summary.Classes[j], 96 + 24 * j, boxY + TopLineYOff);

	for (int r = 0; r < 2; r++)
	{
		for (int c = 0; c < 2; c++)
		{
			int index = r * 2 + c;
			int x = 192 + c * 16;
			int y = boxY + TopLineYOff + r * 16;

			GameMenu::DrawOrb(index, summary.Orbs[index], x, y);
		}
	}

	uint32_t hours = 0;
	uint32_t minutes = 0;
	Global::GetHoursMinutes(summary.TimeMillis, hours, minutes);

	sprintf_s(str, "%02u:%02u", hours, minutes);
	DrawText(str, 40, boxY + BottomLineYOff, 24, WHITE);

	sprintf_s(str, "%6u G", summary.Gil);
	DrawText(str, 40 + 48, boxY + BottomLineYOff, 24, WHITE);

	sprintf_s(str, "Lv %u", summary.Level);
	DrawText(str, 40, boxY + MidLineYOff, 24, WHITE);
}

void SaveLoadMenu::DrawErrorMessage()
{
	const int BoxX = 40;
	const int BoxY = 64;
	const char* Message = "There was an error \nreading or writing \nthe save file.";

	Rectangle dest = { BoxX, BoxY, 176, 112 };
	NPatchInfo ninePatchInfo = { { 0.0f, 0.0f, 64.0f, 64.0f }, 6, 6, 6, 6, NPATCH_NINE_PATCH };
	DrawTextureNPatch(*m_patchTexture, ninePatchInfo, dest, { 0.0f, 0.0f }, 0.0f, WHITE);
	DrawText(Message, BoxX + 16, BoxY + 32, 24, WHITE);
}