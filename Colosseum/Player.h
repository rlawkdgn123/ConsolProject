#pragma once
#include <stdio.h>

constexpr int JOBCOUNT = 3; // 직업 수
constexpr int KNIGHT = 0;
constexpr int ARCHER = 1;
constexpr int BERSERKER = 2;

constexpr int NORMAL = 0;
constexpr int HEAL = 1;
constexpr int STUN = 2;

typedef struct  {
	const char* skillName;
	const char* description;
	int skillDamage;
	int passiveProb; //발동 확률
	//skillEffect;
}SKILL;

typedef struct {
	const char* itemName;
	const char* description;
	int effect;
	int value;
	int itemcount;
}ITEM;

typedef struct  {
	const char* job_name;
	int JOB;
	int hp;
	int atkDamage;
	int skillCount;
	int poisonStack;
	int maxPoisonStk;
	int state;
	int receiveDmg;
	bool isPlayer;
	bool isDied;
	SKILL skill[2];
	ITEM item[2];
}PLAYER;

namespace player {
	void SetItem(ITEM* item);
	void SetPlayer(PLAYER* player);
	void SetEnemy(PLAYER* job, PLAYER* enemy);
	void UseAttack(PLAYER* player, PLAYER* enemy);
	void UseSkill(PLAYER* player, PLAYER* enemy, int skillNum);
	void UseItem(PLAYER* player, int itemNum);
}