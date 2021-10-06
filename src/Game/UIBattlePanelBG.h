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