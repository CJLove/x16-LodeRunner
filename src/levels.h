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

// In-memory map for game play on current level
extern struct map_t map[NO_OF_TILES_X][NO_OF_TILES_Y];

extern uint8_t goldCount;

extern uint32_t currentScore;
