
#include "runner.h"
#include "key.h"
#include "levels.h"
#include "loderunner.h"
#include <cx16.h>
#include <stdio.h>
#include <string.h>

// Runner state information
struct runner_t runner;

// Convert sprite image VRAM address to the sprite attribute 0: Address(12:5)
#define IMAGE_ADDR(addr) ((addr >> 5) & 0xff)

// Animation sequences for runner
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

static uint8_t runnerSequences[MAX_SEQUENCE][MAX_SEQ_LENGTH] = {
    // RUN_SEQUENCE
    {IMAGE_ADDR(RUNNER_1), IMAGE_ADDR(RUNNER_2), IMAGE_ADDR(RUNNER_3)},
    // CLIMB_SEQUENCE
    {IMAGE_ADDR(RUNNER_CLIMB_1), IMAGE_ADDR(RUNNER_CLIMB_2)},
    // RAPPEL_SEQUENCE
    {IMAGE_ADDR(RUNNER_RAPPEL_1), IMAGE_ADDR(RUNNER_RAPPEL_2), IMAGE_ADDR(RUNNER_RAPPEL_3)},
    // FALL_SEQUENCE
    {IMAGE_ADDR(RUNNER_FALLING)}};

static uint8_t runnerSeqSizes[MAX_SEQUENCE] = {RUN_LENGTH, CLIMB_LENGTH, RAPPEL_LENGTH, FALL_LENGTH};

#define STATE_OK_TO_MOVE 1
#define STATE_FALLING 2

// Debug: display runner x & xOffset, y & yOffset
void displayPos()
{
    char buffer[10];
    uint8_t i = 0;
    sprintf(buffer, "%c:%2u %2d", 24, runner.x, runner.xOffset);
    for (i = 0; i < 10; i++) {
        setTile(32 + i, 6, buffer[i], 0);
    }

    sprintf(buffer, "%c:%2u %2d", 25, runner.y, runner.yOffset);
    for (i = 0; i < 10; i++) {
        setTile(32 + i, 8, buffer[i], 0);
    }
}

// Debug current and surrounding tiles for the "act" and "base" maps
void displayTiles(uint8_t x, uint8_t y)
{
    // map[][].act:
    uint8_t t = map[x][y].act;
    setTile(33,1,t,0);
    t = (y > 0) ? map[x][y-1].act : TILE_BLANK;
    setTile(33,0,t,0);
    t = (x > 0) ? map[x-1][y].act : TILE_BLANK;
    setTile(32,1,t,0);
    t = (x < MAX_TILE_X) ? map[x+1][y].act : TILE_BLANK;
    setTile(34,1,t,0);
    if (y < MAX_TILE_Y) {
        t = (x > 0) ? map[x-1][y+1].act : TILE_BLANK;
        setTile(32,2,t,0);
        t = map[x][y+1].act;
        setTile(33,2,t,0);
        t = (x < MAX_TILE_X) ? map[x+1][y+1].act : TILE_BLANK;
        setTile(34,2,t,0);
    }

    // map[][].base:
    t = map[x][y].base;
    setTile(37,1,t,0);
    t = (y > 0) ? map[x][y-1].base : TILE_BLANK;
    setTile(37,0,t,0);
    t = (x > 0) ? map[x-1][y].base : TILE_BLANK;
    setTile(36,1,t,0);
    t = (x < MAX_TILE_X) ? map[x+1][y].base : TILE_BLANK;
    setTile(38,1,t,0);
    if (y < MAX_TILE_Y) {
        t = (x > 0) ? map[x-1][y+1].base : TILE_BLANK;
        setTile(36,2,t,0);
        t = map[x][y+1].act;
        setTile(37,2,t,0);
        t = (x < MAX_TILE_X) ? map[x+1][y+1].base : TILE_BLANK;
        setTile(38,2,t,0);
    }
}

void displayState(uint8_t state, uint8_t stay, uint8_t act)
{
    setTile(34,4,state+48,0);
    setTile(36,4,stay+48,0);
    setTile(38,4,act+48,0);

}

void decGold()
{
    if (--goldCount <= 0) {
        completeLevel();
        if (runner.y > 0) {
            // TODO: Sound for finishing all gold
        }
    }

}

void removeGold(uint8_t x, uint8_t y)
{
    map[x][y].base = TILE_BLANK;
    setTile(x,y,TILE_BLANK,0);
}

void addGold(uint8_t x, uint8_t y)
{
    map[x][y].base = TILE_GOLD;
    setTile(x,y,TILE_GOLD,0);
}

void clearRunner() { memset(&runner, 0, sizeof(runner)); }

