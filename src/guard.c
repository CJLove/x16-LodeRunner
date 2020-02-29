#include "guard.h"
#include "levels.h"
#include "loderunner.h"
#include "runner.h"
#include <cx16.h>
#include <stdio.h>
#include <stdlib.h>

struct guard_t guard[MAX_GUARDS];

struct shakeGuard_t shake[MAX_GUARDS];

uint8_t guardCount;

void addGuardToShakeQueue(uint8_t id);

// Convert sprite image VRAM address to the sprite attribute 0: Address(12:5)
#define IMAGE_ADDR(addr) ((addr >> 5) & 0xff)

// Animation sequences for guard
#define RUN_SEQUENCE 0
#define CLIMB_SEQUENCE 1
#define RAPPEL_SEQUENCE 2
#define FALL_SEQUENCE 3
#define MAX_SEQUENCE 4

// Animation sequence lengths
#define RUN_LENGTH 3
#define RAPPEL_LENGTH 3
#define CLIMB_LENGTH 2
#define FALL_LENGTH 1
#define MAX_SEQ_LENGTH 3

static uint8_t guardSequences[MAX_SEQUENCE][MAX_SEQ_LENGTH] = {
    // RUN_SEQUENCE
    {IMAGE_ADDR(GUARD_1), IMAGE_ADDR(GUARD_2), IMAGE_ADDR(GUARD_3)},
    // CLIMB_SEQUENCE
    {IMAGE_ADDR(GUARD_CLIMB_1), IMAGE_ADDR(GUARD_CLIMB_2)},
    // RAPPEL_SEQUENCE
    {IMAGE_ADDR(GUARD_RAPPEL_1), IMAGE_ADDR(GUARD_RAPPEL_2), IMAGE_ADDR(GUARD_RAPPEL_3)},
    // FALL_SEQUENCE
    {IMAGE_ADDR(GUARD_FALLING)}};

static uint8_t guardSeqSizes[MAX_SEQUENCE] = {RUN_LENGTH, CLIMB_LENGTH, RAPPEL_LENGTH, FALL_LENGTH};

#define NUM_MOVE_ITEMS 6

// Original movePolicy array from Lode Runner Total Recall
static uint8_t movePolicy[12][NUM_MOVE_ITEMS] = {
    {0, 0, 0, 0, 0, 0}, 
    {0, 1, 1, 0, 1, 1}, 
    {1, 1, 1, 1, 1, 1}, 
    {1, 2, 1, 1, 2, 1}, 
    {1, 2, 2, 1, 2, 2}, 
    {2, 2, 2, 2, 2, 2},
    {2, 2, 3, 2, 2, 3}, 
    {2, 3, 3, 2, 3, 3}, 
    {3, 3, 3, 3, 3, 3}, 
    {3, 3, 4, 3, 3, 4}, 
    {3, 4, 4, 3, 4, 4}, 
    {4, 4, 4, 4, 4, 4}
};

// Revised movePolicy array when guard movement was overly aggressive
// static uint8_t movePolicy[7][NUM_MOVE_ITEMS] = {
//     {0, 0, 0, 0, 0, 0},     // 0 guards
//     {0, 1, 0, 1, 0, 0},     // 1 guard
//     {1, 1, 0, 1, 1, 0},     // 2 guards
//     {1, 1, 1, 1, 1, 0},     // 3 guards
//     {1, 2, 2, 1, 2, 2},     // 4 guards
//     {2, 2, 2, 2, 2, 2},     // 5 guards
//     {2, 2, 3, 2, 2, 3}      // 6 guards
// };

// Offset into 
uint8_t moveOffset = 0;
// The ID of the last guard that was moved
uint8_t moveId = 0;

