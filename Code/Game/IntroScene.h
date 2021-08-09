#pragma once

#include "IGameModule.h"

class IntroScene final : public IGameModule
{
public:
	IntroScene() = default;

	void Init() noexcept;

	void Update(float deltaTime) noexcept final;
	void Draw(IGameCamera* camera) noexcept final;

	IPlayfield* AsPlayfield() noexcept final;

	static IGameModule* Make() noexcept;
};