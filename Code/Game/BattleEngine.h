#pragma once

#include "EnemyParty.h"
#include "PlayerParty.h"

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
	bool isPlayer() const noexcept { return (m_members[m_currentMember].type == member::type_::player); }
	bool isEnemy() const noexcept  { return (m_members[m_currentMember].type == member::type_::enemy); }
	bool playerAction() noexcept;
	bool enemyAction() noexcept;
	void selectMember() noexcept;

	void endRound() noexcept;

	PlayerParty& m_playerParty;
	EnemyParty m_enemyParty;
	struct member
	{
		enum class type_ { player, enemy } type;
		unsigned number;
	};
	std::vector<member> m_members; // участники боя расставленные по инициативе
	unsigned m_currentMember = 0; // текущий персонаж

	enum class roundState
	{
		BeginRound,
		Round,
		EndRound
	} m_state = roundState::BeginRound;

	unsigned m_numRound = 0;
	
	

	void selectPlayerCommand();
	void selectAttackTargetEnemy();
	//void selectCell();



	int m_currentPlayerCommand = -1;


	std::shared_ptr<Texture2D> m_patchTexture = nullptr;
	NPatchInfo m_ninePatchInfo = { { 0.0f, 0.0f, 64.0f, 64.0f }, 6, 6, 6, 6, NPATCH_NINE_PATCH };
	std::shared_ptr<Texture2D> m_textureUI_character = nullptr;
	NPatchInfo m_ninePatchInfo_character = { { 0.0f, 0.0f, 64.0f, 64.0f }, 6, 6, 6, 6, NPATCH_NINE_PATCH };

	std::shared_ptr<Texture2D> m_texChar1 = nullptr;
	std::shared_ptr<Texture2D> m_texChar2 = nullptr;
	std::shared_ptr<Texture2D> m_battleBackGround = nullptr;
};