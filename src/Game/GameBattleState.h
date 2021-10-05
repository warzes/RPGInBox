#pragma once

#include "EngineMath.h"
#include "UIBattlePanelBG.h"
#include "UIBattlePlayerMenu.h"

constexpr Point2 LeftTopCoordCells = { 262, 44 };
constexpr Point2 OffsetCoordCells = { 10, 10 };
constexpr Point2 SizeCoordCells = { 160, 160 };

// позиции команд игрока
constexpr Point2 LeftTopCoordPlayerCommand = { 810, 415 };
constexpr Point2 SizeCoordPlayerCommand = { 140, 40 };


constexpr std::array<const char*, 4> PlayerActionMainMenu = { "Attack" , "Skill", "Magic", "Defence" };

constexpr std::array<const char*, 3> PlayerActionMainMenu_Attack = { "Melee" , "Shoot", "Cancel" };

class ICreature;
class Player;
class EnemyParty;
class ResourceManager;



enum class BattleCellStatus
{
	Normal,	// ничего не рисует
	Yellow,	// выделяет желтым цветом (например текущий персонаж)
	Green,	// выделяет зеленым цветом (например возможные клетки для выбора)
	Red,	// выделяет красным цветом (например недоступные клетки для выбора)
	Grey	// выделяет серым цветом
};

enum class BattleState
{
	NewRound,
	SelectAction
};




class ResourceManager;

class GameBattleState final
{
public:
	GameBattleState(Player& player, ResourceManager& resourceMgr) noexcept;
	bool Init() noexcept;

	void StartBattle(EnemyParty* enemies) noexcept;

	void Update(float deltaTime) noexcept;
	void Frame() noexcept;

private:
	void selectPlayerTargetMeleeAttack() noexcept;

	Point2 selectCell() noexcept;
	ResourceManager& m_resourceMgr;
	Player& m_player;
	EnemyParty* m_enemies = nullptr;

	enum class PlayerMenu
	{
		Main,
		Attack
	} m_playerMenuStage;

	UIBattlePlayerMenu m_playerMenu;
	UIBattlePlayerMenu m_playerMenu_attack;
	UIBattlePlayerMenu* m_currentPlayerMenu = nullptr;

	// view
	void drawBackground() noexcept;
	void drawPanels() noexcept;
	void drawCells(const Player& player, EnemyParty* enemies) noexcept;
	void drawPlayerMenu() noexcept;
	void SetStatusCell(size_t x, size_t y, BattleCellStatus status) noexcept;
	void ResetCells() noexcept;

	BattleCellStatus m_cells[3][4] = { BattleCellStatus::Normal };
	UIBattlePanelBG m_background;
	std::shared_ptr<Texture2D> m_battleBackGround = nullptr;


	void newRound() noexcept;
	// текущий раунд
	unsigned m_round = 0;
	// текущее состояние боя
	BattleState m_state = BattleState::NewRound;
	// все участники боя (в будущем возможно будут сортироваться по инициативе)
	std::vector<ICreature*> m_members;
	// текущий участник боя
	unsigned m_currentMember = 0;
};