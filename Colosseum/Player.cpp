#include <stdio.h>
#include <random>

#include "Player.h"

void player::SetItem(ITEM* playeritem)
{
	ITEM item[2];
	item[0].itemName = "Hp����";
	item[0].effect = HEAL;
	item[0].value = 30;
	item[0].itemcount = 2;
	item[1].itemName = "¯��";
	item[1].effect = STUN;
	item[1].value = 1;
	item[1].itemcount = 1;

	playeritem[0] = item[0];
	playeritem[1] = item[1];
}

void player::SetPlayer(PLAYER* player)
{
	//���� Ư�� �Ҵ�
	player[WARRIOR].job_name = "WARRIOR";
	player[WARRIOR].JOB = WARRIOR;
	player[WARRIOR].isPlayer = false;
	player[WARRIOR].hp = 120;
	player[WARRIOR].atkDamage = 15;
	player[WARRIOR].state = NORMAL;
	player::SetItem(player[WARRIOR].item);
	player[WARRIOR].skill[0].skillName = "warriorSkill1(�нú�)";
	player[WARRIOR].skill[0].passiveProb = 20;
	player[WARRIOR].skill[0].description = "20%�� Ȯ���� �޴� �������� �������� �پ���.";
	player[WARRIOR].skill[1].skillName = "warriorSkill2";
	player[WARRIOR].skill[1].description = "�� �Ͽ� ��뿡�� ���� ���ظ�ŭ ���ݷ��� �����Ѵ�.(��� ���� Ƚ�� : 1)";
	player[WARRIOR].skill[1].passiveProb = 100;
	player[WARRIOR].poisonStack = 0;
	player[WARRIOR].skillCount = 0;

	player[THIEF].job_name = "THIEF";
	player[THIEF].JOB = THIEF;
	player[THIEF].isPlayer = false;
	player[THIEF].hp = 100;
	player[THIEF].atkDamage = 0;
	player[THIEF].state = NORMAL;
	player[THIEF].maxPoisonStk = 20;
	player::SetItem(player[THIEF].item);
	player[THIEF].skill[0].skillName = "thiefSkill1";
	player[THIEF].skill[0].passiveProb = 20;
	player[THIEF].skill[0].description = "�������� ������ �� ������ �߰��Ѵ�.(���� : 11 ~ 15)";
	player[THIEF].skill[1].skillName = "thiefSkill2";
	player[THIEF].skill[1].passiveProb = 20;
	player[THIEF].skill[1].description = "���� ���� �� ���� ��ŭ ������ �������� �ְ� ���� �� ������ �ʱ�ȭ�Ѵ�.";
	player[THIEF].poisonStack = 0;
	player[THIEF].skillCount = 0;

	player[WIZARD].job_name = "WIZARD";
	player[WIZARD].JOB = WIZARD;
	player[WIZARD].isPlayer = false;
	player[WIZARD].hp = 100;
	player[WIZARD].atkDamage = 26;
	player[WIZARD	].state = NORMAL;
	player::SetItem(player[WIZARD].item);
	player[WIZARD].skill[0].skillName = "wizardSkill1";
	player[WIZARD].skill[0].passiveProb = 50;
	player[WIZARD].skill[0].description = "50% Ȯ���� ���� ���ݷ��� 5 ���ҽ�Ű�� �ڽ��� HP�� 10 ȸ���Ѵ�.";
	player[WIZARD].skill[1].skillName = "wizardSkill2";
	player[WIZARD].skill[1].skillDamage = 20;
	player[WIZARD].skill[1].passiveProb = 20;
	player[WIZARD].skill[1].description = "������ �������� �ְ� �� ������ ��ŭ HP�� ȸ���Ѵ�.";
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
			j++;
		}
	}
}

