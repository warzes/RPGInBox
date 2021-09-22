#pragma once

#include "GameBattleCore.h"
#include "GameBattleView.h"

class GameApp;
class ResourceManager;

class GameBattle final
{
public:
	GameBattle(GameApp& gameApp, ResourceManager& resourceMgr) noexcept;
	bool Init() noexcept;

	void StartBattle() noexcept;

	void Update(float deltaTime) noexcept;
	void Frame() noexcept;

private:
	GameApp& m_gameApp;
	ResourceManager& m_resourceMgr;

	GameBattleView m_view;
};