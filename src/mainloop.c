#include <stdio.h>
#include <cx16.h>
#include <conio.h>
#include <unistd.h>
#include "loderunner.h"
#include "runner.h"
#include "levels.h"

int mainloop() {

    // Enable
    vpoke(0x01, 0x1f4000);
    
    do {
        moveRunner();

    } while (1);

    return 0;
}

int main()
{
    uint8_t world = WORLD_CLASSIC;
    uint8_t level = 1;
    uint8_t result = 0;

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

    loadLevel(world,level);
    displayLevel(level-1);

    do {
        mainloop();

    } 
    while (1);

    return 0;
}