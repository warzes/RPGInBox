#pragma once

class ResourceManager;

class UIBattlePanelBG
{
public:
	bool Create(ResourceManager* resourceMgr) noexcept;
	void Draw() const noexcept;
private:
	std::shared_ptr<Texture2D> m_patchTexture = nullptr;
};