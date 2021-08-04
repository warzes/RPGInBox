#pragma once

class BattlePlayerAction
{
public:
	void Reset() noexcept;
	bool PlayerAction() noexcept;
	void Draw() noexcept;
	void DrawCommandList() noexcept;

private:
	void selectPlayerCommand() noexcept;
	void selectAttackTargetEnemy() noexcept;

	enum class playerCommandState
	{
		SelectCommand,          // ожидание выбора команд из главного меню
		// TODO: подменю

	} m_playerCommandState = playerCommandState::SelectCommand;

	int m_selectPlayerCommand = -1;
	bool m_selectOneEnemyTagetInMelee = false;// выбор одной вражеской цели в ближнем бою
	bool m_selectOneEnemyTagetInRange = false;// выбор одной вражеской цели в любой клетке (дальний бой)
};