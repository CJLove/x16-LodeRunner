#include <stdint.h>

// World definitions
#define WORLD_CLASSIC   1
#define WORLD_CHAMP     6
#define WORLD_PRO       8
#define WORLD_FANBOOK   13
#define WORLD_REVENGE   15
#define WORLD_CUSTOM    16

// Tile definitions
#define TILE_BRICK  64
#define TILE_BLOCK  65
#define TILE_TRAP   66
#define TILE_LADDER 67
#define TILE_HIDDEN 68
#define TILE_ROPE   69
#define TILE_GOLD   70
#define TILE_GROUND 71
#define TILE_DIG_LEFT_U1 72
#define TILE_DIG_LEFT_L1 73
#define TILE_DIG_LEFT_U2 74
#define TILE_DIG_LEFT_L2 75
#define TILE_DIG_LEFT_U3 76
#define TILE_DIG_LEFT_L3 77
#define TILE_DIG_LEFT_U4 78
#define TILE_DIG_LEFT_L4 79
#define TILE_DIG_LEFT_U5 80
#define TILE_DIG_LEFT_L5 81
#define TILE_DIG_LEFT_U6 82
#define TILE_DIG_LEFT_L6 83

#define TILE_DIG_RIGHT_U1 84
#define TILE_DIG_RIGHT_L1 85
#define TILE_DIG_RIGHT_U2 86
#define TILE_DIG_RIGHT_L2 87
#define TILE_DIG_RIGHT_U3 88
#define TILE_DIG_RIGHT_L3 89
#define TILE_DIG_RIGHT_U4 90
#define TILE_DIG_RIGHT_L4 91
#define TILE_DIG_RIGHT_U5 92
#define TILE_DIG_RIGHT_L5 93
#define TILE_DIG_RIGHT_U6 94
#define TILE_DIG_RIGHT_L6 95
#define TILE_REGEN1 96
#define TILE_REGEN2 97
#define TILE_REGEN3 98

#define TILE_BLANK  32
#define TILE_GUARD  36
#define TILE_RUNNER 37

// Bank definitions
#define BANK_CLASSIC    1
#define BANK_FANBOOK    2

// Level definitions
#define LEVEL_COUNT 0xa000
#define LEVEL_BASE  0xa002
#define LEVEL_ROW_OFFSET 28
#define LEVEL_ROW_BANK_OFFSET 14
#define LEVEL_ROW_COUNT 16
#define LEVEL_OFFSET 224

// Sprite images
#define RUNNER_1        0x11e000
#define RUNNER_2        0x11e020
#define RUNNER_3        0x11e040
#define RUNNER_CLIMB_1  0x11e060
#define RUNNER_CLIMB_2  0x11e080
#define RUNNER_FALLING  0x11e0a0
#define RUNNER_RAPPEL_1 0x11e0c0
#define RUNNER_RAPPEL_2 0x11e0e0
#define RUNNER_RAPPEL_3 0x11e100
#define GUARD_1         0x11e120
#define GUARD_2         0x11e140
#define GUARD_3         0x11e160
#define GUARD_CLIMB_1   0x11e180
#define GUARD_CLIMB_2   0x11e1a0
#define GUARD_FALLING   0x11e1c0
#define GUARD_RAPPEL_1  0x11e1e0
#define GUARD_RAPPEL_2  0x11e200
#define GUARD_RAPPEL_3  0x11e220

// Actions
#define ACT_UNKNOWN  -1
#define ACT_STOP    0
#define ACT_LEFT    1
#define ACT_RIGHT   2
#define ACT_UP      3
#define ACT_DOWN    4
#define ACT_FALL    5
#define ACT_FALL_BAR    6
#define ACT_DIG_LEFT    7
#define ACT_DIG_RIGHT   8
#define ACT_DIGGING     9
#define ACT_IN_HOLE     10
#define ACT_CLIMB_OUT   11
#define ACT_REBORN      12

// Score increments
#define SCORE_GET_GOLD      250
#define SCORE_IN_HOLE       75
#define SCORE_GUARD_DEAD    75
#define SCORE_COMPLETE_LEVEL    1500

// Hole regeneration timing
#define HOLE_REGEN1     256
#define HOLE_REGEN2     264
#define HOLE_REGEN3     272
#define HOLE_REGEN4     284

// Game states
#define GAME_RUNNING 1
#define GAME_NEW_LEVEL 2
#define GAME_NEXT_LEVEL 3
#define GAME_PREV_LEVEL 4
#define GAME_RUNNER_DEAD 5
#define GAME_FINISH 6
#define GAME_WIN 7
#define GAME_OVER 8

// Declarations of all functions

// Loading resources from .BIN files - loader.c
extern int loadFiles();

// Screen configuration and tile set/get - screen.c
extern int screenConfig();
extern void screenReset();
extern void setTile(uint8_t x, uint8_t y, uint8_t tile, uint8_t paletteOffset);
extern uint8_t getTile(uint8_t x, uint8_t y);
extern uint8_t getTileXY (uint16_t x, uint16_t y);
extern uint8_t getTileBelowXY (uint16_t x, uint16_t y);

// Level map utilities - levels.c
extern int loadLevel(uint8_t bank, uint8_t level);
extern int displayLevel(uint8_t level);
extern void dumpLevel(uint8_t level);
extern void completeLevel();
extern void displayScore(uint32_t addScore);

