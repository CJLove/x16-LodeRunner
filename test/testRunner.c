#include "levels.h"
#include "loderunner.h"
#include "runner.h"
#include "x16test.h"

// Stubs
void setTile(uint8_t x, uint8_t y, uint8_t tile, uint8_t paletteOffset) 
{ x = y = tile = paletteOffset = 0; }
void stopAllSoundFx(void) {}
void stopDiggingFx(void) {}
void stopFallingFx(void) {}
void playDeadFx(void) {}
void playDiggingFx(void) {}
void playFallingFx(void) {}
void playGoldFx(void) {}
void clearGuards(void) {}
int8_t guardAlive(uint8_t x, uint8_t y) { x = y; return 0; }
int8_t guardId(uint8_t x, uint8_t y) { x = y; return 0; }
void removeFromShake(uint8_t id) { id = 0;}
void guardReborn(uint8_t x, uint8_t y) { x = y; }
uint8_t initGuard(uint8_t x, uint8_t y) { x = y; return 0; }

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

static struct testInput_t testUp = 
{
    0,
    {ACT_UP, ACT_UP, ACT_UP, ACT_UP, ACT_UP, ACT_UP, ACT_UP, ACT_UP }
};

static struct testInput_t testDown =
{
    0,
    {ACT_DOWN, ACT_DOWN, ACT_DOWN, ACT_DOWN, ACT_DOWN, ACT_DOWN, ACT_DOWN, ACT_DOWN}
};

