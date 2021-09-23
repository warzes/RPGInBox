#pragma once

class ResourceManager;

class UIBattlePanelBG
{
public:
	void Create(ResourceManager* resourceMgr);
	void Draw();
private:
	std::shared_ptr<Texture2D> m_patchTexture = nullptr;
};

class GameBattleView final
{
public:
	GameBattleView(ResourceManager& resourceMgr) noexcept;
	bool Init() noexcept;

	void Frame() noexcept;

private:
	void drawBackground() noexcept;
	void drawPanels() noexcept;

	ResourceManager& m_resourceMgr;

	UIBattlePanelBG m_background;
	
	std::shared_ptr<Texture2D> m_textureUI_character = nullptr;
	NPatchInfo m_ninePatchInfo_character = { { 0.0f, 0.0f, 64.0f, 64.0f }, 6, 6, 6, 6, NPATCH_NINE_PATCH };

	std::shared_ptr<Texture2D> m_battleBackGround = nullptr;
};