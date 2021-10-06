#include "stdafx.h"
#include "GameBattleState.h"
#include "ResourceManager.h"
#include "Creature.h"
#include "Player.h"
#include "EnemyTemplate.h"
#include "EngineMath.h"
#include "DebugNew.h"
//-----------------------------------------------------------------------------
constexpr auto PlayerActionMainMenu = MakeArray<const char*>("Attack", "Skill", "Magic", "Defence");
static_assert(PlayerActionMainMenu.size() == 4);
constexpr auto PlayerActionMainMenu_Attack = MakeArray<const char*>("Melee", "Shoot", "Cancel");
static_assert(PlayerActionMainMenu_Attack.size() == 3);
//-----------------------------------------------------------------------------
GameBattleState::GameBattleState(Player& player, ResourceManager& resourceMgr) noexcept
	: m_resourceMgr(resourceMgr)
	, m_player(player)
{
}
//-----------------------------------------------------------------------------
bool GameBattleState::Init() noexcept
{
	// загрузка текстур
	m_background.Create(&m_resourceMgr);
	m_battleBackGround = m_resourceMgr.GetTexture("../data/temp/textures/character/plains-ground.png");

	// создание меню
	m_playerMenu.AddElements({ 810, 415 }, PlayerActionMainMenu);
	m_playerMenu_attack.AddElements({ 810, 415 }, PlayerActionMainMenu_Attack);

	return true;
}
//-----------------------------------------------------------------------------
void GameBattleState::StartBattle(EnemyParty* enemies) noexcept
{
	m_enemies = enemies;
	// сброс всех переменных
	m_round = 0;
	m_battleState = BattleState::NewRound;
	m_members.clear();
	for (size_t i = 0; i < m_player.heroes.size(); i++)
		m_members.push_back(&m_player.heroes[i]);
	for (size_t i = 0; i < enemies->enemys.size(); i++)
		m_members.push_back(&enemies->enemys[i]);
	m_currentMember = 0;



	ResetCells();
	m_currentPlayerMenu = &m_playerMenu;
	m_playerMenuStage = PlayerMenu::Main;
}
















//-----------------------------------------------------------------------------
void GameBattleState::Update(float deltaTime) noexcept
{
	if (m_battleState == BattleState::NewRound)
	{
		newRound();
	}
	else if (m_battleState == BattleState::SelectAction)
	{
		// TODO: игрок может управлять не только своими существами, а например очарованными. Или наоборот некоторые его герои будут неконтролируемыми
		bool isPlayer = false;
		//if (m_members[m_currentMember]->GetPartyType() == PartyType::Hero)
		//	selectPlayerAction();
		//else
		//	selectEnemyAction();
	}
	m_currentPlayerMenu->Run();

	if (m_playerMenuStage == PlayerMenu::Main) // начальное меню команд
	{
		switch (m_currentPlayerMenu->IsSelect())
		{
		case 0: m_playerMenuStage = PlayerMenu::Attack; m_currentPlayerMenu = &m_playerMenu_attack; break;
		default:
			break;
		}
	}
	else if (m_playerMenuStage == PlayerMenu::Attack) //  меню команд атак
	{
		switch (m_currentPlayerMenu->IsSelect())
		{
		case 2: m_playerMenuStage = PlayerMenu::Main; m_currentPlayerMenu = &m_playerMenu; break;
		default:
			break;
		}
	}
	


	
}
//-----------------------------------------------------------------------------
void GameBattleState::Frame() noexcept
{
	drawBackground();
	drawPanels();
	drawCells(m_player, m_enemies);
	drawPlayerMenu();
	m_currentPlayerMenu->Draw();
}
//-----------------------------------------------------------------------------
void GameBattleState::selectPlayerTargetMeleeAttack() noexcept
{
	//m_view.ResetCells();


	//auto p = selectCell();
	//if (p.x >= 0 && p.y >= 0)
	//{	
	//	
	//	m_view.SetStatusCell(p.x, p.y, BattleCellStatus::Yellow);
	//}
}
//-----------------------------------------------------------------------------
Point2 GameBattleState::selectCell() noexcept // TODO: возможно перенести в класс правил
{
	if (IsMouseButtonPressed(MOUSE_BUTTON_LEFT))
	{
		auto pos = GetMousePosition();

		pos.x -= LeftTopCoordCells.x;
		pos.y -= LeftTopCoordCells.y;
		if (pos.x > 0 && pos.y > 0)
		{
			Point2 p;
			p.x = (int)pos.x / (SizeCoordCells.x + OffsetCoordCells.x);
			p.y = (int)pos.y / (SizeCoordCells.y + OffsetCoordCells.y);
			if (p.x < 3 && p.y < 4)
				return p;
		}
	}
	else if (IsMouseButtonPressed(MOUSE_BUTTON_RIGHT))
	{
		ResetCells();
	}
	return {-1,-1};
}
//-----------------------------------------------------------------------------
void GameBattleState::drawBackground() noexcept
{
	// отрисовка заднего фона окна боя
	m_background.Draw();

	// отрисовка пола
	DrawTextureTiled(*m_battleBackGround, { 0.0f,0.0f,64.0f,64.0f }, { (float)LeftTopCoordCells.x, (float)LeftTopCoordCells.y,500.0f,680.0f }, { 0.0f,0.0f }, 0.0f, 1.0f, WHITE);

	// отрисовка линии разделения поля боя
	DrawRectangle((int)LeftTopCoordCells.x, 383, 500, 3, WHITE);
}
//-----------------------------------------------------------------------------
void GameBattleState::drawPanels() noexcept
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
void GameBattleState::drawCells(const Player& player, EnemyParty* enemies) noexcept
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
				if (player.grid[x][ny] != nullptr && player.grid[x][ny]->IsAlive())
				{
					DrawTexture(*player.grid[x][ny]->battleTexture, cellPos.x, cellPos.y, WHITE);
				}
			}
		}
	}
}
//-----------------------------------------------------------------------------
void GameBattleState::drawPlayerMenu() noexcept
{
}
//-----------------------------------------------------------------------------
void GameBattleState::SetStatusCell(size_t x, size_t y, BattleCellStatus status) noexcept
{
	if (x >= 3 || y >= 4) return;
	m_cells[x][y] = status;
}
//-----------------------------------------------------------------------------
void GameBattleState::ResetCells() noexcept
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
void GameBattleState::newRound() noexcept
{
	m_currentMember = 0; // первый участник боя в новом раунде начинает
	m_battleState = BattleState::SelectAction; // ожидание выбора действия
}