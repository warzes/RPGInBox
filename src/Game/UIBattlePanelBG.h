#pragma once

class ResourceManager;

class UIBattlePanelBG final : NonCopyable, NonMovable
{
public:
	bool Init(ResourceManager* resourceMgr) noexcept;

	void Draw() const noexcept;
private:
	std::shared_ptr<Texture2D> m_patchTexture = nullptr;
};