#include <stdio.h>
#include <random>

#include "Player.h"

void player::SetItem(ITEM* playeritem)
{
	ITEM item[2];
	item[0].itemName = "Hp포션";
	item[0].description = "HP를 40 회복한다.";
	item[0].effect = HEAL;
	item[0].value = 40;
	item[0].itemcount = 2;
	item[1].itemName = "짱돌";
	item[1].description = "적을 1턴 기절 시킨다.";
	item[1].effect = STUN;
	item[1].value = 1;
	item[1].itemcount = 1;

	playeritem[0] = item[0];
	playeritem[1] = item[1];
}

void player::SetPlayer(PLAYER* player)
{
	//직업 특성 할당
	player[KNIGHT].job_name = "KNIGHT";
	player[KNIGHT].JOB = KNIGHT;
	player[KNIGHT].isPlayer = false;
	player[KNIGHT].hp = 120;
	player[KNIGHT].atkDamage = 20;
	player[KNIGHT].state = NORMAL;
	player::SetItem(player[KNIGHT].item);
	player[KNIGHT].skill[0].skillName = "KNIGHTSkill1(패시브)";
	player[KNIGHT].skill[0].passiveProb = 20;
	player[KNIGHT].skill[0].description = "20%의 확률로 받는 데미지가 절반으로 줄어든다.";
	player[KNIGHT].skill[1].skillName = "KNIGHTSkill2";
	player[KNIGHT].skill[1].description = "전 턴에 상대에게 받은 피해만큼 공격력이 증가한다.(사용 가능 횟수 : 1)";
	player[KNIGHT].skill[1].passiveProb = 100;
	player[KNIGHT].poisonStack = 0;
	player[KNIGHT].skillCount = 0;

	player[ARCHER].job_name = "ARCHER";
	player[ARCHER].JOB = ARCHER;
	player[ARCHER].isPlayer = false;
	player[ARCHER].hp = 100;
	player[ARCHER].atkDamage = 0;
	player[ARCHER].state = NORMAL;
	player[ARCHER].maxPoisonStk = 20;
	player::SetItem(player[ARCHER].item);
	player[ARCHER].skill[0].skillName = "ARCHERSkill1";
	player[ARCHER].skill[0].passiveProb = 20;
	player[ARCHER].skill[0].description = "랜덤으로 적에게 독 스택을 추가한다.(범위 : 11 ~ 15)";
	player[ARCHER].skill[1].skillName = "ARCHERSkill2";
	player[ARCHER].skill[1].passiveProb = 20;
	player[ARCHER].skill[1].description = "현재 적의 독 스택 만큼 적에게 데미지를 주고 적의 독 스택을 초기화한다.";
	player[ARCHER].poisonStack = 0;
	player[ARCHER].skillCount = 0;

	player[BERSERKER].job_name = "BERSERKER";
	player[BERSERKER].JOB = BERSERKER;
	player[BERSERKER].isPlayer = false;
	player[BERSERKER].hp = 100;
	player[BERSERKER].atkDamage = 28;
	player[BERSERKER].state = NORMAL;
	player::SetItem(player[BERSERKER].item);
	player[BERSERKER].skill[0].skillName = "BERSERKERSkill1";
	player[BERSERKER].skill[0].passiveProb = 50;
	player[BERSERKER].skill[0].description = "50% 확률로 적의 공격력을 5 감소시키고 자신의 HP를 10 회복한다.";
	player[BERSERKER].skill[1].skillName = "흡혈";
	player[BERSERKER].skill[1].skillDamage = 20;
	player[BERSERKER].skill[1].passiveProb = 20;
	player[BERSERKER].skill[1].description = "적에게 데미지를 주고 준 데미지 만큼 HP를 회복한다.";
	player[BERSERKER].poisonStack = 0;
	player[BERSERKER].skillCount = 0;
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

	if (player->JOB == ARCHER)
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
	else if (enemy->JOB == KNIGHT)
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

	if (player->JOB == ARCHER)
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
	else if (player->JOB == KNIGHT)
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
	else if (player->JOB == BERSERKER)
	{
		switch (skillNum)
		{
		case 0:
			if (dist(gen) == 1)
			{
				player->hp += 10;
				if (enemy->JOB == ARCHER)
				{
					player->poisonStack -= 10;
				}
				else
					enemy->atkDamage -= 5;
			}
			player->skillCount++;
			break;
		case 1:
			if (enemy->JOB == KNIGHT)
			{
				if (dist(gen) == 1)
				{
					enemy->hp -= (player->skill[skillNum].skillDamage / 2); // 워리어 패시브 발동하면
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