void initRunner(uint8_t x, uint8_t y)
{
    uint16_t xPos = x * TILE_W;
    uint16_t yPos = y * TILE_H;

    runner.x = x;
    runner.y = y;
    runner.xOffset = 0;
    runner.yOffset = 0;
    runner.action = ACT_UNKNOWN;
    runner.idx = 0;
    runner.direction = ACT_RIGHT;
    runner.sequence = RUN_SEQUENCE;

    // Sprite attribute settings
    vpoke((RUNNER_1 >> 5) & 0xff, 0x1f5000);  // Attr0
    VERA.data0 = (RUNNER_1 >> 13) & 0xf;      // Attr1
    VERA.data0 = xPos & 0xff;                 // Attr2
    VERA.data0 = xPos >> 8;                   // Attr3
    VERA.data0 = yPos & 0xff;                 // Attr4
    VERA.data0 = yPos >> 8;                   // Attr5
    VERA.data0 = (3 << 2);                    // Attr6
    VERA.data0 = 0;                           // Attr7

    vpoke(0x01, 0x1f4000);

    displayPos();
}

void runnerMoveStep(uint8_t action, uint8_t stayCurrentPos)
{
    uint8_t x = runner.x;
    int8_t xOffset = runner.xOffset;
    uint8_t y = runner.y;
    int8_t yOffset = runner.yOffset;

    uint8_t curToken = 0;
    uint8_t nextToken = 0;
    uint8_t centerX = 0;
    uint8_t centerY = 0;
    uint8_t *imgPtr = NULL;

    centerX = centerY = ACT_STOP;

    switch (action) {
        case ACT_DIG_LEFT:
        case ACT_DIG_RIGHT:
            // TODO
            xOffset = 0;
            yOffset = 0;
            break;
        case ACT_UP:
        case ACT_DOWN:
        case ACT_FALL:
            if (xOffset > 0)
                centerX = ACT_LEFT;
            else if (xOffset < 0)
                centerX = ACT_RIGHT;
            break;
        case ACT_LEFT:
        case ACT_RIGHT:
            if (yOffset > 0)
                centerY = ACT_UP;
            else if (yOffset < 0)
                centerY = ACT_DOWN;
            break;
    }

    curToken = map[x][y].base;

    if (action == ACT_UP) {
        yOffset -= 1;  // yMove

        if (stayCurrentPos && yOffset < 0) {
            // Stay on current position
            yOffset = 0;
        }
        else if (yOffset < -H2) {
            // Move to y-1 position
            if (curToken == TILE_BRICK || TILE_HIDDEN) {
                // In hole or hidden ladder
                // curToken = TILE_BLANK;
                // Debug: show when we hit this condition
                //setTile(38,21,48,0); // 0
            }
            // Runner moves to [x][y-1] so set [x][y].act to previous state
            map[x][y].act = curToken;
            y--;
            yOffset = TILE_H + yOffset;
            // TODO: guard check
            // if (map[x][y].act == TILE_GUARD && guardAlive(x,y)) setRunnerDead();
        }
        // shape = climb
        runner.sequence = CLIMB_SEQUENCE;
        // runner.idx = 0;
    }

    if (centerY == ACT_UP) {
        yOffset -= 1;  // yMove
        if (yOffset < 0)
            yOffset = 0;  // Move to center Y
    }

    if (action == ACT_DOWN || action == ACT_FALL) {
        uint8_t holdOnBar = 0;
        if (curToken == TILE_ROPE) {
            if (yOffset < 0)
                holdOnBar = 1;
            else {
                // if runner is on rope and action is down then switch to falling state
                // except if ladder or guard is below
                if (action == ACT_DOWN && y < MAX_TILE_Y && map[x][y + 1].act != TILE_LADDER &&
                    map[x][y + 1].act != TILE_GUARD) {
                    action = ACT_FALL;
                }
            }
        }

        yOffset += 1;  // yMove

        if (holdOnBar == 1 && yOffset >= 0) {
            yOffset = 0;  // fall and hold on bar
            action = ACT_FALL_BAR;
        }
        if (stayCurrentPos && yOffset > 0) {
            // Stay on current position
            yOffset = 0;
        }
        else if (yOffset > H2) {
            // Move to y + 1 position
            if (curToken == TILE_BRICK || curToken == TILE_HIDDEN) {
                // In hole or hidden ladder
                //curToken = TILE_BLANK;
                // Debug: show when we hit this condition
                //setTile(38,21,49,0); // 1
            }
            map[x][y].act = curToken;
            y++;
            yOffset = yOffset - TILE_H;
            // TODO:
            // if (map[x][y].act == TILE_GUARD && guardAlive(x,y)) setRunnerDead()
        }

        if (action == ACT_DOWN) {
            // shape = runUpDown
            runner.sequence = CLIMB_SEQUENCE;
            // runner.idx = 0;
        }
        else {
            // ACT_FALL or ACT_FALL_BAR
            if (y < MAX_TILE_Y && map[x][y + 1].act == TILE_GUARD) {
                // Over guard, not a collision
                // uint8_t id = getGuardID(x,y+1);
                // if (yOffset > guard[id].pos.yOffset) yOffset = guard[id].pos.yOffset;
            }

            if (action == ACT_FALL_BAR) {
                // Caught the rope
                runner.sequence = RAPPEL_SEQUENCE;
                runner.idx = 0;
            }
            else {
                // Falling
                runner.sequence = FALL_SEQUENCE;
                // runner.idx = 0;
            }
        }
    }

    if (centerY == ACT_DOWN) {
        yOffset += 1;  // yMove
        if (yOffset > 0)
            yOffset = 0;  // Move to center Y
    }

    if (action == ACT_LEFT) {
        xOffset -= 1;  // xMove

        if (stayCurrentPos && xOffset < 0) {
            // stay on current position
            xOffset = 0;
        }
        else if (xOffset < -W2) {
            // Move to x-1 position
            if (curToken == TILE_BRICK || curToken == TILE_LADDER) {
                // In hole or hidden ladder
                //curToken = TILE_BLANK;
                // Debug: show when we hit this condition
                //setTile(38,21,50,0); // 2
            }
            // Runner moves to map[x-1][y] so set map[x][y].act to previous state
            map[x][y].act = curToken;
            x--;
            xOffset += TILE_W;

            // if (map[x][y].act == TILE_GUARD && guardAlive(x,y)) setRunnerDead();
        }
        if (curToken == TILE_ROPE) {
            runner.sequence = RAPPEL_SEQUENCE;
            if (runner.direction != ACT_LEFT) {
                runner.direction = ACT_LEFT;
                // runner.idx = 0;
            }
        }
        else {
            runner.sequence = RUN_SEQUENCE;
            if (runner.direction != ACT_LEFT) {
                runner.direction = ACT_LEFT;
                // runner.idx = 0;
            }
        }
    }

    if (centerX == ACT_LEFT) {
        xOffset -= 1;  // xMove
        if (xOffset < 0)
            xOffset = 0;
    }

    if (action == ACT_RIGHT) {
        xOffset += 1;  // xMove

        if (stayCurrentPos && xOffset > 0) {
            // stay on current position
            xOffset = 0;
        }
        else if (xOffset > W2) {
            // Move to x+1 position
            if (curToken == TILE_BRICK || curToken == TILE_LADDER) {
                // In hole or hidden ladder
                //curToken = TILE_BLANK;
                // Debug: show when we hit this condition
                //setTile(38,21,51,0); // 3
            }
            // runner moves to map[x+1][y], so set map[x][y].act to previous state
            map[x][y].act = curToken; // runner move to [x+1][y], so set [x][y].act to previous state
            x++;
            xOffset -= TILE_W;
            // if (map[x][y].act == TILE_GUARD && guardAlive(x,y)) setRunnerDead();
        }
        if (curToken == TILE_ROPE) {
            runner.sequence = RAPPEL_SEQUENCE;
            if (runner.direction != ACT_RIGHT) {
                runner.direction = ACT_RIGHT;
                // runner.idx = 0;
            }
        }
        else {
            runner.sequence = RUN_SEQUENCE;
            if (runner.direction != ACT_RIGHT) {
                runner.direction = ACT_RIGHT;
                // runner.idx = 0;
            }
        }
    }

    if (centerX == ACT_RIGHT) {
        xOffset += 1;  // xMove
        if (xOffset > 0)
            xOffset = 0;
    }

    if (action == ACT_STOP) {
        // TODO: stop falling sound
        // if (runner.action == ACT_FALL) {
        //    
        //}
        if (runner.action != ACT_STOP) {
            runner.action = ACT_STOP;
        }
    }
    else {
        uint16_t xPos = x * TILE_W + xOffset;
        uint16_t yPos = y * TILE_H + yOffset;
        uint8_t dir = (runner.direction == ACT_LEFT) ? 1 : 0;

        // Update the sprite image and flip bit based on the current sequence and index
        runner.idx++;
        runner.idx = runner.idx % runnerSeqSizes[runner.sequence];
        vpoke(runnerSequences[runner.sequence][runner.idx], 0x1f5000);
        vpoke((3 << 2) | dir, 0x1f5006);

        // sprite x position
        vpoke(xPos & 0xff, 0x1f5002);
        VERA.data0 = xPos >> 8;

        // sprite y position
        vpoke(yPos & 0xff, 0x1f5004);
        VERA.data0 = yPos >> 8;

        runner.x = x;
        runner.y = y;
        runner.xOffset = xOffset;
        runner.yOffset = yOffset;

        displayPos();
    }
    if (action == ACT_LEFT || action == ACT_RIGHT) runner.direction = action;
    map[x][y].act = TILE_RUNNER;

    if (map[x][y].base == TILE_GOLD &&
        ((!xOffset && yOffset >0 && yOffset < H4) ||
         (!yOffset && xOffset >= 0 && xOffset < W4) ||
         (y < MAX_TILE_Y && map[x][y+1].base == TILE_LADDER && yOffset < H4))) {
        removeGold(x,y);
        decGold();
        displayScore(SCORE_GET_GOLD);
        setTile(38,20,goldCount+48,0);
    }
}