// Debug: display each guard's x & xOffset, y & yOffset
void displayGuard(uint8_t id)
{
    struct guard_t *curGuard = &guard[id];
    char buffer[15];
    uint8_t i = 0;

    sprintf(buffer, "%c:%c:%2u %2d %2d", id + 48, 24, curGuard->x, curGuard->xOffset, curGuard->action);
    for (i = 0; i < 15; i++) {
        setTile(28 + i, 10 + (id * 2), buffer[i], 0);
    }
    sprintf(buffer, "%c:%c:%2u %2d", id + 48, 25, curGuard->y, curGuard->yOffset);
    for (i = 0; i < 12; i++) {
        setTile(30 + i, 11 + (id * 2), buffer[i], 0);
    }
}

void clearGuards()
{
    uint8_t i = 0;

    for (i = 0; i < MAX_GUARDS; i++) {
        guard[i].active = 0;

        // Disable the corresponding sprite
        vpoke(0, 0x1f5006 + 8 * (i + 1));  // Attr6

        shake[i].active = 0;
    }
    guardCount = 0;
    moveOffset = 0;
    moveId = 0;
}

void initGuard(uint8_t x, uint8_t y)
{
    uint8_t i = guardCount;
    uint16_t xPos = x * TILE_W;
    uint16_t yPos = y * TILE_H;

    guard[i].x = x;
    guard[i].y = y;
    guard[i].xOffset = 0;
    guard[i].yOffset = 0;
    guard[i].holePosX = 0;
    guard[i].holePosY = 0;
    guard[i].action = ACT_UNKNOWN;
    guard[i].direction = ACT_RIGHT;
    guard[i].idx = 0;

    guard[i].sequence = RUN_SEQUENCE;

    // Sprite attribute settings
    vpoke((GUARD_1 >> 5) & 0xff, 0x1f5000 + 8 * (i + 1));  // Attr0
    VERA.data0 = (GUARD_1 >> 13) & 0xf;                    // Attr1
    VERA.data0 = xPos & 0xff;                              // Attr2
    VERA.data0 = xPos >> 8;                                // Attr3
    VERA.data0 = yPos & 0xff;                              // Attr4
    VERA.data0 = yPos >> 8;                                // Attr5
    VERA.data0 = (3 << 2);                                 // Attr6
    VERA.data0 = 0;                                        // Attr7

#ifdef DEBUG
    // displayGuard(guardCount);
#endif
    guardCount++;
}

int8_t guardId(uint8_t x, uint8_t y)
{
    int8_t i = 0;
    for (i = 0; i < guardCount; i++) {
        if (guard[i].x == x && guard[i].y == y)
            return i;
    }
    return -1;
}

int8_t guardAlive(uint8_t x, uint8_t y)
{
    int8_t i = 0;
    for (i = 0; i < guardCount; i++) {
        if (guard[i].x == x && guard[i].y == y) {
            if (guard[i].action != ACT_REBORN) return 1;
        }
    }
    return 0;
}

void rebornComplete(int8_t id)
{
    uint8_t x = guard[id].x;
    uint8_t y = guard[id].y;

    if (map[x][y].act == TILE_RUNNER) setRunnerDead();

    map[x][y].act = TILE_GUARD;
    guard[id].action = ACT_FALL;
    guard[id].direction = ACT_RIGHT;
}

void guardReborn(uint8_t x, uint8_t y)
{
    int8_t id = guardId(x, y);
    if (id != -1) {
        struct guard_t *curGuard = &guard[id];
        uint8_t bornY = 1;  // Start on line 2
        uint8_t bornX = (uint8_t)(rand() % MAX_TILE_X);
        uint8_t rndStart = bornX;

        while (map[bornX][bornY].act != TILE_BLANK || map[bornX][bornY].base == TILE_GOLD ||
               map[bornX][bornY].base == TILE_BRICK) {
            if ((bornX = (uint8_t)(rand() % MAX_TILE_X)) == rndStart) {
                bornY++;
            }
        }

        map[bornX][bornY].act = TILE_GUARD;
        curGuard->x = bornX;
        curGuard->y = bornY;
        curGuard->xOffset = 0;
        curGuard->yOffset = 0;
        curGuard->sequence = RUN_SEQUENCE;
        curGuard->idx = 0;
        curGuard->action = ACT_REBORN;

        rebornComplete(id);
    }
}

