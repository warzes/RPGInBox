﻿#include "stdafx.h"
#include "GameBattleState.h"
#include "ResourceManager.h"
#include "Creature.h"
#include "Player.h"
#include "EnemyTemplate.h"
#include "EngineMath.h"
#include "GameInput.h"
#include "DebugNew.h"
//-----------------------------------------------------------------------------
constexpr Point2 LeftTopCoordCells = { 262, 44 };
constexpr Point2 OffsetCoordCells = { 10, 10 };
constexpr Point2 SizeCoordCells = { 160, 160 };
constexpr Point2 PlayerMenuLeftTopPos = { 810, 415 };

// пункты меню
struct PlayerMenuLabel { enum { Attack, Skill, Magic, Defence }; };
constexpr std::array PlayerActionMainMenu{ "Attack", "Skill", "Magic", "Defence" };
static_assert(PlayerActionMainMenu.size() == 4);
struct PlayerMenuAttackLabel { enum { Melee, Shoot }; };
constexpr std::array PlayerActionMainMenu_Attack = { "Melee", "Shoot" };
static_assert(PlayerActionMainMenu_Attack.size() == 2);
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

	if (!m_animSwords.Init(m_resourceMgr))
		return false;

	// создание меню
	m_playerMenu.AddElements(PlayerMenuLeftTopPos, PlayerActionMainMenu);
	m_playerMenu_attack.AddElements(PlayerMenuLeftTopPos, PlayerActionMainMenu_Attack);

	return true;
}
//-----------------------------------------------------------------------------
void GameBattleState::StartBattle(EnemyParty* enemies) noexcept
{
	m_enemies = enemies;
	// сброс всех переменных
	m_members.clear();
	for (size_t i = 0; i < m_player.heroes.size(); i++) m_members.push_back(&m_player.heroes[i]);
	for (size_t i = 0; i < enemies->enemys.size(); i++) m_members.push_back(&enemies->enemys[i]);
	m_round = 0;
	m_currentPlayerMenu = nullptr;
	m_battleState = BattleState::NewRound;
}
//-----------------------------------------------------------------------------
void GameBattleState::Update(float deltaTime) noexcept
{
	m_deltaTime = deltaTime;

	if (m_currentPlayerMenu) m_currentPlayerMenu->Run();

	if (m_battleState == BattleState::NewRound) // новый раунд
		newRound();
	else if (m_battleState == BattleState::BeginWaitAction) // ожидание действия (подготовка)
		beginWaitAction();
	else if (m_battleState == BattleState::ActionsPlayer)// действие игрока
		actionsPlayer();
	//else if (m_battleState == BattleState::ActionEnemy)
	//	waitActionEnemy();
}
//-----------------------------------------------------------------------------
void GameBattleState::Frame() noexcept
{
	drawBackground(); // отрисовка фона
	drawPanels(); // отрисовка панелей
	drawCells(); // отрисовка клеток и персонажей
	if (m_currentPlayerMenu) m_currentPlayerMenu->Draw();
}
//-----------------------------------------------------------------------------
void GameBattleState::resetCells() noexcept
{
	for (size_t x = 0; x < 3; x++)
	{
		for (size_t y = 0; y < 4; y++)
		{
			m_statusCells[x][y] = BattleCellStatus::Normal;
		}
	}
}
//-----------------------------------------------------------------------------
void GameBattleState::drawBackground() noexcept
{
	// отрисовка заднего фона окна боя
	m_background.Draw();

	// отрисовка пола
	DrawTextureTiled(*m_battleBackGround, { 0.0f, 0.0f, 64.0f, 64.0f }, { (float)LeftTopCoordCells.x, (float)LeftTopCoordCells.y, 500.0f, 680.0f }, { 0.0f, 0.0f }, 0.0f, 1.0f, WHITE);

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
void GameBattleState::drawCells() noexcept
{
	Point2 cellPos;
	for (size_t x = 0; x < 3; x++)
	{
		for (size_t y = 0; y < 4; y++)
		{
			cellPos.x = LeftTopCoordCells.x + (SizeCoordCells.x + OffsetCoordCells.x) * (int)x;
			cellPos.y = LeftTopCoordCells.y + 5 + (SizeCoordCells.y + OffsetCoordCells.y) * (int)y;

			// cell
			Color colorCell = WHITE;
			switch (m_statusCells[x][y])
			{
			case BattleCellStatus::Normal: colorCell = { 255, 255, 255, 180 }; break;
			case BattleCellStatus::Yellow: colorCell = { 255, 210, 80, 180 }; break;
			case BattleCellStatus::Green: colorCell = { 70, 255, 20, 180 }; break;
			case BattleCellStatus::Red: colorCell = { 255, 30, 30, 180 }; break;
			case BattleCellStatus::Blue: colorCell = { 100, 30, 255, 180 }; break;
			case BattleCellStatus::Grey: colorCell = { 100, 100, 100, 180 }; break;
			default: break;
			}
			DrawRectangle(cellPos.x, cellPos.y, SizeCoordCells.x, SizeCoordCells.y, colorCell);

			// enemy
			if (y < 2)
			{
				if (m_enemies->grid[x][y] != nullptr && m_enemies->grid[x][y]->IsAlive())
				{
					DrawTexture(*m_enemies->grid[x][y]->battleTexture, cellPos.x, cellPos.y, WHITE);
				}
			}
			// hero
			else
			{
				const size_t ny = y - 2;
				if (m_player.grid[x][ny] != nullptr && m_player.grid[x][ny]->IsAlive())
				{
					DrawTexture(*m_player.grid[x][ny]->battleTexture, cellPos.x, cellPos.y, WHITE);
				}
			}
			// Draw Melee Attack

			if ((m_isAnimSwords == false && m_statusCells[x][y] == BattleCellStatus::Green) || m_statusCells[x][y] == BattleCellStatus::Blue)
				m_animSwords.Draw(m_deltaTime, { cellPos.x + 20, cellPos.y + 20 }, (SizeCoordCells.x - 40) / 32.0f);
		}
	}
}
//-----------------------------------------------------------------------------
void GameBattleState::newRound() noexcept
{
	m_round++; // увеличивается счетчик кол-ва раундов
	m_currentMember = m_members.size(); // первый участник боя в новом раунде
	for (size_t i = 0; i < m_members.size(); i++) // ищем первого живого участника боя
	{
		if (m_members[i]->IsAlive())
		{
			m_currentMember = i;
			break;
		}
	}
	
	m_battleState = BattleState::BeginWaitAction; // ожидание выбора действия
}
//-----------------------------------------------------------------------------
void GameBattleState::beginWaitAction() noexcept
{
	resetCells();

	if (m_members[m_currentMember]->GetPartyType() == PartyType::Hero) // ожидание действия героя
	{
		// выделение клетки активного героя
		const int cellX = m_currentMember % 3;
		const int cellY = m_currentMember / 3 + 2;
		setStatusCell(cellX, cellY, BattleCellStatus::Yellow);

		m_currentPlayerMenu = &m_playerMenu;
		m_battleState = BattleState::ActionsPlayer;
		m_actionPlayerState = ActionPlayerState::SelectMainCommand;
	}
	else // ожидание действия ИИ
	{
		m_currentPlayerMenu = nullptr;
		nextMembers();
	}
}
//-----------------------------------------------------------------------------
void GameBattleState::actionsPlayer() noexcept
{
	static int selectTargetCell = 0;

	if (m_actionPlayerState == ActionPlayerState::SelectMainCommand) // ожидание выбора команды в меню игрока
	{
		// выбор команды в основном меню игрока
		switch (m_playerMenu.IsSelect())
		{
		case PlayerMenuLabel::Attack:
			m_currentPlayerMenu = &m_playerMenu_attack; // переключаем меню на следующее
			m_actionPlayerState = ActionPlayerState::Attack;			
			break;
		default:
			break;
		}
	}
	else if (m_actionPlayerState == ActionPlayerState::Attack)// ожидание выбора команды в меню атаки игрока
	{
		// выбор команды в меню атаки
		switch (m_playerMenu_attack.IsSelect())
		{
		case PlayerMenuAttackLabel::Melee:
			m_actionPlayerState = ActionPlayerState::SelectTargetMeleeAttack;
			m_currentPlayerMenu = nullptr;
			selectTargetCell = 0;
			break;
		default:
			break;
		}
		// отмена
		if (Input::IsPressed(GameKey::Cancel)) 
		{
			m_actionPlayerState = ActionPlayerState::SelectMainCommand;
			m_currentPlayerMenu = &m_playerMenu;
		}
	}
	else if (m_actionPlayerState == ActionPlayerState::SelectTargetMeleeAttack) // выбор цели для ближнего удара от игрока
	{
		m_isAnimSwords = false;
		m_animSwords.Reset();
		for (int i = 0; i < 3; i++) // подсветить доступные цели
		{
			if (selectTargetCell == i)
				setStatusCell(i, 1, BattleCellStatus::Blue);
			else
				setStatusCell(i, 1, BattleCellStatus::Green);
		}

		if (Input::IsPressed(GameKey::Left))
		{
			if (selectTargetCell > 0) selectTargetCell--;
		}
		if (Input::IsPressed(GameKey::Right))
		{
			if (selectTargetCell < 2) selectTargetCell++;
		}
		if (Input::IsPressed(GameKey::Cancel)) // отмена выбора цели
		{
			m_actionPlayerState = ActionPlayerState::Attack;
			m_currentPlayerMenu = &m_playerMenu_attack;
			selectTargetCell = 0;
			resetCells();
		}
		if (Input::IsPressed(GameKey::Ok)) // атака по цели
		{
			m_actionPlayerState = ActionPlayerState::MeleeAttack;			
		}
	}
	else if (m_actionPlayerState == ActionPlayerState::MeleeAttack) // выполнение атаки
	{
		if (m_isAnimSwords == false)
		{
			m_isAnimSwords = true;
			m_animSwords.Start();
		}		
		if (m_animSwords.IsFinal())
		{
			m_actionPlayerState = ActionPlayerState::EndMeleeAttack;
		}			
	}
	else if (m_actionPlayerState == ActionPlayerState::EndMeleeAttack)
	{
		resetCells();
		m_actionPlayerState = ActionPlayerState::SelectMainCommand;
		m_currentPlayerMenu = &m_playerMenu;
	}
}
//-----------------------------------------------------------------------------
void GameBattleState::nextMembers() noexcept
{
	while (true)
	{
		m_currentMember++;
		if (m_currentMember >= m_members.size()) // всех перебрали, новый раунд
		{
			newRound();
			break;
		}
		if (m_members[m_currentMember]->IsAlive())
		{
			break;
		}
	}
}
//-----------------------------------------------------------------------------
void GameBattleState::setStatusCell(size_t x, size_t y, BattleCellStatus status) noexcept
{
	if (x >= 3 || y >= 4) return;
	m_statusCells[x][y] = status;
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
		resetCells();
	}
	return {-1,-1};
}
//-----------------------------------------------------------------------------