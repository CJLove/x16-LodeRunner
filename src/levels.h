//
// Constants and info regarding levels
//
#include <stdint.h>

// World definitions - starting bank number for each world
#define WORLD_CLASSIC   1
#define WORLD_CHAMP     6
#define WORLD_PRO       8
#define WORLD_FANBOOK   13
#define WORLD_REVENGE   15
#define WORLD_CUSTOM    16

#define WORLD_IDX_CLASSIC     0
#define WORLD_IDX_CHAMP       1
#define WORLD_IDX_PRO         2
#define WORLD_IDX_FANBOOK     3
#define WORLD_IDX_REVENGE     4
#define WORLD_IDX_CUSTOM      5
#define MAX_WORLD       6

#define NO_OF_TILES_X   28
#define NO_OF_TILES_Y   16

#define MAX_TILE_X  27
#define MAX_TILE_Y  15

// Tile dimensions
#define TILE_H  8
#define TILE_W  8

// Half tile dimensions
#define W2  4
#define H2  4

// Quarter tile dimensions
#define W4  2
#define H4  2

// Struct defining each tile of the map
struct map_t {
    // Default tile value as stored in banked RAM
    uint8_t base;
    // Current tile value based on game play
    uint8_t act;
};

// Struct defining hole being dug by the runner
struct dig_t {
    // Is hole being dug?
    uint8_t action;
    // x/y location of hole
    uint8_t x;
    uint8_t y;
    // Count of times dig has been checked from processDigHole()
    uint8_t idx;
};

// Struct defining each hole dug by the runner
struct hole_t {
    // Is hole "active"
    uint8_t active;
    // x/y location of hole 
    uint8_t x;
    uint8_t y;

    uint8_t pad;
    // Count of times hole has been checked from processFillHole()
    uint16_t count;
};
#define MAX_HOLES 30

// Struct defining state for the current level
struct level_t {
    // Count of gold pieces remaining
    uint8_t goldCount;
    // Flag that gold collection is complete
    uint8_t goldComplete;
};

// Struct defining state for the current game
struct game_t {
    // Score
    uint32_t currentScore;
    // Game state
    uint8_t gameState;
    // Lives
    uint8_t lives;
    // World
    uint8_t world;
    // Level
    uint8_t level;
    // God mode indicator
    uint8_t godMode;
    // Speed selection
    uint8_t speed;
    // Sound selection
    uint8_t sound;
    // Max level info
    uint8_t maxLevels[MAX_WORLD];
};

#define SPEED_SLOW    0
#define SPEED_MEDIUM  1
#define SPEED_FAST    2
#define SPEED_OPTIONS 3
#define SPEED_TICKS   6

// Sound selection
#define SOUND_ON 1
#define SOUND_OFF 2

// Level map utilities - levels.c
extern void initLevels();
extern int loadLevel(uint8_t bank, uint8_t level);
extern int displayLevel(uint8_t level);
extern void dumpLevel(uint8_t level);
extern void completeLevel(void);
extern void displayScore(uint32_t addScore);
extern void displayLives(void);

// In-memory map for game play on current level
extern struct map_t map[NO_OF_TILES_X][NO_OF_TILES_Y];

extern struct dig_t hole;

extern struct hole_t holes[MAX_HOLES];

// State information for the current level
extern struct level_t currentLevel;

extern struct game_t currentGame;

extern void gameOver(void);

extern void worldComplete(void);