uint8_t dropGold(uint8_t id)
{
    struct guard_t *curGuard = &guard[id];
    uint8_t nextToken = 0;
    uint8_t drop = 0;

    if (curGuard->hasGold > 1) {
        // Decrease count but don't drop gold
        curGuard->hasGold--;
    } else if (curGuard->hasGold == 1) {
        uint8_t x = curGuard->x;
        uint8_t y = curGuard->y;
        if (map[x][y].base == TILE_BLANK &&
            (y >= MAX_TILE_Y || (nextToken = map[x][y+1].base) == TILE_BRICK ||
            nextToken == TILE_BLOCK || nextToken == TILE_LADDER)) {
            addGold(x,y);
            curGuard->hasGold = -1;
            drop = 1;
        }
    } else if (curGuard->hasGold < 0) {
        curGuard->hasGold++;
    }
    return drop;
}

uint8_t bestRating = 0;
uint8_t bestPath = 0;
uint8_t curRating = 0;
uint8_t startX = 0;
uint8_t startY = 0;
uint8_t leftEnd = 0;
uint8_t rightEnd = 0;

void scanDown(uint8_t x, uint8_t curPath)
{
    uint8_t y;
    uint8_t nextBelow;
    uint8_t runnerX = runner.x;
    uint8_t runnerY = runner.y;

    y = startY;

    while (y < MAX_TILE_Y && (nextBelow = map[x][y + 1].base) != TILE_BRICK && nextBelow != TILE_BLOCK) {
        if (map[x][y].base != TILE_BLANK && map[x][y].base != TILE_HIDDEN) {
            // If not falling, try to move left or right
            if (x > 0) {
                // If not at left edge check left side
                if ((nextBelow = map[x - 1][y + 1].base) == TILE_BRICK || nextBelow == TILE_LADDER || nextBelow == TILE_BLOCK ||
                    map[x - 1][y].base == TILE_ROPE) {
                    // Can move left
                    if (y >= runnerY) {
                        // No need to go on, already below runner
                        break;
                    }
                }
            }
            if (x < MAX_TILE_X) {
                // If not at right edge check right side
                if ((nextBelow = map[x + 1][y + 1].base) == TILE_BRICK || nextBelow == TILE_LADDER || nextBelow == TILE_BLOCK ||
                    map[x + 1][y].base == TILE_ROPE) {
                    // Can move right
                    if (y >= runnerY) {
                        // No need to go on, already below runner
                        break;
                    }
                }
            }
        }
        y++;
    }
    if (y == runnerY) {
        curRating = abs(startX - x);
    }
    else if (y > runnerY) {
        // Position below runner
        curRating = y - runnerY + 200;
    }
    else {
        curRating = runnerY - y + 100;
    }

    if (curRating < bestRating) {
        bestRating = curRating;
        bestPath = curPath;
    }
}

void scanUp(uint8_t x, uint8_t curPath)
{
    uint8_t y;
    uint8_t nextBelow;
    uint8_t runnerX = runner.x;
    uint8_t runnerY = runner.y;

    y = startY;

    while (y > 0 && map[x][y].base == TILE_LADDER) {
        // While guard can go up
        --y;
        if (x > 0) {
            // If not at left edge check left side
            if ((nextBelow = map[x - 1][y + 1].base) == TILE_BRICK || nextBelow == TILE_BLOCK || nextBelow == TILE_LADDER ||
                map[x - 1][y].base == TILE_ROPE) {
                if (y <= runnerY) {
                    break;
                }
            }
        }

        if (x < MAX_TILE_X) {
            // If not at right edge check right side
            if ((nextBelow = map[x + 1][y + 1].base) == TILE_BRICK || nextBelow == TILE_BLOCK || nextBelow == TILE_LADDER ||
                map[x + 1][y].base == TILE_ROPE) {
                if (y <= runnerY) {
                    break;
                }
            }
        }
    }

    if (y == runnerY) {
        curRating = abs(startX - x);
    }
    else if (y > runnerY) {
        // position below runner
        curRating = y - runnerY + 200;
    }
    else {
        // position above runner
        curRating = runnerY - y + 100;
    }

    if (curRating < bestRating) {
        bestRating = curRating;
        bestPath = curPath;
    }
}

