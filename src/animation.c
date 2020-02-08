#include <stdio.h>
#include <cx16.h>
#include <conio.h>
#include <unistd.h>
#include "loderunner.h"

// Test application which tests animation of runner and guard

void displayPos(uint16_t x, uint16_t y)
{
    char buffer[10];
    sprintf(buffer,"%d",x);
    setTile(36,5,buffer[0],0);
    setTile(37,5,buffer[1],0);
    setTile(38,5,buffer[2],0);

    sprintf(buffer,"%d",y);
    setTile(36,7,buffer[0],0);
    setTile(37,7,buffer[1],0);
    setTile(38,7,buffer[2],0);    
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
    uint8_t idx = 0;
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
    setTile(14,14,32,0);    // Hide the runner tile

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
            uint8_t tile = 0;
            char c = cgetc();
            switch (c)
            {
                case 'k':
                    if (dir) {
                        // Change direction - adjust horizontal flip bit
                        dir = 0;    
                        vpoke((3<<2)|dir, 0x1f5006);
                    }
                    idx++;
                    idx = idx % 3;
                    vpoke(run[idx],0x1f5000);
                    x=x+1;
                    vpoke(x & 0xff, 0x1f5002);
                    VERA.data0 = x >> 8;
                    // Display the tiles under and below the runner
                    tile = getTileXY(x,y);
                    setTile(38,1,tile,0);
                    tile = getTileBelowXY(x,y);
                    setTile(38,3,tile,0);
                    break;
                case 'j':
                    if (!dir) {
                        // Change in direction - adjust horizontal flip bit
                        dir = 1;
                        vpoke((3<<2)|dir, 0x1f5006);
                    }
                    if (idx) {
                        idx--;
                    } else
                    {
                        idx = 2;
                    }
                    vpoke(run[idx],0x1f5000);
                    x=x-1;
                    vpoke(x & 0xff, 0x1f5002);
                    VERA.data0 = x >> 8;
                    // Display the tile under the runner
                    tile = getTileXY(x,y);
                    setTile(1,38,tile,0);
                    tile = getTileBelowXY(x,y);
                    setTile(3,38,tile,0);
                    break;
                default:
                    break;
            }
            displayPos(x,y);
        }



    } while (1);
    cgetc();

    return result;
}