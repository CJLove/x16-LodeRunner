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

// Main loop while playing the game
void playGame() {
    if (goldComplete && runner.y == 0 && runner.yOffset == 0) {
        gameState = GAME_FINISH;
        return;
    }

    if (!isDigging()) {
        moveRunner();
    } else {
        processDigHole();
    }
    if (gameState != GAME_RUNNER_DEAD) moveGuard();

    processGuardShake();
    processFillHole();
    // TODO: processReborn();
}

static uint8_t scoreCount;


void mainTick()
{
    switch(gameState) {
        case GAME_SPLASH:
            splash();
            break;
        case GAME_RUNNING:
            playGame();
            break;
        case GAME_NEW_LEVEL:
            if (loadLevel(world,level)) {
                displayLevel(level-1);
                // Enable sprites
                vpoke(0x01, 0x1f4000);
                gameState = GAME_RUNNING;
            } else {
                worldComplete();
                gameState = GAME_OVER;
            }
            break;
        case GAME_FINISH:
            // Disable sprites
            vpoke(0x0, 0x1f4000);
            // Increase score for level completion
            scoreCount = 0;
            
            gameState = GAME_FINISH_SCORE_COUNT;
            break;
        case GAME_FINISH_SCORE_COUNT:
            ++scoreCount;
            displayScore(SCORE_INCREMENT);
            if (scoreCount == SCORE_COUNT) {
                lives++;
                sleep(1);
                gameState = GAME_NEXT_LEVEL;
            }
            break;
        case GAME_NEXT_LEVEL:
            level++;
            gameState = GAME_NEW_LEVEL;
            break;
        case GAME_PREV_LEVEL:
            if (level) level--;
            gameState = GAME_NEW_LEVEL;
            break;
        case GAME_RUNNER_DEAD:
            lives--;
            displayLives();
            if (lives <= 0) {
                // TODO: Game Over
                gameOver();
                gameState = GAME_OVER;
            } else {
                gameState = GAME_NEW_LEVEL;
            }
            break;
        case GAME_OVER:
            // Keep "Game Over" or "WORLD COMPLETE" displayed for 5 seconds then go back to splash
            sleep(5);
            gameState = GAME_SPLASH;
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

    world = WORLD_CLASSIC;
    // Test world for benchmarks and isolated testing
    //world = WORLD_CUSTOM;
    level = 1;
    gameState = GAME_SPLASH;
    lives = 5;

 

    printf("Loading resources...\n");
    
    result = loadFiles();

    if (result) {
        printf("Loaded resources successfully\n");
    } else {
        printf("Failed to load all resources\n");
        return result;
    }

    currentScore = 0;

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