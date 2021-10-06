#pragma once

class UIBattlePlayerMenu final
{
public:
	void Draw() noexcept;

	void Run() noexcept;

	void Reset(const Point2& leftUpPos) noexcept;
	void AddElement(const std::string& text) noexcept;

	template <typename T>
	void AddElements(const Point2& leftUpPos, const T& array) noexcept
	{
		Reset(leftUpPos);
		for (size_t i = 0; i < array.size(); i++)
		{
			AddElement(array[i]);
		}
	}

	// если -1 то ничего не выбрано, иначе номер пункта меню [0, n]
	int IsSelect() const { return m_select; }
private:
	struct element
	{
		std::string text;
		//bool enable;
		Point2 pos;
		Size2 size;
	};
	std::vector<element> m_elements;
	int m_actionElement = 0;

	Point2 m_leftUpPos = { 0 };
	const Size2 m_commonSize = { 140, 40 };

	int m_select = -1;
};