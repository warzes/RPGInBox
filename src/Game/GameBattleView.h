#pragma once

#include "BattleCore.h"

class ResourceManager;

class UIBattlePanelBG
{
public:
	void Create(ResourceManager* resourceMgr);
	void Draw();
private:
	std::shared_ptr<Texture2D> m_patchTexture = nullptr;
};

enum class BattleCellStatus
{
	Normal,	// ничего не рисует
	Yellow,	// выделяет желтым цветом (например текущий персонаж)
	Green,	// выделяет зеленым цветом (например возможные клетки для выбора)
	Red,	// выделяет красным цветом (например недоступные клетки для выбора)
	Grey	// выделяет серым цветом
};

class GameBattleView final
{
public:
	GameBattleView(ResourceManager& resourceMgr) noexcept;
	bool Init() noexcept;

	void Frame() noexcept;

	void SetStatusCell(size_t x, size_t y, BattleCellStatus status) noexcept;
	void ResetCells() noexcept;

private:
	void drawBackground() noexcept;
	void drawPanels() noexcept;
	void drawCells() noexcept;

	BattleCellStatus m_cells[3][4] = { BattleCellStatus::Normal };

	ResourceManager& m_resourceMgr;
	UIBattlePanelBG m_background;	
	std::shared_ptr<Texture2D> m_battleBackGround = nullptr;
};