uint8_t scanFloor(uint8_t id)
{
    uint8_t x = startX = guard[id].x;
    uint8_t y = startY = guard[id].y;
    uint8_t curToken;
    uint8_t curPath = 0;
    uint8_t nextBelow;

    // Start with worst rating
    bestRating = 255;
    curRating = 255;
    bestPath = ACT_STOP;

    // Get ends for the search along floor
    while (x > 0) {
        curToken = map[x - 1][y].act;
        if (curToken == TILE_BRICK || curToken == TILE_BLOCK) {
            break;
        }
        if (curToken == TILE_LADDER || curToken == TILE_ROPE || y >= MAX_TILE_Y ||
            (y < MAX_TILE_Y &&
             ((nextBelow = map[x - 1][y + 1].base) == TILE_BRICK || nextBelow == TILE_BLOCK || nextBelow == TILE_LADDER))) {
            --x;
        }
        else {
            // Go left anyway
            --x;
            break;
        }
    }
    leftEnd = x;
    x = startX;
    while (x < MAX_TILE_X) {
        curToken = map[x + 1][y].act;
        if (curToken == TILE_BRICK || curToken == TILE_BLOCK) {
            break;
        }
        if (curToken == TILE_LADDER || curToken == TILE_ROPE || y >= MAX_TILE_Y ||
            (y < MAX_TILE_Y &&
             ((nextBelow = map[x + 1][y + 1].base) == TILE_BRICK || nextBelow == TILE_BLOCK || nextBelow == TILE_LADDER))) {
            ++x;
        }
        else {
            // Go right anyway
            ++x;
            break;
        }
    }
    rightEnd = x;

    // Do middle scan first for best rating and direction
    x = startX;
    if (y < MAX_TILE_Y && (nextBelow = map[x][y + 1].base) != TILE_BRICK && nextBelow != TILE_BLOCK) {
        scanDown(x, ACT_DOWN);
    }

    if (map[x][y].base == TILE_LADDER) {
        scanUp(x, ACT_UP);
    }

    // Next scan both sides of floor for best rating
    curPath = ACT_LEFT;
    x = leftEnd;
    while (1) {
        if (x == startX) {
            if (curPath == ACT_LEFT && rightEnd != startX) {
                curPath = ACT_RIGHT;
                x = rightEnd;
            }
            else {
                break;
            }
        }

        if (y < MAX_TILE_Y && (nextBelow = map[x][y + 1].base) != TILE_BRICK && nextBelow != TILE_BLOCK) {
            scanDown(x, curPath);
        }

        if (map[x][y].base == TILE_LADDER) {
            scanUp(x, curPath);
        }

        if (curPath == ACT_LEFT) {
            x++;
        }
        else {
            x--;
        }
    }

    return bestPath;
}

