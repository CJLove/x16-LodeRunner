#include <stdio.h>
#include <cx16.h>
#include <conio.h>
#include <unistd.h>
#include "loderunner.h"

// Test application which displays all tiles

// Animation sequences for digging/filling holes to the left and right
uint8_t upperLeft[] = { TILE_BLANK, TILE_DIG_LEFT_U1, TILE_DIG_LEFT_U2, TILE_DIG_LEFT_U3, TILE_DIG_LEFT_U4,TILE_DIG_LEFT_U5, TILE_DIG_LEFT_U6, TILE_BLANK, TILE_BLANK, TILE_BLANK, TILE_BLANK };
uint8_t lowerLeft[] = { TILE_BRICK, TILE_DIG_LEFT_L1, TILE_DIG_LEFT_L2, TILE_DIG_LEFT_L3, TILE_DIG_LEFT_L4, TILE_DIG_LEFT_L5, TILE_DIG_LEFT_L6, TILE_BLANK, TILE_REGEN1, TILE_REGEN2, TILE_REGEN3 };

uint8_t upperRight[] = { TILE_BLANK, TILE_DIG_RIGHT_U1, TILE_DIG_RIGHT_U2, TILE_DIG_RIGHT_U3, TILE_DIG_RIGHT_U4, TILE_DIG_RIGHT_U5, TILE_DIG_RIGHT_U6, TILE_BLANK, TILE_BLANK, TILE_BLANK, TILE_BLANK };
uint8_t lowerRight[] = { TILE_BRICK, TILE_DIG_RIGHT_L1, TILE_DIG_RIGHT_L2, TILE_DIG_RIGHT_L3, TILE_DIG_RIGHT_L4, TILE_DIG_RIGHT_L5, TILE_DIG_RIGHT_L6, TILE_BLANK, TILE_REGEN1, TILE_REGEN2, TILE_REGEN3 };

// Tiles for "GAME OVER"
static const uint8_t gameOverTiles[3][11] = {
    { 101, 100, 100, 100, 100, 100, 100, 100, 100, 100, 102 },
    { 99, 7, 1, 13, 5, 32, 15, 22, 5, 18, 99 },
    { 103, 100, 100, 100, 100, 100, 100, 100, 100, 100, 104 }
};

int main()
{
    int result = 0;
    uint8_t idx = 0;
    uint8_t tile = 0;
    uint8_t i = 0;
    printf("loading resources...\n");
    
    result = loadFiles();

    if (result) {
        printf("loaded resources successfully\n");
    } else {
        printf("failed to load all resources\n");
        return result;
    }

    screenConfig();

    // Alphabetic characters
    for (tile = 0; tile < 10; tile++) {
        setTile(2*tile + 2,3,tile+1,0);
        setTile(2*tile + 2,5,tile+10,0);
        if (tile < 7) {
            setTile(2*tile + 2, 7,tile+20,0);
        }
    }

    // Numeric characters
    for (tile = 0; tile < 10; tile++) {
        setTile(2*tile+2,9,tile+48,0);
    }

    // Level tiles
    for (tile = 0; tile < 8; tile++) {
        setTile(2*tile+2,11,tile+64,0);
    }

    for (i = 0; i < 11; i++) {
        setTile(10+i,16,gameOverTiles[0][i],0);
        setTile(10+i,17,gameOverTiles[1][i],0);
        setTile(10+i,18,gameOverTiles[2][i],0);
    }

    // Test tile animation for hole digging/filling
    setTile(20,13,TILE_BLANK,0);
    setTile(22,13,TILE_BLANK,0);
    setTile(21,13,TILE_RUNNER,0);
    for (tile = 0; tile < 5; tile++)
        setTile(19+tile,14,TILE_BRICK,0);    

    do {
        if (kbhit()) {
            char c = cgetc();

            idx++;
            if (idx == 11) idx = 0;

            setTile(20,13,upperLeft[idx],0);
            setTile(20,14,lowerLeft[idx],0);

            setTile(22,13,upperRight[idx],0);
            setTile(22,14,lowerRight[idx],0);
        }
    } while (1);

    return result;
}