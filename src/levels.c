#include <cx16.h>
#include <stdio.h>
#include "loderunner.h"


static const unsigned char labelScore[] = {
    19, 3, 15, 18, 5
};
static const unsigned char labelMen[] = {
    13, 5, 14
};
static const unsigned char labelLevel[] = {
    12, 5, 22, 5, 12
};

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

void dumpLevel(unsigned char bank, unsigned char level)
{
    uint16_t max = 0;
    bank = VIA1.pra;
    max = *(uint16_t*)LEVEL_COUNT;
//    printf("Max levels = %u\n",max);
    printf("\nBank: %d Level: %d\n",bank,level);
    level--; // zero-based indexing for level data
    if (level < max) {
        unsigned char *levelPtr = (unsigned char*)(LEVEL_BASE + (LEVEL_OFFSET * level));
        unsigned char *endPtr = (unsigned char*)0xc000;

        unsigned char col = 0;
        unsigned char row = 0;
        for (row = 0; row < LEVEL_ROW_COUNT; row++) {
            for (col = 0; col < LEVEL_ROW_OFFSET; col++) {
                unsigned char tile = 0;
                if (levelPtr + row*LEVEL_ROW_OFFSET + col == endPtr) {
                    bank++;
//                    printf("***Bank=%d\n",bank);

                    VIA1.pra = bank;
                    levelPtr = (unsigned char*)0xa000;
                }
                tile = *(levelPtr + row*LEVEL_ROW_OFFSET + col);
                printf("%c",tile);
            }
            printf("\n");
        }
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
                setTile(row,col,TILE_LADDER,0);
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
        unsigned char *endPtr = (unsigned char*)0xc000;
        unsigned char row = 0;
        unsigned char col = 0;
        unsigned char tile = 0;

        for (row = 0; row < LEVEL_ROW_COUNT; row++) {
            for (col = 0; col < LEVEL_ROW_OFFSET; col++) {
                if (levelPtr + row*LEVEL_ROW_OFFSET + col == endPtr) {
                    bank++;
                    VIA1.pra = bank;
                    levelPtr = (unsigned char*)0xa000;
                }
                tile = *(levelPtr + row*LEVEL_ROW_OFFSET + col);

                // TODO: Treat tile values marking guard or runner as blanks in the actual tile map
                if (tile == TILE_GUARD) {
                    setTile(col,row,tile,0);
                } else {
                    setTile(col,row,tile,0);
                }
            }
        }

        // Display the ground (17th row)
        for (col = 0; col < LEVEL_ROW_OFFSET; col++) {
            setTile(col,LEVEL_ROW_COUNT,TILE_GROUND,0);
        }
        displayStatus(0,0,level);

        return 1;
    }
    // Invalid level for this bank
    return 0;
}