uint8_t bestMove(uint8_t id)
{
    struct guard_t *guarder = &guard[id];
    uint8_t x = guarder->x;
    uint8_t y = guarder->y;
    int8_t xOffset = guarder->xOffset;
    int8_t yOffset = guarder->yOffset;

    uint8_t curToken = map[x][y].base;
    uint8_t nextBelow = 0;
    uint8_t nextMove;
    uint8_t checkSameLevelOnly = 0;

    if (guarder->action == ACT_CLIMB_OUT) {
        if (y == guarder->holePosY) {
            // Climb out of hole
            return ACT_UP;
        }
        else {
            checkSameLevelOnly = 1;
            if (x != guarder->holePosX) {
                // out of hole
                guarder->action = ACT_LEFT;
            }
        }
    }

    if (!checkSameLevelOnly) {
        // Next check if guard must fall; if so then return ACT_FALL and
        // skip remaining logic
        if (curToken == TILE_LADDER || (curToken == TILE_ROPE && yOffset == 0)) {
            // No guard fall
        }
        else if (yOffset < 0) {
            return ACT_FALL;
        }
        else if (y < MAX_TILE_Y) {
            nextBelow = map[x][y + 1].act;

            if ((nextBelow == TILE_BLANK || nextBelow == TILE_RUNNER)) {
                return ACT_FALL;
            }
            else if (nextBelow == TILE_BRICK || nextBelow == TILE_BLOCK || nextBelow == TILE_GUARD ||
                     nextBelow == TILE_LADDER) {
                // No guard fall
            }
            else {
                return ACT_FALL;
            }
        }
    }

    // Next check if runner is on same level and whether guard can get him.  Ignore walls
    if (y == runner.y && runner.action != ACT_FALL) {
        // Guard on ladder and falling -> don't catch it
        while (x != runner.x) {
            if (y < MAX_TILE_Y) {
                nextBelow = map[x][y + 1].base;
            }
            else {
                nextBelow = TILE_BLOCK;
            }
            curToken = map[x][y].base;

            if (curToken == TILE_LADDER || curToken == TILE_ROPE || nextBelow == TILE_BLOCK || nextBelow == TILE_LADDER ||
                nextBelow == TILE_BRICK || map[x][y + 1].act == TILE_GUARD || nextBelow == TILE_ROPE ||
                nextBelow == TILE_GOLD) {
                if (x < runner.x) {
                    // Guard to left of runner
                    ++x;
                }
                else if (x > runner.x) {
                    // Guard to right of runner
                    --x;
                }
            }
            else {
                // Exit loop with closest x if no path to runner
                break;
            }
        }

        if (x == runner.x) {
            // scan for a path ignoring walls is a success
            if (guarder->x < runner.x) {
                nextMove = ACT_RIGHT;
            }
            else if (guarder->x > runner.x) {
                nextMove = ACT_LEFT;
            }
            else {
                // Guard x == runner x
                if (guarder->xOffset < runner.xOffset) {
                    nextMove = ACT_RIGHT;
                }
                else {
                    nextMove = ACT_LEFT;
                }
            }
            return nextMove;
        }
    }
    // If guard can't reach runner on current level then scan floor
    // (ignoring walls) and look up and down for best move
    return scanFloor(id);
}

