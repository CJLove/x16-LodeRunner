#include <cx16.h>
#include "loderunner.h"

// Offset between rows in tile map
#define OFFSET 256

int screenConfig()
{
    uint8_t x = 0;
    uint8_t y = 0;

    VERA.control = 0;
//    vera_layer_enable(0x1); // Enable layer 1, disable layer 2
    VERA.display.video |= 0x20;      // Layer 1 enable

    VERA.display.hscale = 128;       // HSCALE=1
    VERA.display.vscale = 128;       // VSCALE=1
    VERA.layer0.config = 0x0;       // Disable Layer 0

    VERA.layer1.config = 0xa1;      // 128x64 map, color depth 1
    VERA.layer1.mapbase = 0x0;      // Map base at 0x00000
    VERA.layer1.tilebase = 0x80;    // Tile base at 0x10000, 8x8 tiles

    videomode(VIDEOMODE_40x30);

    // Clear the full tilemap
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

static uint8_t xOffset = 0;
static uint8_t yOffset = 0;

void setTileOffsets(uint8_t x, uint8_t y)
{
    xOffset = x;
    yOffset = y;
}

void setTile(uint8_t x, uint8_t y, uint8_t tile, uint8_t paletteOffset)
{
    unsigned long addr = ((uint32_t)VERA_INC_1 << 16) | (y+yOffset)*OFFSET + (x+xOffset)*2;
    vpoke(tile, addr);
    VERA.data0 = paletteOffset;
    //vpoke(paletteOffset, (y+yOffset)*OFFSET + (x+xOffset)*2 +1);
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