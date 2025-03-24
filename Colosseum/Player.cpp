#include <stdio.h>
#include <random>

#include "Player.h"

void SetJob(PLAYER* player)
{
	//직업 특성 할당
	player[WARRIOR].JOB = WARRIOR;
	player[WARRIOR].isPlayer = false;
	player[WARRIOR].hp = 120;
	player[WARRIOR].atkDamage = 20;
	player[WARRIOR].skill[0].skillName = "warriorSkill1";
	player[WARRIOR].skill[0].passiveProb = 20;
	player[WARRIOR].poisonStack = 0;

	player[THIEF].JOB = THIEF;
	player[THIEF].isPlayer = false;
	player[THIEF].hp = 100;
	player[THIEF].atkDamage = 10;
	player[THIEF].skill[0].skillName = "thiefSkill1";
	player[THIEF].skill[0].passiveProb = 20;
	player[THIEF].poisonStack = 0;

	player[THIEF].JOB = WIZARD;
	player[THIEF].isPlayer = false;
	player[THIEF].hp = 100;
	player[THIEF].atkDamage = 26;
	player[THIEF].skill[0].skillName = "wizardSkill1";
	player[THIEF].skill[0].passiveProb = 50;
	player[THIEF].poisonStack = 0;
}

//PLAYER* SelectPlayer(int x, int y)
//{
//	//커서 포지션 정해지면 수정할 것
//	if (x == warrior.x)
//	{
//		warrior.isPlayer = true;
//		return &warrior;
//	}
//	else if (x == thief.x)
//	{
//		thief.isPlayer = true;
//		return &thief;
//	}
//	else if (x == wizard.x)
//	{
//		wizard.isPlayer = true;
//		return &wizard;
//	}
//}

void UseAttack(PLAYER* player, PLAYER* enemy)
{
	std::random_device rd;

	unsigned long long seed = rd();

	std::mt19937 gen(seed);

	std::uniform_int_distribution<int> dist(1, (100 / enemy->skill[0].passiveProb));

	if (player->JOB == THIEF)
	{
		enemy->hp -= player->atkDamage;
		enemy->poisonStack += 2;
	}
	else if (enemy->JOB == WARRIOR)
	{
		if (dist(gen) == 1)
			enemy->hp -= (player->atkDamage / 2);
		else
			enemy->hp -= player->atkDamage;
	}
	else
		enemy->hp -= player->atkDamage;

	if (enemy->poisonStack > 0)
		enemy->hp -= enemy->poisonStack--;
}

void UseSkill(PLAYER* player, PLAYER* enemy)
{
	std::random_device rd;

	unsigned long long seed = rd();

	std::mt19937 gen(seed);

	std::uniform_int_distribution<int> dist(1, (100 / player->skill[0].passiveProb));

	if (player->JOB == THIEF)
	{
		enemy->poisonStack += dist(gen) + 10;
	}
	else if (player->JOB == WIZARD)
	{
		if (dist(gen) == 1)
		{
			enemy->hp -= 10;
			enemy->atkDamage -= 10;
		}
	}
	if (enemy->poisonStack > 0)
		enemy->hp -= enemy->poisonStack--;
}

void EnemyDied(PLAYER* player)
{
	if (player->isPlayer)
	{
		//GameOver();
	}
	else
	{
		//GoNextStage();
	}
}