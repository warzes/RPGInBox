#pragma once

#include "BattleCore.h"
#include "oCreature.h"

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
	Normal,	// ������ �� ������
	Yellow,	// �������� ������ ������ (�������� ������� ��������)
	Green,	// �������� ������� ������ (�������� ��������� ������ ��� ������)
	Red,	// �������� ������� ������ (�������� ����������� ������ ��� ������)
	Grey	// �������� ����� ������
};

class GameBattleView final
{
public:
	GameBattleView(ResourceManager& resourceMgr) noexcept;
	bool Init() noexcept;

	void Frame(const Player& player, EnemyParty* enemies) noexcept;

	void SetStatusCell(size_t x, size_t y, BattleCellStatus status) noexcept;
	void ResetCells() noexcept;

private:
	void drawBackground() noexcept;
	void drawPanels() noexcept;
	void drawCells(const Player& player, EnemyParty* enemies) noexcept;
	void drawPlayerMenu() noexcept;

	BattleCellStatus m_cells[3][4] = { BattleCellStatus::Normal };

	ResourceManager& m_resourceMgr;
	UIBattlePanelBG m_background;	
	std::shared_ptr<Texture2D> m_battleBackGround = nullptr;
};