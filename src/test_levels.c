#include <stdio.h>
#include <cx16.h>
#include <conio.h>
#include <unistd.h>
#include "loderunner.h"

// Test application which allows browsing levels in all loaded worlds

int main()
{
    int result = 0;
    uint8_t world = WORLD_CLASSIC;
    uint8_t level = 1;
    printf("Loading resources...\n");
    
    result = loadFiles();

    if (result) {
        printf("Loaded resources successfully\n");
    } else {
        printf("Failed to load all resources\n");
        return result;
    }

#if 0
    VIA1.pra = 1;
    loadLevel(WORLD_CLASSIC,level);
    dumpLevel(level);
    VIA1.pra = 1;

    do {
        if (kbhit()) {
            char c = cgetc();
            if (c == ' ') {
                level++;
                loadLevel(WORLD_CLASSIC,level);
                dumpLevel(level);
            } else {
                printf("getc() %d\n",c);
            }
        }
    } while(1);
#else
    screenConfig();

    loadLevel(world,level);
    displayLevel(level-1);

    do {

        if (kbhit()) {

            char c = cgetc();
            switch (c)
            {
                case 'c':
                    completeLevel();
                    break;
                case '+':
                    level++;
                    loadLevel(world,level);
                    displayLevel(level-1);
                    break;
                case '-':
                    level--;
                    loadLevel(world,level);
                    displayLevel(level-1);
                    break;
                case '1':
                    world = WORLD_CLASSIC;
                    level = 1;
                    loadLevel(world,level);
                    displayLevel(level-1);
                    break;
                case '2':
                    world = WORLD_CHAMP;
                    level = 1;
                    loadLevel(world,level);
                    displayLevel(level-1);
                    break;
                case '3':
                    world = WORLD_PRO;
                    level = 1;
                    loadLevel(world,level);
                    displayLevel(level-1);
                    break;
                case '4':
                    world = WORLD_FANBOOK;
                    level = 1;
                    loadLevel(world,level);
                    displayLevel(level-1);
                    break;
                case '5':
                    world = WORLD_REVENGE;
                    level = 1;
                    loadLevel(world,level);
                    displayLevel(level-1);
                    break;
                default:
                    break;
            }

        }

    } while (1);
    cgetc();
#endif
    return result;
}
