#include "levels.h"
#include "loderunner.h"
#include "runner.h"
#include "x16test.h"

// Stubs
void setTile(uint8_t x, uint8_t y, uint8_t tile, uint8_t paletteOffset) {}
void stopAllSoundFx(void) {}
void stopDiggingFx(void) {}
void stopFallingFx(void) {}
void playDeadFx(void) {}
void playDiggingFx(void) {}
void playFallingFx(void) {}
void playGoldFx(void) {}
void clearGuards(void) {}
int8_t guardAlive(uint8_t x, uint8_t y) { return 0; }
int8_t guardId(uint8_t x, uint8_t y) { return 0; }
void removeFromShake(uint8_t id) {}
void guardReborn(uint8_t x, uint8_t y) {}
uint8_t initGuard(uint8_t x, uint8_t y) { return 0; }

struct testInput_t {
    uint8_t idx;
    int8_t input[];
};

struct testInput_t *inputPtr = NULL;

static int8_t testAction()
{
    if (inputPtr) {
        uint8_t idx = (inputPtr->idx++);
        int8_t action = inputPtr->input[inputPtr->idx];
        inputPtr->idx = idx;
        return action;
    }
    return ACT_UNKNOWN;
}

static struct testInput_t testLeft = 
{
    0, 
    {ACT_LEFT, ACT_LEFT, ACT_LEFT, ACT_LEFT, ACT_LEFT, ACT_LEFT, ACT_LEFT, ACT_LEFT, ACT_LEFT }
};

static struct testInput_t testRight = 
{
    0, 
    {ACT_RIGHT, ACT_RIGHT, ACT_RIGHT, ACT_RIGHT, ACT_RIGHT, ACT_RIGHT, ACT_RIGHT, ACT_RIGHT, ACT_RIGHT }
};