int testRunnerLeft()
{
    TEST_INIT()
    // Change to the test input handler
    inputHandler = testAction;
    inputPtr = &testLeft;

    {
        // Re-load custom level 1.  Set runner @ 1,1 test movement left
        loadLevel(WORLD_CUSTOM,1);
        runner.x = runner.y = 1;
        runner.xOffset = runner.yOffset = 0;

        moveRunner();
        EXPECT_EQ(1,runner.x);
        EXPECT_EQ(1,runner.y);
        EXPECT_EQ(-1,runner.xOffset);
        EXPECT_EQ(0,runner.yOffset);

        moveRunner();
        EXPECT_EQ(1,runner.x);
        EXPECT_EQ(1,runner.y);
        EXPECT_EQ(-2,runner.xOffset);
        EXPECT_EQ(0,runner.yOffset);        

        moveRunner();
        EXPECT_EQ(1,runner.x);
        EXPECT_EQ(1,runner.y);
        EXPECT_EQ(-3,runner.xOffset);
        EXPECT_EQ(0,runner.yOffset);        

        moveRunner();
        EXPECT_EQ(1,runner.x);
        EXPECT_EQ(1,runner.y);
        EXPECT_EQ(-4,runner.xOffset);
        EXPECT_EQ(0,runner.yOffset);        

        moveRunner();
        EXPECT_EQ(0,runner.x);
        EXPECT_EQ(1,runner.y);
        EXPECT_EQ(3,runner.xOffset);
        EXPECT_EQ(0,runner.yOffset);  

        moveRunner();
        EXPECT_EQ(0,runner.x);
        EXPECT_EQ(1,runner.y);
        EXPECT_EQ(2,runner.xOffset);
        EXPECT_EQ(0,runner.yOffset);         

        moveRunner();
        EXPECT_EQ(0,runner.x);
        EXPECT_EQ(1,runner.y);
        EXPECT_EQ(1,runner.xOffset);
        EXPECT_EQ(0,runner.yOffset);         

        moveRunner();
        EXPECT_EQ(0,runner.x);
        EXPECT_EQ(1,runner.y);
        EXPECT_EQ(0,runner.xOffset);
        EXPECT_EQ(0,runner.yOffset);     
   
    }
    {
        testLeft.idx = 0;
        // Re-load custom level 1.  Set runner @ 5,1 test movement left over rope
        loadLevel(WORLD_CUSTOM,1);

        map[5][1].act = TILE_RUNNER;
        runner.x = 5; runner.y = 1;
        runner.xOffset = runner.yOffset = 0;

        moveRunner();
        EXPECT_EQ(5,runner.x);
        EXPECT_EQ(1,runner.y);
        EXPECT_EQ(-1,runner.xOffset);
        EXPECT_EQ(0,runner.yOffset);

        moveRunner();
        EXPECT_EQ(5,runner.x);
        EXPECT_EQ(1,runner.y);
        EXPECT_EQ(-2,runner.xOffset);
        EXPECT_EQ(0,runner.yOffset);        

        moveRunner();
        EXPECT_EQ(5,runner.x);
        EXPECT_EQ(1,runner.y);
        EXPECT_EQ(-3,runner.xOffset);
        EXPECT_EQ(0,runner.yOffset);        

        moveRunner();
        EXPECT_EQ(5,runner.x);
        EXPECT_EQ(1,runner.y);
        EXPECT_EQ(-4,runner.xOffset);
        EXPECT_EQ(0,runner.yOffset);        

        moveRunner();
        EXPECT_EQ(4,runner.x);
        EXPECT_EQ(1,runner.y);
        EXPECT_EQ(3,runner.xOffset);
        EXPECT_EQ(0,runner.yOffset);  
    }
    {
        testLeft.idx = 0;
        // Re-load custom level 1.  Set runner @ 9,1 test movement left over hidden ladder
        loadLevel(WORLD_CUSTOM,1);
        map[9][1].act = TILE_RUNNER;

        runner.x = 9; runner.y = 1;
        runner.xOffset = runner.yOffset = 0;

        moveRunner();
        EXPECT_EQ(9,runner.x);
        EXPECT_EQ(1,runner.y);
        EXPECT_EQ(-1,runner.xOffset);
        EXPECT_EQ(0,runner.yOffset);

        moveRunner();
        EXPECT_EQ(9,runner.x);
        EXPECT_EQ(1,runner.y);
        EXPECT_EQ(-2,runner.xOffset);
        EXPECT_EQ(0,runner.yOffset);        

        moveRunner();
        EXPECT_EQ(9,runner.x);
        EXPECT_EQ(1,runner.y);
        EXPECT_EQ(-3,runner.xOffset);
        EXPECT_EQ(0,runner.yOffset);        

        moveRunner();
        EXPECT_EQ(9,runner.x);
        EXPECT_EQ(1,runner.y);
        EXPECT_EQ(-4,runner.xOffset);
        EXPECT_EQ(0,runner.yOffset);        

        moveRunner();
        EXPECT_EQ(8,runner.x);
        EXPECT_EQ(1,runner.y);
        EXPECT_EQ(3,runner.xOffset);
        EXPECT_EQ(0,runner.yOffset);   
    }
    {
        testLeft.idx = 0;
        // Re-load custom level 1.  Set runner @ 13,1 test movement left over ladder
        loadLevel(WORLD_CUSTOM,1);
        map[13][1].act = TILE_RUNNER;

        runner.x = 13; runner.y = 1;
        runner.xOffset = runner.yOffset = 0;

        moveRunner();
        EXPECT_EQ(13,runner.x);
        EXPECT_EQ(1,runner.y);
        EXPECT_EQ(-1,runner.xOffset);
        EXPECT_EQ(0,runner.yOffset);

        moveRunner();
        EXPECT_EQ(13,runner.x);
        EXPECT_EQ(1,runner.y);
        EXPECT_EQ(-2,runner.xOffset);
        EXPECT_EQ(0,runner.yOffset);        

        moveRunner();
        EXPECT_EQ(13,runner.x);
        EXPECT_EQ(1,runner.y);
        EXPECT_EQ(-3,runner.xOffset);
        EXPECT_EQ(0,runner.yOffset);        

        moveRunner();
        EXPECT_EQ(13,runner.x);
        EXPECT_EQ(1,runner.y);
        EXPECT_EQ(-4,runner.xOffset);
        EXPECT_EQ(0,runner.yOffset);        

        moveRunner();
        EXPECT_EQ(12,runner.x);
        EXPECT_EQ(1,runner.y);
        EXPECT_EQ(3,runner.xOffset);
        EXPECT_EQ(0,runner.yOffset);   
    } 
    {
        
        testLeft.idx = 0;
        // Re-load custom level 1.  Set runner @ 17,1 test movement left over gold
        map[17][1].act = TILE_RUNNER;
        runner.x = 17; runner.y = 1;
        runner.xOffset = runner.yOffset = 0;

        moveRunner();
        EXPECT_EQ(17,runner.x);
        EXPECT_EQ(1,runner.y);
        EXPECT_EQ(-1,runner.xOffset);
        EXPECT_EQ(0,runner.yOffset);

        moveRunner();
        EXPECT_EQ(17,runner.x);
        EXPECT_EQ(1,runner.y);
        EXPECT_EQ(-2,runner.xOffset);
        EXPECT_EQ(0,runner.yOffset);        

        moveRunner();
        EXPECT_EQ(17,runner.x);
        EXPECT_EQ(1,runner.y);
        EXPECT_EQ(-3,runner.xOffset);
        EXPECT_EQ(0,runner.yOffset);        

        moveRunner();
        EXPECT_EQ(17,runner.x);
        EXPECT_EQ(1,runner.y);
        EXPECT_EQ(-4,runner.xOffset);
        EXPECT_EQ(0,runner.yOffset);        

        moveRunner();
        EXPECT_EQ(16,runner.x);
        EXPECT_EQ(1,runner.y);
        EXPECT_EQ(3,runner.xOffset);
        EXPECT_EQ(0,runner.yOffset);   
    }
    {
        testLeft.idx = 0;
        // Re-load custom level 1.  Set runner @ 21,1 test non-movement due to brick

        map[21][1].act = TILE_RUNNER;
        runner.x = 21; runner.y = 1;
        runner.xOffset = runner.yOffset = 0;

        moveRunner();
        EXPECT_EQ(21,runner.x);
        EXPECT_EQ(1,runner.y);
        EXPECT_EQ(0,runner.xOffset);
        EXPECT_EQ(0,runner.yOffset);

    }
    {
        testLeft.idx = 0;
        // Re-load custom level 1.  Set runner @ 25,1 test non-movement due to brick

        map[25][1].act = TILE_RUNNER;
        runner.x = 25; runner.y = 1;
        runner.xOffset = runner.yOffset = 0;

        moveRunner();
        EXPECT_EQ(25,runner.x);
        EXPECT_EQ(1,runner.y);
        EXPECT_EQ(0,runner.xOffset);
        EXPECT_EQ(0,runner.yOffset);

    }

    TEST_COMPLETE()
}

