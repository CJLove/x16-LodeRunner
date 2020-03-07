#include "loderunner.h"
#include "levels.h"
#include "key.h"
#include <cx16.h>
#include <cbm.h>
#include <conio.h>
#include <stdio.h>

struct splashWorld_t {
    uint8_t y;
    uint8_t world;
};

static struct splashWorld_t splashWorlds[] = {
    { 17, WORLD_CLASSIC },
    { 19, WORLD_CHAMP },
    { 21, WORLD_PRO },
    { 23, WORLD_REVENGE },
    { 25, WORLD_FANBOOK },
    { 27, WORLD_CUSTOM }
};
#define MAX_WORLDS 6

static void displaySplash()
{
    uint8_t x = 0;
    uint8_t y = 0;
    uint8_t *tilemap = (uint8_t*)0xa000;
    // Bank 21 for splash tilemap
    VIA1.pra = 21;

    for (x = 0; x < 30; x++) {
        for (y = 0; y < 30; y++) {
            uint8_t tile = tilemap[x + y * 30];
            setTile(x+5,y,tile,0);

            if (x < 5) {
                setTile(x,y,0,0);
            }
        }
    }
}

static void clearSplash()
{
    uint8_t x = 0;
    uint8_t y = 0;
    for (x = 0; x < 30; x++) {
        for (y = 0; y < 30; y++) {
            setTile(x+5,y,0,0);
        }
    }       
}

static void clearKeyboardBuffer()
{
    // Bank 0
    VIA1.pra = 0;
    {
        uint8_t *keyCount = (uint8_t*)0xa00b;
        *keyCount = 0;
    }
}

// Character codes returned by cgetc() for up arrow, 
// down arrow and enter keys
#define CHAR_UP 145
#define CHAR_DOWN 17
#define CHAR_ENTER 13

void splash()
{

    int8_t act = ACT_UNKNOWN;
    uint8_t worldIdx = 0;

    displaySplash();

    clearKeyboardBuffer();

    // Default to WORLD_CLASSIC
    currentGame.world = WORLD_CLASSIC;
    // Default to 5 lives
    currentGame.lives = 5;
    setTile(3,17,28,0);

    while (1) {
        waitvsync();

        if (kbhit()) {
            uint8_t c = cgetc();

            switch (c) {
                case CHAR_UP:
                    setTile(3,splashWorlds[worldIdx].y,0,0);
                    if (worldIdx == 0) 
                        worldIdx = 5;
                    else
                        worldIdx--;
                    setTile(3,splashWorlds[worldIdx].y,28,0);
                    currentGame.world = splashWorlds[worldIdx].world;
                    break;
                case CHAR_DOWN:
                    setTile(3,splashWorlds[worldIdx].y,0,0);
                    if (worldIdx == 5) 
                        worldIdx = 0;
                    else
                        worldIdx++;
                    setTile(3,splashWorlds[worldIdx].y,28,0);
                    currentGame.world = splashWorlds[worldIdx].world;
                    break;
                case CHAR_ENTER:
                    currentGame.gameState = GAME_NEW_LEVEL;
                    setTile(3,splashWorlds[worldIdx].y,0,0);
                    clearSplash();
                    return;
                default:
                    break;
            }
        }
    }
}