#include "stdafx.h"
#include "BattleEngine.h"
#include "BattleCore.h"
#include "ResourceManager.h"
#include "DebugNew.h"
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
*	EndRound
*		переход в BeginRound
* 
* =============================================================================
* 3 Действие игрока
* =============================================================================
* также разбито на состояния m_playerCommandState
* =============================================================================
* X Draw
* =============================================================================
* TODO
* =============================================================================
* есть переменная m_numRound - кол-во прошедших раундов. показывать на экране или еще где
*/
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
void BattleEngine::StartBattle(ResourceManager& resources, const oEnemyParty& enemys) noexcept
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
	// отрисовка заднего фона окна боя
	Rectangle dest = { 20.0f, 20.0f, 0.0f, 0.0f };
	dest.width  = (float)GetScreenWidth()  - dest.x * 2.0f;
	dest.height = (float)GetScreenHeight() - dest.y * 2.0f;
	DrawTextureNPatch(*m_patchTexture, m_ninePatchInfo, dest, { 0.0f, 0.0f }, 0.0f, { 255, 255, 255, 180 });

	// отрисовка пола
	DrawTextureTiled(*m_battleBackGround, { 0,0,64,64 }, { battleCell[0].x, battleCell[0].y,500,680 }, { 0,0 }, 0.0, 1.0, WHITE);

	// отрисовка линии разделения поля боя
	DrawRectangle(battleCell[0].x, 383, 500, 3, WHITE);

	// отрисовка маркеров
	// draw player select marker
	if (isPlayer())
	{
		const Point2& pos = m_playerParty.positionCharactersInParty[m_members[m_currentMember].number];
		const unsigned index = (pos.y) * ColumnWidthBattleCells + pos.x;
		DrawTextureNPatch(*m_textureUI_character, m_ninePatchInfo_character, battleCell[index + 6], { 0.0f, 0.0f }, 0.0f, { 0, 228, 48, 140 });

		if (m_playerCommand.GetState() == PlayerCommandState::SelectMeleeAttackTarget && 
			m_members[m_currentMember].number >=0 && 
			m_members[m_currentMember].number < 3) // игрок выбрал действие атака и только персонажами первого ряда
		{
			// 6,7,8 - первый ряд игрока, можетбить по первому ряду противника 3, 4, 5 (почему по всем трем? потому что в реальном бою они не стоят в таких вот ровненьких рядах, поэтому можно бить в любого ближника - участники маневрируют)

			DrawTextureNPatch(*m_textureUI_character, m_ninePatchInfo_character, battleCell[3], { 0.0f, 0.0f }, 0.0f, { 230, 41, 55, 140 });
			DrawTextureNPatch(*m_textureUI_character, m_ninePatchInfo_character, battleCell[4], { 0.0f, 0.0f }, 0.0f, { 230, 41, 55, 140 });
			DrawTextureNPatch(*m_textureUI_character, m_ninePatchInfo_character, battleCell[5], { 0.0f, 0.0f }, 0.0f, { 230, 41, 55, 140 });
		}
	}

	// отрисовка портретов участников боя
	for (unsigned i = 0; i < MaxNumCharacterInPlayerParty; i++)
	{
		// TODO: по другому рисовать убитых и под разными статусами
		const Point2 &pos = m_playerParty.positionCharactersInParty[i];
		const unsigned index = (pos.y + 2)* ColumnWidthBattleCells + pos.x;
		DrawTexture(*m_texChar2, battleCell[index].x, battleCell[index].y, WHITE);
		
		//DrawText(m_playerParty.characters[i].statistics.hp.GetFullText().c_str(), battleCell[index].x+25, battleCell[index].y, 30, BLACK);
	}
	for (unsigned i = 0; i < m_enemyParty.enemys.size(); i++)
	{
		// TODO: по другому рисовать убитых и под разными статусами
		const Point2& pos = m_enemyParty.positionCharactersInParty[i];
		const unsigned index = (pos.y) * ColumnWidthBattleCells + pos.x;
		DrawTexture(*m_texChar1, battleCell[index].x, battleCell[index].y, WHITE);
		//DrawText(m_enemyParty.enemys[i].statistics.hp.GetFullText().c_str(), battleCell[index].x + 25, battleCell[index].y, 30, BLACK);
	}

	// отрисовка боковых панелей

	// левая верхняя панель
	{
		DrawRectangle(29, 45, 230, 335, { 0, 0, 0, 180 });
		DrawText("Enemy", 135, 100, 40, RED);
		DrawText("Enemy", 135, 270, 40, RED);
	}

	// левая нижняя панель
	{
		DrawRectangle(29, 390, 230, 335, { 0, 0, 0, 180 });
		DrawText("Player", 125, 450, 40, GREEN);
		DrawText("Player", 125, 620, 40, GREEN);
	}

	// правая верхняя панель
	{
		DrawRectangle(765, 45, 230, 335, { 0, 0, 0, 180 });
		DrawText("INFO WINDOW", 775, 50, 30, WHITE);
	}

	// правая нижняя панель
	{
		DrawRectangle(765, 390, 230, 335, { 0, 0, 0, 180 });

		if (isPlayer())
		{
			m_playerCommand.DrawCommandList();
		}		
	}
}
//-----------------------------------------------------------------------------
void BattleEngine::setInitiative() noexcept
{
	m_members.clear(); // сброс старого списка инициативы
	for (unsigned i = 0; i < MaxNumCharacterInPlayerParty; i++)
		m_members.push_back({ BattleMember::type::player, i});
	for (unsigned i = 0; i < m_enemyParty.enemys.size(); i++)
		m_members.push_back({ BattleMember::type::enemy, i});
}
//-----------------------------------------------------------------------------
void BattleEngine::newRound() noexcept
{
	m_playerCommand.Reset();
	m_currentMember = 0;
	m_numRound++;
	m_state = roundState::Round;
	// TODO: возможно показывать окно начала раунда
}
//-----------------------------------------------------------------------------
void BattleEngine::currentRound() noexcept
{
	bool memberEnd = false;
	if (isPlayer())     memberEnd = m_playerCommand.PlayerAction();
	else if (isEnemy()) memberEnd = enemyAction();

	if (memberEnd) // текущий участник подействовал, переключаемся на следующего
		selectNextMember();
}
//-----------------------------------------------------------------------------
void BattleEngine::endRound() noexcept
{
	// TODO: пока ничего
	m_state = roundState::BeginRound;
}
//-----------------------------------------------------------------------------
bool BattleEngine::enemyAction() noexcept
{
	return true;// враги пока пропускают ход
}
//-----------------------------------------------------------------------------
void BattleEngine::selectNextMember() noexcept
{
	m_currentMember++;
	if (m_currentMember >= m_members.size())
	{
		m_state = roundState::EndRound;
		m_currentMember = 0;
	}		
}
//-----------------------------------------------------------------------------

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