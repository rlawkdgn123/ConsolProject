
#include <stdio.h>
#include <random>

#include"player.h"

void SetJob()
{
	//직업 특성 할당
	warrior.JOB = player::WARRIOR;
	warrior.hp += 20;
	warrior.skill[0].skillName = "warriorSkill1";
	warrior.skill[0].passiveProb = 20;

	thief.JOB = player::THIEF;
	thief.atkDamage = 10;
	thief.skill[0].skillName = "thiefSkill1";
	thief.skill[0].passiveProb = 20;
	
	wizard.JOB = player::WIZARD;
	wizard.atkDamage += 6;
	wizard.skill[0].skillName = "wizardSkill1";
	wizard.skill[0].passiveProb = 50;
}

void SelectPlayer(int x, PLAYER* player)
{
	//커서 포지션 정해지면 수정할 것
	/*if (x == warrior.x)
	{
		player[WARRIOR].isPlayer = true;
	}
	else if (x == thief.x)
	{
		player[THIEF].isPlayer = true;
	}
	else if (x == wizard.x)
	{
		wizard.isPlayer = true;
	}*/
}

void UseAttack(PLAYER* player, PLAYER* enemy)
{
	std::random_device rd;

	unsigned long long seed = rd();

	std::mt19937 gen(seed);

	std::uniform_int_distribution<int> dist(1, (100/enemy->skill[0].passiveProb));

	if (player->JOB == player::THIEF)
	{
		enemy->hp -= player->atkDamage;
		enemy->poisonStack += 2;
	}
	else if (enemy->JOB == player::WARRIOR)
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

	if (player->JOB == player::THIEF)
	{
		enemy->poisonStack += dist(gen) + 10;
	}
	else if (player->JOB == player::WIZARD)
	{
		if (dist(gen) == 1)
		{
			enemy->hp -= 10;
			enemy->atkDamage -= 10;
		}
	}
	if(enemy->poisonStack > 0)
		enemy->hp -= enemy->poisonStack--;
}

void SetEnemy(PLAYER* job, PLAYER* player, PLAYER* enemy)
{
	for (int i = 0; i < JOB; i++)
	{

	}
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

void Update()
{
	
}

int main()
{
	static bool isPlayerTurn = false;
	static bool selectPlayer = false;
	PLAYER job[JOB];
	PLAYER player;
	PLAYER enemy[JOB-1];
	
	while (1)
	{
		if (!selectPlayer)
		{
			SetJob();
			SelectPlayer(x, job);
			SetEnemy();
		}

		Update();
	}

	return 0;
}