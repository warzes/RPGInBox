#pragma once

#include "EnemyParty.h"
#include "PlayerParty.h"

class ResourceManager;

class BattleEngine final : NonCopyable, NonMovable
{
public:
	BattleEngine(PlayerParty& player) noexcept : m_player(player) {}

	void StartBattle(ResourceManager& resources, const EnemyParty& enemy) noexcept;
	void Update(float deltaTime) noexcept;





	void StopBattle() noexcept;
	void Draw() noexcept;
	
private:
	void setInitiative() noexcept;
	void currentRound() noexcept;
	bool playerAction() noexcept;
	bool enemyAction() noexcept;
	void selectMember() noexcept;
	void newRound() noexcept;

	enum class battleState 
	{
		BeginRound,

		Round,

			PlayerSelectCommand,
			PlayerAttackCommand,

			// enemy

		EndRound
	} m_state;

















	
	

	void selectPlayerCommand();
	void selectAttackTargetEnemy();
	//void selectCell();

	bool isPlayer() const noexcept { return (m_currentMember < m_members.size()) && (m_members[m_currentMember].type == member::type_::player); }
	bool isEnemy() const noexcept  { return (m_currentMember < m_members.size()) && (m_members[m_currentMember].type == member::type_::enemy); }

	PlayerParty& m_player;
	EnemyParty m_enemy;

	struct member
	{
		enum class type_{player, enemy} type;
		unsigned number;
	};

	std::vector<member> m_members;

	size_t m_currentMember = 0;
	int m_currentPlayerCommand = -1;


	std::shared_ptr<Texture2D> m_patchTexture = nullptr;
	NPatchInfo m_ninePatchInfo = { { 0.0f, 0.0f, 64.0f, 64.0f }, 6, 6, 6, 6, NPATCH_NINE_PATCH };
	std::shared_ptr<Texture2D> m_textureUI_character = nullptr;
	NPatchInfo m_ninePatchInfo_character = { { 0.0f, 0.0f, 64.0f, 64.0f }, 6, 6, 6, 6, NPATCH_NINE_PATCH };

	std::shared_ptr<Texture2D> m_texChar1 = nullptr;
	std::shared_ptr<Texture2D> m_texChar2 = nullptr;
	std::shared_ptr<Texture2D> m_battleBackGround = nullptr;

};