void guardMoveStep(uint8_t id, uint8_t action)
{
    struct guard_t *curGuard = &guard[id];
    uint8_t x = curGuard->x;
    int8_t xOffset = curGuard->xOffset;
    uint8_t y = curGuard->y;
    int8_t yOffset = curGuard->yOffset;

    uint8_t curToken, nextToken;
    uint8_t centerX, centerY;
    uint8_t stayCurrentPos;

    centerX = centerY = ACT_STOP;

    if (curGuard->action == ACT_CLIMB_OUT && action == ACT_STOP) {
        curGuard->action = ACT_STOP;
    }

    switch (action) {
        case ACT_UP:
        case ACT_DOWN:
        case ACT_FALL:
            if (action == ACT_UP) {
                stayCurrentPos = (y <= 0 || (nextToken = map[x][y - 1].act) == TILE_BRICK || nextToken == TILE_BLOCK ||
                                  nextToken == TILE_TRAP || nextToken == TILE_GUARD);
                if (yOffset <= 0 && stayCurrentPos) {
                    action = ACT_STOP;
                }
            }
            else {
                // ACT_DOWN || ACT_FALL
                stayCurrentPos = (y >= MAX_TILE_Y || (nextToken = map[x][y + 1].act) == TILE_BRICK || nextToken == TILE_BLOCK ||
                                  nextToken == TILE_GUARD);

                if (action == ACT_FALL && yOffset < 0 && map[x][y].base == TILE_BRICK) {
                    action = ACT_IN_HOLE;
                    stayCurrentPos = 1;
                }
                else {
                    if (yOffset >= 0 && stayCurrentPos) {
                        action = ACT_STOP;
                    }
                }
            }

            if (action != ACT_STOP) {
                if (xOffset > 0) {
                    centerX = ACT_LEFT;
                }
                else if (xOffset < 0) {
                    centerX = ACT_RIGHT;
                }
            }
            break;
        case ACT_LEFT:
        case ACT_RIGHT:
            if (action == ACT_LEFT) {
                stayCurrentPos = (x <= 0 || (nextToken = map[x - 1][y].act) == TILE_BRICK || nextToken == TILE_BLOCK ||
                                  nextToken == TILE_GUARD || map[x - 1][y].base == TILE_TRAP);
                if (xOffset <= 0 && stayCurrentPos) {
                    action = ACT_STOP;
                }
            }
            else {
                // ACT_RIGHT
                stayCurrentPos = (x >= MAX_TILE_X || (nextToken = map[x + 1][y].act) == TILE_BRICK || nextToken == TILE_BLOCK ||
                                  nextToken == TILE_GUARD || map[x + 1][y].base == TILE_TRAP);
                if (xOffset >= 0 && stayCurrentPos) {
                    action = ACT_STOP;
                }
            }

            if (action != ACT_STOP) {
                if (yOffset > 0) {
                    centerY = ACT_UP;
                }
                else if (yOffset < 0) {
                    centerY = ACT_DOWN;
                }
            }
            break;
    }

    curToken = map[x][y].base;

    if (action == ACT_UP) {
        yOffset -= 1;  // yMove

        if (stayCurrentPos && yOffset < 0)
            yOffset = 0;
        else if (yOffset < -H2) {
            // Move to y-1 position
            map[x][y].act = curToken;
            y--;
            yOffset += TILE_H;
            if (map[x][y].act == TILE_RUNNER) setRunnerDead();
        }

        if (yOffset <= 0 && yOffset > -1) {
            dropGold(id);
        }
        curGuard->sequence = CLIMB_SEQUENCE;
    }

    if (centerY == ACT_UP) {
        yOffset -= 1;  // yMove
        if (yOffset < 0)
            yOffset = 0;
    }

    if (action == ACT_DOWN || action == ACT_FALL || action == ACT_IN_HOLE) {
        uint8_t holdOnBar = 0;
        if (curToken == TILE_ROPE) {
            if (yOffset < 0)
                holdOnBar = 1;
            else if (action == ACT_DOWN && y < MAX_TILE_Y && map[x][y + 1].act != TILE_LADDER) {
                action = ACT_FALL;
            }
        }

        yOffset += 1;  // yMove

        if (holdOnBar == 1 && yOffset >= 0) {
            // Fall and hold on bar
            yOffset = 0;
            action = ACT_FALL_BAR;
        }
        if (stayCurrentPos && yOffset > 0)
            yOffset = 0;
        else if (yOffset > H2) {
            // Move to y+1 position
            // if (curToken == TILE_BRICK || curToken == TILE_HIDDEN) curToken = TILE_BLANK; // In hole or hidden ladder
            map[x][y].act = curToken;
            y++;
            yOffset -= TILE_H;
            if (map[x][y].act == TILE_RUNNER) setRunnerDead();
        }

        // Drop gold while guard falls
        if ((action == ACT_FALL || action == ACT_DOWN) && yOffset >= 0 && yOffset < 1) {
            dropGold(id);
        }

        if (action == ACT_IN_HOLE) {
            // Check whether guard is in a hole or still falling
            if (yOffset < 0) {
                action = ACT_FALL;  // still falling

                // If guard has gold then drop it before falling into hole totally
                if (curGuard->hasGold > 0) {
                    if (map[x][y - 1].base == TILE_BLANK) {
                        // Drop gold above
                        addGold(x,y-1);
                    }
                    else {
                        decGold(); // Gold disappears
                    }
                    curGuard->hasGold = 0;
                }
            }
            else {
                // Fall into hole (yOffset must = 0)
                if (curGuard->hasGold > 0) {
                    if (map[x][y - 1].base == TILE_BLANK) {
                        // Drop ggold above
                        addGold(x,y-1);
                    }
                    else {
                        decGold(); // Gold disappears
                    }
                    curGuard->hasGold = 0;
                }

                curGuard->sequence = FALL_SEQUENCE;
                curGuard->idx = 0;
                addGuardToShakeQueue(id);
                displayScore(SCORE_IN_HOLE);
            }
        }

        if (action == ACT_DOWN) {
            curGuard->sequence = CLIMB_SEQUENCE;
        }
        else {
            // ACT_FALL or ACT_FALL_BAR
            if (action == ACT_FALL_BAR) {
                curGuard->sequence = RAPPEL_SEQUENCE;
            }
            else {
                curGuard->sequence = FALL_SEQUENCE;
            }
        }
    }

    if (centerY == ACT_DOWN) {
        yOffset += 1;  // yMove
        if (yOffset > 0)
            yOffset = 0;
    }

    if (action == ACT_LEFT) {
        xOffset -= 1;  // xMove

        if (stayCurrentPos && xOffset < 0)
            xOffset = 0;
        else if (xOffset < -W2) {
            // Move to x-1 position
            // if (curToken == TILE_BRICK || curToken == TILE_LADDER) curToken = TILE_BLANK;
            map[x][y].act = curToken;  // Runner move to [x-1][y], so set [x][y] to previous state
            x--;
            xOffset += TILE_W;
            if (map[x][y].act == TILE_RUNNER) setRunnerDead();
        }
        if (xOffset <= 0 && xOffset > -1) {
            dropGold(id);   // Try to drop gold
        }
        if (curToken == TILE_ROPE)
            curGuard->sequence = RAPPEL_SEQUENCE;
        else
            curGuard->sequence = RUN_SEQUENCE;
        curGuard->direction = ACT_LEFT;
    }

    if (centerX == ACT_LEFT) {
        xOffset -= 1;  // xMove
        if (xOffset < 0)
            xOffset = 0;
    }

    if (action == ACT_RIGHT) {
        xOffset += 1;  // xMove

        if (stayCurrentPos && xOffset > 0)
            xOffset = 0;
        else if (xOffset > W2) {
            // Move to x+1 position
            // if (curToken == TILE_BRICK || curToken == TILE_LADDER) curToken = TILE_BLANK;
            map[x][y].act = curToken;
            x++;
            xOffset = xOffset - TILE_W;
            if (map[x][y].act == TILE_RUNNER) setRunnerDead();
        }
        if (xOffset >= 0 && xOffset < 1) {
            dropGold(id);
        }
        if (curToken == TILE_ROPE)
            curGuard->sequence = RAPPEL_SEQUENCE;
        else
            curGuard->sequence = RUN_SEQUENCE;
        curGuard->direction = ACT_RIGHT;
    }

    if (centerX == ACT_RIGHT) {
        xOffset += 1;  // xMove
        if (xOffset > 0)
            xOffset = 0;
    }

    if (action == ACT_STOP) {
        if (curGuard->action != ACT_CLIMB_OUT) {
            curGuard->action = ACT_STOP;
        }
    }
    else {
        uint16_t xPos = x * TILE_W + xOffset;
        uint16_t yPos = y * TILE_H + yOffset;
        uint8_t dir = (curGuard->direction == ACT_LEFT) ? 1 : 0;
        if (curGuard->action == ACT_CLIMB_OUT)
            action = ACT_CLIMB_OUT;

        // Update the sprite image and flip bit
        curGuard->idx++;
        curGuard->idx = curGuard->idx % guardSeqSizes[curGuard->sequence];
        vpoke(guardSequences[curGuard->sequence][curGuard->idx], 0x1f5000 + 8 * (id + 1));
        vpoke((3 << 2) | dir, 0x1f5006 + 8 * (id + 1));

        // sprite x position
        vpoke(xPos & 0xff, 0x1f5002 + 8 * (id + 1));
        VERA.data0 = xPos >> 8;

        // sprite y position
        vpoke(yPos & 0xff, 0x1f5004 + 8 * (id + 1));
        VERA.data0 = yPos >> 8;

        curGuard->x = x;
        curGuard->y = y;
        curGuard->xOffset = xOffset;
        curGuard->yOffset = yOffset;
        curGuard->action = action;
    }
    map[x][y].act = TILE_GUARD;

    // check if there is gold to pick up and cary
    if (map[x][y].base == TILE_GOLD && curGuard->hasGold == 0 &&
        ((!xOffset && yOffset >= 0 && yOffset < H4) ||
         (!yOffset && xOffset >= 0 && xOffset < W4) ||
         (y < MAX_TILE_Y && map[x][y+1].base == TILE_LADDER && yOffset < H4)    // gold above ladder
        )) {
        curGuard->hasGold = rand() % 37;
        removeGold(x,y);
    }
}

