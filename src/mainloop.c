#include <stdio.h>
#include <stdlib.h>
#include <cx16.h>
#include <conio.h>
#include <cbm.h>
#include <unistd.h>
#include <joystick.h>
#include "loderunner.h"
#include "runner.h"
#include "guard.h"
#include "levels.h"
#include "sound.h"

static uint8_t processTick[SPEED_OPTIONS][SPEED_TICKS] = {
    // SLOW - 3 out of 6 ticks
    { 1, 0, 1, 0, 1, 0 },
    // MEDIUM - 4 out of 6 ticks
    { 1, 1, 0, 1, 1, 0 },
    // FAST - 6 out of 6 ticks
    { 1, 1, 1, 1, 1, 1 }
};

// Main loop while playing the game
void playGame() {
    static uint8_t tick = 0;

    if (processTick[currentGame.speed][tick] ) {
        if (currentLevel.goldComplete && runner.y == 0 && runner.yOffset == 0) {
            currentGame.gameState = GAME_FINISH;
            return;
        }

        if (!isDigging()) {
            moveRunner();
        } else {
            processDigHole();
        }
        if (currentGame.gameState != GAME_RUNNER_DEAD) moveGuard();

        processGuardShake();
    }

    tick++;
    if (tick == SPEED_TICKS) tick = 0;

    processFillHole();
    playSoundFx();

}

static uint8_t scoreCount;


void mainTick()
{
    switch(currentGame.gameState) {
        case GAME_SPLASH:
            splash();
            break;
        case GAME_RUNNING:
            playGame();
            break;
        case GAME_NEW_LEVEL:
            if (loadLevel(currentGame.world,currentGame.level)) {
                displayLevel(currentGame.level-1);
                // Enable sprites
                vpoke(0x01, 0x1f4000);
                currentGame.gameState = GAME_RUNNING;
            } else {
                worldComplete();
                currentGame.level = 1;
                currentGame.gameState = GAME_OVER;
            }
            break;
        case GAME_FINISH:
            // Disable sprites
            vpoke(0x0, 0x1f4000);
            // Increase score for level completion
            scoreCount = 0;
            // Start the sound effect for updating the score
            playScoringFx();
            currentGame.gameState = GAME_FINISH_SCORE_COUNT;
            break;
        case GAME_FINISH_SCORE_COUNT:
            while (scoreCount <= SCORE_COUNT) {
                ++scoreCount;
                displayScore(SCORE_INCREMENT);

                playSoundFx();

                waitvsync();
            }

            stopScoringFx();
            currentGame.lives++;
            currentGame.gameState = GAME_NEXT_LEVEL;
            break;
        case GAME_NEXT_LEVEL:
            currentGame.level++;
            currentGame.gameState = GAME_NEW_LEVEL;
            break;
        case GAME_PREV_LEVEL:
            if (currentGame.level) currentGame.level--;
            currentGame.gameState = GAME_NEW_LEVEL;
            break;
        case GAME_RUNNER_DEAD:
            currentGame.lives--;
            displayLives();
            if (currentGame.lives <= 0) {
                // TODO: Game Over
                gameOver();
                currentGame.gameState = GAME_OVER;
            } else {
                currentGame.gameState = GAME_NEW_LEVEL;
            }
            break;
        case GAME_OVER:
            // Keep "Game Over" or "WORLD COMPLETE" displayed for 5 seconds then go back to splash
            sleep(5);
            currentGame.gameState = GAME_SPLASH;
            break;
        default:
            break;
    }
}

int main()
{
    uint8_t result = 0;
    // Read VIA register in memory as a seed
    uint16_t *seed = (uint16_t *)0x9f64;
    srand(*seed);

    // Install CX16 joystick driver
    joy_install(cx16_std_joy);

    // Not supported for CX16
    // kbrepeat(KBREPEAT_ALL);

    currentGame.world = WORLD_CLASSIC;
    // Test world for benchmarks and isolated testing
    //world = WORLD_CUSTOM;
    currentGame.level = 1;
    currentGame.gameState = GAME_SPLASH;
    currentGame.lives = 5;

 

    printf("loading resources...\n");
    
    result = loadFiles();

    if (result) {
        printf("loaded resources successfully\n");
    } else {
        printf("failed to load all resources\n");
        return result;
    }
    initLevels();
    currentGame.currentScore = 0;

    screenConfig();

    // Enable sprites
    //vpoke(0x01, 0x1f4000);

    do {
        // Wait for next VSYNC interrupt
        waitvsync();

        mainTick();

    } 
    while (1);

    return 0;
}