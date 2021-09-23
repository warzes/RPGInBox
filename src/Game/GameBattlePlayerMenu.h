#pragma once

class GameBattlePlayerMenu final
{
public:
	void Draw() noexcept;

	// возвращает -1 если ничего еще не выбрано, иначе номер индекса
	int Run() noexcept;

	void Reset() noexcept;
	void AddElement(const std::string& text) noexcept;
	void SetSelect(int sel) noexcept;
	void SetEnable(bool enable, int idElement) noexcept;
private:
	struct element
	{
		std::string text;
		bool enable;
		Point2 pos;
		Size2 size;
	};

	std::vector<element> m_elements;
	int m_select = 0;
};