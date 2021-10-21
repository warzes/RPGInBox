#include "stdafx.h"
#include "BattleCell.h"
#include "Player.h"
#include "EnemyTemplate.h"
#include "DebugNew.h"
//-----------------------------------------------------------------------------
constexpr Point2 LeftTopCoordCells = { 262, 44 };
constexpr Point2 OffsetCoordCells = { 10, 10 };
constexpr Point2 SizeCoordCells = { 160, 160 };
//-----------------------------------------------------------------------------
void BattleCell::ResetCellStatus() noexcept
{
	m_status = BattleCellStatus::Normal;
}
//-----------------------------------------------------------------------------
void BattleCell::Draw(float deltaTime, const Point2& pos, bool isAnimSwords, UIAnimSwords& animSwords) noexcept
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
		animSwords.UpdateAndDraw(deltaTime, { pos.x + 20, pos.y + 20 }, (SizeCoordCells.x - 40) / 32.0f);
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
			m_cells[x][y].m_creature.position = { (int)x, (int)y };

			m_cells[x][y + 2].m_creature.creature = player->grid[x][y];
			m_cells[x][y + 2].m_creature.position = { (int)x, (int)y + 2 };
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
void BattleCells::SetFirstMember() noexcept
{
	m_currentMember = m_members.size();
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
		++m_currentMember;
		if (m_currentMember >= m_members.size()) // всех перебрали, новый раунд
			return -1;
		if (m_members[m_currentMember]->creature && m_members[m_currentMember]->creature->IsAlive())
			break;
	}
	return m_currentMember;
}
//-----------------------------------------------------------------------------
void BattleCells::ViewCurrentMember() noexcept
{
	SetStatusCell(GetCurrentMember().position.x, GetCurrentMember().position.y, BattleCellStatus::Yellow);
}
//-----------------------------------------------------------------------------
CreatureInCell& BattleCells::GetCurrentMember() noexcept
{
	return *m_members[m_currentMember];
}
//-----------------------------------------------------------------------------
CreatureInCell* BattleCells::GetSelectMember() noexcept
{
	for (size_t x = 0; x < 3; x++)
	{
		for (size_t y = 0; y < 4; y++)
		{
			if (m_cells[x][y].GetStatus() == BattleCellStatus::Blue)
			{
				return m_cells[x][y].GetCreature();
			}
		}
	}
	return nullptr;
}
//-----------------------------------------------------------------------------
void BattleCells::ResetStatusCells() noexcept
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
void BattleCells::ResetAnimSword() noexcept
{
	m_isAnimSwords = false;
	m_animSwords.Reset();
}
//-----------------------------------------------------------------------------
int BattleCells::GetNumberHero() const noexcept
{
	int num = 0;
	for (int i = 0; i < m_members.size(); i++)
	{
		if (m_members[i]->creature && m_members[i]->creature->IsAlive() && m_members[i]->creature->GetPartyType() == PartyType::Hero)
			num++;
	}
	return num;
}
//-----------------------------------------------------------------------------
int BattleCells::GetNumberEnemy() const noexcept
{
	int num = 0;
	for (int i = 0; i < m_members.size(); i++)
	{
		if (m_members[i]->creature && m_members[i]->creature->IsAlive() && m_members[i]->creature->GetPartyType() == PartyType::Enemy)
			num++;
	}
	return num;
}
//-----------------------------------------------------------------------------
TargetMeleeAttack BattleCells::GetTargetMeleeAttack() noexcept
{
	TargetMeleeAttack ret;

	// кто сейчас управляет текущим существом?
	auto &currentCreature = GetCurrentMember();
	if (!currentCreature.creature || !currentCreature.creature->IsAlive())
		return ret;

	if (currentCreature.creature->GetPartyType() == PartyType::Hero)
	{
		int currentPosX = currentCreature.position.x;
		int currentPosY = currentCreature.position.y-1;
		if (currentPosY < 0) return ret;

		bool inFirstLine = false;
		for (int i = 0; i < 3; i++)
		{
			if (m_cells[i][currentPosY].m_creature.creature && m_cells[i][currentPosY].m_creature.creature->IsAlive())
			{
				inFirstLine = true;
				if (m_cells[i][currentPosY].m_creature.creature->GetPartyType() == PartyType::Enemy) // а нельзя бить своих
					ret.target[i] = &m_cells[i][currentPosY];
			}
		}		
		if (inFirstLine == false)
		{
			currentPosY--;
			if (currentPosY < 0) return ret;
			for (int i = 0; i < 3; i++)
			{
				if (m_cells[i][currentPosY].m_creature.creature && m_cells[i][currentPosY].m_creature.creature->IsAlive())
				{
					ret.target[i] = &m_cells[i][currentPosY];
				}
			}
		}
	}
	else
	{
		// TODO: цели для атаки от вражеского моба
	}

	return ret;
}
//-----------------------------------------------------------------------------
TargetRangeAttack BattleCells::GetTargetRangeAttack() noexcept
{
	TargetRangeAttack ret;

	// кто сейчас управляет текущим существом?
	auto& currentCreature = GetCurrentMember();
	if (!currentCreature.creature || !currentCreature.creature->IsAlive())
		return ret;

	if (currentCreature.creature->GetPartyType() == PartyType::Hero)
	{
		for (int y = 0; y < 2; y++)
		{
			for (int x = 0; x < 3; x++)
			{
				if (m_cells[x][y].m_creature.creature && m_cells[x][y].m_creature.creature->IsAlive())
				{
					if (m_cells[x][y].m_creature.creature->GetPartyType() == PartyType::Enemy) // а нельзя бить своих
					{
						ret.target[x][y] = &m_cells[x][y];
					}
				}
			}
		}
	}
	else
	{
		// TODO: цели для атаки от вражеского моба
	}

	return ret;
}
//-----------------------------------------------------------------------------
bool BattleCells::IsMeleeAttack() noexcept
{
	bool ret = false;
	auto& currentCreature = GetCurrentMember();
	if (!currentCreature.creature || !currentCreature.creature->IsAlive())
		return ret;

	if (currentCreature.creature->GetPartyType() == PartyType::Hero)
	{
		const int currentPosY = currentCreature.position.y;
		if (currentPosY == 2) return true; // герой стоит в первом ряду, а значит может бить ближним ударом
		else if (currentPosY == 3)
		{
			const int posY = currentPosY - 1;
			for (int i = 0; i < 3; i++)
			{
				if (m_cells[i][posY].m_creature.creature && m_cells[i][posY].m_creature.creature->IsAlive()) // TODO: возможно еще проверять на сознание - типа если без сознания то тыл может бить
				{
					return false; // перед героем стоит другой герой, а значит этот не может бить ближним ударом
				}
			}
			return true;
		}
	}
	else
	{

	}

	return ret;
}
//-----------------------------------------------------------------------------
bool BattleCells::IsRangeAttack() noexcept
{
	bool ret = false;
	auto& currentCreature = GetCurrentMember();
	if (!currentCreature.creature || !currentCreature.creature->IsAlive())
		return ret;

	if (currentCreature.creature->GetPartyType() == PartyType::Hero)
	{
		const int currentPosY = currentCreature.position.y;
		if (currentPosY == 2) return false; // герой стоит в первом ряду, а значит не может стрелять
		else if (currentPosY == 3)
		{
			const int posY = currentPosY - 1;
			for (int i = 0; i < 3; i++)
			{
				if (m_cells[i][posY].m_creature.creature && m_cells[i][posY].m_creature.creature->IsAlive()) // TODO: возможно еще проверять на сознание -  если без сознания то тыл может бить
				{
					return true; // перед героем стоит другой герой, а значит этот может стрелять
				}
			}
			return false;
		}
	}
	else
	{

	}

	return ret;
}
//-----------------------------------------------------------------------------