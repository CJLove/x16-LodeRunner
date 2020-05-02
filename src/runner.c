
#include "runner.h"
#include "guard.h"
#include "key.h"
#include "levels.h"
#include "loderunner.h"
#include "sound.h"
#include <cx16.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>



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

#define DIG_LENGTH 12
#define DIG_UPPER 0
#define DIG_LOWER 1

static uint8_t digLeft[2][DIG_LENGTH] = {
    // DIG_UPPER
    { TILE_BLANK, TILE_DIG_LEFT_U1, TILE_DIG_LEFT_U1, TILE_DIG_LEFT_U2, 
      TILE_DIG_LEFT_U2, TILE_DIG_LEFT_U3, TILE_DIG_LEFT_U4, TILE_DIG_LEFT_U4, 
      TILE_DIG_LEFT_U5, TILE_DIG_LEFT_U6, TILE_DIG_LEFT_U6, TILE_BLANK },
    // DIG_LOWER
    { TILE_BRICK, TILE_DIG_LEFT_L1, TILE_DIG_LEFT_L1, TILE_DIG_LEFT_L2, 
      TILE_DIG_LEFT_L2, TILE_DIG_LEFT_L3, TILE_DIG_LEFT_L4, TILE_DIG_LEFT_L4,
      TILE_DIG_LEFT_L5, TILE_DIG_LEFT_L6, TILE_DIG_LEFT_L6, TILE_BLANK },
};

static uint8_t digRight[2][DIG_LENGTH] = {
    // DIG_UPPER
    { TILE_BLANK, TILE_DIG_RIGHT_U1, TILE_DIG_RIGHT_U1, TILE_DIG_RIGHT_U2, 
      TILE_DIG_RIGHT_U2, TILE_DIG_RIGHT_U3, TILE_DIG_RIGHT_U4, TILE_DIG_RIGHT_U4,
      TILE_DIG_RIGHT_U5, TILE_DIG_RIGHT_U6, TILE_DIG_RIGHT_U6, TILE_BLANK },
    // DIG_LOWER
    { TILE_BRICK, TILE_DIG_RIGHT_L1, TILE_DIG_RIGHT_L1, TILE_DIG_RIGHT_L2, 
      TILE_DIG_RIGHT_L2, TILE_DIG_RIGHT_L3, TILE_DIG_RIGHT_L4, TILE_DIG_RIGHT_L4,
      TILE_DIG_RIGHT_L5, TILE_DIG_RIGHT_L6, TILE_DIG_RIGHT_L6, TILE_BLANK },
};

#define STATE_OK_TO_MOVE 1
#define STATE_FALLING 2

// Define this to enable runner debug information
// #define DEBUG
// Define this to debug hole fill (NOTE: slows down the game)
//#define DEBUG_FILL