int testRunnerLeft()
{
    TEST_INIT()
    // Change to the test input handler
    inputHandler = testAction;
    inputPtr = &testLeft;

    {
        // 3x3 subset of map[][] for testing with no obstruction
        map[1][1].base = map[2][1].base = map[3][1].base = TILE_BLANK;
        map[1][1].act = map[2][1].act = map[3][1].act = TILE_BLANK;
        map[1][2].base = TILE_BLANK;
        map[2][2].base = TILE_RUNNER;
        map[3][2].base = TILE_BLANK;
        map[1][2].act = map[2][2].act = map[3][2].act = TILE_BLANK;
        map[1][3].base = map[2][3].base = map[3][3].base = TILE_BRICK;
        map[1][3].act = map[2][3].act = map[3][3].act = TILE_BRICK;
        runner.x = runner.y = 2;
        runner.xOffset = runner.yOffset = 0;

        moveRunner();
        EXPECT_EQ(2,runner.x);
        EXPECT_EQ(2,runner.y);
        EXPECT_EQ(-1,runner.xOffset);
        EXPECT_EQ(0,runner.yOffset);

        moveRunner();
        EXPECT_EQ(2,runner.x);
        EXPECT_EQ(2,runner.y);
        EXPECT_EQ(-2,runner.xOffset);
        EXPECT_EQ(0,runner.yOffset);        

        moveRunner();
        EXPECT_EQ(2,runner.x);
        EXPECT_EQ(2,runner.y);
        EXPECT_EQ(-3,runner.xOffset);
        EXPECT_EQ(0,runner.yOffset);        

        moveRunner();
        EXPECT_EQ(2,runner.x);
        EXPECT_EQ(2,runner.y);
        EXPECT_EQ(-4,runner.xOffset);
        EXPECT_EQ(0,runner.yOffset);        

        moveRunner();
        EXPECT_EQ(1,runner.x);
        EXPECT_EQ(2,runner.y);
        EXPECT_EQ(3,runner.xOffset);
        EXPECT_EQ(0,runner.yOffset);  

        moveRunner();
        EXPECT_EQ(1,runner.x);
        EXPECT_EQ(2,runner.y);
        EXPECT_EQ(2,runner.xOffset);
        EXPECT_EQ(0,runner.yOffset);         

        moveRunner();
        EXPECT_EQ(1,runner.x);
        EXPECT_EQ(2,runner.y);
        EXPECT_EQ(1,runner.xOffset);
        EXPECT_EQ(0,runner.yOffset);         

        moveRunner();
        EXPECT_EQ(1,runner.x);
        EXPECT_EQ(2,runner.y);
        EXPECT_EQ(0,runner.xOffset);
        EXPECT_EQ(0,runner.yOffset);     
   
    }
    {
        testLeft.idx = 0;

        // 3x3 subset of map[][] for testing with rope and no obstruction
        map[1][1].base = map[2][1].base = map[3][1].base = TILE_BLANK;
        map[1][1].act = map[2][1].act = map[3][1].act = TILE_BLANK;
        map[1][2].base = TILE_ROPE;
        map[2][2].base = TILE_RUNNER;
        map[3][2].base = TILE_ROPE;
        map[1][2].act = map[2][2].act = map[3][2].act = TILE_ROPE;
        map[1][3].base = map[2][3].base = map[3][3].base = TILE_BRICK;
        map[1][3].act = map[2][3].act = map[3][3].act = TILE_BRICK;
        runner.x = runner.y = 2;
        runner.xOffset = runner.yOffset = 0;

        moveRunner();
        EXPECT_EQ(2,runner.x);
        EXPECT_EQ(2,runner.y);
        EXPECT_EQ(-1,runner.xOffset);
        EXPECT_EQ(0,runner.yOffset);

        moveRunner();
        EXPECT_EQ(2,runner.x);
        EXPECT_EQ(2,runner.y);
        EXPECT_EQ(-2,runner.xOffset);
        EXPECT_EQ(0,runner.yOffset);        

        moveRunner();
        EXPECT_EQ(2,runner.x);
        EXPECT_EQ(2,runner.y);
        EXPECT_EQ(-3,runner.xOffset);
        EXPECT_EQ(0,runner.yOffset);        

        moveRunner();
        EXPECT_EQ(2,runner.x);
        EXPECT_EQ(2,runner.y);
        EXPECT_EQ(-4,runner.xOffset);
        EXPECT_EQ(0,runner.yOffset);        

        moveRunner();
        EXPECT_EQ(1,runner.x);
        EXPECT_EQ(2,runner.y);
        EXPECT_EQ(3,runner.xOffset);
        EXPECT_EQ(0,runner.yOffset);  

        moveRunner();
        EXPECT_EQ(1,runner.x);
        EXPECT_EQ(2,runner.y);
        EXPECT_EQ(2,runner.xOffset);
        EXPECT_EQ(0,runner.yOffset);         

        moveRunner();
        EXPECT_EQ(1,runner.x);
        EXPECT_EQ(2,runner.y);
        EXPECT_EQ(1,runner.xOffset);
        EXPECT_EQ(0,runner.yOffset);         

        moveRunner();
        EXPECT_EQ(1,runner.x);
        EXPECT_EQ(2,runner.y);
        EXPECT_EQ(0,runner.xOffset);
        EXPECT_EQ(0,runner.yOffset);     
   
    }
    {
        testLeft.idx = 0;

        // 3x3 subset of map[][] for testing with hole to left
        map[1][1].base = map[2][1].base = map[3][1].base = TILE_BLANK;
        map[1][1].act = map[2][1].act = map[3][1].act = TILE_BLANK;
        map[1][2].base = TILE_BRICK;
        map[2][2].base = TILE_RUNNER;
        map[3][2].base = TILE_BLANK;
        map[1][2].act = map[2][2].act = map[3][2].act = TILE_BLANK;
        map[1][3].base = map[2][3].base = map[3][3].base = TILE_BRICK;
        map[1][3].act = map[2][3].act = map[3][3].act = TILE_BRICK;
        runner.x = runner.y = 2;
        runner.xOffset = runner.yOffset = 0;

        moveRunner();
        EXPECT_EQ(2,runner.x);
        EXPECT_EQ(2,runner.y);
        EXPECT_EQ(-1,runner.xOffset);
        EXPECT_EQ(0,runner.yOffset);

        moveRunner();
        EXPECT_EQ(2,runner.x);
        EXPECT_EQ(2,runner.y);
        EXPECT_EQ(-2,runner.xOffset);
        EXPECT_EQ(0,runner.yOffset);        

        moveRunner();
        EXPECT_EQ(2,runner.x);
        EXPECT_EQ(2,runner.y);
        EXPECT_EQ(-3,runner.xOffset);
        EXPECT_EQ(0,runner.yOffset);        

        moveRunner();
        EXPECT_EQ(2,runner.x);
        EXPECT_EQ(2,runner.y);
        EXPECT_EQ(-4,runner.xOffset);
        EXPECT_EQ(0,runner.yOffset);        

        moveRunner();
        EXPECT_EQ(1,runner.x);
        EXPECT_EQ(2,runner.y);
        EXPECT_EQ(3,runner.xOffset);
        EXPECT_EQ(0,runner.yOffset);   
    }
    {
        testLeft.idx = 0;

        // 3x3 subset of map[][] for testing with rope to left
        map[1][1].base = map[2][1].base = map[3][1].base = TILE_BLANK;
        map[1][1].act = map[2][1].act = map[3][1].act = TILE_BLANK;
        map[1][2].base = TILE_ROPE;
        map[2][2].base = TILE_RUNNER;
        map[3][2].base = TILE_BLANK;
        map[1][2].act = TILE_ROPE; map[2][2].act = map[3][2].act = TILE_BLANK;
        map[1][3].base = map[2][3].base = map[3][3].base = TILE_BRICK;
        map[1][3].act = map[2][3].act = map[3][3].act = TILE_BRICK;
        runner.x = runner.y = 2;
        runner.xOffset = runner.yOffset = 0;

        moveRunner();
        EXPECT_EQ(2,runner.x);
        EXPECT_EQ(2,runner.y);
        EXPECT_EQ(-1,runner.xOffset);
        EXPECT_EQ(0,runner.yOffset);

        moveRunner();
        EXPECT_EQ(2,runner.x);
        EXPECT_EQ(2,runner.y);
        EXPECT_EQ(-2,runner.xOffset);
        EXPECT_EQ(0,runner.yOffset);        

        moveRunner();
        EXPECT_EQ(2,runner.x);
        EXPECT_EQ(2,runner.y);
        EXPECT_EQ(-3,runner.xOffset);
        EXPECT_EQ(0,runner.yOffset);        

        moveRunner();
        EXPECT_EQ(2,runner.x);
        EXPECT_EQ(2,runner.y);
        EXPECT_EQ(-4,runner.xOffset);
        EXPECT_EQ(0,runner.yOffset);        

        moveRunner();
        EXPECT_EQ(1,runner.x);
        EXPECT_EQ(2,runner.y);
        EXPECT_EQ(3,runner.xOffset);
        EXPECT_EQ(0,runner.yOffset);   
    }
    {
        testLeft.idx = 0;

        // 3x3 subset of map[][] for testing with gold to left
        map[1][1].base = map[2][1].base = map[3][1].base = TILE_BLANK;
        map[1][1].act = map[2][1].act = map[3][1].act = TILE_BLANK;
        map[1][2].base = TILE_GOLD;
        map[2][2].base = TILE_RUNNER;
        map[3][2].base = TILE_BLANK;
        map[1][2].act = TILE_GOLD; map[2][2].act = map[3][2].act = TILE_BLANK;
        map[1][3].base = map[2][3].base = map[3][3].base = TILE_BRICK;
        map[1][3].act = map[2][3].act = map[3][3].act = TILE_BRICK;
        runner.x = runner.y = 2;
        runner.xOffset = runner.yOffset = 0;

        moveRunner();
        EXPECT_EQ(2,runner.x);
        EXPECT_EQ(2,runner.y);
        EXPECT_EQ(-1,runner.xOffset);
        EXPECT_EQ(0,runner.yOffset);

        moveRunner();
        EXPECT_EQ(2,runner.x);
        EXPECT_EQ(2,runner.y);
        EXPECT_EQ(-2,runner.xOffset);
        EXPECT_EQ(0,runner.yOffset);        

        moveRunner();
        EXPECT_EQ(2,runner.x);
        EXPECT_EQ(2,runner.y);
        EXPECT_EQ(-3,runner.xOffset);
        EXPECT_EQ(0,runner.yOffset);        

        moveRunner();
        EXPECT_EQ(2,runner.x);
        EXPECT_EQ(2,runner.y);
        EXPECT_EQ(-4,runner.xOffset);
        EXPECT_EQ(0,runner.yOffset);        

        moveRunner();
        EXPECT_EQ(1,runner.x);
        EXPECT_EQ(2,runner.y);
        EXPECT_EQ(3,runner.xOffset);
        EXPECT_EQ(0,runner.yOffset);   
    }
#if 0
    {
        testLeft.idx = 0;

        // 3x3 subset of map[][] for testing vertical centering  on move left
        map[1][1].base = TILE_BLANK; map[2][1].base = TILE_RUNNER; map[3][1].base = TILE_BLANK;
        map[1][1].act = TILE_BLANK; map[2][1].act = TILE_LADDER; map[3][1].act = TILE_BLANK;
        map[1][2].base = TILE_BLANK;
        map[2][2].base = TILE_BRICK;
        map[3][2].base = TILE_BLANK;
        map[1][2].act = map[2][2].act = map[3][2].act = TILE_BRICK;
        map[1][3].base = map[2][3].base = map[3][3].base = TILE_BRICK;
        map[1][3].act = map[2][3].act = map[3][3].act = TILE_BRICK;
        runner.x = 2;
        runner.y = 1;
        runner.xOffset = 0;
        runner.yOffset = 3;

        moveRunner();
        EXPECT_EQ(2,runner.x);
        EXPECT_EQ(1,runner.y);
        EXPECT_EQ(-1,runner.xOffset);
        EXPECT_EQ(2,runner.yOffset);

        moveRunner();
        EXPECT_EQ(2,runner.x);
        EXPECT_EQ(1,runner.y);
        EXPECT_EQ(-2,runner.xOffset);
        EXPECT_EQ(1,runner.yOffset);        

        moveRunner();
        EXPECT_EQ(2,runner.x);
        EXPECT_EQ(1,runner.y);
        EXPECT_EQ(-3,runner.xOffset);
        EXPECT_EQ(0,runner.yOffset);        

        moveRunner();
        EXPECT_EQ(2,runner.x);
        EXPECT_EQ(1,runner.y);
        EXPECT_EQ(-4,runner.xOffset);
        EXPECT_EQ(0,runner.yOffset);        

        moveRunner();
        EXPECT_EQ(1,runner.x);
        EXPECT_EQ(2,runner.y);
        EXPECT_EQ(3,runner.xOffset);
        EXPECT_EQ(0,runner.yOffset);   
    }
#endif
    {
        testLeft.idx = 0;

        // Negative test: 3x3 subset of map[][] for testing w/brick to left
        map[1][1].base = map[2][1].base = map[3][1].base = TILE_BLANK;
        map[1][1].act = map[2][1].act = map[3][1].act = TILE_BLANK;
        map[1][2].base = TILE_BRICK;
        map[2][2].base = TILE_RUNNER;
        map[3][2].base = TILE_BLANK;
        map[1][2].act = TILE_BRICK; map[2][2].act = map[3][2].act = TILE_BLANK;
        map[1][3].base = map[2][3].base = map[3][3].base = TILE_BRICK;
        map[1][3].act = map[2][3].act = map[3][3].act = TILE_BRICK;
        runner.x = runner.y = 2;
        runner.xOffset = runner.yOffset = 0;

        moveRunner();
        EXPECT_EQ(2,runner.x);
        EXPECT_EQ(2,runner.y);
        EXPECT_EQ(0,runner.xOffset);
        EXPECT_EQ(0,runner.yOffset);

    }
    {
        testLeft.idx = 0;

        // Negative test: 3x3 subset of map[][] for testing w/block to left
        map[1][1].base = map[2][1].base = map[3][1].base = TILE_BLANK;
        map[1][1].act = map[2][1].act = map[3][1].act = TILE_BLANK;
        map[1][2].base = TILE_BLOCK;
        map[2][2].base = TILE_RUNNER;
        map[3][2].base = TILE_BLANK;
        map[1][2].act = TILE_BLOCK; map[2][2].act = map[3][2].act = TILE_BLANK;
        map[1][3].base = map[2][3].base = map[3][3].base = TILE_BRICK;
        map[1][3].act = map[2][3].act = map[3][3].act = TILE_BRICK;
        runner.x = runner.y = 2;
        runner.xOffset = runner.yOffset = 0;

        moveRunner();
        EXPECT_EQ(2,runner.x);
        EXPECT_EQ(2,runner.y);
        EXPECT_EQ(0,runner.xOffset);
        EXPECT_EQ(0,runner.yOffset);

    }    
    TEST_COMPLETE()
}

