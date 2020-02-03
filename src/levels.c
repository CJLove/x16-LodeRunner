#include <cx16.h>
#include <stdio.h>
#include "loderunner.h"


static const unsigned char status[] = {
    19, 3, 15, 18, 5, 48, 48, 48, 48, 48, 48, 48, 32, 13, 5, 14, 48, 48, 48, 
    32, 12, 5, 22, 5, 12, 48, 48, 48
};

void dumpLevel(unsigned char bank, unsigned char level, unsigned char row)
{
    uint16_t max = 0;
    VIA1.pra = bank;

    max = *(uint16_t*)LEVEL_COUNT;
    printf("Max levels = %d\n",max);

    level--; // zero-based indexing for level data
    if (level < max) {
        unsigned char *levelPtr = (unsigned char*)(LEVEL_BASE + (LEVEL_OFFSET * level));
        unsigned char col = 0;

        for (col = 0; col < LEVEL_ROW_OFFSET; col++) {
            unsigned char tile = *(unsigned char*)(levelPtr + row*LEVEL_ROW_OFFSET + col);

            if ((col % 7) == 0) {
                printf("\n");
            }

            printf("Col%2d = %2d ",col,tile);


        }
        printf("\n");
    }
}

void completeLevel()
{
    unsigned char row = 0;
    unsigned char col = 0;

    for (row = 0; row < LEVEL_ROW_COUNT; row++) {
        for (col = 0; col < LEVEL_ROW_OFFSET; col++) {
            unsigned char tile = getTile(row,col);
            if (tile == TILE_HIDDEN) {
                tile = TILE_LADDER;
                setTile(row,col,tile,0);
            }
        }
    }
}

int displayLevel(unsigned char bank, unsigned char level)
{
    uint16_t max = 0;
    // Switch to bank corresponding to the desired set of levels
    VIA1.pra = bank;
    
    max = *(uint16_t*)LEVEL_COUNT;

    level--; // zero-based indexing for level data
    if (level < max) {
        unsigned char *levelPtr = (unsigned char*)(LEVEL_BASE + (LEVEL_OFFSET * level));
        unsigned char row = 0;
        unsigned char col = 0;

        for (row = 0; row < LEVEL_ROW_COUNT; row++) {
            for (col = 0; col < LEVEL_ROW_OFFSET; col++) {
                unsigned char tile = *(levelPtr + row*LEVEL_ROW_OFFSET + col);

                // TODO: Treat tile values marking guard or runner as blanks in the actual tile map
                setTile(col,row,tile,0);
            }
        }

        // Display the ground (17th row)
        for (col = 0; col < LEVEL_ROW_OFFSET; col++) {
            setTile(col,LEVEL_ROW_COUNT,TILE_GROUND,0);
        }

        // Display status row (18th row)
        for (col = 0; col < LEVEL_ROW_OFFSET; col++) {
            setTile(col,LEVEL_ROW_COUNT+1,status[col],0);
        }
        return 1;
    }
    // Invalid level for this bank
    return 0;
}