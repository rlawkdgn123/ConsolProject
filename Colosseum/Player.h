#pragma once
#include <stdio.h>
namespace player {
	constexpr int WARRIOR = 0;
	constexpr int THIEF = 1;
	constexpr int WIZARD = 2;
	namespace state {

	}
}

typedef struct  {
	const char* skillName;
	int skillDamage;
	int passiveProb; //¹ßµ¿ È®·ü
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

void SetPlayer(PLAYER* warrior, PLAYER* thief, PLAYER* wizard);
void UseAttack(PLAYER* player, PLAYER* enemy);
void UseSkill(PLAYER* player, PLAYER* enemy);
void EnemyDied(PLAYER* player);