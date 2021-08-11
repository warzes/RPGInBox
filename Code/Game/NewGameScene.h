#pragma once

#include "IGameModule.h"
#include "GameMenu.h"

class NewGameScene final : public IGameModule
{
public:
	NewGameScene() = default;

	void Init() noexcept;

	void Update() noexcept final;
	void Draw() noexcept final;

	IPlayfield* AsPlayfield() noexcept final;

	static IGameModule* Make() noexcept;

private:
	GameMenu m_mainMenu;
};