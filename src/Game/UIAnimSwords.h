#pragma once

class ResourceManager;

class UIAnimSwords final
{
public:
	bool Init(ResourceManager& resourceMgr) noexcept;

	void Draw(float deltaTime, const Point2& pos, float scale) noexcept;

	void Reset() noexcept;
	void Start() noexcept;

	bool IsFinal() const noexcept;

private:
	std::shared_ptr<Texture2D> m_texture = nullptr;

	float m_rot1 = 0.0f;
	float m_rot2 = 270.0f;
	bool m_runAnim = false;
	bool m_isFinal = false;
};