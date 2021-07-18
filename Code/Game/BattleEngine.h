#pragma once

#include "EnemyParty.h"
#include "PlayerParty.h"

class BattleEngine final : NonCopyable, NonMovable
{
public:
	BattleEngine(PlayerParty& player) noexcept : m_player(player) {}

	void StartBattle(const EnemyParty& enemy);
	void StopBattle();
	void Draw();
	void Update(float deltaTime);
private:
	void computInitiative();
	PlayerParty& m_player;
	EnemyParty m_enemy;

	struct member
	{
		enum class type_{player, enemy} type;
		unsigned number = 0;
	} membersInitiative;

	unsigned m_numberMembers = 0;
};