#if 0
int testRunnerRight()
{
    TEST_INIT()
    // Change to the test input handler
    inputHandler = testAction;
    inputPtr = &testRight;

    {
        // 3x3 subset of map[][] for testing with no obstruction
        map[1][1].base = map[2][1].base = map[3][1].base = TILE_BLANK;
        map[1][1].act = map[2][1].act = map[3][1].act = TILE_BLANK;
        map[1][2].base = TILE_BLANK;
        map[2][2].base = TILE_RUNNER;
        map[3][2].base = TILE_BLANK;
        map[1][2].act = map[2][2].act = map[3][2].act = TILE_BLANK;
        map[1][3].base = map[2][3].base = map[3][3].base = TILE_BRICK;
        map[1][3].act = map[2][3].act = map[3][3].act = TILE_BRICK;
        runner.x = runner.y = 2;
        runner.xOffset = runner.yOffset = 0;

        moveRunner();
        EXPECT_EQ(2,runner.x);
        EXPECT_EQ(2,runner.y);
        EXPECT_EQ(1,runner.xOffset);
        EXPECT_EQ(0,runner.yOffset);

        moveRunner();
        EXPECT_EQ(2,runner.x);
        EXPECT_EQ(2,runner.y);
        EXPECT_EQ(2,runner.xOffset);
        EXPECT_EQ(0,runner.yOffset);        

        moveRunner();
        EXPECT_EQ(2,runner.x);
        EXPECT_EQ(2,runner.y);
        EXPECT_EQ(3,runner.xOffset);
        EXPECT_EQ(0,runner.yOffset);        

        moveRunner();
        EXPECT_EQ(2,runner.x);
        EXPECT_EQ(2,runner.y);
        EXPECT_EQ(4,runner.xOffset);
        EXPECT_EQ(0,runner.yOffset);        

        moveRunner();
        EXPECT_EQ(3,runner.x);
        EXPECT_EQ(2,runner.y);
        EXPECT_EQ(-3,runner.xOffset);
        EXPECT_EQ(0,runner.yOffset);  

        moveRunner();
        EXPECT_EQ(3,runner.x);
        EXPECT_EQ(2,runner.y);
        EXPECT_EQ(-2,runner.xOffset);
        EXPECT_EQ(0,runner.yOffset);         

        moveRunner();
        EXPECT_EQ(3,runner.x);
        EXPECT_EQ(2,runner.y);
        EXPECT_EQ(-1,runner.xOffset);
        EXPECT_EQ(0,runner.yOffset);         

        moveRunner();
        EXPECT_EQ(3,runner.x);
        EXPECT_EQ(2,runner.y);
        EXPECT_EQ(0,runner.xOffset);
        EXPECT_EQ(0,runner.yOffset);     
   
    }
    {
        testRight.idx = 0;

        // 3x3 subset of map[][] for testing with rope and no obstruction
        map[1][1].base = map[2][1].base = map[3][1].base = TILE_BLANK;
        map[1][1].act = map[2][1].act = map[3][1].act = TILE_BLANK;
        map[1][2].base = TILE_ROPE;
        map[2][2].base = TILE_RUNNER;
        map[3][2].base = TILE_ROPE;
        map[1][2].act = map[2][2].act = map[3][2].act = TILE_ROPE;
        map[1][3].base = map[2][3].base = map[3][3].base = TILE_BRICK;
        map[1][3].act = map[2][3].act = map[3][3].act = TILE_BRICK;
        runner.x = runner.y = 2;
        runner.xOffset = runner.yOffset = 0;

        moveRunner();
        EXPECT_EQ(2,runner.x);
        EXPECT_EQ(2,runner.y);
        EXPECT_EQ(1,runner.xOffset);
        EXPECT_EQ(0,runner.yOffset);

        moveRunner();
        EXPECT_EQ(2,runner.x);
        EXPECT_EQ(2,runner.y);
        EXPECT_EQ(2,runner.xOffset);
        EXPECT_EQ(0,runner.yOffset);        

        moveRunner();
        EXPECT_EQ(2,runner.x);
        EXPECT_EQ(2,runner.y);
        EXPECT_EQ(3,runner.xOffset);
        EXPECT_EQ(0,runner.yOffset);        

        moveRunner();
        EXPECT_EQ(2,runner.x);
        EXPECT_EQ(2,runner.y);
        EXPECT_EQ(4,runner.xOffset);
        EXPECT_EQ(0,runner.yOffset);        

        moveRunner();
        EXPECT_EQ(3,runner.x);
        EXPECT_EQ(2,runner.y);
        EXPECT_EQ(-3,runner.xOffset);
        EXPECT_EQ(0,runner.yOffset);  

        moveRunner();
        EXPECT_EQ(3,runner.x);
        EXPECT_EQ(2,runner.y);
        EXPECT_EQ(-2,runner.xOffset);
        EXPECT_EQ(0,runner.yOffset);         

        moveRunner();
        EXPECT_EQ(3,runner.x);
        EXPECT_EQ(2,runner.y);
        EXPECT_EQ(-1,runner.xOffset);
        EXPECT_EQ(0,runner.yOffset);         

        moveRunner();
        EXPECT_EQ(3,runner.x);
        EXPECT_EQ(2,runner.y);
        EXPECT_EQ(0,runner.xOffset);
        EXPECT_EQ(0,runner.yOffset);     
   
    }
    {
        testLeft.idx = 0;

        // 3x3 subset of map[][] for testing with hole to right
        map[1][1].base = map[2][1].base = map[3][1].base = TILE_BLANK;
        map[1][1].act = map[2][1].act = map[3][1].act = TILE_BLANK;
        map[1][2].base = TILE_BLANK;
        map[2][2].base = TILE_RUNNER;
        map[3][2].base = TILE_BRICK;
        map[1][2].act = map[2][2].act = map[3][2].act = TILE_BLANK;
        map[1][3].base = map[2][3].base = map[3][3].base = TILE_BRICK;
        map[1][3].act = map[2][3].act = map[3][3].act = TILE_BRICK;
        runner.x = runner.y = 2;
        runner.xOffset = runner.yOffset = 0;

        moveRunner();
        EXPECT_EQ(2,runner.x);
        EXPECT_EQ(2,runner.y);
        EXPECT_EQ(1,runner.xOffset);
        EXPECT_EQ(0,runner.yOffset);

        moveRunner();
        EXPECT_EQ(2,runner.x);
        EXPECT_EQ(2,runner.y);
        EXPECT_EQ(2,runner.xOffset);
        EXPECT_EQ(0,runner.yOffset);        

        moveRunner();
        EXPECT_EQ(2,runner.x);
        EXPECT_EQ(2,runner.y);
        EXPECT_EQ(3,runner.xOffset);
        EXPECT_EQ(0,runner.yOffset);        

        moveRunner();
        EXPECT_EQ(2,runner.x);
        EXPECT_EQ(2,runner.y);
        EXPECT_EQ(4,runner.xOffset);
        EXPECT_EQ(0,runner.yOffset);        

        moveRunner();
        EXPECT_EQ(3,runner.x);
        EXPECT_EQ(2,runner.y);
        EXPECT_EQ(-3,runner.xOffset);
        EXPECT_EQ(0,runner.yOffset);   
    }
    {
        testLeft.idx = 0;

        // 3x3 subset of map[][] for testing with rope to right
        map[1][1].base = map[2][1].base = map[3][1].base = TILE_BLANK;
        map[1][1].act = map[2][1].act = map[3][1].act = TILE_BLANK;
        map[1][2].base = TILE_BLANK;
        map[2][2].base = TILE_RUNNER;
        map[3][2].base = TILE_ROPE;
        map[1][2].act = map[2][2].act; map[3][2].act = TILE_ROPE;
        map[1][3].base = map[2][3].base = map[3][3].base = TILE_BRICK;
        map[1][3].act = map[2][3].act = map[3][3].act = TILE_BRICK;
        runner.x = runner.y = 2;
        runner.xOffset = runner.yOffset = 0;

        moveRunner();
        EXPECT_EQ(2,runner.x);
        EXPECT_EQ(2,runner.y);
        EXPECT_EQ(1,runner.xOffset);
        EXPECT_EQ(0,runner.yOffset);

        moveRunner();
        EXPECT_EQ(2,runner.x);
        EXPECT_EQ(2,runner.y);
        EXPECT_EQ(2,runner.xOffset);
        EXPECT_EQ(0,runner.yOffset);        

        moveRunner();
        EXPECT_EQ(2,runner.x);
        EXPECT_EQ(2,runner.y);
        EXPECT_EQ(3,runner.xOffset);
        EXPECT_EQ(0,runner.yOffset);        

        moveRunner();
        EXPECT_EQ(2,runner.x);
        EXPECT_EQ(2,runner.y);
        EXPECT_EQ(4,runner.xOffset);
        EXPECT_EQ(0,runner.yOffset);        

        moveRunner();
        EXPECT_EQ(3,runner.x);
        EXPECT_EQ(2,runner.y);
        EXPECT_EQ(-3,runner.xOffset);
        EXPECT_EQ(0,runner.yOffset);   
    }
    {
        testLeft.idx = 0;

        // 3x3 subset of map[][] for testing with gold to right
        map[1][1].base = map[2][1].base = map[3][1].base = TILE_BLANK;
        map[1][1].act = map[2][1].act = map[3][1].act = TILE_BLANK;
        map[1][2].base = TILE_BLANK;
        map[2][2].base = TILE_RUNNER;
        map[3][2].base = TILE_GOLD;
        map[1][2].act = map[2][2].act = TILE_BLANK; map[3][2].act = TILE_GOLD;
        map[1][3].base = map[2][3].base = map[3][3].base = TILE_BRICK;
        map[1][3].act = map[2][3].act = map[3][3].act = TILE_BRICK;
        runner.x = runner.y = 2;
        runner.xOffset = runner.yOffset = 0;

        moveRunner();
        EXPECT_EQ(2,runner.x);
        EXPECT_EQ(2,runner.y);
        EXPECT_EQ(1,runner.xOffset);
        EXPECT_EQ(0,runner.yOffset);

        moveRunner();
        EXPECT_EQ(2,runner.x);
        EXPECT_EQ(2,runner.y);
        EXPECT_EQ(2,runner.xOffset);
        EXPECT_EQ(0,runner.yOffset);        

        moveRunner();
        EXPECT_EQ(2,runner.x);
        EXPECT_EQ(2,runner.y);
        EXPECT_EQ(3,runner.xOffset);
        EXPECT_EQ(0,runner.yOffset);        

        moveRunner();
        EXPECT_EQ(2,runner.x);
        EXPECT_EQ(2,runner.y);
        EXPECT_EQ(4,runner.xOffset);
        EXPECT_EQ(0,runner.yOffset);        

        moveRunner();
        EXPECT_EQ(3,runner.x);
        EXPECT_EQ(2,runner.y);
        EXPECT_EQ(-3,runner.xOffset);
        EXPECT_EQ(0,runner.yOffset);   
    }
#if 0
    {
        testLeft.idx = 0;

        // 3x3 subset of map[][] for testing vertical centering  on move left
        map[1][1].base = TILE_BLANK; map[2][1].base = TILE_RUNNER; map[3][1].base = TILE_BLANK;
        map[1][1].act = TILE_BLANK; map[2][1].act = TILE_LADDER; map[3][1].act = TILE_BLANK;
        map[1][2].base = TILE_BLANK;
        map[2][2].base = TILE_BRICK;
        map[3][2].base = TILE_BLANK;
        map[1][2].act = map[2][2].act = map[3][2].act = TILE_BRICK;
        map[1][3].base = map[2][3].base = map[3][3].base = TILE_BRICK;
        map[1][3].act = map[2][3].act = map[3][3].act = TILE_BRICK;
        runner.x = 2;
        runner.y = 1;
        runner.xOffset = 0;
        runner.yOffset = 3;

        moveRunner();
        EXPECT_EQ(2,runner.x);
        EXPECT_EQ(1,runner.y);
        EXPECT_EQ(-1,runner.xOffset);
        EXPECT_EQ(2,runner.yOffset);

        moveRunner();
        EXPECT_EQ(2,runner.x);
        EXPECT_EQ(1,runner.y);
        EXPECT_EQ(-2,runner.xOffset);
        EXPECT_EQ(1,runner.yOffset);        

        moveRunner();
        EXPECT_EQ(2,runner.x);
        EXPECT_EQ(1,runner.y);
        EXPECT_EQ(-3,runner.xOffset);
        EXPECT_EQ(0,runner.yOffset);        

        moveRunner();
        EXPECT_EQ(2,runner.x);
        EXPECT_EQ(1,runner.y);
        EXPECT_EQ(-4,runner.xOffset);
        EXPECT_EQ(0,runner.yOffset);        

        moveRunner();
        EXPECT_EQ(1,runner.x);
        EXPECT_EQ(2,runner.y);
        EXPECT_EQ(3,runner.xOffset);
        EXPECT_EQ(0,runner.yOffset);   
    }
#endif
    {
        testLeft.idx = 0;

        // Negative test: 3x3 subset of map[][] for testing w/brick to right
        map[1][1].base = map[2][1].base = map[3][1].base = TILE_BLANK;
        map[1][1].act = map[2][1].act = map[3][1].act = TILE_BLANK;
        map[1][2].base = TILE_BLANK;
        map[2][2].base = TILE_RUNNER;
        map[3][2].base = TILE_BRICK;
        map[1][2].act = map[2][2].act = TILE_BLANK; map[3][2].act = TILE_BRICK;
        map[1][3].base = map[2][3].base = map[3][3].base = TILE_BRICK;
        map[1][3].act = map[2][3].act = map[3][3].act = TILE_BRICK;
        runner.x = runner.y = 2;
        runner.xOffset = runner.yOffset = 0;

        moveRunner();
        EXPECT_EQ(2,runner.x);
        EXPECT_EQ(2,runner.y);
        EXPECT_EQ(0,runner.xOffset);
        EXPECT_EQ(0,runner.yOffset);

    }
    {
        testLeft.idx = 0;

        // Negative test: 3x3 subset of map[][] for testing w/block to left
        map[1][1].base = map[2][1].base = map[3][1].base = TILE_BLANK;
        map[1][1].act = map[2][1].act = map[3][1].act = TILE_BLANK;
        map[1][2].base = TILE_BLANK;
        map[2][2].base = TILE_RUNNER;
        map[3][2].base = TILE_BLOCK;
        map[1][2].act = map[2][2].act = TILE_BLANK; map[3][2].act = TILE_BLOCK;
        map[1][3].base = map[2][3].base = map[3][3].base = TILE_BRICK;
        map[1][3].act = map[2][3].act = map[3][3].act = TILE_BRICK;
        runner.x = runner.y = 2;
        runner.xOffset = runner.yOffset = 0;

        moveRunner();
        EXPECT_EQ(2,runner.x);
        EXPECT_EQ(2,runner.y);
        EXPECT_EQ(0,runner.xOffset);
        EXPECT_EQ(0,runner.yOffset);

    }    
    TEST_COMPLETE()
}
#endif