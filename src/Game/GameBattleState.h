#pragma once

#include "EngineMath.h"
#include "UIBattlePanelBG.h"
#include "UIBattlePlayerMenu.h"

// позиции команд игрока
constexpr Point2 LeftTopCoordPlayerCommand = { 810, 415 };
constexpr Point2 SizeCoordPlayerCommand = { 140, 40 };

class ICreature;
class Player;
class EnemyParty;
class ResourceManager;

enum class BattleCellStatus
{
	Normal,	// ничего не дорисовывает
	Yellow,	// выделяет желтым цветом (например текущий персонаж)
	Green,	// выделяет зеленым цветом (например возможные клетки для выбора)
	Red,	// выделяет красным цветом (например недоступные клетки для выбора)
	Blue,	// выделяет красным цветом (например текущий выбор)
	Grey	// выделяет серым цветом
};

enum class BattleState
{
	NewRound,          // начинается новый раунд
	BeginWaitAction,   // старт ожидания выбора команды, переходит в WaitActionPlayer или WaitActionEnemy в зависимости от того чей ход	
	WaitActionPlayer,
	WaitActionEnemy
};

enum class ActionPlayerState
{
	WaitActionPlayer_SelectComand_Main,// ожидание выбора команды из начального меню игрока
	WaitActionPlayer_SelectComand_Attack,// ожидание выбора команды из меню атаки игрока
	WaitActionPlayer_SelectTargetMeleeAttack,// ожидание выбора цели ближнего удара от игрока
	WaitActionPlayer_MeleeAttack,// выполнение ближней атаки по цели
};

class GameBattleState final
{
public:
	GameBattleState(Player& player, ResourceManager& resourceMgr) noexcept;
	bool Init() noexcept;

	void StartBattle(EnemyParty* enemies) noexcept;

	void Update(float deltaTime) noexcept;
	void Frame() noexcept;

private:
	void resetCells() noexcept;
	void drawBackground() noexcept;
	void drawPanels() noexcept;
	void drawCells() noexcept;
	void newRound() noexcept;
	void beginWaitAction() noexcept;
	void waitActionPlayer() noexcept;
	void nextMembers() noexcept;
	void setStatusCell(size_t x, size_t y, BattleCellStatus status) noexcept;

	ResourceManager& m_resourceMgr;
	Player& m_player;
	EnemyParty* m_enemies = nullptr;
	// текущий раунд
	unsigned m_round = 0;
	// текущее состояние боя
	BattleState m_battleState = BattleState::NewRound;
	ActionPlayerState m_actionPlayerState = ActionPlayerState::WaitActionPlayer_SelectComand_Main;
	// все участники боя (в будущем возможно будут сортироваться по инициативе)
	std::vector<ICreature*> m_members;
	// текущий участник боя
	unsigned m_currentMember = 0;
	BattleCellStatus m_statusCells[3][4] = { BattleCellStatus::Normal };

	UIBattlePanelBG m_background;
	std::shared_ptr<Texture2D> m_battleBackGround = nullptr;

	UIBattlePlayerMenu m_playerMenu;
	UIBattlePlayerMenu m_playerMenu_attack;
	UIBattlePlayerMenu* m_currentPlayerMenu = nullptr;

	void selectPlayerTargetMeleeAttack() noexcept;
	Point2 selectCell() noexcept;
};