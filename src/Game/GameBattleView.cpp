#include "stdafx.h"
#include "GameBattleView.h"
#include "ResourceManager.h"
#include "DebugNew.h"
// позиции клеток
constexpr Rectangle battleCell[] =
{
	{ 262.0f,  44.0f, 160.0f, 160.0f }, // 1x1
	{ 432.0f,  44.0f, 160.0f, 160.0f }, // 2x1
	{ 602.0f,  44.0f, 160.0f, 160.0f }, // 3x1

	{ 262.0f, 214.0f, 160.0f, 160.0f }, // 1x2
	{ 432.0f, 214.0f, 160.0f, 160.0f }, // 2x2
	{ 602.0f, 214.0f, 160.0f, 160.0f }, // 3x2

	{ 262.0f, 394.0f, 160.0f, 160.0f }, // 1x3
	{ 432.0f, 394.0f, 160.0f, 160.0f }, // 2x3
	{ 602.0f, 394.0f, 160.0f, 160.0f }, // 3x3

	{ 262.0f, 564.0f, 160.0f, 160.0f }, // 1x4
	{ 432.0f, 564.0f, 160.0f, 160.0f }, // 2x4
	{ 602.0f, 564.0f, 160.0f, 160.0f }, // 3x4
};
//-----------------------------------------------------------------------------
void UIBattlePanelBG::Create(ResourceManager* resourceMgr)
{
	m_patchTexture = resourceMgr->GetTexture("../data/ui/battleui_background.png");
}
//-----------------------------------------------------------------------------
void UIBattlePanelBG::Draw()
{
	constexpr NPatchInfo ninePatchInfo = { { 0.0f, 0.0f, 64.0f, 64.0f }, 6, 6, 6, 6, NPATCH_NINE_PATCH };

	Rectangle dest = { 20.0f, 20.0f, 0.0f, 0.0f };
	dest.width = (float)GetScreenWidth() - dest.x * 2.0f;
	dest.height = (float)GetScreenHeight() - dest.y * 2.0f;

	DrawTextureNPatch(*m_patchTexture, ninePatchInfo, dest, { 0.0f, 0.0f }, 0.0f, { 255, 255, 255, 180 });
}
//-----------------------------------------------------------------------------
GameBattleView::GameBattleView(ResourceManager& resourceMgr) noexcept
	: m_resourceMgr(resourceMgr)
{
}
//-----------------------------------------------------------------------------
bool GameBattleView::Init() noexcept
{
	m_background.Create(&m_resourceMgr);

	m_textureUI_character = m_resourceMgr.GetTexture("../data/ui/battleui_member.png");
	m_battleBackGround = m_resourceMgr.GetTexture("../data/temp/textures/character/plains-ground.png");

	return true;
}
//-----------------------------------------------------------------------------
void GameBattleView::Frame() noexcept
{
	drawBackground();
	drawPanels();
}
//-----------------------------------------------------------------------------
void GameBattleView::drawBackground() noexcept
{
	// отрисовка заднего фона окна боя
	m_background.Draw();

	// отрисовка пола
	DrawTextureTiled(*m_battleBackGround, { 0,0,64,64 }, { battleCell[0].x, battleCell[0].y,500,680 }, { 0,0 }, 0.0, 1.0, WHITE);

	// отрисовка линии разделения поля боя
	DrawRectangle((int)battleCell[0].x, 383, 500, 3, WHITE);
}
//-----------------------------------------------------------------------------
void GameBattleView::drawPanels() noexcept
{
	// отрисовка боковых панелей

	// левая верхняя панель
	{
		DrawRectangle(29, 45, 230, 335, { 0, 0, 0, 180 });
		DrawText("Enemy", 135, 100, 38, RED);
		DrawText("Enemy", 135, 270, 38, RED);
	}

	// левая нижняя панель
	{
		DrawRectangle(29, 390, 230, 335, { 0, 0, 0, 180 });
		DrawText("Player", 125, 450, 38, GREEN);
		DrawText("Player", 125, 620, 38, GREEN);
	}

	// правая верхняя панель
	{
		DrawRectangle(765, 45, 230, 335, { 0, 0, 0, 180 });
		DrawText("INFO WINDOW", 775, 50, 30, WHITE);
	}

	// правая нижняя панель
	{
		DrawRectangle(765, 390, 230, 335, { 0, 0, 0, 180 });
	}
}
//-----------------------------------------------------------------------------