#ifdef DEBUG
// Debug: display runner x & xOffset, y & yOffset
void displayPos()
{
    char buffer[8];
    uint8_t i = 0;
    sprintf(buffer, "%c:%2u %2d", 24, runner.x, runner.xOffset);
    for (i = 0; i < 8; i++) {
        setTile(30 + i, 6, buffer[i], 0);
    }

    sprintf(buffer, "%c:%2u %2d", 25, runner.y, runner.yOffset);
    for (i = 0; i < 8; i++) {
        setTile(30 + i, 8, buffer[i], 0);
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

// Debug: display state regarding runner movement
void displayState(uint8_t state, uint8_t stay, uint8_t act)
{
    setTile(34,4,state+48,0);
    setTile(36,4,stay+48,0);
    setTile(38,4,act+48,0);
}

// Debug: display state regarding digging
void displayDig()
{
    char buffer[15];
    uint8_t i = 0;
    if (hole.action) {
        // Format: "A:x xx xx xx"
        sprintf(buffer,"%c:%1d %2d %2d %2d",1,hole.action, hole.idx, hole.x, hole.y);
    
        for (i = 0; i < 12; i++) {
            setTile(28+i,10,buffer[i],0);
        }
    }
}

// Debug: display state regarding hole filling (expensive!)
void displayFill()
{
    char buffer[15];
    uint8_t i = 0;
    uint8_t j = 0;
    uint8_t r = 11;
    for (i = 0; i < MAX_HOLES; i++) {
        if (holes[i].active) {
            // Format: X:x Y:y ccc 
            sprintf(buffer,"%c:%d%c:%d %03d",24,holes[i].x,25,holes[i].y,holes[i].count);
            for (j = 0; j < 12; j++) {
                setTile(28+j,r,buffer[j],0);
            }
            r++;
        }
    }
    for (; r < 30; r++) {
        for (j = 0; j < 12; j++) {
            setTile(28+j,r,0,0);
        }
    }

}

// Debug: display gold count
void displayGold()
{
    uint8_t i;
    char buffer[5];
    sprintf(buffer,"%02d %d",currentLevel.goldCount, currentLevel.goldComplete);
    for (i = 0; i < 4; i++) {
        setTile(36+i,20,buffer[i],0);
    }
}
#endif

void decGold()
{
    currentLevel.goldCount--;
    if (currentLevel.goldCount <= 0) {
        completeLevel();
        if (runner.y > 0) {
            // TODO: Sound for finishing all gold
        }
    }
#ifdef DEBUG        
    displayGold();
#endif
}

void removeGold(uint8_t x, uint8_t y)
{
    map[x][y].base = TILE_BLANK;
    setTile(x,y,TILE_BLANK,0);
#ifdef DEBUG        
    displayGold();
#endif
}

void addGold(uint8_t x, uint8_t y)
{
    map[x][y].base = TILE_GOLD;
    setTile(x,y,TILE_GOLD,0);
#ifdef DEBUG        
    displayGold();
#endif
}

void setRunnerDead()
{
    if (currentGame.godMode != GOD_MODE) {
        stopAllSoundFx();
        playDeadFx();
        currentGame.gameState = GAME_RUNNER_DEAD;
    }
}

uint8_t ok2dig(uint8_t action)
{
    uint8_t x = runner.x;
    uint8_t y = runner.y;
    uint8_t rc = 0;
    switch (action) {
        case ACT_DIG_LEFT:
            if (y < MAX_TILE_Y && x > 0 && map[x-1][y+1].act == TILE_BRICK && map[x-1][y].act == TILE_BLANK && map[x-1][y].base != TILE_GOLD) {
                rc = 1;
            }
            break;
        case ACT_DIG_RIGHT:
            if (y < MAX_TILE_Y && x < MAX_TILE_X && map[x+1][y+1].act == TILE_BRICK && map[x+1][y].act == TILE_BLANK && map[x+1][y].base != TILE_GOLD) {
                rc = 1;
            }
            break;
        default:
            break;
    }
    return rc;
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
    vpoke((RUNNER_1 >> 5) & 0xff, 0x11fc00);  // Attr0
    vpoke((RUNNER_1 >> 13) & 0xf, 0x1fc01);  // Attr1
    vpoke(xPos & 0xff,0x1fc02);              // Attr2
    vpoke(xPos >> 8, 0x1fc03);               // Attr3
    vpoke(yPos & 0xff, 0x1fc04);             // Attr4
    vpoke(yPos >> 8, 0x1fc05);               // Attr5
    vpoke((3 << 2), 0x1fc06);                // Attr6
    vpoke(0, 0x1fc07);                       // Attr7
    // VERA.data0 = (RUNNER_1 >> 13) & 0xf;      // Attr1
    // VERA.data0 = xPos & 0xff;                 // Attr2
    // VERA.data0 = xPos >> 8;                   // Attr3
    // VERA.data0 = yPos & 0xff;                 // Attr4
    // VERA.data0 = yPos >> 8;                   // Attr5
    // VERA.data0 = (3 << 2);                    // Attr6
    // VERA.data0 = 0;                           // Attr7
    VERA.dc_video |= 0x40;

#ifdef DEBUG
    displayPos();
#endif
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

    //printf("Here w/x=%d y=%d action=%d stayCurrentPos=%d\n",x,y,action,stayCurrentPos);
    
    centerX = centerY = ACT_STOP;

    switch (action) {
        case ACT_DIG_LEFT:
        case ACT_DIG_RIGHT:
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
                // Note: This somehow breaks climbing up the last ladder tile
                //curToken = TILE_BLANK;
            }
            // Runner moves to [x][y-1] so set [x][y].act to previous state
            map[x][y].act = curToken;
            y--;
            yOffset = TILE_H + yOffset;
            if (map[x][y].act == TILE_GUARD && guardAlive(x,y)) setRunnerDead();
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
                curToken = TILE_BLANK;
            }
            map[x][y].act = curToken;
            y++;
            yOffset = yOffset - TILE_H;
            if (map[x][y].act == TILE_GUARD && guardAlive(x,y)) setRunnerDead();
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
            if (curToken == TILE_BRICK) { // || curToken == TILE_LADDER) {
                // In hole or hidden ladder
                curToken = TILE_BLANK;
                // Debug: show when we hit this condition
            }
            // Runner moves to map[x-1][y] so set map[x][y].act to previous state
            map[x][y].act = curToken;
            x--;
            xOffset += TILE_W;

            if (map[x][y].act == TILE_GUARD && guardAlive(x,y)) setRunnerDead();
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
            if (curToken == TILE_BRICK) { // || curToken == TILE_LADDER) {
                // In hole or hidden ladder
                curToken = TILE_BLANK;
            }
            // runner moves to map[x+1][y], so set map[x][y].act to previous state
            map[x][y].act = curToken; // runner move to [x+1][y], so set [x][y].act to previous state
            x++;
            xOffset -= TILE_W;
            if (map[x][y].act == TILE_GUARD && guardAlive(x,y)) setRunnerDead();
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
        // stop falling sound
        if (runner.action == ACT_FALL) {
            stopFallingFx();    
        }
    
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
        vpoke(runnerSequences[runner.sequence][runner.idx], 0x1fc00);
        vpoke((3 << 2) | dir, 0x1fc06);

        // sprite x position
        vpoke(xPos & 0xff, 0x1fc02);
        vpoke(xPos >> 8, 0x1fc03);

        // sprite y position
        vpoke(yPos & 0xff, 0x1fc04);
        vpoke(yPos >> 8, 0x1fc05);

        runner.x = x;
        runner.y = y;
        runner.xOffset = xOffset;
        runner.yOffset = yOffset;

        if (action != runner.action) {
            if (runner.action == ACT_FALL) {
                stopFallingFx();
            } else if (action == ACT_FALL) {
                playFallingFx();
            }
        }
        runner.action = action;
#ifdef DEBUG
        displayPos();
#endif
    }
    if (action == ACT_LEFT || action == ACT_RIGHT) runner.direction = action;
    map[x][y].act = TILE_RUNNER;

    if (map[x][y].base == TILE_GOLD &&
        ((!xOffset && yOffset >0 && yOffset < H4) ||
         (!yOffset && xOffset >= 0 && xOffset < W4) ||
         (y < MAX_TILE_Y && map[x][y+1].base == TILE_LADDER && yOffset < H4))) {
        removeGold(x,y);
        decGold();
        playGoldFx();
        displayScore(SCORE_GET_GOLD);
    }
}

