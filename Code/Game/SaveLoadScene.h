#pragma once

#include "IGameModule.h"
#include "GameMenu.h"

class SaveLoadScene final : public IGameModule
{
public:
	SaveLoadScene() = default;

	void InitSave() noexcept;
	void InitLoad() noexcept;

	void Update() noexcept final;
	void Draw() noexcept final;

	IPlayfield* AsPlayfield() noexcept final;

	static IGameModule* MakeSave() noexcept;
	static IGameModule* MakeLoad() noexcept;

private:
	GameMenu m_mainMenu;
};