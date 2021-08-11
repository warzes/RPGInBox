#pragma once

#include "IGameModule.h"

class TitleScene final : public IGameModule
{
public:
	TitleScene() noexcept;

	void Update() noexcept final;
	void Draw() noexcept final;

	IPlayfield* AsPlayfield() noexcept final;

	static IGameModule* Make() noexcept;
private:
	int m_selIndex = 0;

	std::shared_ptr<Texture> m_cursor = nullptr;
};