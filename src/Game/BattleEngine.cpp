#include "stdafx.h"
#include "BattleEngine.h"
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
BattleEngine::BattleEngine(Player& player, ResourceManager& resourceMgr) noexcept
	: m_resourceMgr(resourceMgr)
	, m_player(player)
	, m_ui(resourceMgr)
{
}
//-----------------------------------------------------------------------------
bool BattleEngine::Init() noexcept
{
	if (!m_ui.Init())
		return false;

	if (!m_battleCells.Init(m_resourceMgr))
		return false;

	// создание меню
	m_playerMenu.AddElements(PlayerMenuLeftTopPos, PlayerActionMainMenu);
	m_playerMenu_attack.AddElements(PlayerMenuLeftTopPos, PlayerActionMainMenu_Attack);

	return true;
}
//-----------------------------------------------------------------------------
void BattleEngine::StartBattle(EnemyParty* enemies) noexcept
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
void BattleEngine::Update(float deltaTime) noexcept
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
		m_battleState = BattleState::EndBattleAndLose;
	}
	if (m_battleCells.GetNumberEnemy() == 0)
	{
		// победа в битве
		m_battleState = BattleState::EndBattleAndWin;
	}
}
//-----------------------------------------------------------------------------
void BattleEngine::Frame() noexcept
{
	m_ui.Draw();
	m_battleCells.Draw(m_deltaTime);
	if (m_currentPlayerMenu) m_currentPlayerMenu->Draw();
}
//-----------------------------------------------------------------------------
void BattleEngine::newRound() noexcept
{
	m_round++; // увеличивается счетчик кол-ва раундов
	m_battleCells.SetFirstMember();
	m_battleState = BattleState::BeginWaitAction; // ожидание выбора действия
}
//-----------------------------------------------------------------------------
void BattleEngine::beginWaitAction() noexcept
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
void BattleEngine::actionsPlayer() noexcept
{
	static int selectTargetCell = 0;
	static std::vector<BattleCell*> selectCellAttack = {};

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
			selectCellAttack = m_battleCells.GetTargetMeleeAttack();
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
		m_battleCells.ResetAnimSword();
		if (selectCellAttack.size() == 0)
		{
			m_battleCells.ResetStatusCells();
			activePlayerMenuAttack();
		}
		else
		{
			for (int i = 0; i < selectCellAttack.size(); i++)
			{
				selectCellAttack[i]->SetStatus(BattleCellStatus::Green);
			}

			if (Input::IsPressed(GameKey::Left))
			{
				selectTargetCell--;
				if (selectTargetCell < 0) selectTargetCell = selectCellAttack.size() - 1;
			}
			if (Input::IsPressed(GameKey::Right))
			{
				selectTargetCell++;
				if (selectTargetCell >= selectCellAttack.size()) selectTargetCell = 0;
			}

			selectCellAttack[selectTargetCell]->SetStatus(BattleCellStatus::Blue);			

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
void BattleEngine::nextMembers() noexcept
{
	m_battleState = BattleState::BeginWaitAction;
	int currentMember = m_battleCells.NextMembers();
	if (currentMember == -1)
		newRound();
}
//-----------------------------------------------------------------------------
Point2 BattleEngine::selectCell() noexcept // TODO: возможно перенести в класс правил
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
void BattleEngine::activePlayerMainMenu() noexcept
{
	m_battleCells.ViewCurrentMember();
	m_currentPlayerMenu = &m_playerMenu;
	m_actionPlayerState = ActionPlayerState::SelectMainCommand;
}
//-----------------------------------------------------------------------------
void BattleEngine::activePlayerMenuAttack() noexcept
{
	m_battleCells.ViewCurrentMember();
	m_currentPlayerMenu = &m_playerMenu_attack;
	m_actionPlayerState = ActionPlayerState::Attack;
}
//-----------------------------------------------------------------------------
void BattleEngine::selectPlayerTargetMeleeAttack() noexcept
{
	m_battleCells.ViewCurrentMember();
	m_currentPlayerMenu = nullptr;
	m_actionPlayerState = ActionPlayerState::SelectTargetMeleeAttack;
}
//-----------------------------------------------------------------------------