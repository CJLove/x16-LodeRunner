#include <cx16.h>
#include "loderunner.h"

// Offset between rows in tile map
#define OFFSET 256

int screenConfig()
{
    uint8_t x = 0;
    uint8_t y = 0;

    VERA.control = 0;
    VERA.dc_video |= 0x20;      // Layer 1 enable
    VERA.dc_hscale = 128;       // HSCALE=1
    VERA.dc_vscale = 128;       // VSCALE=1
    VERA.l0_config = 0x0;       // Disable Layer 0

    VERA.l1_config = 0xa1;      // 128x64 map, color depth 1
    VERA.l1_mapbase = 0x0;      // Map base at 0x00000
    VERA.l1_tilebase = 0x80;    // Tile base at 0x10000, 8x8 tiles

    videomode(VIDEOMODE_40x30);

    // Clear the tilemap
    for (x = 0; x < 128; x++) {
        for (y = 0; y < 30; y++) {
            setTile(x,y,TILE_BLANK,0);
        }
    }
    return 1;
}

void screenReset()
{
    videomode(VIDEOMODE_80x60);
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