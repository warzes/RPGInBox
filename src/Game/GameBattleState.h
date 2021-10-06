#pragma once

#include "EngineMath.h"
#include "UIBattlePanelBG.h"
#include "UIBattlePlayerMenu.h"

// ������� ������ ������
constexpr Point2 LeftTopCoordPlayerCommand = { 810, 415 };
constexpr Point2 SizeCoordPlayerCommand = { 140, 40 };

class ICreature;
class Player;
class EnemyParty;
class ResourceManager;

enum class BattleCellStatus
{
	Normal,	// ������ �� ������������
	Yellow,	// �������� ������ ������ (�������� ������� ��������)
	Green,	// �������� ������� ������ (�������� ��������� ������ ��� ������)
	Red,	// �������� ������� ������ (�������� ����������� ������ ��� ������)
	Grey	// �������� ����� ������
};

enum class BattleState
{
	NewRound,          // ���������� ����� �����

	WaitAction,        // �������� ������ �������, ��������� � WaitActionPlayer ��� WaitActionEnemy � ����������� �� ���� ��� ���

	WaitActionPlayer,  // �������� ������ ������� �� ������
	WaitActionPlayer_SelectComand_Main,// �������� ������ ������� �� ���������� ���� ������
	WaitActionPlayer_SelectComand_Attack,// �������� ������ ������� �� ���� ����� ������
	WaitActionPlayer_SelectTargetMeleeAttack,// �������� ������ ���� �������� ����� �� ������

	WaitActionEnemy,   // �������� ������ ������� �� ��
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
	void nextMembers() noexcept;
	void setStatusCell(size_t x, size_t y, BattleCellStatus status) noexcept;

	ResourceManager& m_resourceMgr;
	Player& m_player;
	EnemyParty* m_enemies = nullptr;
	// ������� �����
	unsigned m_round = 0;
	// ������� ��������� ���
	BattleState m_battleState = BattleState::NewRound;
	// ��� ��������� ��� (� ������� �������� ����� ������������� �� ����������)
	std::vector<ICreature*> m_members;
	// ������� �������� ���
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