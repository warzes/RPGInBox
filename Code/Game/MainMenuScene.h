#pragma once

#include "IGameModule.h"

class MainMenuScene final : public IGameModule
{
public:
	MainMenuScene() noexcept;

	void Update(float deltaTime) noexcept override;
	void Draw(IGameCamera* camera) noexcept override;

	IPlayfield* AsPlayfield() noexcept override;
	static IGameModule* Make() noexcept;
private:
	int selIndex = 0;
};