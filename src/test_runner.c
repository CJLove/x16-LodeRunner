#include <stdio.h>
#include <cx16.h>
#include <cbm.h>
#include <conio.h>
#include <unistd.h>
#include <joystick.h>
#include "loderunner.h"
#include "levels.h"
#include "runner.h"

// Test application which tests animation of runner and guard

int main()
{
    int result = 0;
    currentGame.world = WORLD_CLASSIC;
    currentGame.level = 1;
    currentGame.lives = 5;
 
    printf("loading resources...\n");
    
    result = loadFiles();

    if (result) {
        printf("loaded resources successfully\n");
    } else {
        printf("failed to load all resources\n");
        return result;
    }

    // Install CX16 joystick driver
    joy_install(cx16_std_joy);

    screenConfig();

    loadLevel(currentGame.world,currentGame.level);
    displayLevel(currentGame.level-1);

    do {
        waitvsync();

        // Replicated logic from playGame() to test runner movement
        if (!isDigging()){
            moveRunner();
        }
        else {
            processDigHole();
        }

        processFillHole();
    } while (1);


    return result;
}