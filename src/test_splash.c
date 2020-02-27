#include <stdio.h>
#include <cx16.h>
#include <conio.h>
#include <unistd.h>
#include "loderunner.h"

// Test application which displays splash screen

int main()
{
    int result = 0;
    uint8_t idx = 0;
    uint8_t *tilemap = (uint8_t*)0xa000;
    uint8_t x = 0;
    uint8_t y = 0;
    printf("Loading resources...\n");
    
    result = loadFiles();

    if (result) {
        printf("Loaded resources successfully\n");
    } else {
        printf("Failed to load all resources\n");
        return result;
    }

    screenConfig();

    // Bank 0
    VIA1.pra = 21;
    for (x = 0; x < 30; x++) {
        for (y = 0; y < 28; y++) {
            uint8_t tile = tilemap[x + y*30];
            setTile(x+5,y,tile,0);
        }
    }

    setTile(3,17,28,0);

    sleep(10);

    return result;
}