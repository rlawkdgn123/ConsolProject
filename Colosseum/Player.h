#pragma once
#include <stdio.h>
namespace player {
	constexpr int WARRIOR = 0;
	constexpr int THIEF = 1;
	constexpr int WIZARD = 2;
	namespace state {

	}
}

struct SKILL {
	const char* skillName;
	int skillDamage = 0;
	int passiveProb = 0; //¹ßµ¿ È®·ü
	//skillEffect;
};

struct PLAYER {
	int JOB;
	int posX;
	int posY;
	int hp = 100;
	int atkDamage = 20;
	int skillCount = 0;
	int poisonStack = 0;
	int state;
	bool isPlayer = false;
	SKILL skill[2];
};

PLAYER warrior;
PLAYER thief;
PLAYER wizard;

bool isPlayerTurn;