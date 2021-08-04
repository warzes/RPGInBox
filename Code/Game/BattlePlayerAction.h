#pragma once

enum class PlayerCommandState
{
	SelectCommand,          // ожидание выбора команд из главного меню
	SelectMeleeAttackTarget	// выбор цели для ближней атаки
	// TODO: подменю

};

class BattlePlayerAction
{
public:
	void Reset() noexcept;
	bool PlayerAction() noexcept;
	void DrawCommandList() noexcept;

	PlayerCommandState GetState() const noexcept { return m_playerCommandState;  }

private:
	void selectPlayerCommand() noexcept;
	void selectMeleeAttackTarget() noexcept;

	PlayerCommandState m_playerCommandState = PlayerCommandState::SelectCommand;

	int m_selectPlayerCommand = -1;
};