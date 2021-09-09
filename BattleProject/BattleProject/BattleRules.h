#pragma once

struct BattleRules
{
	void MeleeAttack(Hero* hero, Enemy* enemyTarget)
	{
		int d = rand() % 3;
		d += hero->att;
		enemyTarget->hp -= d;
	}
	void MeleeAttack(Enemy* enemy, Hero* heroTarget)
	{

	}

};