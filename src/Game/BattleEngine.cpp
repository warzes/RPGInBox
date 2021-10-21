#include "stdafx.h"
#include "BattleEngine.h"
#include "Player.h"
#include "GameInput.h"
#include "BattleRule.h"
#include "DebugNew.h"
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

	return true;
}
//-----------------------------------------------------------------------------
void BattleEngine::StartBattle(EnemyParty* enemies) noexcept
{
	m_enemies = enemies;
	// сброс всех переменных
	m_round = 0;
	m_battleState = BattleState::NewRound;

	// заполнение карты боя существами
	m_battleCells.SetCreature(&m_player, m_enemies);
}
//-----------------------------------------------------------------------------
void BattleEngine::Update(float deltaTime) noexcept
{
	m_deltaTime = deltaTime;

	m_ui.Update();

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
	m_ui.ResetPlayerMenu();
	m_battleCells.ResetStatusCells();

	if (m_battleCells.GetCurrentMember().creature->GetPartyType() == PartyType::Hero) // ожидание действия героя
	{
		m_battleState = BattleState::ActionsPlayer;
		activePlayerMainMenu();
	}
	else // ожидание действия ИИ
	{
		m_ui.ResetPlayerMenu();
		nextMembers();
	}
}
//-----------------------------------------------------------------------------
void BattleEngine::actionsPlayer() noexcept
{
	static TargetMeleeAttack targetMeleeAttack;
	static TargetRangeAttack targetRangeAttack;

	if (m_actionPlayerState == ActionPlayerState::SelectMainCommand) // ожидание выбора команды в меню игрока
	{
		// выбор команды в основном меню игрока
		switch (m_ui.SelectMenu())
		{
		case PlayerMenuLabel::Attack:
			activePlayerMenuAttack();
			break;
		case PlayerMenuLabel::Defence:
			nextMembers();
			break;
		default:
			break;
		}
	}
	else if (m_actionPlayerState == ActionPlayerState::Attack)// ожидание выбора команды в меню атаки игрока
	{
		// выбор команды в меню атаки
		switch (m_ui.SelectMenu())
		{
		case PlayerMenuAttackLabel::Melee:
			if (m_battleCells.IsMeleeAttack())
			{
				selectPlayerTargetMeleeAttack();
				targetMeleeAttack = m_battleCells.GetTargetMeleeAttack();
				targetMeleeAttack.SetPos(m_battleCells.GetCurrentMember().position.x);
			}				
			break;
		case PlayerMenuAttackLabel::Shoot:
			if (m_battleCells.IsRangeAttack())
			{
				selectPlayerTargetRangeAttack();
				targetRangeAttack = m_battleCells.GetTargetRangeAttack();
				targetRangeAttack.SetPos(m_battleCells.GetCurrentMember().position.x, 1);
			}
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
		if (targetMeleeAttack.IsZero())
		{
			m_battleCells.ResetStatusCells();
			activePlayerMenuAttack();
		}
		else
		{
			targetMeleeAttack.Update();	

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
	else if (m_actionPlayerState == ActionPlayerState::SelectTargetRangeAttack) // выбор цели для выстрела от игрока
	{
		m_battleCells.ResetAnimSword();
		if (targetRangeAttack.IsZero())
		{
			m_battleCells.ResetStatusCells();
			activePlayerMenuAttack();
		}
		else
		{
			targetRangeAttack.Update();			

			if (Input::IsPressed(GameKey::Cancel)) // отмена выбора цели
			{
				m_battleCells.ResetStatusCells();
				activePlayerMenuAttack();
			}
			if (Input::IsPressed(GameKey::Ok)) // атака по цели
			{
				m_actionPlayerState = ActionPlayerState::RangeAttack;
			}
		}
	}
	else if (m_actionPlayerState == ActionPlayerState::RangeAttack) // выполнение атаки
	{
		if (m_battleCells.IsFinalAnimSworld())
		{
			m_actionPlayerState = ActionPlayerState::EndRangeAttack;
		}
	}
	else if (m_actionPlayerState == ActionPlayerState::EndRangeAttack)
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
void BattleEngine::activePlayerMainMenu() noexcept
{
	m_battleCells.ViewCurrentMember();
	m_ui.ActivePlayerMainMenu();
	m_actionPlayerState = ActionPlayerState::SelectMainCommand;
}
//-----------------------------------------------------------------------------
void BattleEngine::activePlayerMenuAttack() noexcept
{
	m_battleCells.ViewCurrentMember();
	m_ui.ActivePlayerMenuAttack();
	m_actionPlayerState = ActionPlayerState::Attack;
}
//-----------------------------------------------------------------------------
void BattleEngine::selectPlayerTargetMeleeAttack() noexcept
{
	m_battleCells.ViewCurrentMember();
	m_ui.ResetPlayerMenu();
	m_actionPlayerState = ActionPlayerState::SelectTargetMeleeAttack;
}
//-----------------------------------------------------------------------------
void BattleEngine::selectPlayerTargetRangeAttack() noexcept
{
	m_battleCells.ViewCurrentMember();
	m_ui.ResetPlayerMenu();
	m_actionPlayerState = ActionPlayerState::SelectTargetRangeAttack;
}
//-----------------------------------------------------------------------------