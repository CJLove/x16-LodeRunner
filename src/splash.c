#include "key.h"
#include "levels.h"
#include "loderunner.h"
#include "bank_control.h"
#include <cbm.h>
#include <conio.h>
#include <cx16.h>
#include <stdio.h>

struct splashWorld_t {
    uint8_t y;
    uint8_t world;
};

static struct splashWorld_t splashWorlds[] = {{16, WORLD_CLASSIC}, {18, WORLD_CHAMP},   {20, WORLD_PRO},
                                              {22, WORLD_REVENGE}, {24, WORLD_FANBOOK}, {26, WORLD_CUSTOM}};
#define MAX_WORLDS 6

struct splashSpeed_t {
    uint8_t y;
    uint8_t speed;
};

static struct splashSpeed_t speedOptions[] = {{16, SPEED_SLOW}, {17, SPEED_MEDIUM}, {18, SPEED_FAST}};

struct splashSound_t {
    uint8_t y;
    uint8_t sound;
};

static struct splashSound_t soundOptions[] = {{22, SOUND_ON}, {23, SOUND_OFF}};

static void displaySplash()
{
    uint8_t x = 0;
    uint8_t y = 0;
    uint8_t *tilemap = (uint8_t *)0xa000;
    // Bank 21 for splash tilemap
    BANK_CTRL.ram_bank = 21;
    setTileOffsets(0,0);

    for (x = 0; x < 40; x++) {
        for (y = 0; y < 30; y++) {
            uint8_t tile = tilemap[x + y * 40];
            setTile(x, y, tile, 0);
        }
    }
}

static void clearSplash()
{
    uint8_t x = 0;
    uint8_t y = 0;
    for (x = 0; x < 40; x++) {
        for (y = 0; y < 30; y++) {
            setTile(x, y, 0, 0);
        }
    }
}

static void clearKeyboardBuffer()
{
    // Hack to clear the keyboard buffer by setting the KEY_COUNT
    // memory location in Bank 0 to zero.  Revisit the memory location
    // for each rom version.

    // Switch to Bank 0
    BANK_CTRL.ram_bank = 0;
    {
        uint8_t *keyCount = (uint8_t *)0xa00a;
        *keyCount = 0;
    }
}

// Character codes returned by cgetc() for up arrow,
// down arrow and enter keys
#define CHAR_UP 145
#define CHAR_DOWN 17
#define CHAR_ENTER 13
#define CHAR_LT 60
#define CHAR_GT 62
#define CHAR_LEFT 157
#define CHAR_RIGHT 29

void updateLevel(uint8_t level)
{
    char buf[4];
    uint8_t i = 0;
    sprintf(buf, "%03d", level);
    for (i = 0; i < 3; i++) {
        setTile(29 + i, 26, buf[i], 0);
    }
}

void splash()
{

    int8_t act = ACT_UNKNOWN;
    uint8_t worldIdx = 0;
    uint8_t speedIdx = 2;
    uint8_t soundIdx = 0;
    uint8_t level = 1;

    displaySplash();

    clearKeyboardBuffer();

    // Default to WORLD_CLASSIC
    currentGame.world = WORLD_CLASSIC;
    // Default to 5 lives
    currentGame.lives = 5;
    // Default to fast speed
    currentGame.speed = SPEED_FAST;
    // Default to sound on
    currentGame.sound = SOUND_ON;

    setTile(1, splashWorlds[worldIdx].y, 28, 0);
    setTile(28, speedOptions[speedIdx].y, 28, 0);
    setTile(28, soundOptions[soundIdx].y, 28, 0);

    while (1) {
        waitvsync();

        if (kbhit()) {
            uint8_t c = cgetc();

            switch (c) {
                case CHAR_UP:
                    setTile(1, splashWorlds[worldIdx].y, 0, 0);
                    if (worldIdx == 0)
                        worldIdx = 5;
                    else
                        worldIdx--;
                    setTile(1, splashWorlds[worldIdx].y, 28, 0);
                    currentGame.world = splashWorlds[worldIdx].world;
                    if (currentGame.level > currentGame.maxLevels[worldIdx]) {
                        level = 1;
                        updateLevel(level);
                        currentGame.level = level;
                    }
                    break;
                case CHAR_DOWN:
                    setTile(1, splashWorlds[worldIdx].y, 0, 0);
                    if (worldIdx == 5)
                        worldIdx = 0;
                    else
                        worldIdx++;
                    setTile(1, splashWorlds[worldIdx].y, 28, 0);
                    currentGame.world = splashWorlds[worldIdx].world;
                    if (currentGame.level > currentGame.maxLevels[worldIdx]) {
                        level = 1;
                        updateLevel(level);
                        currentGame.level = level;
                    }
                    break;
                case CHAR_ENTER:
                    currentGame.gameState = GAME_NEW_LEVEL;
                    setTile(1, splashWorlds[worldIdx].y, 0, 0);
                    clearSplash();
                    return;
                case CHAR_LT:
                    setTile(28, speedOptions[speedIdx].y, 0, 0);
                    if (speedIdx == 0) 
                        speedIdx = 2;
                    else
                        speedIdx--;
                    setTile(28, speedOptions[speedIdx].y,28,0);
                    currentGame.speed = speedOptions[speedIdx].speed;
                    break;
                case CHAR_GT:
                    setTile(28, speedOptions[speedIdx].y, 0, 0);
                    if (speedIdx == 2) 
                        speedIdx = 0;
                    else
                        speedIdx++;
                    setTile(28, speedOptions[speedIdx].y,28,0);
                    currentGame.speed = speedOptions[speedIdx].speed;                    
                    break;
                case CHAR_LEFT:
                case CHAR_RIGHT:
                    setTile(28, soundOptions[soundIdx].y, 0, 0);
                    if (soundIdx == 1) 
                        soundIdx = 0;
                    else
                        soundIdx = 1;
                    setTile(28, soundOptions[soundIdx].y, 28, 0);
                    currentGame.sound = soundOptions[soundIdx].sound;
                    break;
                case '-':
                    if (level > 1) {
                        level--;
                        updateLevel(level);
                        currentGame.level = level;
                    }
                    break;
                case '+':
                    if (level < currentGame.maxLevels[worldIdx]) {
                        level++;
                        updateLevel(level);
                        currentGame.level = level;
                    }

                default:
                    break;
            }
        }
    }
}