#include <stdio.h>
#include <cx16.h>
#include <conio.h>
#include <unistd.h>
#include "levels.h"
#include "loderunner.h"

// Test application which displays splash screen

int main()
{
    int result = 0;
    uint8_t idx = 0;
    uint8_t *tilemap = (uint8_t*)0xa000;
    uint8_t x = 0;
    uint8_t y = 0;
    printf("loading resources...\n");
    
    result = loadFiles();

    if (result) {
        printf("loaded resources successfully\n");
    } else {
        printf("failed to load all resources\n");
        return result;
    }
    initLevels();

    // while (1) {
    //     if (kbhit()) {
    //         uint8_t c = cgetc();
    //         printf("%d\n",c);
    //     }
    // }
    screenConfig();

    splash();

    // // Bank 0
    // VIA1.pra = 21;
    // for (x = 0; x < 40; x++) {
    //     for (y = 0; y < 29; y++) {
    //         uint8_t tile = tilemap[x + y*40];
    //         setTile(x,y,tile,0);
    //     }
    // }

    // setTile(1,16,28,0);
    // setTile(28,16,28,0);
    // setTile(28,22,28,0);

    // sleep(10);

    return result;
}