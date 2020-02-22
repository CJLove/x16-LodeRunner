#include "levels.h"
#include "loderunner.h"
#include "runner.h"
#include "guard.h"
#include <cx16.h>
#include <stdio.h>

// The tileMap[] table maps a nibble (0-15) from
// banked memory to the tile to be used in VRAM and
// the current level's map.
// This mapping matches what is done in levels.py
// when building the .BIN file for each world
static const uint8_t tileMap[] = {
    TILE_BLANK, TILE_GUARD, TILE_RUNNER, TILE_BRICK, TILE_BLOCK, TILE_TRAP, TILE_LADDER, TILE_HIDDEN, TILE_ROPE, TILE_GOLD,
    TILE_BLANK,  // Placeholder
    TILE_BLANK,  // Placeholder
    TILE_BLANK,  // Placeholder
    TILE_BLANK,  // Placeholder
    TILE_BLANK,  // Placeholder
    TILE_BLANK   // Placeholder
};

// Map of data for the current level
struct map_t map[NO_OF_TILES_X][NO_OF_TILES_Y];

// State information for holes dug by the runner
struct hole_t holes[MAX_HOLES];

// State information for hole being dug by the runner
struct dig_t hole;

// Count of gold pieces on the current level
uint8_t goldCount;

// Indicator that gold collection is complete for the current level
uint8_t goldComplete;

// Current game score
uint32_t currentScore;

// Number of lives
uint8_t lives;

// State of the game
uint8_t gameState;

// Current level
uint8_t level = 1;

// Current world
uint8_t world = WORLD_CLASSIC;


// Tiles for text on the status line 
static const uint8_t labelScore[] = {19, 3, 15, 18, 5};
static const uint8_t labelMen[] = {13, 5, 14};
static const uint8_t labelLevel[] = {12, 5, 22, 5, 12};

void completeLevel()
{
    uint8_t row = 0;
    uint8_t col = 0;

    for (row = 0; row < LEVEL_ROW_COUNT; row++) {
        for (col = 0; col < LEVEL_ROW_OFFSET; col++) {
            if (map[col][row].base == TILE_HIDDEN) {
                map[col][row].base = TILE_LADDER;
                map[col][row].act = TILE_LADDER;
                setTile(col,row,TILE_LADDER,0);
            }
        }
    }
    goldComplete = 1;
}

void displayScore(uint32_t addScore)
{
    uint8_t col = 0;
    char buffer[10];
    currentScore += addScore;

    snprintf(buffer, 10, "%07lu", currentScore);
    for (col = 0; col < 7; col++) {
        setTile(col + 5, LEVEL_ROW_COUNT + 1, buffer[col], 0);
    }
}

void displayStatus(uint32_t score, uint8_t men, uint8_t level)
{
    uint8_t col = 0;
    char buffer[10];
    // Display status row (18th row)

    // SCORE
    for (col = 0; col < 5; col++) {
        setTile(col, LEVEL_ROW_COUNT + 1, labelScore[col], 0);
    }
    displayScore(score);

    // MEN
    for (col = 0; col < 3; col++) {
        setTile(col + 13, LEVEL_ROW_COUNT + 1, labelMen[col], 0);
    }
    snprintf(buffer, 10, "%03d", (int)men);
    for (col = 0; col < 3; col++) {
        setTile(col + 16, LEVEL_ROW_COUNT + 1, buffer[col], 0);
    }
    // LEVEL
    for (col = 0; col < 5; col++) {
        setTile(col + 20, LEVEL_ROW_COUNT + 1, labelLevel[col], 0);
    }
    snprintf(buffer, 10, "%03d", (int)level + 1);
    for (col = 0; col < 3; col++) {
        setTile(col + 25, LEVEL_ROW_COUNT + 1, buffer[col], 0);
    }
}

