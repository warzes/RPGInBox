#pragma once

struct BattleMember
{
	enum class type { player, enemy } type;
	unsigned number;
};

// ������� ������
constexpr Rectangle battleCell[] =
{
	{ 262.0f,  44.0f, 160.0f, 160.0f }, // 1x1
	{ 432.0f,  44.0f, 160.0f, 160.0f }, // 2x1
	{ 602.0f,  44.0f, 160.0f, 160.0f }, // 3x1

	{ 262.0f, 214.0f, 160.0f, 160.0f }, // 1x2
	{ 432.0f, 214.0f, 160.0f, 160.0f }, // 2x2
	{ 602.0f, 214.0f, 160.0f, 160.0f }, // 3x2

	{ 262.0f, 394.0f, 160.0f, 160.0f }, // 1x3
	{ 432.0f, 394.0f, 160.0f, 160.0f }, // 2x3
	{ 602.0f, 394.0f, 160.0f, 160.0f }, // 3x3

	{ 262.0f, 564.0f, 160.0f, 160.0f }, // 1x4
	{ 432.0f, 564.0f, 160.0f, 160.0f }, // 2x4
	{ 602.0f, 564.0f, 160.0f, 160.0f }, // 3x4
};
//-----------------------------------------------------------------------------
constexpr unsigned ColumnWidthBattleCells = (unsigned)Countof(battleCell) / 2u / 2u; // ������ ���� = ���-�� ����� / ��� ������� ���� / ��� ����

// ������� ������ ������
constexpr Rectangle playerCommandRect[] =
{
	{ 810.0f, 415.0f, 140.0f, 40.0f },
	{ 810.0f, 455.0f, 140.0f, 40.0f },
	{ 810.0f, 495.0f, 140.0f, 40.0f },
	{ 810.0f, 535.0f, 140.0f, 40.0f },
};