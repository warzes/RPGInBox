#include "stdafx.h"
#include "BattleEngine.h"
#include "ResourceManager.h"
#include <Engine/DebugNew.h>
//-----------------------------------------------------------------------------
void BattleEngine::StartBattle(ResourceManager& resources, const EnemyParty& enemy)
{
	m_enemy = enemy;
	m_patchTexture = resources.GetTexture("../data/ui/battleui_background.png");
	m_textureUI_character = resources.GetTexture("../data/ui/battleui_member.png");

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
	m_currentMember = m_members.size();
}
//-----------------------------------------------------------------------------
void BattleEngine::StopBattle()
{
}
//-----------------------------------------------------------------------------
void BattleEngine::Draw()
{
	if (!m_patchTexture) return;
	Rectangle dest = { 20.0f, 20.0f, 0.0f, 0.0f };
	dest.width = (float)GetScreenWidth() - dest.x * 2.0f;
	dest.height = (float)GetScreenHeight() - dest.y * 2.0f;
    DrawTextureNPatch(*m_patchTexture, m_ninePatchInfo, dest, { 0.0f, 0.0f }, 0.0f, WHITE);

	// 1x1
	{
		Rectangle dest2 = { 262.0f, 44.0f, 160.0f, 160.0f };
		DrawTextureNPatch(*m_textureUI_character, m_ninePatchInfo_character, dest2, { 0.0f, 0.0f }, 0.0f, WHITE);
	}

	// 2x1
	{
		Rectangle dest2 = { 432.0f, 44.0f, 160.0f, 160.0f };
		DrawTextureNPatch(*m_textureUI_character, m_ninePatchInfo_character, dest2, { 0.0f, 0.0f }, 0.0f, WHITE);
	}
	// 3x1
	{
		Rectangle dest2 = { 602.0f, 44.0f, 160.0f, 160.0f };
		DrawTextureNPatch(*m_textureUI_character, m_ninePatchInfo_character, dest2, { 0.0f, 0.0f }, 0.0f, WHITE);
	}
	// 1x2
	{
		Rectangle dest2 = { 262.0f, 214.0f, 160.0f, 160.0f };
		DrawTextureNPatch(*m_textureUI_character, m_ninePatchInfo_character, dest2, { 0.0f, 0.0f }, 0.0f, WHITE);
	}

	// 2x2
	{
		Rectangle dest2 = { 432.0f, 214.0f, 160.0f, 160.0f };
		DrawTextureNPatch(*m_textureUI_character, m_ninePatchInfo_character, dest2, { 0.0f, 0.0f }, 0.0f, WHITE);
	}
	// 3x2
	{
		Rectangle dest2 = { 602.0f, 214.0f, 160.0f, 160.0f };
		DrawTextureNPatch(*m_textureUI_character, m_ninePatchInfo_character, dest2, { 0.0f, 0.0f }, 0.0f, WHITE);
	}
	

	// 1x3
	{
		Rectangle dest2 = { 262.0f, 394.0f, 160.0f, 160.0f };
		DrawTextureNPatch(*m_textureUI_character, m_ninePatchInfo_character, dest2, { 0.0f, 0.0f }, 0.0f, WHITE);
	}

	// 2x3
	{
		Rectangle dest2 = { 432.0f, 394.0f, 160.0f, 160.0f };
		DrawTextureNPatch(*m_textureUI_character, m_ninePatchInfo_character, dest2, { 0.0f, 0.0f }, 0.0f, WHITE);
	}
	// 3x3
	{
		Rectangle dest2 = { 602.0f, 394.0f, 160.0f, 160.0f };
		DrawTextureNPatch(*m_textureUI_character, m_ninePatchInfo_character, dest2, { 0.0f, 0.0f }, 0.0f, WHITE);
	}
	// 1x4
	{
		Rectangle dest2 = { 262.0f, 564.0f, 160.0f, 160.0f };
		DrawTextureNPatch(*m_textureUI_character, m_ninePatchInfo_character, dest2, { 0.0f, 0.0f }, 0.0f, WHITE);
	}

	// 2x4
	{
		Rectangle dest2 = { 432.0f, 564.0f, 160.0f, 160.0f };
		DrawTextureNPatch(*m_textureUI_character, m_ninePatchInfo_character, dest2, { 0.0f, 0.0f }, 0.0f, WHITE);
	}
	// 3x4
	{
		Rectangle dest2 = { 602.0f, 564.0f, 160.0f, 160.0f };
		DrawTextureNPatch(*m_textureUI_character, m_ninePatchInfo_character, dest2, { 0.0f, 0.0f }, 0.0f, WHITE);
	}


	DrawText("Test text", 25, 25, 20, WHITE);
}
//-----------------------------------------------------------------------------
void BattleEngine::Update(float deltaTime)
{
	// все участники участвовали (или бой только начался)
	if (m_currentMember == m_members.size())
		nextRound();

	// обработка текущего раунда
	currentRound();

	// TODO: ивент завершения боя по состоянию участников боя
}
//-----------------------------------------------------------------------------
void BattleEngine::nextRound()
{
	m_currentMember = 0;
}
//-----------------------------------------------------------------------------
void BattleEngine::currentRound()
{
	bool nextMember = false;
	if (m_members[m_currentMember].type == member::type_::player)
		nextMember = playerAction();
	else if (m_members[m_currentMember].type == member::type_::enemy)
		nextMember = enemyAction();

	if (nextMember) // текущий участник подействовал, переключаемся на следующего
		m_currentMember++;
}
//-----------------------------------------------------------------------------
bool BattleEngine::playerAction()
{
	return false;
}
//-----------------------------------------------------------------------------
bool BattleEngine::enemyAction()
{
	return false;
}
//-----------------------------------------------------------------------------