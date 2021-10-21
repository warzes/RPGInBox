#pragma once

#include "ResourceManager.h"
#include "GameAdventure.h"
#include "BattleEngine.h"
#include "EnemyTemplate.h"
#include "Player.h"

enum class GameState
{
	MainMenu,

	Adventure,

	//-------------------------------------------------------------------------
	// Battle state
	BeginBattle,
	Battle
};

class GameApp final : NonCopyable, NonMovable
{
public:
	GameApp() noexcept;
	~GameApp();

	bool Init() noexcept;

	void Update(float deltaTime) noexcept;
	void Frame() noexcept;

	bool IsEnd() const noexcept { return m_isEnd; }
	void End() noexcept { m_isEnd = true; }

	void SetState(GameState state) noexcept;

private:
	void createFrame() noexcept;
	bool initGameData() noexcept;
	void beginFrame() noexcept;
	void endFrame() noexcept;

	Player m_player;
	ResourceManager m_resourceMgr;

	GameState m_state = GameState::Adventure;
	GameAdventure m_adventureState;
	BattleEngine m_battleEngine;
	EnemyParty m_enemies;
	bool m_isEnd = false;

#if MAIN_FRAME_TO_RENDER_TEXTURE
	RenderTexture2D m_target = {};
	Rectangle m_sourceRec = {};
	Rectangle m_destRec = {};
	Vector2 m_origin = { 0.0f, 0.0f };
#endif
};