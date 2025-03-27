#pragma once
#define MAP_CNT_LMT 10
#define MAP_WIDTH 200
#define MAP_PWIDTH 198
#define MAP_PHEIGHT 48

#define SCREEN_BIAS 30
#define SCREEN_WIDTH 200
#define SCREEN_HEIGHT 50

#define SOUND_LIMIT 10

enum STAGE {
    TITLE = 0,
    HEROCHOICE = 1,
    MAIN = 2,
    BATTLE = 3,
    BATTLE_SKILL = 4,
    BATTLE_ITEM = 5,
    BATTLE_STATE = 6,
    BATTLE_END = 7,
    END_CLEAR = 8,
    END_GAMEOVER = 9,
    HOWTOPLAY = 10,
    GAMEINFO = 11
};

enum CHOICE {
    POS1 = 10,
    POS2 = 51,
    POS3 = 91,
    POS4 = 131,
    DEF_Y = 41,
    PLAYER_HP_POSX = 5,
    PLAYER_HP_POSY = 3,
    ENEMY_HP_POSX = 195,
    ENEMY_HP_POSY = 3,
};