void digHole(uint8_t action)
{
    uint8_t x = 0;
    uint8_t y = 0;
    if (action == ACT_DIG_LEFT) {
        x = runner.x-1;
        y = runner.y;
    } else { // Dig right
        x = runner.x+1;
        y = runner.y;
    }
    hole.action = action;
    hole.x = x;
    hole.y = y;
    hole.idx = 0;
    playDiggingFx();
}

void fillHole(uint8_t x, uint8_t y)
{
    uint8_t i = 0;
    for (i = 0; i < MAX_HOLES; i++) {
        if (!holes[i].active) {
            holes[i].active = 1;
            holes[i].x = x;
            holes[i].y = y;
            holes[i].count = 0;
            break;
        }
    }
}

void digComplete()
{
    uint8_t x = hole.x;
    uint8_t y = hole.y+1;
    map[x][y].act = TILE_BLANK;
    // No longer digging
    hole.action = ACT_STOP;

    fillHole(x,y);
}

// Routine called by main loop to move the runner
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

#ifdef DEBUG
    displayPos();
    displayTiles(x,y);
#endif
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
#ifdef DEBUG
        displayState(curState,stayCurrentPos,ACT_FALL);
        displayPos();
#endif
        runnerMoveStep(ACT_FALL, stayCurrentPos);
        return;
    }

    // Check key action
    act = inputHandler();// keyAction();
    // Debug: show key action @ 38,3
    // if (act != ACT_UNKNOWN) {
    //     setTile(38, 3, act + 48, 0);
    // }
    stayCurrentPos = 1;
    switch (act) {
        case ACT_START:
            setRunnerDead();
            break;
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
            if (ok2dig(act)) {
                runnerMoveStep(act, stayCurrentPos);
                digHole(act);
            } else {
                runnerMoveStep(ACT_STOP, stayCurrentPos);
            }
            return;
        default:
            break;
    }
    // Debug: show moveStep and stayCurrentPos @ 36,4 and 38,4
