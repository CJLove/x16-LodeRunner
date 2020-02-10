#include <stdio.h>
#include <cx16.h>
#include <conio.h>
#include <unistd.h>
#include "loderunner.h"

// Test application which tests animation of runner and guard

void displayPos(uint16_t x, uint16_t y)
{
    char buffer[10];
    sprintf(buffer,"%3d",x);
    setTile(36,5,buffer[0],0);
    setTile(37,5,buffer[1],0);
    setTile(38,5,buffer[2],0);

    sprintf(buffer,"%3d",x%8);
    setTile(36,6,buffer[0],0);
    setTile(37,6,buffer[1],0);
    setTile(38,6,buffer[2],0);

    sprintf(buffer,"%3d",y);
    setTile(36,7,buffer[0],0);
    setTile(37,7,buffer[1],0);
    setTile(38,7,buffer[2],0);    

    sprintf(buffer,"%3d",y%8);
    setTile(36,8,buffer[0],0);
    setTile(37,8,buffer[1],0);
    setTile(38,8,buffer[2],0);
}

int main()
{
    int result = 0;
    uint8_t world = WORLD_CLASSIC;
    uint8_t level = 1;
    uint16_t x = 112;
    uint16_t y = 112;
    uint8_t flip = 0;
    uint8_t run[] = { 0, 1, 2 };
    uint8_t climb[] = { 3, 4 };
    uint8_t rapell[] = { 6, 7, 8 };
    uint8_t idx = 0;
    uint8_t isRunning = 0;
    uint8_t dir = 0;    // Right (corresponds to flip bit)
    printf("Loading resources...\n");
    
    result = loadFiles();

    if (result) {
        printf("Loaded resources successfully\n");
    } else {
        printf("Failed to load all resources\n");
        return result;
    }

    screenConfig();

    displayLevel(world,level);
    setTile(14,14,TILE_BLANK,0);    // Hide the runner tile

    // Sprite attributes for runner
    vpoke((RUNNER_1 >> 5) & 0xff, 0x1f5000);    // Attr0
    VERA.data0 = (RUNNER_1 >>13) & 0xf;         // Attr1
    VERA.data0 = x & 0xff;                      // Attr2
    VERA.data0 = x >> 8;                        // Attr3
    VERA.data0 = y & 0xff;                      // Attr4
    VERA.data0 = y >> 8;                        // Attr5
    VERA.data0 = (3 << 2);                      // Attr6 - z pos in front of layer 1
    VERA.data0 = 0;                             // Attr7
    // Enable
    vpoke(0x01, 0x1f4000);

    displayPos(x,y);

    do {
        if (kbhit()) {
            uint8_t tile = getTileXY(x,y);
            uint8_t tileBelow = getTileBelowXY(x,y);
            char c = cgetc();
            switch (c)
            {
                case 'i':
                    if (tile == TILE_LADDER || tileBelow == TILE_LADDER) {
                        y=y-1;
                    }
                    idx++;
                    idx = idx % 2;
                    vpoke(climb[idx],0x1f5000);
                    vpoke(y & 0xff, 0x1f5004);
                    VERA.data0 = y >> 8;
                    break;
                case 'm':
                    if (tile == TILE_LADDER || tileBelow == TILE_LADDER) {
                        y=y+1;
                    }
                    idx++;
                    idx = idx %2;
                    vpoke(climb[idx],0x1f5000);
                    vpoke(y & 0xff, 0x1f5004);
                    VERA.data0 = y >> 8;
                    break;
                case 'k':
                    if (y % 8 == 0) {
                        if (dir) {
                            // Change direction - adjust horizontal flip bit
                            dir = 0;    
                            vpoke((3<<2)|dir, 0x1f5006);
                        }
                        idx++;
                        idx = idx % 3;
                        if (tile == TILE_ROPE) {
                            vpoke(rapell[idx],0x1f5000);
                        } else {
                            vpoke(run[idx],0x1f5000);
                        }
                        x=x+1;
                        vpoke(x & 0xff, 0x1f5002);
                        VERA.data0 = x >> 8;
                    }
                    break;
                case 'j':
                    if (y % 8 == 0) {
                        if (!dir) {
                            // Change in direction - adjust horizontal flip bit
                            dir = 1;
                            vpoke((3<<2)|dir, 0x1f5006);
                        }
                        idx++;
                        idx = idx % 3;
                        if (tile == TILE_ROPE) {
                            vpoke(rapell[idx],0x1f5000);
                        } else {
                            vpoke(run[idx],0x1f5000);
                        }
                        x=x-1;
                        vpoke(x & 0xff, 0x1f5002);
                        VERA.data0 = x >> 8;
                    }
                    break;
                default:
                    break;
            }
            // Display the tiles under and below the runner and the current X/Y position
            tile = getTileXY(x,y);
            setTile(38,1,tile,0);
            tileBelow = getTileBelowXY(x,y);
            setTile(38,3,tileBelow,0);
            displayPos(x,y);
        }



    } while (1);
    cgetc();

    return result;
}