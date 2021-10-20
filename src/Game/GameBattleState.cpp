﻿#include "stdafx.h"
#include "GameBattleState.h"
#include "ResourceManager.h"
#include "Creature.h"
#include "Player.h"
#include "EnemyTemplate.h"
#include "EngineMath.h"
#include "GameInput.h"
#include "BattleRule.h"
#include "DebugNew.h"
//-----------------------------------------------------------------------------
constexpr Point2 LeftTopCoordCells = { 262, 44 };
constexpr Point2 OffsetCoordCells = { 10, 10 };
constexpr Point2 SizeCoordCells = { 160, 160 };
constexpr Point2 PlayerMenuLeftTopPos = { 810, 415 };
//-----------------------------------------------------------------------------
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

	if (!m_battleCells.Init(m_resourceMgr))
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
	m_round = 0;
	m_battleState = BattleState::NewRound;
	m_currentPlayerMenu = nullptr;

	// заполнение карты боя существами
	m_battleCells.SetCreature(&m_player, m_enemies);
}
//-----------------------------------------------------------------------------
void GameBattleState::Update(float deltaTime) noexcept
{
	m_deltaTime = deltaTime;

	if (m_currentPlayerMenu) m_currentPlayerMenu->Run();

	if (m_battleState == BattleState::NewRound)             // новый раунд
		newRound();
	else if (m_battleState == BattleState::BeginWaitAction) // ожидание действия (подготовка)
		beginWaitAction();
	else if (m_battleState == BattleState::ActionsPlayer)   // действие игрока
		actionsPlayer();
	//else if (m_battleState == BattleState::ActionEnemy)
	//	waitActionEnemy();

	if (m_battleCells.GetNumberHero() == 0)
	{
		// проигрыш битвы
	}
	if (m_battleCells.GetNumberEnemy() == 0)
	{
		// победа в битве
	}
}
//-----------------------------------------------------------------------------
void GameBattleState::Frame() noexcept
{
	drawBackground(); // отрисовка фона
	drawPanels(); // отрисовка панелей
	m_battleCells.Draw(m_deltaTime);
	if (m_currentPlayerMenu) m_currentPlayerMenu->Draw();
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
void GameBattleState::newRound() noexcept
{
	m_round++; // увеличивается счетчик кол-ва раундов
	m_battleCells.SetFirstMember();
	m_battleState = BattleState::BeginWaitAction; // ожидание выбора действия
}
//-----------------------------------------------------------------------------
void GameBattleState::beginWaitAction() noexcept
{
	m_battleCells.ResetStatusCells();

	if (m_battleCells.GetCurrentMember().creature->GetPartyType() == PartyType::Hero) // ожидание действия героя
	{
		m_battleState = BattleState::ActionsPlayer;
		activePlayerMainMenu();
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
			activePlayerMenuAttack();					
			break;
		case PlayerMenuLabel::Defence:
			nextMembers();
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
			selectPlayerTargetMeleeAttack();
			selectTargetCell = 0;
			break;
		default:
			break;
		}
		// отмена
		if (Input::IsPressed(GameKey::Cancel)) 
		{
			activePlayerMainMenu();
		}
	}
	else if (m_actionPlayerState == ActionPlayerState::SelectTargetMeleeAttack) // выбор цели для ближнего удара от игрока
	{
		if (Input::IsPressed(GameKey::Left))
		{
			if (selectTargetCell > 0) selectTargetCell--;
		}
		if (Input::IsPressed(GameKey::Right))
		{
			if (selectTargetCell < 2) selectTargetCell++;
		}

		bool viewMembers = m_battleCells.ViewMeleeAttack(selectTargetCell);
		if (!viewMembers)
		{
			// TODO: конец боя?
		}

		if (Input::IsPressed(GameKey::Cancel)) // отмена выбора цели
		{
			m_battleCells.ResetStatusCells();
			activePlayerMenuAttack();			
		}
		if (Input::IsPressed(GameKey::Ok)) // атака по цели
		{
			m_actionPlayerState = ActionPlayerState::MeleeAttack;			
		}
	}
	else if (m_actionPlayerState == ActionPlayerState::MeleeAttack) // выполнение атаки
	{
		if (m_battleCells.IsFinalAnimSworld())
		{
			m_actionPlayerState = ActionPlayerState::EndMeleeAttack;
		}			
	}
	else if (m_actionPlayerState == ActionPlayerState::EndMeleeAttack)
	{
		auto currentCreature = m_battleCells.GetCurrentMember();
		auto selectCreature = m_battleCells.GetSelectMember();
		if (currentCreature.creature && selectCreature && selectCreature->creature) 
			BattleRule::MeleeDamage(*m_battleCells.GetCurrentMember().creature, *selectCreature->creature);
		m_battleCells.ResetStatusCells();
		nextMembers();
	}
}
//-----------------------------------------------------------------------------
void GameBattleState::nextMembers() noexcept
{
	m_battleState = BattleState::BeginWaitAction;
	int currentMember = m_battleCells.NextMembers();
	if (currentMember == -1)
		newRound();
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
		m_battleCells.ResetStatusCells();
	}
	return {-1,-1};
}
//-----------------------------------------------------------------------------
void GameBattleState::activePlayerMainMenu() noexcept
{
	m_battleCells.ViewCurrentMember();
	m_currentPlayerMenu = &m_playerMenu;
	m_actionPlayerState = ActionPlayerState::SelectMainCommand;
}
//-----------------------------------------------------------------------------
void GameBattleState::activePlayerMenuAttack() noexcept
{
	m_battleCells.ViewCurrentMember();
	m_currentPlayerMenu = &m_playerMenu_attack;
	m_actionPlayerState = ActionPlayerState::Attack;
}
//-----------------------------------------------------------------------------
void GameBattleState::selectPlayerTargetMeleeAttack() noexcept
{
	m_battleCells.ViewCurrentMember();
	m_currentPlayerMenu = nullptr;
	m_actionPlayerState = ActionPlayerState::SelectTargetMeleeAttack;
}
//-----------------------------------------------------------------------------