#ifdef DEBUG    
    displayState(curState,stayCurrentPos,moveStep);
#endif

    runnerMoveStep(moveStep, stayCurrentPos);
}

void fillComplete(uint8_t holeIdx)
{
    uint8_t x = holes[holeIdx].x;
    uint8_t y = holes[holeIdx].y;

    // Debug: show the tile for anything currently in the hole
    //setTile(1,20,map[x][y].act,0);

    switch (map[x][y].act) {
        case TILE_RUNNER:
            // Runner dead
            setRunnerDead();
            break;
        case TILE_GUARD:
            {
                // Guard dead
                int8_t id = guardId(x,y);
                removeFromShake(id);
                if (id != -1) {
                    if (guard[id].hasGold > 0) {
                        decGold();
                        guard[id].hasGold = 0;
                    }
                }
                guardReborn(x,y);
                displayScore(SCORE_GUARD_DEAD);
            }
            break;
    }
    // Restore the tile in the map
    map[x][y].act = TILE_BRICK;
    // Hole is no longer active
    holes[holeIdx].active = 0;
}

// Routine called by the main loop to fill holes dug by the runner
void processFillHole()
{
    uint8_t i = 0;
#ifdef DEBUG_FILL
    displayFill();
#endif
    for (i = 0; i < MAX_HOLES; i++) {
        if (holes[i].active) {
            uint8_t x = holes[i].x;
            uint8_t y = holes[i].y;
            holes[i].count++;
            if (holes[i].count == 1) {
                // Clear the blast debris from the tile above the hole
                setTile(x,y-1,TILE_BLANK,0);
            } else if (holes[i].count == HOLE_REGEN1) {
                setTile(x,y,TILE_REGEN1,0);
            } else if (holes[i].count == HOLE_REGEN2) {
                setTile(x,y,TILE_REGEN2,0);
            } else if (holes[i].count == HOLE_REGEN3) {
                setTile(x,y,TILE_REGEN3,0);
            } else if (holes[i].count >= HOLE_REGEN4) {
                setTile(x,y,TILE_BRICK,0);
                fillComplete(i);
            }
        }
    }

}

void stopDigging(uint8_t x, uint8_t y)
{
    // Disable hole
    hole.action = ACT_STOP;

    // Fill hole and clear tile above hole
    y++;
    map[x][y].act = map[x][y].base; // TILE_BRICK
    setTile(x,y,TILE_BRICK,0);
    setTile(x,y-1,TILE_BLANK,0);

    // Change runner action
    runner.action = ACT_STOP;

    // stop sound of digging
    stopDiggingFx();
}

uint8_t isDigging()
{
    uint8_t rc = 0;
    // Debug: display info regarding any dig currently in process
#ifdef DEBUG    
    displayDig();
#endif
    if (hole.action != ACT_STOP) {
        uint8_t x = hole.x;
        uint8_t y = hole.y;
        if (map[x][y].act == TILE_GUARD) {
            uint8_t id = guardId(x,y);
            if (hole.idx < DIG_LENGTH && guard[id].yOffset > -H4) {
                // Check if guard is too close to the digging
                stopDigging(x,y);
            } else {
                map[x][y].act = TILE_BLANK; // assume hole complete
                rc = 1;
            }
        } else {
            // No need to change runner image (run left or run right)
            runner.action = ACT_STOP;
            rc = 1;
        }
    }
    return rc;
}

void processDigHole()
{
    // Do nothing if we aren't digging
    if (hole.action == ACT_STOP) return;

    hole.idx++;
    if (hole.idx < DIG_LENGTH) {
        if (hole.action == ACT_DIG_LEFT) {
            setTile(hole.x,hole.y,digLeft[DIG_UPPER][hole.idx],0);
            setTile(hole.x,hole.y+1,digLeft[DIG_LOWER][hole.idx],0);
        } else {
            setTile(hole.x,hole.y,digRight[DIG_UPPER][hole.idx],0);
            setTile(hole.x,hole.y+1,digRight[DIG_LOWER][hole.idx],0);
        }
    } else {
        digComplete();
    }

}