int testRunnerRight()
{
    TEST_INIT()
    // Change to the test input handler
    inputHandler = testAction;
    inputPtr = &testRight;

    {
        // Re-load custom level 1.  Set runner @ 1,1 test movement right
        loadLevel(WORLD_CUSTOM,1);
        runner.x = runner.y = 1;
        runner.xOffset = runner.yOffset = 0;

        moveRunner();
        EXPECT_EQ(1,runner.x);
        EXPECT_EQ(1,runner.y);
        EXPECT_EQ(1,runner.xOffset);
        EXPECT_EQ(0,runner.yOffset);

        moveRunner();
        EXPECT_EQ(1,runner.x);
        EXPECT_EQ(1,runner.y);
        EXPECT_EQ(2,runner.xOffset);
        EXPECT_EQ(0,runner.yOffset);        

        moveRunner();
        EXPECT_EQ(1,runner.x);
        EXPECT_EQ(1,runner.y);
        EXPECT_EQ(3,runner.xOffset);
        EXPECT_EQ(0,runner.yOffset);        

        moveRunner();
        EXPECT_EQ(1,runner.x);
        EXPECT_EQ(1,runner.y);
        EXPECT_EQ(4,runner.xOffset);
        EXPECT_EQ(0,runner.yOffset);        

        moveRunner();
        EXPECT_EQ(2,runner.x);
        EXPECT_EQ(1,runner.y);
        EXPECT_EQ(-3,runner.xOffset);
        EXPECT_EQ(0,runner.yOffset);  

        moveRunner();
        EXPECT_EQ(2,runner.x);
        EXPECT_EQ(1,runner.y);
        EXPECT_EQ(-2,runner.xOffset);
        EXPECT_EQ(0,runner.yOffset);         

        moveRunner();
        EXPECT_EQ(2,runner.x);
        EXPECT_EQ(1,runner.y);
        EXPECT_EQ(-1,runner.xOffset);
        EXPECT_EQ(0,runner.yOffset);         

        moveRunner();
        EXPECT_EQ(2,runner.x);
        EXPECT_EQ(1,runner.y);
        EXPECT_EQ(0,runner.xOffset);
        EXPECT_EQ(0,runner.yOffset);     
   
    }
    {
        testLeft.idx = 0;
        // Re-load custom level 1.  Set runner @ 5,1 test movement right over rope
        loadLevel(WORLD_CUSTOM,1);

        map[5][1].act = TILE_RUNNER;
        runner.x = 5; runner.y = 1;
        runner.xOffset = runner.yOffset = 0;

        moveRunner();
        EXPECT_EQ(5,runner.x);
        EXPECT_EQ(1,runner.y);
        EXPECT_EQ(1,runner.xOffset);
        EXPECT_EQ(0,runner.yOffset);

        moveRunner();
        EXPECT_EQ(5,runner.x);
        EXPECT_EQ(1,runner.y);
        EXPECT_EQ(2,runner.xOffset);
        EXPECT_EQ(0,runner.yOffset);        

        moveRunner();
        EXPECT_EQ(5,runner.x);
        EXPECT_EQ(1,runner.y);
        EXPECT_EQ(3,runner.xOffset);
        EXPECT_EQ(0,runner.yOffset);        

        moveRunner();
        EXPECT_EQ(5,runner.x);
        EXPECT_EQ(1,runner.y);
        EXPECT_EQ(4,runner.xOffset);
        EXPECT_EQ(0,runner.yOffset);        

        moveRunner();
        EXPECT_EQ(6,runner.x);
        EXPECT_EQ(1,runner.y);
        EXPECT_EQ(-3,runner.xOffset);
        EXPECT_EQ(0,runner.yOffset);  
    }
    {
        testLeft.idx = 0;
        // Re-load custom level 1.  Set runner @ 9,1 test movement right over hidden ladder
        loadLevel(WORLD_CUSTOM,1);
        map[9][1].act = TILE_RUNNER;

        runner.x = 9; runner.y = 1;
        runner.xOffset = runner.yOffset = 0;

        moveRunner();
        EXPECT_EQ(9,runner.x);
        EXPECT_EQ(1,runner.y);
        EXPECT_EQ(1,runner.xOffset);
        EXPECT_EQ(0,runner.yOffset);

        moveRunner();
        EXPECT_EQ(9,runner.x);
        EXPECT_EQ(1,runner.y);
        EXPECT_EQ(2,runner.xOffset);
        EXPECT_EQ(0,runner.yOffset);        

        moveRunner();
        EXPECT_EQ(9,runner.x);
        EXPECT_EQ(1,runner.y);
        EXPECT_EQ(3,runner.xOffset);
        EXPECT_EQ(0,runner.yOffset);        

        moveRunner();
        EXPECT_EQ(9,runner.x);
        EXPECT_EQ(1,runner.y);
        EXPECT_EQ(4,runner.xOffset);
        EXPECT_EQ(0,runner.yOffset);        

        moveRunner();
        EXPECT_EQ(10,runner.x);
        EXPECT_EQ(1,runner.y);
        EXPECT_EQ(-3,runner.xOffset);
        EXPECT_EQ(0,runner.yOffset);   
    }
    {
        testLeft.idx = 0;
        // Re-load custom level 1.  Set runner @ 13,1 test movement right over ladder
        loadLevel(WORLD_CUSTOM,1);
        map[13][1].act = TILE_RUNNER;

        runner.x = 13; runner.y = 1;
        runner.xOffset = runner.yOffset = 0;

        moveRunner();
        EXPECT_EQ(13,runner.x);
        EXPECT_EQ(1,runner.y);
        EXPECT_EQ(1,runner.xOffset);
        EXPECT_EQ(0,runner.yOffset);

        moveRunner();
        EXPECT_EQ(13,runner.x);
        EXPECT_EQ(1,runner.y);
        EXPECT_EQ(2,runner.xOffset);
        EXPECT_EQ(0,runner.yOffset);        

        moveRunner();
        EXPECT_EQ(13,runner.x);
        EXPECT_EQ(1,runner.y);
        EXPECT_EQ(3,runner.xOffset);
        EXPECT_EQ(0,runner.yOffset);        

        moveRunner();
        EXPECT_EQ(13,runner.x);
        EXPECT_EQ(1,runner.y);
        EXPECT_EQ(4,runner.xOffset);
        EXPECT_EQ(0,runner.yOffset);        

        moveRunner();
        EXPECT_EQ(14,runner.x);
        EXPECT_EQ(1,runner.y);
        EXPECT_EQ(-3,runner.xOffset);
        EXPECT_EQ(0,runner.yOffset);   
    } 
    {
        
        testLeft.idx = 0;
        // Re-load custom level 1.  Set runner @ 17,1 test movement right over gold
        loadLevel(WORLD_CUSTOM,1);
        map[17][1].act = TILE_RUNNER;
        runner.x = 17; runner.y = 1;
        runner.xOffset = runner.yOffset = 0;

        moveRunner();
        EXPECT_EQ(17,runner.x);
        EXPECT_EQ(1,runner.y);
        EXPECT_EQ(1,runner.xOffset);
        EXPECT_EQ(0,runner.yOffset);

        moveRunner();
        EXPECT_EQ(17,runner.x);
        EXPECT_EQ(1,runner.y);
        EXPECT_EQ(2,runner.xOffset);
        EXPECT_EQ(0,runner.yOffset);        

        moveRunner();
        EXPECT_EQ(17,runner.x);
        EXPECT_EQ(1,runner.y);
        EXPECT_EQ(3,runner.xOffset);
        EXPECT_EQ(0,runner.yOffset);        

        moveRunner();
        EXPECT_EQ(17,runner.x);
        EXPECT_EQ(1,runner.y);
        EXPECT_EQ(4,runner.xOffset);
        EXPECT_EQ(0,runner.yOffset);        

        moveRunner();
        EXPECT_EQ(18,runner.x);
        EXPECT_EQ(1,runner.y);
        EXPECT_EQ(-3,runner.xOffset);
        EXPECT_EQ(0,runner.yOffset);   
    }
    {
        testLeft.idx = 0;
        // Re-load custom level 1.  Set runner @ 21,1 test non-movement due to brick
        loadLevel(WORLD_CUSTOM,1);
        map[21][1].act = TILE_RUNNER;
        runner.x = 21; runner.y = 1;
        runner.xOffset = runner.yOffset = 0;

        moveRunner();
        EXPECT_EQ(21,runner.x);
        EXPECT_EQ(1,runner.y);
        EXPECT_EQ(0,runner.xOffset);
        EXPECT_EQ(0,runner.yOffset);

    }
    {
        testLeft.idx = 0;
        // Re-load custom level 1.  Set runner @ 25,1 test non-movement due to brick
        loadLevel(WORLD_CUSTOM,1);
        map[25][1].act = TILE_RUNNER;
        runner.x = 25; runner.y = 1;
        runner.xOffset = runner.yOffset = 0;

        moveRunner();
        EXPECT_EQ(25,runner.x);
        EXPECT_EQ(1,runner.y);
        EXPECT_EQ(0,runner.xOffset);
        EXPECT_EQ(0,runner.yOffset);

    }    

    TEST_COMPLETE()
}

