#pragma once
#include <stdio.h>

constexpr int JOBCOUNT = 3; // 직업 수
constexpr int WARRIOR = 0;
constexpr int THIEF = 1;
constexpr int WIZARD = 2;

constexpr int NORMAL = 0;
constexpr int HEAL = 1;
constexpr int STUN = 2;

typedef struct  {
	const char* skillName;
	int skillDamage;
	int passiveProb; //발동 확률
	//skillEffect;
}SKILL;

typedef struct {
	const char* itemName;
	int effect;
	int num;
	int itemcount;
	//skillEffect;
}ITEM;

typedef struct  {
	int JOB;
	int hp;
	int atkDamage;
	int skillCount;
	int poisonStack;
	int state;
	bool isPlayer;
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
	void EnemyDied(PLAYER* player);
}