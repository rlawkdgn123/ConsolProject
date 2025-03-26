#include <stdio.h>
#include <random>

#include "Player.h"

void player::SetItem(ITEM* playeritem)
{
	ITEM item[2];
	item[0].itemName = "Hp Potion";
	item[0].effect = HEAL;
	item[0].num = 30;
	item[0].itemcount = 2;
	item[1].itemName = "Stun";
	item[1].effect = STUN;
	item[1].num = 1;
	item[0].itemcount = 1;

	playeritem[0] = item[0];
	playeritem[1] = item[1];
}

void player::SetPlayer(PLAYER* player)
{
	//직업 특성 할당
	player[WARRIOR].JOB = WARRIOR;
	player[WARRIOR].isPlayer = false;
	player[WARRIOR].hp = 120;
	player[WARRIOR].atkDamage = 20;
	player[WARRIOR].state = NORMAL;
	player::SetItem(player[WARRIOR].item);
	player[WARRIOR].skill[0].skillName = "warriorSkill1";
	player[WARRIOR].skill[0].passiveProb = 20;
	player[WARRIOR].skill[1].skillName = "warriorSkill2";
	player[WARRIOR].skill[1].passiveProb = 20;
	player[WARRIOR].poisonStack = 0;
	player[WARRIOR].skillCount = 0;


	player[THIEF].JOB = THIEF;
	player[THIEF].isPlayer = false;
	player[THIEF].hp = 100;
	player[THIEF].atkDamage = 10;
	player[THIEF].state = NORMAL;
	player::SetItem(player[THIEF].item);
	player[THIEF].skill[0].skillName = "thiefSkill1";
	player[THIEF].skill[0].passiveProb = 20;
	player[THIEF].skill[1].skillName = "thiefSkill2";
	player[THIEF].skill[1].passiveProb = 20;
	player[THIEF].poisonStack = 0;
	player[THIEF].skillCount = 0;

	player[WIZARD].JOB = WIZARD;
	player[WIZARD].isPlayer = false;
	player[WIZARD].hp = 100;
	player[WIZARD].atkDamage = 26;
	player[WIZARD	].state = NORMAL;
	player::SetItem(player[WIZARD].item);
	player[WIZARD].skill[0].skillName = "wizardSkill1";
	player[WIZARD].skill[0].passiveProb = 50;
	player[WIZARD].skill[1].skillName = "wizardSkill2";
	player[WIZARD].skill[1].passiveProb = 50;
	player[WIZARD].poisonStack = 0;
	player[WIZARD].skillCount = 0;
}

void player::SetEnemy(PLAYER* job, PLAYER* enemy)
{
	int j = 0;
	for (int i = 0; i < JOBCOUNT; i++)
	{
		if (!(job[i].isPlayer))
		{
			enemy[j] = job[i];
		}
	}
}

void player::UseAttack(PLAYER* player, PLAYER* enemy)
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

void player::UseSkill(PLAYER* player, PLAYER* enemy, int skillNum)
{
	std::random_device rd;

	unsigned long long seed = rd();

	std::mt19937 gen(seed);

	std::uniform_int_distribution<int> dist(1, (100 / player->skill[skillNum].passiveProb));

	if (player->JOB == THIEF && skillNum == 0)
	{
		enemy->poisonStack += dist(gen) + 10;
	}
	else if (player->JOB == WIZARD && skillNum == 0)
	{
		if (dist(gen) == 1)
		{
			enemy->hp -= 10;
			enemy->atkDamage -= 10;
		}
	}
	else if (player->JOB == THIEF && skillNum == 1)
	{
		enemy->poisonStack += dist(gen) + 10;
	}
	else if (player->JOB == WIZARD && skillNum == 1)
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

void player::UseItem(PLAYER* player, int itemNum)
{
	if (player->item[itemNum].effect == HEAL)
	{
		player->hp += player->item[itemNum].num;
		player->item[itemNum].itemcount--;
	}
	else if (player->item[itemNum].effect == STUN)
	{
		player->state = STUN;
		player->item[itemNum].itemcount--;
	}
}

void player::EnemyDied(PLAYER* player)
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