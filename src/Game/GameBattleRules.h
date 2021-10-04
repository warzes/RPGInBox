#pragma once

class oICreature;
class Player;
class EnemyParty;

enum class BattleState
{
	NewRound,
	SelectAction
};

class GameBattleRules final
{
public:
	// запускается в начале боя, сбрасывает все переменные
	void StartBattle(Player* player, EnemyParty* enemies) noexcept;

	void Run() noexcept;

private:
	void newRound() noexcept;
	void selectPlayerAction() noexcept;
	void selectEnemyAction() noexcept;
	// группа игрока
	Player* m_player = nullptr;
	// группа врагов
	EnemyParty* m_enemies = nullptr;
	// текущий раунд
	unsigned m_round = 0; 
	// текущее состояние боя
	BattleState m_state = BattleState::NewRound;
	// все участники боя (в будущем возможно будут сортироваться по инициативе)
	std::vector<oICreature*> m_members;
	// текущий участник боя
	unsigned m_currentMember = 0;
};