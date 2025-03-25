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
    END = 4,
    HOWTOPLAY = 5,
    GAMEINFO = 6
};

enum CHOICE {
    POS1 = 30,
    POS2 = 60,
    POS3 = 90
};