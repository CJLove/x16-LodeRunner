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

void setTile(uint8_t x, uint8_t y, uint8_t tile, uint8_t paletteOffset)
{
    vpoke(tile, y*OFFSET + x*2);
    vpoke(paletteOffset, y*OFFSET + x*2 +1);
}

uint8_t getTile(uint8_t x, uint8_t y)
{
    return vpeek(y*OFFSET + x*2);
}

uint8_t getTileXY(uint16_t x, uint16_t y)
{
    // Convert pixel position to tile position
    x = x / 8;
    y = y / 8;
    return vpeek(y*OFFSET + x*2);
}

uint8_t getTileBelowXY(uint16_t x, uint16_t y)
{
    x = x / 8;
    y = y / 8;
    y++;
    return vpeek(y*OFFSET + x*2);
}