int testRunnerUp()
{
    TEST_INIT()
    inputHandler = testAction;
    inputPtr = &testUp;

    printf("Action=%d\n",inputPtr->input[0]);

    {
        // Reload custom level 1.  Set runner @ 13,4 test movement up ladder
        loadLevel(WORLD_CUSTOM,1);
        map[13][4].act = TILE_RUNNER;
        runner.x = 13; runner.y = 4;
        runner.xOffset = runner.yOffset = 0;

        moveRunner();
        EXPECT_EQ(13,runner.x);
        EXPECT_EQ(4,runner.y);
        EXPECT_EQ(0,runner.xOffset);
        EXPECT_EQ(-1,runner.yOffset);

        moveRunner();
        EXPECT_EQ(13,runner.x);
        EXPECT_EQ(4,runner.y);
        EXPECT_EQ(-2,runner.yOffset);

        moveRunner();
        EXPECT_EQ(13,runner.x);
        EXPECT_EQ(4,runner.y);
        EXPECT_EQ(-3,runner.yOffset);

        moveRunner();
        EXPECT_EQ(13,runner.x);
        EXPECT_EQ(4,runner.y);
        EXPECT_EQ(-4,runner.yOffset);

        moveRunner();
        EXPECT_EQ(13,runner.x);
        EXPECT_EQ(3,runner.y);
        EXPECT_EQ(3,runner.yOffset);
    }

    TEST_COMPLETE()
}