void moveRunner()
{
    uint8_t x = runner.x;
    int8_t xOffset = runner.xOffset;
    uint8_t y = runner.y;
    int8_t yOffset = runner.yOffset;

    uint8_t curState = 0;
    uint8_t curToken = map[x][y].base;
    uint8_t nextToken = 0;

    int8_t act = 0;
    uint8_t stayCurrentPos = 0;
    uint8_t moveStep = 0;

    displayPos();
    displayTiles(x,y);

    if (curToken == TILE_LADDER || (curToken == TILE_ROPE && yOffset == 0)) {
        // OK to move (on ladder or bar)
        curState = STATE_OK_TO_MOVE;
    }
    else if (yOffset < 0) {
        // no ladder and yOffset < 0 ==> falling
        curState = STATE_FALLING;
    }
    else if (y < MAX_TILE_Y) {
        // No ladder and y < maxTileY and yOffset >= 0
        nextToken = map[x][y + 1].act;

        if (nextToken == TILE_BLANK) {
            curState = STATE_FALLING;
        }
        else if (nextToken == TILE_BLOCK || nextToken == TILE_LADDER || nextToken == TILE_BRICK) {
            curState = STATE_OK_TO_MOVE;
        }
        else if (nextToken == TILE_GUARD) {
            curState = STATE_OK_TO_MOVE;
        }
        else {
            curState = STATE_FALLING;
        }
    }
    else {
        // No ladder and y == maxTileY
        curState = STATE_OK_TO_MOVE;
    }

    if (curState == STATE_FALLING) {
        nextToken = map[x][y + 1].act;
        stayCurrentPos = (y >= MAX_TILE_Y || nextToken == TILE_BRICK || nextToken == TILE_BLOCK || nextToken == TILE_GUARD);

        // Debug: display state for falling runner
        displayState(curState,stayCurrentPos,ACT_FALL);
        displayPos();

        runnerMoveStep(ACT_FALL, stayCurrentPos);
        return;
    }

    // Check key action
    act = keyAction();
    // Debug: show key action @ 38,3
    // if (act != ACT_UNKNOWN) {
    //     setTile(38, 3, act + 48, 0);
    // }
    stayCurrentPos = 1;
    switch (act) {
        case ACT_UP:
            nextToken = map[x][y - 1].act;
            stayCurrentPos = (y <= 0 || nextToken == TILE_BRICK || nextToken == TILE_BLOCK || nextToken == TILE_TRAP);

            if (y > 0 && map[x][y].base != TILE_LADDER && yOffset < H4 && yOffset > 0 && map[x][y + 1].base == TILE_LADDER) {
                stayCurrentPos = 1;
                moveStep = ACT_UP;
            }
            else if (!(map[x][y].base != TILE_LADDER && (yOffset <= 0 || map[x][y + 1].base != TILE_LADDER) ||
                       (yOffset <= 0 && stayCurrentPos))) {
                moveStep = ACT_UP;
            }

            break;
        case ACT_DOWN:
            nextToken = map[x][y + 1].act;
            stayCurrentPos = (y >= MAX_TILE_Y || nextToken == TILE_BRICK || nextToken == TILE_BLOCK);

            if (!(yOffset >= 0 && stayCurrentPos)) {
                moveStep = ACT_DOWN;
            }
            break;
        case ACT_LEFT:
            nextToken = map[x - 1][y].act;
            stayCurrentPos = (x <= 0 || nextToken == TILE_BRICK || nextToken == TILE_BLOCK || nextToken == TILE_TRAP);

            if (!(xOffset <= 0 && stayCurrentPos)) {
                moveStep = ACT_LEFT;
            }
            break;
        case ACT_RIGHT:
            nextToken = map[x + 1][y].act;
            stayCurrentPos = (x >= MAX_TILE_X || nextToken == TILE_BRICK || nextToken == TILE_BLOCK || nextToken == TILE_TRAP);

            if (!(xOffset >= 0 && stayCurrentPos)) {
                moveStep = ACT_RIGHT;
            }
            break;
        case ACT_DIG_LEFT:
        case ACT_DIG_RIGHT:
            // TODO
            break;
        default:
            break;
    }
    // Debug: show moveStep and stayCurrentPos @ 36,4 and 38,4
    //setTile(36, 4, moveStep + 48, 0);
    displayState(curState,stayCurrentPos,moveStep);
    runnerMoveStep(moveStep, stayCurrentPos);
}