// Load a level from banked RAM into the map[] representation
int loadLevel(uint8_t world, uint8_t level)
{
    uint8_t max = 0;
    uint8_t i = 0;
    // Switch to bank corresponding to the desired world and level
    uint8_t bank = world + (level / 36);
    uint8_t foundRunner = 0;

    VIA1.pra = bank;

    max = *(uint8_t *)(LEVEL_COUNT + 1);
    // Clear the runner, gold count, and all state info regarding
    // digging in progress or holes
    clearRunner();
    clearGuards();
    goldCount = 0;
    for (i = 0; i < MAX_HOLES; i++) {
        holes[i].active = 0;
    }
    hole.action = ACT_STOP;

    level--;  // zero-based indexing for level data
    if (level < max) {
        uint8_t *levelPtr = (uint8_t *)(LEVEL_BASE + (LEVEL_OFFSET * (level % 36)));
        uint8_t row = 0;
        uint8_t idx = 0;

        for (row = 0; row < LEVEL_ROW_COUNT; row++) {
            for (idx = 0; idx < LEVEL_ROW_BANK_OFFSET; idx++) {
                uint8_t data = *(levelPtr + row * LEVEL_ROW_BANK_OFFSET + idx);
                uint8_t tile1 = tileMap[(data & 0xf0) >> 4];
                uint8_t tile2 = tileMap[(data & 0xf)];

                switch (tile1) {
                    case TILE_GUARD:
                        // TODO: Initiaze guard and set tile as blank
                        map[idx * 2][row].act = TILE_GUARD;
                        map[idx * 2][row].base = TILE_BLANK;
                        if (guardCount == MAX_GUARDS) {
                            // Too many guards
                            map[idx * 2][row].act = TILE_BLANK;
                            continue;
                        }
                        initGuard(idx*2,row);
                        break;
                    case TILE_RUNNER:
                        // If runner hasn't been found yet, then initialize
                        if (!foundRunner) {
                            initRunner(idx * 2, row);
                            foundRunner = 1;
                        }
                        map[idx * 2][row].base = TILE_BLANK;
                        map[idx * 2][row].act = TILE_RUNNER;
                        break;
                    case TILE_HIDDEN:
                        map[idx * 2][row].base = tile1;
                        map[idx * 2][row].act = TILE_BLANK;
                        break;
                    case TILE_GOLD:
                        goldCount++;
                        // Fall through
                    default:
                        map[idx * 2][row].base = tile1;
                        map[idx * 2][row].act = tile1;
                        break;
                }
                switch (tile2) {
                    case TILE_GUARD:
                        // TODO: Initiaze guard and set tile as blank
                        map[idx * 2 + 1][row].act = TILE_GUARD;
                        map[idx * 2 + 1][row].base = TILE_BLANK;
                        if (guardCount == MAX_GUARDS) {
                            // Too many guards
                            map[idx * 2 + 1][row].act = TILE_BLANK;
                            continue;
                        }
                        initGuard(idx*2+1,row);
                        break;
                    case TILE_RUNNER:
                        // If runner hasn't been found yet, then initialize
                        if (!foundRunner) {
                            initRunner(idx * 2 + 1, row);
                            foundRunner = 1;
                        }
                        map[idx * 2 + 1][row].base = TILE_BLANK;
                        map[idx * 2 + 1][row].act = TILE_RUNNER;
                        break;
                    case TILE_HIDDEN:
                        map[idx * 2 + 1][row].base = tile2;
                        map[idx * 2 + 1][row].act = TILE_BLANK;
                        break;
                    case TILE_GOLD:
                        goldCount++;
                        // Fall through
                    default:
                        map[idx * 2 + 1][row].base = tile2;
                        map[idx * 2 + 1][row].act = tile2;
                        break;
                }
            }
        }

        return 1;
    }

    VIA1.pra = 0;

    // Invalid level for this bank
    return 0;
}

// Display the level held in the map[] representation into VRAM
int displayLevel(uint8_t level)
{
    uint8_t row = 0;
    uint8_t col = 0;
    
    for (row = 0; row < LEVEL_ROW_COUNT; row++) {
        for (col = 0; col < LEVEL_ROW_OFFSET; col++) {
            uint8_t tile = map[col][row].base;

            setTile(col,row,tile,0);

        }
    }

    // Display the ground (17th row)
    for (col = 0; col < LEVEL_ROW_OFFSET; col++) {
        setTile(col, LEVEL_ROW_COUNT, TILE_GROUND, 0);
    }
    displayStatus(0, lives, level);

    return 1;

}

// Dump the level held in the map[] and runner representations in text form
void dumpLevel(uint8_t level)
{
    uint8_t row;
    uint8_t col;
    printf("Level %d:\n",level);
    for (row = 0; row < LEVEL_ROW_COUNT; row++) {
        for (col = 0; col < LEVEL_ROW_OFFSET; col++) {
            uint8_t tile = map[col][row].act;
            // Dump tile as a character
            printf("%c", tile);
            // Dump integer form of tile
            // printf("%2d ",tile);
        }
        printf("\n");
    }
    printf("Runner @ %d,%d\n",runner.x,runner.y);
}
