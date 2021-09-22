#include "stdafx.h"
#include "EnemyTemplateData.h"
#include "DebugNew.h"
//-----------------------------------------------------------------------------
oEnemy EnemyTemplateData::CreateTest() noexcept
{
	oEnemy enemy;
	{
		enemy.name = "Test monstr";
		enemy.statistics.hp = 10;
		enemy.statistics.mp = 10;
	}	
	return enemy;
}
//-----------------------------------------------------------------------------