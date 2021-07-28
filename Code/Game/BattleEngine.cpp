#include "stdafx.h"
#include "BattleEngine.h"
#include "ResourceManager.h"
#include <Engine/DebugNew.h>
/*
* =============================================================================
* 1 Подготовка к бою 
* =============================================================================
* Когда возникает событие боя, вызывается функция StartBattle(), она:
*	получает список группы врагов от World (так как списки монстров зависят от окружения)
*		список группы игрока константен и устанавливается при старте игры (нет смысла его передавать)
*	получает список всех нужных текстур (текстуры кешируются, поэтому можно не заботиться об этом)
*		текстуры интерфейса
*		текстуры портретов игрока
*		текстуры портретов монстров
*	Настройка инициативы
*		На данный момент так - сначала ходят персонажи игрока, потом монстры
*		для списка используется внутренняя структура member - конечно можно было сделать через полиморфизм, и возможно в будущем так и будет, но и использование внешнего списка members тоже вполне работает
* 
* =============================================================================
* 2 Update
* =============================================================================
* текущее состояние боя определяется по roundState m_state
*	BeginRound
*		установка текущего участника на самого первого по списку участников
*		переход в следующий статус раунда Round
*	Round
*		смотрим на m_currentMember
*			если игрок, ожидаем действие игрока
*			если монстр, ожидаем действие монстра
*			переходим на следующего участника, если они закончились, переходим в EndRound
* 
* =============================================================================
* TODO
* =============================================================================
* есть переменная m_numRound - кол-во прошедших раундов. показывать на экране или еще где
*/
//-----------------------------------------------------------------------------
// позиции клеток
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
// позиции команд игрока
constexpr Rectangle playerCommandRect[] =
{
	{ 810.0f, 415.0f, 120.0f, 40.0f },
	{ 810.0f, 455.0f, 120.0f, 40.0f },
	{ 810.0f, 495.0f, 120.0f, 40.0f },
};
//-----------------------------------------------------------------------------
constexpr unsigned ColumnWidthBattleCells = Countof(battleCell) / 2 / 2; // ширина ряда = кол-во ячеек / две стороны поля / два ряда
//-----------------------------------------------------------------------------
void BattleEngine::StartBattle(ResourceManager& resources, const EnemyParty& enemys) noexcept
{
	// сброс и установка переменных
	m_state = roundState::BeginRound;
	m_numRound = 0;
	m_enemyParty = enemys;
	// установка текстур
	m_patchTexture = resources.GetTexture("../data/ui/battleui_background.png");
	m_textureUI_character = resources.GetTexture("../data/ui/battleui_member.png");
	m_battleBackGround = resources.GetTexture("../data/temp/textures/character/plains-ground.png");
	m_texChar1 = resources.GetTexture("../data/temp/textures/character/mon-goblin.png");
	m_texChar2 = resources.GetTexture("../data/temp/textures/character/mon-rogue3.png");
	// настройка инициативы
	setInitiative();
}
//-----------------------------------------------------------------------------
void BattleEngine::Update(float deltaTime) noexcept
{
	if (m_state == roundState::BeginRound)    // новый раунд
		newRound();
	else if (m_state == roundState::Round)    // обработка текущего раунда	
		currentRound();
	else if (m_state == roundState::EndRound) // обработка конца раунда	
		endRound();
}
//-----------------------------------------------------------------------------
void BattleEngine::Draw() noexcept
{
	// draw background
	Rectangle dest = { 20.0f, 20.0f, 0.0f, 0.0f };
	dest.width = (float)GetScreenWidth() - dest.x * 2.0f;
	dest.height = (float)GetScreenHeight() - dest.y * 2.0f;
	DrawTextureNPatch(*m_patchTexture, m_ninePatchInfo, dest, { 0.0f, 0.0f }, 0.0f, { 255, 255, 255, 180 });

	// draw battle floor
	DrawTextureTiled(*m_battleBackGround, { 0,0,64,64 }, { battleCell[0].x, battleCell[0].y,500,680 }, { 0,0 }, 0.0, 1.0, WHITE);

	// draw line
	DrawRectangle(battleCell[0].x, 383, 500, 3, WHITE);


	//// draw player select marker
	//if (isPlayer())
	//{
	//	DrawTextureNPatch(*m_textureUI_character, m_ninePatchInfo_character, battleCell[m_currentMember + 6], { 0.0f, 0.0f }, 0.0f, { 0, 228, 48, 140 });

	//	if (m_currentPlayerCommand == 0 && m_currentMember >=0 && m_currentMember < 3) // игрок выбрал действие атака
	//	{
	//		// 6,7,8 - первый ряд игрока, можетбить по первому ряду противника 3, 4, 5 (почему по всем трем? потому что в реальном бою они не стоят в таких вот ровненьких рядах, поэтому можно бить в любого ближника - участники маневрируют)

	//		DrawTextureNPatch(*m_textureUI_character, m_ninePatchInfo_character, battleCell[3], { 0.0f, 0.0f }, 0.0f, { 230, 41, 55, 140 });
	//		DrawTextureNPatch(*m_textureUI_character, m_ninePatchInfo_character, battleCell[4], { 0.0f, 0.0f }, 0.0f, { 230, 41, 55, 140 });
	//		DrawTextureNPatch(*m_textureUI_character, m_ninePatchInfo_character, battleCell[5], { 0.0f, 0.0f }, 0.0f, { 230, 41, 55, 140 });
	//	}
	//}

	//for (int i = 0; i < Countof(battleCell); i++)
	//{
	//	//DrawTextureNPatch(*m_textureUI_character, m_ninePatchInfo_character, battleCell[i], { 0.0f, 0.0f }, 0.0f, WHITE);
	//				
	//	if (i < 6)
	//		DrawTexture(*m_texChar1, battleCell[i].x, battleCell[i].y, WHITE);
	//	else
	//		DrawTexture(*m_texChar2, battleCell[i].x, battleCell[i].y, WHITE);
	//}

	//// draw info

	//// draw info window
	//{
	//	// draw background
	//	DrawRectangle(765, 45, 230, 335, { 0, 0, 0, 180 });
	//	DrawText("INFO WINDOW", 770, 50, 30, WHITE);
	//}

	//// draw command window	

	//{
	//	// draw background
	//	DrawRectangle(765, 390, 230, 335, { 0, 0, 0, 180 });

	//	if (isPlayer())
	//	{			
	//		// draw command
	//		for (int i = 0; i < Countof(playerCommandRect); i++)
	//			DrawRectangleLinesEx(playerCommandRect[i], 1.0f, WHITE);

	//		DrawText("Attack", 820, 420, 30, WHITE);
	//		DrawText("Skill", 820, 460, 30, WHITE);
	//		DrawText("Magic", 820, 500, 30, WHITE);
	//		// draw marker
	//		if (m_currentPlayerCommand >= 0)
	//			DrawText(">>>", 770, 420 + m_currentPlayerCommand*40, 30, GREEN);
	//	}		
	//}


	//DrawText("Enemy", 140, 100, 40, RED);
	//DrawText("Enemy", 140, 270, 40, RED);

	//DrawText("Player", 130, 450, 40, GREEN);
	//DrawText("Player", 130, 620, 40, GREEN);
}
//-----------------------------------------------------------------------------
void BattleEngine::setInitiative() noexcept
{
	m_members.clear(); // сброс старого списка инициативы
	for (unsigned i = 0; i < MaxNumCharacterInPlayerParty; i++)
		m_members.push_back({ member::type_::player, i});
	for (unsigned i = 0; i < m_enemyParty.enemys.size(); i++)
		m_members.push_back({ member::type_::enemy, i});
}
//-----------------------------------------------------------------------------
void BattleEngine::newRound() noexcept
{
	m_currentMember = 0;
	m_numRound++;
	m_state = roundState::Round;
	// TODO: возможно показывать окно начала раунда
}
//-----------------------------------------------------------------------------
void BattleEngine::currentRound() noexcept
{
	bool memberEnd = false;
	if (isPlayer())     memberEnd = playerAction();
	else if (isEnemy()) memberEnd = enemyAction();

	if (memberEnd) // текущий участник подействовал, переключаемся на следующего
		selectMember();
}
//-----------------------------------------------------------------------------
bool BattleEngine::playerAction() noexcept
{
	//selectPlayerCommand(); // выбор команды
	//if (m_currentPlayerCommand >= 0)
	//	selectAttackTargetEnemy();

	////selectCell(); // обработка выбора мышью

	return false;
}
//-----------------------------------------------------------------------------
bool BattleEngine::enemyAction() noexcept
{
	return true;// враги пока пропускают ход
}
//-----------------------------------------------------------------------------
void BattleEngine::selectMember() noexcept
{
	m_currentMember++;
	// TODO: проверить статус что может ходить
	if (m_currentMember >= m_members.size())
		m_state = roundState::EndRound;
}
//-----------------------------------------------------------------------------
void BattleEngine::endRound() noexcept
{
	// TODO: пока ничего
	m_state = roundState::BeginRound;
}
//-----------------------------------------------------------------------------



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