int testRunnerDown()
{
    TEST_INIT()
    inputHandler = testAction;
    inputPtr = &testDown;

    printf("Action=%d\n",inputPtr->input[0]);

    {
        // Reload custom level 1.  Set runner @ 13,3 test movement downladder
        loadLevel(WORLD_CUSTOM,1);
        map[13][3].act = TILE_RUNNER;
        runner.x = 13; runner.y = 3;
        runner.xOffset = runner.yOffset = 0;

        moveRunner();
        EXPECT_EQ(13,runner.x);
        EXPECT_EQ(3,runner.y);
        EXPECT_EQ(0,runner.xOffset);
        EXPECT_EQ(1,runner.yOffset);

        moveRunner();
        EXPECT_EQ(13,runner.x);
        EXPECT_EQ(3,runner.y);
        EXPECT_EQ(2,runner.yOffset);

        moveRunner();
        EXPECT_EQ(13,runner.x);
        EXPECT_EQ(3,runner.y);
        EXPECT_EQ(3,runner.yOffset);

        moveRunner();
        EXPECT_EQ(13,runner.x);
        EXPECT_EQ(3,runner.y);
        EXPECT_EQ(4,runner.yOffset);

        moveRunner();
        EXPECT_EQ(13,runner.x);
        EXPECT_EQ(4,runner.y);
        EXPECT_EQ(-3,runner.yOffset);
    }

    TEST_COMPLETE()
}

