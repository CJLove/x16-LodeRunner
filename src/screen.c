#include <cx16.h>
#include "loderunner.h"

// Offset between rows in tile map
#define OFFSET 256

int screenConfig()
{
    vpoke(128, 0xf0001);
    vpoke(128, 0xf0002);

    vpoke(0x41, 0xf3000);    // Mode 2, enabled
    vpoke(0x6, 0xf3001);     // 8x8 tiles, 128 x 64 map

    vpoke(0, 0xf3002);       // Map base at 0x0000   
    vpoke(0, 0xf3003);

    vpoke(0, 0xf3004);        // Tile base at 0x10000
    vpoke(0x40, 0xf3005);

    videomode(VIDEOMODE_40x30);

    return 1;
}

void setTile(unsigned char x, unsigned char y, unsigned char tile, unsigned char paletteOffset)
{
    vpoke(tile, y*OFFSET + x*2);
    vpoke(paletteOffset, y*OFFSET + x*2 +1);
}

unsigned char getTile(unsigned char x, unsigned char y)
{
    return vpeek(y*OFFSET + x*2);
}

unsigned char getTileXY(uint16_t x, uint16_t y)
{
    // Convert pixel position to tile position
    // TBD
    x = y;
    return 0;
}