void moveGuard()
{
    uint8_t moves = 0;
    struct guard_t *curGuard = 0;
    uint8_t x = 0, y = 0, yOffset = 0;

    if (!guardCount)
        return;  // No guard

    if (++moveOffset >= NUM_MOVE_ITEMS)
        moveOffset = 0;
    moves = movePolicy[guardCount][moveOffset];  // get next moves

    while (moves-- > 0) {
        if (++moveId >= guardCount)
            moveId = 0;
        curGuard = &guard[moveId];

        if (curGuard->action == ACT_IN_HOLE || curGuard->action == ACT_REBORN) {
            continue;
        }

        guardMoveStep(moveId, bestMove(moveId));

        // Debug: display guard position
#ifdef DEBUG
        // displayGuard(moveId);
#endif
    }
}

void climbOut(uint8_t id)
{
    struct guard_t *curGuard = &guard[id];
    curGuard->action = ACT_CLIMB_OUT;
    curGuard->sequence = CLIMB_SEQUENCE;
    curGuard->holePosX = curGuard->x;
    curGuard->holePosY = curGuard->y;
}

void addGuardToShakeQueue(uint8_t id)
{
    uint8_t i = 0;
    for (i = 0; i < guardCount; i++) {
        if (!shake[i].active) {
            shake[i].active = 1;
            shake[i].id = id;
            shake[i].count = 0;
            shake[i].idx = 0;
        }
    }
}

void removeFromShake(uint8_t id)
{
    uint8_t i = 0;
    for (i = 0; i < guardCount; i++) {
        if (shake[i].active && shake[i].id == id) {
            shake[i].active = 0;
        }
    }
}

#define SHAKE_LENGTH 5
static uint16_t shakeTimes[] = {140, 146, 152, 158, 162 };

void processGuardShake()
{
    uint8_t i = 0;
    for (i = 0; i < guardCount; i++) {
        if (shake[i].active) {
            struct guard_t *curGuard = &guard[shake[i].id];

            ++shake[i].count;

            if (shake[i].count >= shakeTimes[shake[i].idx]) {
                // Shake the guard by shifting its X position
                uint16_t xPos = curGuard->x * TILE_W;
                if (shake[i].idx % 2) {
                    xPos -= 2;
                }
                else {
                    xPos += 2;
                }
                vpoke(xPos & 0xff, 0x1f5002 + 8 * (shake[i].id + 1));
                VERA.data0 = xPos >> 8;

                shake[i].idx++;

                if (shake[i].idx == SHAKE_LENGTH) {
                    // Shake sequence complete
                    shake[i].active = 0;

                    climbOut(shake[i].id);
                    continue;
                }
            }
        }
    }
}