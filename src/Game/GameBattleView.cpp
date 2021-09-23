#include "stdafx.h"
#include "GameBattleView.h"
#include "ResourceManager.h"
#include "Creature.h"
#include "EngineMath.h"
#include "DebugNew.h"
//-----------------------------------------------------------------------------
void UIBattlePanelBG::Create(ResourceManager* resourceMgr)
{
	m_patchTexture = resourceMgr->GetTexture("../data/ui/battleui_background.png");
}
//-----------------------------------------------------------------------------
void UIBattlePanelBG::Draw()
{
	constexpr NPatchInfo ninePatchInfo = { { 0.0f, 0.0f, 64.0f, 64.0f }, 6, 6, 6, 6, NPATCH_NINE_PATCH };
	const Rectangle dest = { 20.0f, 20.0f, (float)GetScreenWidth() - dest.x * 2.0f, (float)GetScreenHeight() - dest.y * 2.0f };
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
	m_battleBackGround = m_resourceMgr.GetTexture("../data/temp/textures/character/plains-ground.png");
	return true;
}
//-----------------------------------------------------------------------------
void GameBattleView::Frame(const Player& player, EnemyParty* enemies) noexcept
{
	drawBackground();
	drawPanels();
	drawCells(player, enemies);
	drawPlayerMenu();
}
//-----------------------------------------------------------------------------
void GameBattleView::SetStatusCell(size_t x, size_t y, BattleCellStatus status) noexcept
{
	if (x >= 3 || y >= 4) return;
	m_cells[x][y] = status;
}
//-----------------------------------------------------------------------------
void GameBattleView::ResetCells() noexcept
{
	for (size_t x = 0; x < 3; x++)
	{
		for (size_t y = 0; y < 4; y++)
		{
			m_cells[x][y] = BattleCellStatus::Normal;
		}
	}
}
//-----------------------------------------------------------------------------
void GameBattleView::drawBackground() noexcept
{
	// отрисовка заднего фона окна боя
	m_background.Draw();

	// отрисовка пола
	DrawTextureTiled(*m_battleBackGround, { 0.0f,0.0f,64.0f,64.0f }, { (float)LeftTopCoordCells.x, (float)LeftTopCoordCells.y,500.0f,680.0f }, { 0.0f,0.0f }, 0.0f, 1.0f, WHITE);

	// отрисовка линии разделения поля боя
	DrawRectangle((int)LeftTopCoordCells.x, 383, 500, 3, WHITE);
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
void GameBattleView::drawCells(const Player& player, EnemyParty* enemies) noexcept
{
	Point2 cellPos;
	for (size_t x = 0; x < 3; x++)
	{
		for (size_t y = 0; y < 4; y++)
		{			
			cellPos.x = LeftTopCoordCells.x + (SizeCoordCells.x + OffsetCoordCells.x) * (int)x;
			cellPos.y = LeftTopCoordCells.y + 5 + (SizeCoordCells.y + OffsetCoordCells.y) * (int)y;

			// cell
			switch (m_cells[x][y])
			{
			case BattleCellStatus::Normal: 
				DrawRectangle(cellPos.x, cellPos.y, SizeCoordCells.x, SizeCoordCells.y, { 255, 255, 255, 180 });
				break;
			case BattleCellStatus::Yellow:
				DrawRectangle(cellPos.x, cellPos.y, SizeCoordCells.x, SizeCoordCells.y, { 255, 210, 80, 180 });
				break;
			case BattleCellStatus::Green:
				DrawRectangle(cellPos.x, cellPos.y, SizeCoordCells.x, SizeCoordCells.y, { 70, 255, 20, 180 });
				break;
			case BattleCellStatus::Red:
				DrawRectangle(cellPos.x, cellPos.y, SizeCoordCells.x, SizeCoordCells.y, { 255, 30, 30, 180 });
				break;
			case BattleCellStatus::Grey:
				DrawRectangle(cellPos.x, cellPos.y, SizeCoordCells.x, SizeCoordCells.y, { 100, 100, 100, 180 });
				break;
			default: break;
			}

			// enemy creature
			if (y < 2)
			{
				if (enemies->grid[x][y] != nullptr && enemies->grid[x][y]->IsAlive())
				{
					DrawTexture(*enemies->grid[x][y]->battleTexture, cellPos.x, cellPos.y, WHITE);
				}
			}
			else
			{
				size_t ny = y - 2;
				if (player.grid[x][ny] != nullptr  && player.grid[x][ny]->IsAlive())
				{
					DrawTexture(*player.grid[x][ny]->battleTexture, cellPos.x, cellPos.y, WHITE);
				}
			}
		}
	}
}
//-----------------------------------------------------------------------------
void GameBattleView::drawPlayerMenu() noexcept
{
	//for (size_t i = 0; i < Countof(playerCommandRect); i++)
	//	DrawRectangleLinesEx(playerCommandRect[i], 1.0f, WHITE);

	//if (m_selectPlayerCommand >= 0)
	//	DrawText(">>>            <<<", 770, 420 + m_selectPlayerCommand * 40, 30, GREEN);

	DrawText("Attack", 820, 420, 30, WHITE);
	DrawText("Skill", 820, 460, 30, WHITE);
	DrawText("Magic", 820, 500, 30, WHITE);
	DrawText("Defence", 820, 540, 30, WHITE);
}
//-----------------------------------------------------------------------------