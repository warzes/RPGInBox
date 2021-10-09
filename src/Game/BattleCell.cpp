﻿#include "stdafx.h"
#include "BattleCell.h"
#include "Player.h"
#include "EnemyTemplate.h"
#include "DebugNew.h"
constexpr Point2 LeftTopCoordCells = { 262, 44 };
constexpr Point2 OffsetCoordCells = { 10, 10 };
constexpr Point2 SizeCoordCells = { 160, 160 };
//-----------------------------------------------------------------------------
void BattleCell::ResetCellStatus() noexcept
{
	m_status = BattleCellStatus::Normal;
}
//-----------------------------------------------------------------------------
void BattleCell::Draw(float deltaTime, const Point2& pos, bool isAnimSwords, UIAnimSwords& animSwords) const noexcept
{
	// cell
	Color colorCell = WHITE;
	switch (m_status)
	{
	case BattleCellStatus::Normal: colorCell = { 255, 255, 255, 180 }; break;
	case BattleCellStatus::Yellow: colorCell = { 255, 210, 80, 180 }; break;
	case BattleCellStatus::Green: colorCell = { 70, 255, 20, 180 }; break;
	case BattleCellStatus::Red: colorCell = { 255, 30, 30, 180 }; break;
	case BattleCellStatus::Blue: colorCell = { 100, 30, 255, 180 }; break;
	case BattleCellStatus::Grey: colorCell = { 100, 100, 100, 180 }; break;
	default: break;
	}
	DrawRectangle(pos.x, pos.y, SizeCoordCells.x, SizeCoordCells.y, colorCell);

	if (m_creature.creature && m_creature.creature->IsAlive())
		DrawTexture(*m_creature.creature->battleTexture, pos.x, pos.y, WHITE);

	// Draw Melee Attack
	if ((isAnimSwords == false && m_status == BattleCellStatus::Green) || m_status == BattleCellStatus::Blue)
		animSwords.Draw(deltaTime, { pos.x + 20, pos.y + 20 }, (SizeCoordCells.x - 40) / 32.0f);
}
//-----------------------------------------------------------------------------
bool BattleCells::Init(ResourceManager& resourceMgr) noexcept
{
	if (!m_animSwords.Init(resourceMgr))
		return false;
	return true;
}
//-----------------------------------------------------------------------------
void BattleCells::SetCreature(Player* player, EnemyParty* enemy) noexcept
{
	for (size_t x = 0; x < 3; x++)
	{
		for (size_t y = 0; y < 2; y++)
		{
			m_cells[x][y].m_creature.creature = enemy->grid[x][y];
			m_cells[x][y].m_creature.posInCell = { (int)x, (int)y };

			m_cells[x][y + 2].m_creature.creature = player->grid[x][y];
			m_cells[x][y + 2].m_creature.posInCell = { (int)x, (int)y+2 };
		}
	}

	m_members.clear();
	for (size_t y = 0; y < 4; y++)
	{
		for (size_t x = 0; x < 3; x++)
		{
			m_members.push_back(&m_cells[x][y].m_creature);
		}
	}
}
//-----------------------------------------------------------------------------
void BattleCells::ResetCells() noexcept
{
	for (size_t x = 0; x < 3; x++)
	{
		for (size_t y = 0; y < 4; y++)
		{
			m_cells[x][y].ResetCellStatus();
		}
	}
}
//-----------------------------------------------------------------------------
void BattleCells::SetStatusCell(size_t x, size_t y, BattleCellStatus status) noexcept
{
	if (x >= 3 || y >= 4) return;
	m_cells[x][y].m_status = status;
}
//-----------------------------------------------------------------------------
void BattleCells::Draw(float deltaTime) noexcept
{
	Point2 cellPos;
	for (size_t x = 0; x < 3; x++)
	{
		for (size_t y = 0; y < 4; y++)
		{
			cellPos.x = LeftTopCoordCells.x + (SizeCoordCells.x + OffsetCoordCells.x) * (int)x;
			cellPos.y = LeftTopCoordCells.y + 5 + (SizeCoordCells.y + OffsetCoordCells.y) * (int)y;
			m_cells[x][y].Draw(deltaTime, cellPos, m_isAnimSwords, m_animSwords);
		}
	}
}
//-----------------------------------------------------------------------------
void BattleCells::SetFirstMember() noexcept
{
	m_currentMember = m_members.size(); // первый участник боя в новом раунде
	for (size_t i = 0; i < m_members.size(); i++) // ищем первого живого участника боя
	{
		if (m_members[i]->creature && m_members[i]->creature->IsAlive())
		{
			m_currentMember = i;
			break;
		}
	}
}
//-----------------------------------------------------------------------------
int BattleCells::NextMembers() noexcept
{
	while (true)
	{
		m_currentMember++;
		if (m_currentMember >= m_members.size()) // всех перебрали, новый раунд
			return -1;
		if (m_members[m_currentMember]->creature && m_members[m_currentMember]->creature->IsAlive())
			break;
	}
	return m_currentMember;
}
//-----------------------------------------------------------------------------
CreatureInCell& BattleCells::GetMember() noexcept
{
	return *m_members[m_currentMember];
}
//-----------------------------------------------------------------------------
void BattleCells::ResetAnimSword() noexcept
{
	m_isAnimSwords = false;
	m_animSwords.Reset();
}
//-----------------------------------------------------------------------------
bool BattleCells::IsFinalAnimSworld() noexcept
{
	if (m_isAnimSwords == false)
	{
		m_isAnimSwords = true;
		m_animSwords.Start();
	}
	return m_animSwords.IsFinal();
}
//-----------------------------------------------------------------------------
bool BattleCells::ViewMeleeAttack(int selectTargetCell) noexcept
{
	int y = m_members[m_currentMember]->posInCell.y - 1;
	bool inFirstLine = false;
	for (int i = 0; i < 3; i++) // подсветить доступные цели для ближней атаки
	{
		if (m_cells[i][y].m_creature.creature && m_cells[i][y].m_creature.creature->IsAlive())
		{
			inFirstLine = true;
			if (selectTargetCell == i) SetStatusCell(i, y, BattleCellStatus::Blue);
			else SetStatusCell(i, y, BattleCellStatus::Green);
		}
	}
	if (inFirstLine == false)
	{
		y--;
		for (int i = 0; i < 3; i++) // подсветить доступные цели для ближней атаки
		{
			if (m_cells[i][y].m_creature.creature && m_cells[i][y].m_creature.creature->IsAlive())
			{
				inFirstLine = true;
				if (selectTargetCell == i) SetStatusCell(i, y, BattleCellStatus::Blue);
				else SetStatusCell(i, y, BattleCellStatus::Green);
			}
		}
	}
	return inFirstLine;
}
//-----------------------------------------------------------------------------