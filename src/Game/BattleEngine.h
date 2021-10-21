#pragma once

#include "EngineMath.h"
#include "UIBattlePanelBG.h"
#include "UIBattlePlayerMenu.h"
#include "UIAnimSwords.h"
#include "BattleCell.h"

// позиции команд игрока
constexpr Point2 LeftTopCoordPlayerCommand = { 810, 415 };
constexpr Point2 SizeCoordPlayerCommand = { 140, 40 };

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
	void drawBackground() noexcept;
	void drawPanels() noexcept;
	void newRound() noexcept;
	void beginWaitAction() noexcept;
	void actionsPlayer() noexcept;
	void nextMembers() noexcept;
	void activePlayerMainMenu() noexcept;
	void activePlayerMenuAttack() noexcept;
	void selectPlayerTargetMeleeAttack() noexcept;

	ResourceManager& m_resourceMgr;
	Player& m_player;
	EnemyParty* m_enemies = nullptr;

	// текущий раунд
	unsigned m_round = 0;
	// текущее состояние боя
	BattleState m_battleState = BattleState::NewRound;
	ActionPlayerState m_actionPlayerState = ActionPlayerState::SelectMainCommand;
	
	BattleCells m_battleCells;

	UIBattlePlayerMenu m_playerMenu;
	UIBattlePlayerMenu m_playerMenu_attack;
	UIBattlePlayerMenu* m_currentPlayerMenu = nullptr;

	UIBattlePanelBG m_background;
	std::shared_ptr<Texture2D> m_battleBackGround = nullptr;

	Point2 selectCell() noexcept;

	float m_deltaTime = 0.0f;
};