#pragma once

#include "EnemyParty.h"
#include "PlayerParty.h"

class ResourceManager;

class BattleEngine final : NonCopyable, NonMovable
{
public:
	BattleEngine(PlayerParty& player) noexcept : m_player(player) {}

	void StartBattle(ResourceManager& resources, const EnemyParty& enemy);
	void StopBattle();
	void Draw();
	void Update(float deltaTime);
private:
	void nextRound();
	void currentRound();
	bool playerAction();
	bool enemyAction();
	PlayerParty& m_player;
	EnemyParty m_enemy;

	struct member
	{
		enum class type_{player, enemy} type;
		unsigned number;
	};

	std::vector<member> m_members;

	size_t m_currentMember = 0;


	std::shared_ptr<Texture2D> m_patchTexture = nullptr;
	Rectangle m_dest = { 20.0f, 20.0f, 0.0f, 0.0f };
	NPatchInfo m_ninePatchInfo = { { 0.0f, 0.0f, 64.0f, 64.0f }, 6, 6, 6, 6, NPATCH_NINE_PATCH };
};