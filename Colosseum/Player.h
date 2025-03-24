#pragma once
#include <stdio.h>

constexpr int JOB = 3; // 직업 수
constexpr int WARRIOR = 0;
constexpr int THIEF = 1;
constexpr int WIZARD = 2;

typedef struct  {
	const char* skillName;
	int skillDamage;
	int passiveProb; //발동 확률
	//skillEffect;
}SKILL;

typedef struct  {
	int JOB;
	int hp;
	int atkDamage;
	//int skillCount;
	int poisonStack;
	//int state;
	bool isPlayer;
	SKILL skill[2];
}PLAYER;

void SetJob(PLAYER* player);
void SelectPlayer(int x, PLAYER* player);
void UseAttack(PLAYER* player, PLAYER* enemy);
void UseSkill(PLAYER* player, PLAYER* enemy);
void EnemyDied(PLAYER* player);