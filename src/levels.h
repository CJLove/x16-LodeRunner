//
// Constants and info regarding levels
//
#include <stdint.h>

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
};

// In-memory map for game play on current level
extern struct map_t map[NO_OF_TILES_X][NO_OF_TILES_Y];

extern struct dig_t hole;

extern struct hole_t holes[MAX_HOLES];

// State information for the current level
extern struct level_t currentLevel;

extern struct game_t currentGame;

extern void gameOver(void);

extern void worldComplete(void);
