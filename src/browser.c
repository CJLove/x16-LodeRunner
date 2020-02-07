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

#if defined(TEXT_BROWSE)
    VIA1.pra = 1;
    dumpLevel(WORLD_CLASSIC,level);
    VIA1.pra = 1;

    do {
        if (kbhit()) {
            char c = cgetc();
            if (c == ' ') {
                level++;
                dumpLevel(1,level);
            }
        }
    } while(1);
#endif
    screenConfig();

    displayLevel(world,level);

    //completeLevel();
    do {
        setTile(38,1,TILE_GOLD,0);

        if (kbhit()) {

            char c = cgetc();
            setTile(38,2,TILE_GOLD,0);
            switch (c)
            {
                case 'c':
                    completeLevel();
                    break;
                case '+':
                    level++;
                    displayLevel(world,level);
                    break;
                case '-':
                    level--;
                    displayLevel(world,level);
                    break;
                case '1':
                    world = WORLD_CLASSIC;
                    level = 1;
                    displayLevel(world,level);
                    break;
                case '2':
                    world = WORLD_CHAMP;
                    level = 1;
                    displayLevel(world,level);
                    break;
                case '3':
                    world = WORLD_PRO;
                    level = 1;
                    displayLevel(world,level);
                    break;
                case '4':
                    world = WORLD_FANBOOK;
                    level = 1;
                    displayLevel(world,level);
                    break;
                case '5':
                    world = WORLD_REVENGE;
                    level = 1;
                    displayLevel(world,level);
                    break;
                default:
                    break;
            }

        }

        setTile(38,3,TILE_GOLD,0);

    } while (1);
    cgetc();

    return result;
}