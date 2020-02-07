#include <cx16.h>
#include <stdio.h>
#include "loderunner.h"

// The tileMap[] table maps a nibble (0-15) from
// banked memory to the tile to be used in VRAM.
// This mapping matches what is done in levels.py
// when building the .BIN file for each world
static const uint8_t tileMap[] = {
    TILE_BLANK,
    TILE_GUARD,
    TILE_RUNNER,
    TILE_BRICK,
    TILE_BLOCK,
    TILE_TRAP,
    TILE_LADDER,
    TILE_HIDDEN,
    TILE_ROPE,
    TILE_GOLD,
    TILE_BLANK, // Placeholder
    TILE_BLANK, // Placeholder
    TILE_BLANK, // Placeholder
    TILE_BLANK, // Placeholder
    TILE_BLANK, // Placeholder
    TILE_BLANK  // Placeholder
};

static const uint8_t labelScore[] = {
    19, 3, 15, 18, 5
};
static const uint8_t labelMen[] = {
    13, 5, 14
};
static const uint8_t labelLevel[] = {
    12, 5, 22, 5, 12
};

void completeLevel()
{
    uint8_t row = 0;
    uint8_t col = 0;

    for (row = 0; row < LEVEL_ROW_COUNT; row++) {
        for (col = 0; col < LEVEL_ROW_OFFSET; col++) {
            uint8_t tile = getTile(row,col);
            if (tile == TILE_HIDDEN) {
                setTile(row,col,TILE_LADDER,0);
            }
        }
    }
}

void displayScore(uint32_t score) {
    uint8_t col = 0;
    char buffer[10];
    snprintf(buffer,10,"%07d",score);
    for (col = 0; col < 7; col++) {
        setTile(col+5,LEVEL_ROW_COUNT+1,buffer[col],0);
    }
}

void displayStatus(uint32_t score, uint8_t men, uint8_t level) {
    uint8_t col = 0;
    char buffer[10];
    // Display status row (18th row)

    // SCORE
    for (col = 0; col < 5; col++) {
        setTile(col,LEVEL_ROW_COUNT+1,labelScore[col],0);
    }
    displayScore(score);

    // MEN
    for (col = 0; col < 3; col++) {
        setTile(col+13,LEVEL_ROW_COUNT+1,labelMen[col],0);
    }
    snprintf(buffer,10,"%03d",(int)men);
    for (col = 0; col < 3; col++) {
        setTile(col+16,LEVEL_ROW_COUNT+1,buffer[col],0);
    }
    // LEVEL
    for (col = 0; col < 5; col++) {
        setTile(col+20,LEVEL_ROW_COUNT+1,labelLevel[col],0);
    }
    snprintf(buffer,10,"%03d",(int)level+1);
    for (col = 0; col < 3; col++) {
        setTile(col+25,LEVEL_ROW_COUNT+1,buffer[col],0);
    }
}

int displayLevel(uint8_t world, uint8_t level)
{
    uint8_t max = 0;
    // Switch to bank corresponding to the desired world and level
    uint8_t bank = world + (level / 36);

    VIA1.pra = bank;
    
    max = *(uint8_t*)(LEVEL_COUNT+1);

    level--; // zero-based indexing for level data
    if (level < max) {
        uint8_t *levelPtr = (uint8_t*)(LEVEL_BASE + (LEVEL_OFFSET * (level % 36)));
        uint8_t row = 0;
        uint8_t idx = 0;

        for (row = 0; row < LEVEL_ROW_COUNT; row++) {
            for (idx = 0; idx < LEVEL_ROW_BANK_OFFSET; idx++) {
                uint8_t data = *(levelPtr + row*LEVEL_ROW_BANK_OFFSET + idx);
                uint8_t tile1 = tileMap[(data & 0xf0) >> 4];
                uint8_t tile2 = tileMap[(data & 0xf)];

                // TODO: Treat tile values marking guard or runner as blanks in the actual tile map
                if (tile1 == TILE_GUARD) {
                    setTile(idx*2,row,tile1,0);
                } else {
                    setTile(idx*2,row,tile1,0);
                }
                if (tile2 == TILE_GUARD) {
                    setTile(idx*2+1,row,tile2,0);
                } else {
                    setTile(idx*2+1,row,tile2,0);
                }
            }
        }

        // Display the ground (17th row)
        for (idx = 0; idx < LEVEL_ROW_OFFSET; idx++) {
            setTile(idx,LEVEL_ROW_COUNT,TILE_GROUND,0);
        }
        displayStatus(0,0,level);

        return 1;
    }

    VIA1.pra = 0;

    // Invalid level for this bank
    return 0;
}

#if defined(TEXT_BROWSE)
void dumpLevel(uint8_t world, uint8_t level)
{
    uint8_t max = 0;
    uint8_t bank = world + (level / 36);
    bank = VIA1.pra;
    max = *(uint8_t*)(LEVEL_COUNT+1);
    printf("Max levels = %u\n",max);
    printf("Level=%d\n",level);
    level--; // zero-based indexing for level data
    if (level < max) {
        uint8_t *levelPtr = (uint8_t*)(LEVEL_BASE + (LEVEL_OFFSET * (level % 36)));

        uint8_t idx = 0;
        uint8_t row = 0;
        for (row = 0; row < LEVEL_ROW_COUNT; row++) {
            for (idx = 0; idx < LEVEL_ROW_BANK_OFFSET; idx++) {
                uint8_t data = *(levelPtr + row*LEVEL_ROW_BANK_OFFSET + idx);
                uint8_t tile1 = tileMap[(data & 0xf0) >> 4];
                uint8_t tile2 = tileMap[data & 0xf];
                printf("%c%c",tile1,tile2);
            }
            printf("\n");
        }
    }
    printf("-------\n");
}
#endif