#pragma once

#include "EnemyParty.h"
#include "PlayerParty.h"
#include "BattlePlayerAction.h"
#include "BattleCore.h"

class ResourceManager;

class BattleEngine final : NonCopyable, NonMovable
{
public:
	BattleEngine(PlayerParty& player) noexcept : m_playerParty(player) {}

	void StartBattle(ResourceManager& resources, const EnemyParty& enemy) noexcept;

	void Update(float deltaTime) noexcept;
	void Draw() noexcept;

private:
	void setInitiative() noexcept;
	void newRound() noexcept;
	void currentRound() noexcept;
	void endRound() noexcept;
	bool isPlayer() const noexcept { return (m_members[m_currentMember].type == BattleMember::type::player); }
	bool isEnemy() const noexcept  { return (m_members[m_currentMember].type == BattleMember::type::enemy); }
	void selectNextMember() noexcept;
	bool enemyAction() noexcept;

	std::shared_ptr<Texture2D> m_patchTexture = nullptr;
	NPatchInfo m_ninePatchInfo = { { 0.0f, 0.0f, 64.0f, 64.0f }, 6, 6, 6, 6, NPATCH_NINE_PATCH };
	std::shared_ptr<Texture2D> m_textureUI_character = nullptr;
	NPatchInfo m_ninePatchInfo_character = { { 0.0f, 0.0f, 64.0f, 64.0f }, 6, 6, 6, 6, NPATCH_NINE_PATCH };

	std::shared_ptr<Texture2D> m_texChar1 = nullptr;
	std::shared_ptr<Texture2D> m_texChar2 = nullptr;
	std::shared_ptr<Texture2D> m_battleBackGround = nullptr;

	PlayerParty& m_playerParty;
	EnemyParty m_enemyParty;

	enum class roundState
	{
		BeginRound,
		Round,
		EndRound,
		EndBattle
	} m_state = roundState::BeginRound;
	unsigned m_numRound = 0;

	
	std::vector<BattleMember> m_members; // участники боя расставленные по инициативе
	unsigned m_currentMember = 0; // текущий персонаж

	BattlePlayerAction m_playerCommand;
};