int testRunnerFall()
{
#if 0    
    TEST_INIT()
    inputHandler = testAction;
    inputPtr = &testDown;

    printf("Action=%d\n",inputPtr->input[0]);

    {
        // Reload custom level 1.  Set runner @ 12,3 test movement up ladder
        loadLevel(WORLD_CUSTOM,1);
        map[12][3].act = TILE_RUNNER;
        runner.x = 12; runner.y = 3;
        runner.xOffset = runner.yOffset = 0;

        moveRunner();
        EXPECT_EQ(12,runner.x);
        EXPECT_EQ(3,runner.y);
        EXPECT_EQ(0,runner.xOffset);
        EXPECT_EQ(1,runner.yOffset);

        moveRunner();
        EXPECT_EQ(12,runner.x);
        EXPECT_EQ(3,runner.y);
        EXPECT_EQ(2,runner.yOffset);

        moveRunner();
        EXPECT_EQ(12,runner.x);
        EXPECT_EQ(3,runner.y);
        EXPECT_EQ(3,runner.yOffset);

        moveRunner();
        EXPECT_EQ(12,runner.x);
        EXPECT_EQ(3,runner.y);
        EXPECT_EQ(4,runner.yOffset);

        moveRunner();
        EXPECT_EQ(12,runner.x);
        EXPECT_EQ(4,runner.y);
        EXPECT_EQ(-3,runner.yOffset);
    }

    TEST_COMPLETE()
#endif
    return 0;
}