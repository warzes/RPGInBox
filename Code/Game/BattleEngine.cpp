#include "stdafx.h"
#include "BattleEngine.h"
#include "ResourceManager.h"
#include <Engine/DebugNew.h>

#error здесь
- разбить draw на подфункции... А возможно выделить отдельный класс BattleDrawUI
- прописать цикл выбора действия и по нему отрефакторить код




/*
- ход игрока, он выбирает любого своего перса которым не ходил, и совершает ход,
- затем один из монстров ходит,
- затем игрок снова выбирает любого своего перса которым не ходил, и совершает ход
походившие участники рисуются черно-белыми

*/
//-----------------------------------------------------------------------------
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
constexpr Rectangle playerCommandRect[] =
{
	{ 810.0f, 415.0f, 120.0f, 40.0f },
	{ 810.0f, 455.0f, 120.0f, 40.0f },
	{ 810.0f, 495.0f, 120.0f, 40.0f },
};
//-----------------------------------------------------------------------------
constexpr unsigned ColumnWidthBattleCells = Countof(battleCell) / 2 / 2; // ширина ряда = кол-во ячеек / две стороны поля / два ряда
//-----------------------------------------------------------------------------
void BattleEngine::StartBattle(ResourceManager& resources, const EnemyParty& enemy)
{
	m_enemy = enemy;
	m_patchTexture = resources.GetTexture("../data/ui/battleui_background.png");
	m_textureUI_character = resources.GetTexture("../data/ui/battleui_member.png");

	m_texChar1 = resources.GetTexture("../data/temp/textures/character/mon-goblin.png");
	m_texChar2 = resources.GetTexture("../data/temp/textures/character/mon-rogue3.png");
	m_battleBackGround = resources.GetTexture("../data/temp/textures/character/plains-ground.png");

 	//пока без инициативы, по такому принципу - сначала действуют все персонажи игрока с первого по последний, потом действуют все враги
	//а возможно такую механику и оставить надолго
	for (size_t i = 0; i < MaxNumCharacterInPlayerParty; i++)
	{
		member m;
		m.type = member::type_::player;
		m.number = i;
		m_members.push_back(m);
	}
	for (size_t i = 0; i < m_enemy.enemys.size(); i++)
	{
		member m;
		m.type = member::type_::enemy;
		m.number = i;
		m_members.push_back(m);
	}

	nextRound();
}
//-----------------------------------------------------------------------------
void BattleEngine::StopBattle()
{
}
//-----------------------------------------------------------------------------
void BattleEngine::Draw()
{
	// draw background
	Rectangle dest = { 20.0f, 20.0f, 0.0f, 0.0f };
	dest.width = (float)GetScreenWidth() - dest.x * 2.0f;
	dest.height = (float)GetScreenHeight() - dest.y * 2.0f;
	DrawTextureNPatch(*m_patchTexture, m_ninePatchInfo, dest, { 0.0f, 0.0f }, 0.0f, {255, 255, 255, 180});

	// draw battle cell
	DrawTextureTiled(*m_battleBackGround, {0,0,64,64}, { battleCell[0].x, battleCell[0].y,500,680 }, { 0,0 }, 0.0, 1.0, WHITE);
		
	// draw line
	DrawRectangle(battleCell[0].x, 383, 500, 3, WHITE);



	// draw player select marker
	if (isPlayer())
	{
		DrawTextureNPatch(*m_textureUI_character, m_ninePatchInfo_character, battleCell[m_currentMember + 6], { 0.0f, 0.0f }, 0.0f, { 0, 228, 48, 140 });

		if (m_currentPlayerCommand == 0 && m_currentMember >=0 && m_currentMember < 3) // игрок выбрал действие атака
		{
			// 6,7,8 - первый ряд игрока, можетбить по первому ряду противника 3, 4, 5 (почему по всем трем? потому что в реальном бою они не стоят в таких вот ровненьких рядах, поэтому можно бить в любого ближника - участники маневрируют)

			DrawTextureNPatch(*m_textureUI_character, m_ninePatchInfo_character, battleCell[3], { 0.0f, 0.0f }, 0.0f, { 230, 41, 55, 140 });
			DrawTextureNPatch(*m_textureUI_character, m_ninePatchInfo_character, battleCell[4], { 0.0f, 0.0f }, 0.0f, { 230, 41, 55, 140 });
			DrawTextureNPatch(*m_textureUI_character, m_ninePatchInfo_character, battleCell[5], { 0.0f, 0.0f }, 0.0f, { 230, 41, 55, 140 });
		}
	}

	for (int i = 0; i < Countof(battleCell); i++)
	{
		//DrawTextureNPatch(*m_textureUI_character, m_ninePatchInfo_character, battleCell[i], { 0.0f, 0.0f }, 0.0f, WHITE);
					
		if (i < 6)
			DrawTexture(*m_texChar1, battleCell[i].x, battleCell[i].y, WHITE);
		else
			DrawTexture(*m_texChar2, battleCell[i].x, battleCell[i].y, WHITE);
	}

	// draw info

	// draw info window
	{
		// draw background
		DrawRectangle(765, 45, 230, 335, { 0, 0, 0, 180 });
		DrawText("INFO WINDOW", 770, 50, 30, WHITE);
	}

	// draw command window	

	{
		// draw background
		DrawRectangle(765, 390, 230, 335, { 0, 0, 0, 180 });

		if (isPlayer())
		{			
			// draw command
			for (int i = 0; i < Countof(playerCommandRect); i++)
				DrawRectangleLinesEx(playerCommandRect[i], 1.0f, WHITE);

			DrawText("Attack", 820, 420, 30, WHITE);
			DrawText("Skill", 820, 460, 30, WHITE);
			DrawText("Magic", 820, 500, 30, WHITE);
			// draw marker
			if (m_currentPlayerCommand >= 0)
				DrawText(">>>", 770, 420 + m_currentPlayerCommand*40, 30, GREEN);
		}		
	}


	DrawText("Enemy", 140, 100, 40, RED);
	DrawText("Enemy", 140, 270, 40, RED);

	DrawText("Player", 130, 450, 40, GREEN);
	DrawText("Player", 130, 620, 40, GREEN);
}
//-----------------------------------------------------------------------------
void BattleEngine::Update(float deltaTime)
{
	// обработка текущего раунда
	currentRound();
	// TODO: ивент завершения боя по состоянию участников боя
}
//-----------------------------------------------------------------------------
void BattleEngine::nextRound()
{
	m_currentMember = 0;
	m_currentPlayerCommand = -1;
}
//-----------------------------------------------------------------------------
void BattleEngine::currentRound()
{
	bool memberEnd = false;
	if (isPlayer()) memberEnd = playerAction();
	else if (isEnemy()) memberEnd = enemyAction();

	if (memberEnd) // текущий участник подействовал, переключаемся на следующего
		nextMember();
}
//-----------------------------------------------------------------------------
bool BattleEngine::playerAction()
{	
	selectPlayerCommand(); // выбор команды
	if (m_currentPlayerCommand >= 0)
		selectAttackTargetEnemy();

	//selectCell(); // обработка выбора мышью

	return false;
}
//-----------------------------------------------------------------------------
bool BattleEngine::enemyAction()
{
	m_currentPlayerCommand = -1;
	return false;
}
//-----------------------------------------------------------------------------
void BattleEngine::nextMember()
{
	m_currentMember++;
	// все участники участвовали
	if (m_currentMember >= m_members.size())
		nextRound();
}
//-----------------------------------------------------------------------------
void BattleEngine::selectPlayerCommand()
{
	if (IsMouseButtonPressed(MOUSE_BUTTON_LEFT))
	{
		const auto mousePos = GetMousePosition();
		int select = 0;
		for (; select < Countof(playerCommandRect); select++)
		{
			const auto& rect = playerCommandRect[select];
			if (mousePos.x > rect.x && mousePos.x < rect.x + rect.width &&
				mousePos.y > rect.y && mousePos.y < rect.y + rect.height)
				break;
		}
		if (select < Countof(playerCommandRect))
		{
			m_currentPlayerCommand = select;
		}
	}
	else if (IsMouseButtonPressed(MOUSE_BUTTON_RIGHT))
	{
		m_currentPlayerCommand = -1; // отмена выбранной команды по правой кнопке мыши
	}
}
//-----------------------------------------------------------------------------
void BattleEngine::selectAttackTargetEnemy()
{
	if (IsMouseButtonPressed(MOUSE_BUTTON_LEFT))
	{
		const auto mousePos = GetMousePosition();
		int cell = 0;
		for (; cell < Countof(battleCell); cell++)
		{
			const auto& rect = battleCell[cell];
			if (mousePos.x > rect.x && mousePos.x < rect.x + rect.width &&
				mousePos.y > rect.y && mousePos.y < rect.y + rect.height)
				break;
		}
		if (cell < Countof(battleCell))
		{
			if (m_currentMember >= 0 && m_currentMember < 3 && cell >= 3 && cell < 6)
			{
				const unsigned x = cell % ColumnWidthBattleCells;
				const unsigned y = cell / ColumnWidthBattleCells;

				printf("%d - %d:%d\n", cell, x + 1, y + 1);
			}			
		}
	}
}
//-----------------------------------------------------------------------------
//void BattleEngine::selectCell()
//{
//	if (IsMouseButtonPressed(MOUSE_BUTTON_LEFT))
//	{
//		const auto mousePos = GetMousePosition();
//		int cell = 0;
//		for (; cell < Countof(battleCell); cell++)
//		{
//			const auto& rect = battleCell[cell];
//			if (mousePos.x > rect.x && mousePos.x < rect.x + rect.width &&
//				mousePos.y > rect.y && mousePos.y < rect.y + rect.height)
//				break;
//		}
//		if (cell < Countof(battleCell))
//		{
//			const unsigned x = cell % ColumnWidthBattleCells;
//			const unsigned y = cell / ColumnWidthBattleCells;
//
//			printf("%d - %d:%d\n", cell, x + 1, y + 1);
//		}
//	}	
//}
//-----------------------------------------------------------------------------