#include <stdio.h>
#include <cx16.h>
#include <conio.h>
#include <unistd.h>
#include "loderunner.h"

// Test application which displays all tiles

int main()
{
    int result = 0;
    uint8_t tile = 0;
    printf("Loading resources...\n");
    
    result = loadFiles();

    if (result) {
        printf("Loaded resources successfully\n");
    } else {
        printf("Failed to load all resources\n");
        return result;
    }

    screenConfig();

    // Alphabetic characters
    for (tile = 0; tile < 10; tile++) {
        setTile(2*tile + 2,3,tile+1,0);
        setTile(2*tile + 2,5,tile+10,0);
        if (tile < 7) {
            setTile(2*tile + 2, 7,tile+20,0);
        }
    }

    // Numeric characters
    for (tile = 0; tile < 10; tile++) {
        setTile(2*tile+2,9,tile+48,0);
    }

    // Level tiles
    for (tile = 0; tile < 11; tile++) {
        setTile(2*tile+2,11,tile+64,0);
    }

    return result;
}