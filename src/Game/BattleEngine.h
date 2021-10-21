#pragma once

#include "BattleCell.h"
#include "UIBattle.h"

class ICreature;
class Player;
class EnemyParty;
class ResourceManager;

enum class BattleState
{
	NewRound,          // начинается новый раунд
	BeginWaitAction,   // старт ожидания выбора команды, переходит в WaitActionPlayer или WaitActionEnemy в зависимости от того чей ход	
	ActionsPlayer,
	ActionsEnemy,
	EndBattleAndWin,
	EndBattleAndLose
};

enum class ActionPlayerState
{
	SelectMainCommand,// ожидание выбора команды из начального меню игрока
	Attack,// ожидание выбора команды из меню атаки игрока

	SelectTargetMeleeAttack,// ожидание выбора цели ближнего удара от игрока
	MeleeAttack,// выполнение ближней атаки по цели
	EndMeleeAttack, // конец ближней атаки

	SelectTargetRangeAttack,// ожидание выбора цели выстрела от игрока
	RangeAttack,// выполнение выстрела по цели
	EndRangeAttack, // конец выстрела
};

class BattleEngine final
{
public:
	BattleEngine(Player& player, ResourceManager& resourceMgr) noexcept;
	bool Init() noexcept;

	void StartBattle(EnemyParty* enemies) noexcept;
	bool IsWinEndBattle() const noexcept { return m_battleState == BattleState::EndBattleAndWin; }
	bool IsLoseEndBattle() const noexcept { return m_battleState == BattleState::EndBattleAndLose; }

	void Update(float deltaTime) noexcept;
	void Frame() noexcept;

private:
	void newRound() noexcept;
	void beginWaitAction() noexcept;
	void actionsPlayer() noexcept;
	void nextMembers() noexcept;
	void activePlayerMainMenu() noexcept;
	void activePlayerMenuAttack() noexcept;
	void selectPlayerTargetMeleeAttack() noexcept;
	void selectPlayerTargetRangeAttack() noexcept;

	ResourceManager& m_resourceMgr;
	Player& m_player;
	EnemyParty* m_enemies = nullptr;

	// текущий раунд
	unsigned m_round = 0;
	// текущее состояние боя
	BattleState m_battleState = BattleState::NewRound;
	ActionPlayerState m_actionPlayerState = ActionPlayerState::SelectMainCommand;
	
	BattleCells m_battleCells;

	UIBattle m_ui;
	
	float m_deltaTime = 0.0f;
};