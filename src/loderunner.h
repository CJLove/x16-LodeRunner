#include <stdint.h>

// Tile definitions
#define TILE_BRICK  64
#define TILE_BLOCK  65
#define TILE_TRAP   66
#define TILE_LADDER 67
#define TILE_HIDDEN 68
#define TILE_ROPE   69
#define TILE_GOLD   70
#define TILE_HOLE   71
#define TILE_REGEN1 72
#define TILE_REGEN2 73
#define TILE_REGEN3 74
#define TILE_BLAST  75
#define TILE_GROUND 76

// Bank definitions
#define BANK_CLASSIC    1
#define BANK_FANBOOK    2

// Level definitions
#define LEVEL_COUNT 0xa000
#define LEVEL_BASE  0xa002
#define LEVEL_ROW_OFFSET 28
#define LEVEL_ROW_COUNT 16
#define LEVEL_OFFSET 448


// Declarations of all functions

// Loading resources from .BIN files - loader.c
extern int loadFiles();

// Screen configuration and tile set/get - screen.c
extern int screenConfig();
extern void setTile(unsigned char x, unsigned char y, unsigned char tile, unsigned char paletteOffset);
extern unsigned char getTile(unsigned char x, unsigned char y);
extern unsigned char getTileXY (uint16_t x, uint16_t y);

// Level map utilities - levels.c
extern int displayLevel(unsigned char bank, unsigned char level);
extern void dumpLevel(unsigned char bank, unsigned char level, unsigned char row);
extern void completeLevel();

