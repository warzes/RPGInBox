#pragma once

class ResourceManager;

// анимация скрещенных мечей
class UIAnimSwords final : NonCopyable, NonMovable
{
public:
	bool Init(ResourceManager& resourceMgr) noexcept;

	void UpdateAndDraw(float deltaTime, const Point2& pos, float scale) noexcept;

	void Reset() noexcept;
	void Start() noexcept;

	// анимация закончилась?
	bool IsFinal() const noexcept;

private:
	std::shared_ptr<Texture2D> m_texture = nullptr;
	Rectangle m_textureRect = {};

	float m_rot1 = 0.0f;
	float m_rot2 = 270.0f;
	bool m_runAnim = false;
	bool m_isFinal = false;
};