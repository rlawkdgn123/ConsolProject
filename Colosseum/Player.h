#pragma once

typedef struct {
	char* skillName;
	int skillDamage;
	//skillEffect;
}SKILL;

typedef struct {
	int posX;
	int posY;
	int hp;
	int atkDamage;
	int skillCount = 0;
	SKILL skill[3];
}PLAYER;