void player::UseAttack(PLAYER* player, PLAYER* enemy)
{
	std::random_device rd;

	unsigned long long seed = rd();

	std::mt19937 gen(seed);

	std::uniform_int_distribution<int> dist(1, 5);

	enemy->receiveDmg = 0;

	if (player->JOB == THIEF)
	{
		enemy->poisonStack += dist(gen) * 3;

		if (enemy->poisonStack > 0)
		{
			if (enemy->poisonStack >= player->maxPoisonStk)
			{
				enemy->hp -= player->maxPoisonStk;
				enemy->receiveDmg = player->maxPoisonStk;
				enemy->poisonStack--;
			}
			else
			{
				enemy->hp -= enemy->poisonStack;
				enemy->receiveDmg = enemy->poisonStack;
				enemy->poisonStack--;
			}
		}
	}
	else if (enemy->JOB == WARRIOR)
	{
		if (dist(gen) == 1)
		{
			enemy->hp -= (player->atkDamage / 2);
			enemy->receiveDmg = (player->atkDamage / 2);
		}
		else
		{
			enemy->hp -= player->atkDamage;
			enemy->receiveDmg = player->atkDamage;
		}
	}
	else
	{
		enemy->hp -= player->atkDamage;
		enemy->receiveDmg = player->atkDamage;
	}
	
}

void player::UseSkill(PLAYER* player, PLAYER* enemy, int skillNum)
{
	std::random_device rd;

	unsigned long long seed = rd();

	std::mt19937 gen(seed);

	std::uniform_int_distribution<int> dist(1, (100 / player->skill[skillNum].passiveProb));

	enemy->receiveDmg = 0;

	if (player->JOB == THIEF)
	{
		switch (skillNum)
		{
		case 0:
			enemy->poisonStack += dist(gen) + 10;
			player->skillCount++;
			break;
		case 1:
			enemy->hp -= enemy->poisonStack;
			enemy->receiveDmg = enemy->poisonStack;
			enemy->poisonStack = 0;
			player->skillCount++;
			break;
		default:
			break;
		}
		if (enemy->poisonStack > 0)
		{
			if (enemy->poisonStack >= player->maxPoisonStk)
			{
				enemy->hp -= player->maxPoisonStk;
				enemy->receiveDmg = player->maxPoisonStk;
				enemy->poisonStack--;
			}
			else
			{
				enemy->hp -= enemy->poisonStack;
				enemy->receiveDmg = enemy->poisonStack;
				enemy->poisonStack--;
			}
		}
	}
	else if (player->JOB == WARRIOR)
	{
		switch (skillNum)
		{
		case 0:
			break;
		case 1:
			player->atkDamage += player->receiveDmg;
			player->skillCount = 3;
			break;
		default:
			break;
		}
	}
	else if (player->JOB == WIZARD)
	{
		switch (skillNum)
		{
		case 0:
			if (dist(gen) == 1)
			{
				player->hp += 10;
				if (enemy->JOB == THIEF)
				{
					player->poisonStack -= 10;
				}
				else
					enemy->atkDamage -= 5;
			}
			player->skillCount++;
			break;
		case 1:
			if (enemy->JOB == WARRIOR)
			{
				if (dist(gen) == 1)
				{
					enemy->hp -= (player->skill[skillNum].skillDamage / 2); // ������ �нú� �ߵ��ϸ�
					player->hp += (player->skill[skillNum].skillDamage / 2);
					enemy->receiveDmg = (player->skill[skillNum].skillDamage / 2);
				}
				else
				{
					enemy->hp -= player->skill[skillNum].skillDamage;
					player->hp += player->skill[skillNum].skillDamage;
					enemy->receiveDmg = player->skill[skillNum].skillDamage;
				}
			}
			else
			{
				enemy->hp -= player->skill[skillNum].skillDamage;
				player->hp += player->skill[skillNum].skillDamage;
				enemy->receiveDmg = player->skill[skillNum].skillDamage;
			}
			player->skillCount++;
			break;
		default:
			break;
		}
	}
}

void player::UseItem(PLAYER* player, int itemNum)
{
	if (player->item[itemNum].effect == HEAL)
	{
		player->hp += player->item[itemNum].value;
		player->item[itemNum].itemcount--;
	}
	else if (player->item[itemNum].effect == STUN)
	{
		player->state = STUN;
		player->item[itemNum].itemcount--;
	}
}
