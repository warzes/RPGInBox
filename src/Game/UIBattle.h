#pragma once

#include "UIBattlePanelBG.h"

class ResourceManager;

class UIBattle final
{
public:
	UIBattle(ResourceManager& resourceMgr) noexcept;

	bool Init() noexcept;

	void Draw() const noexcept;

private:
	void drawPanels() const noexcept;

	ResourceManager& m_resourceMgr;

	UIBattlePanelBG m_background;
	std::shared_ptr<Texture2D> m_battleBackGround = nullptr;
};