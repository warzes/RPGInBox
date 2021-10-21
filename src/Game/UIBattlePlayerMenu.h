#pragma once

class UIBattlePlayerMenu final : NonCopyable, NonMovable
{
public:
	void Draw() const noexcept;
	void Update() noexcept;

	template <typename T>
	void AddElements(const Point2& leftUpPos, const T& array) noexcept
	{
		m_elements.clear();
		m_leftUpPos = leftUpPos;

		for (size_t i = 0; i < array.size(); i++)
		{
			const int yOffset = (int)m_elements.size() * 40;
			m_elements.push_back({ array[i], true, {m_leftUpPos.x, m_leftUpPos.y + yOffset}, m_commonSize });
		}
	}

	// если -1 то ничего не выбрано, иначе номер пункта меню [0, n]
	int IsSelect() const { return m_select; }
	void ResetActionElement() noexcept;

private:
	struct element
	{
		std::string text;
		bool enable;
		Point2 pos;
		Size2 size;
	};
	std::vector<element> m_elements;
	int m_actionElement = 0;

	Point2 m_leftUpPos = { 0 };
	const Size2 m_commonSize = { 140, 40 };

	int m_select = -1;
};