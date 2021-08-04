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
		SelectCommand,          // �������� ������ ������ �� �������� ����
		// TODO: �������

	} m_playerCommandState = playerCommandState::SelectCommand;

	int m_selectPlayerCommand = -1;
	bool m_selectOneEnemyTagetInMelee = false;// ����� ����� ��������� ���� � ������� ���
	bool m_selectOneEnemyTagetInRange = false;// ����� ����� ��������� ���� � ����� ������ (������� ���)
};